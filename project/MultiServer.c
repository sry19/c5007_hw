#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>


#include "QueryProtocol.h"
#include "MovieSet.h"
#include "MovieIndex.h"
#include "DocIdMap.h"
#include "Hashtable.h"
#include "QueryProcessor.h"
#include "FileParser.h"
#include "DirectoryParser.h"
#include "FileCrawler.h"
#include "Util.h"

#define BUFFER_SIZE 1000
#define N 5

int Cleanup();

DocIdMap docs;
MovieTitleIndex docIndex;

#define SEARCH_RESULT_LENGTH 1500

char movieSearchResult[SEARCH_RESULT_LENGTH];

void sigchld_handler(int s) {
  write(0, "Handling zombies...\n", 20);
  // waitpid() might overwrite errno, so we save and restore it:
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}


void sigint_handler(int sig) {
  write(0, "Ahhh! SIGINT!\n", 14);
  Cleanup();
  exit(0);
}

/**
 * Return 0 for successful connection;
 * Return -1 for some error.
 */
int HandleClient(int client_fd, char* query) {
  int len;
  // Run query and get responses
  SearchResultIter iter = FindMovies(docIndex, query);
  if (iter == NULL) {
    printf("no result\n");
    int t = 0;
    write(client_fd, &t, sizeof(t));
    char buf_ack[1000];
    len = read(client_fd, buf_ack, sizeof(buf_ack) - 1);
    buf_ack[len] = '\0';
    SendGoodbye(client_fd);
    close(client_fd);
    return 0;
  }
  // Send number of responses
  int number = NumResultsInIter(iter);
  char msg[SEARCH_RESULT_LENGTH];
  sprintf(msg, "%d", number);
  write(client_fd, &number, sizeof(number));
  // Wait for ACK
  char buffer_ack[BUFFER_SIZE];
  len = read(client_fd, buffer_ack, sizeof(buffer_ack) - 1);
  buffer_ack[len] = '\0';
  if (CheckAck(buffer_ack) == -1) {
    printf("not receive ack\n");
    return -1;
  }
  // For each response

    // Send response

    // Wait for ACK
  SearchResult output = (SearchResult)malloc(sizeof(*output));

  while (SearchResultIterHasMore(iter)) {
    SearchResultGet(iter, output);
    // Send response
    CopyRowFromFile(output, docs, movieSearchResult);
    write(client_fd, movieSearchResult, strlen(movieSearchResult));
    // Wait for ACK
    char buffer_output[BUFFER_SIZE];
    int len = read(client_fd, buffer_output, sizeof(buffer_output) - 1);
    buffer_output[len] = '\0';
    if (CheckAck(buffer_output) == -1) {
      printf("not receive ack from the client\n");
      return -1;
    }
    SearchResultNext(iter);
  }
  SearchResultGet(iter, output);
  CopyRowFromFile(output, docs, movieSearchResult);
  write(client_fd, movieSearchResult, strlen(movieSearchResult));
  char buffer_output_final[BUFFER_SIZE];
  len = read(client_fd, buffer_output_final, sizeof(buffer_output_final) - 1);
  buffer_output_final[len] = '\0';
  if (CheckAck(buffer_output_final) == -1) {
    printf("not receive ack from the client\n");
    return -1;
  }

  free(output);
  DestroySearchResultIter(iter);
  // Send GOODBYE
  SendGoodbye(client_fd);
  // close connection.
  close(client_fd);
  return 0;
}

/**
 *
 */
int HandleConnections(int sock_fd, int debug) {
  // Step 5: Accept connection
  // Fork on every connection
  pid_t pid;
  printf("Waiting for connection...\n");
  int client_fd = accept(sock_fd, NULL, NULL);
  printf("Connection made: client_fd=%d\n", client_fd);
  // Step 6: Read, then write if you want
  printf("Client connected\n");
  // Send ACK
  SendAck(client_fd);
  // Listen for query
  // If query is GOODBYE close ocnnection
  char buf[BUFFER_SIZE];
  int len = read(client_fd, buf, sizeof(buf) - 1);
  buf[len] = '\0';
  printf("checking goodbye... %s \n", buf);
  if (CheckGoodbye(buf) == 0) {
    close(client_fd);
    return 1;
  }
  pid = fork();
  if (pid == 0) {
    if (debug == 1) {
      sleep(10);
    }
    int result = HandleClient(client_fd, buf);
    Cleanup();
    exit(0);
  }
  return 0;
}

int Setup(char *dir) {
  struct sigaction sa;

  sa.sa_handler = sigchld_handler;  // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  struct sigaction kill;

  kill.sa_handler = sigint_handler;
  kill.sa_flags = 0;  // or SA_RESTART
  sigemptyset(&kill.sa_mask);

  if (sigaction(SIGINT, &kill, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("Crawling directory tree starting at: %s\n", dir);
  // Create a DocIdMap
  docs = CreateDocIdMap();
  CrawlFilesToMap(dir, docs);
  printf("Crawled %d files.\n", NumElemsInHashtable(docs));

  // Create the index
  docIndex = CreateMovieTitleIndex();

  if (NumDocsInMap(docs) < 1) {
    printf("No documents found.\n");
    return 0;
  }

  // Index the files
  printf("Parsing and indexing files...\n");
  ParseTheFiles(docs, docIndex);
  printf("%d entries in the index.\n", NumElemsInHashtable(docIndex->ht));
  return NumElemsInHashtable(docIndex->ht);
}

int Cleanup() {
  DestroyMovieTitleIndex(docIndex);
  DestroyDocIdMap(docs);
  return 0;
}

int main(int argc, char **argv) {
  // port
  char *port = NULL;
  char *dir_to_crawl = NULL;

  int debug_flag = 0;
  int c;

  while ((c = getopt(argc, argv, "dp:f:")) != -1) {
    switch (c) {
      case 'd':
        debug_flag = 1;
        break;
      case 'p':
        port = optarg;
        break;
      case 'f':
        dir_to_crawl = optarg;
        break;
      }
  }

  if (port == NULL) {
    printf("No port provided; please include with a -p flag.\n");
    exit(0);
  }

  if (dir_to_crawl == NULL) {
    printf("No directory provided; please include with a -f flag.\n");
    exit(0);
  }

  int num_entries = Setup(dir_to_crawl);
  if (num_entries == 0) {
    printf("No entries in index. Quitting. \n");
    exit(0);
  }

  int s;

  // Step 1: Get address stuff
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  s = getaddrinfo(NULL, port, &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }

  // Step 2: Open socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Step 3: Bind socket
  int yes = 1;
  // lose the pesky "Address already in use" error message
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    perror("setsockopt");
    exit(1);
  }
  if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
    perror("bind()");
    exit(1);
  }

  // Step 4: Listen on the socket
  if (listen(sock_fd, 10) != 0) {
    perror("listen()");
    exit(1);
  }
  freeaddrinfo(result);

  while (1) {
    HandleConnections(sock_fd, debug_flag);
  }

  // Got Kill signal
  close(sock_fd);
  Cleanup();
  return 0;
}
