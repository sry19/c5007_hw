#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#include "DirectoryParser_MT.h"
#include "QueryProtocol.h"
#include "MovieSet.h"
#include "MovieIndex.h"
#include "MovieTitleIndex.h"
#include "DocIdMap.h"
#include "Hashtable.h"
#include "QueryProcessor.h"
#include "FileParser.h"
#include "FileCrawler.h"
#include "DirectoryParser.h"
#include "Util.h"

DocIdMap docs;
MovieTitleIndex docIndex;

#define BUFFER_SIZE 1000

#define SEARCH_RESULT_LENGTH 1500
char movieSearchResult[SEARCH_RESULT_LENGTH];

int Cleanup();

void sigint_handler(int sig) {
  write(0, "Exit signal sent. Cleaning up...\n", 34);
    Cleanup();
  exit(0);
}

int HandleClient(int sock_fd) {
  // Step 5: Accept connection
  printf("Waiting for connection...\n");
  int client_fd = accept(sock_fd, NULL, NULL);
  printf("Connection made: client_fd=%d\n", client_fd);
  // Step 6: Read, then write if you want

  // Send ACK
  printf("send ack\n");
   SendAck(client_fd);
   //  char buf[1000];
   //printf("read and checkgoodbye\n");
   //int l = read(client_fd, buf, sizeof(buf) - 1);
   //buf[l] = '\0';
   //if (CheckGoodbye(buf) == -1) {
   //printf("not receive goodbye\n");
   //return 1;
   //}
  //l = read(client_fd, buf, sizeof(buf) - 1);
  //buf[l] = '\0';
 
  //SendAck(client_fd);
  // Listen for query
  // If query is GOODBYE close ocnnection
  printf("wait for a search\n");
  char buffer[1000];
  int len = read(client_fd, buffer, sizeof(buffer) - 1);
  buffer[len] = '\0';
  printf("SERVER RECEIVED: %s \n", buffer);
  if (CheckGoodbye(buffer) == 0) {
    //    close(client_fd);
    return 1;
  }
  // Run query and get responses
  SearchResultIter iter = FindMovies(docIndex, buffer);
  // Send number of responses
  int number = NumResultsInIter(iter);
  char msg[SEARCH_RESULT_LENGTH];
  sprintf(msg, "%d", number);
  write(client_fd, msg, strlen(msg));
  // Wait for ACK
  char buffer_ack[1000];
  len = read(client_fd, buffer_ack, sizeof(buffer_ack) - 1);
  buffer_ack[len] = '\0';
  if (CheckAck(buffer_ack) == -1) {
    printf("not receive ack\n");
    return 1;
  }
  // For each response
  SearchResult output = (SearchResult)malloc(sizeof(*output));
  //  char * movieSearchResult = (char*)malloc(sizeof(*movieSearchResult));
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
  //free(movieSearchResult);
  // Cleanup
  //  Cleanup();
  // Send GOODBYE
  SendGoodbye(client_fd);
  // close connection.
  close(client_fd);
  return 0;
}

int Setup(char *dir) {
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
  char *port = NULL;
  char *dir_to_crawl = NULL;

  int debug_flag = 0;
  int c;

  while ((c = getopt (argc, argv, "dp:f:")) != -1) {
    switch (c)
      {
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

  // Setup graceful exit
  struct sigaction kill;

  kill.sa_handler = sigint_handler;
  kill.sa_flags = 0;  // or SA_RESTART
  sigemptyset(&kill.sa_mask);

  if (sigaction(SIGINT, &kill, NULL) == -1) {
    perror("sigaction");
    exit(1);
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

  s = getaddrinfo(NULL, "1501", &hints, &result);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }
  // Step 2: Open socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  // Step 3: Bind socket
  if (bind(sock_fd, result->ai_addr, result->ai_addrlen) != 0) {
    perror("bind()");
    exit(1);
  }
  // Step 4: Listen on the socket
  if (listen(sock_fd, 10) != 0) {
    perror("listen()");
    exit(1);
  }
  int client_fd = accept(sock_fd, NULL, NULL);
  //printf("response to ip check\n");
  int r =  SendAck(client_fd);
  char buffer[1000];
  int len = read(client_fd, buffer, sizeof(buffer) - 1);
  buffer[len] = '\0';
  printf("check goodbye\n");
  if (CheckGoodbye(buffer) == -1) {
    printf("not receive goodbye\n");
    return -1;
  }
  close(client_fd);
  printf("start handle\n");
  while (1) {
    int result = HandleClient(sock_fd);
    if (result == -1) {
      break;
    }
  }
  // Got Kill signal
  close(sock_fd);

  freeaddrinfo(result);

  Cleanup();

  return 0;
}
