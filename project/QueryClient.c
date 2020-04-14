#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "QueryProtocol.h"

char *port_string = "1500";
unsigned short int port;
char *ip = "127.0.0.1";

#define BUFFER_SIZE 1000

void RunQuery(char *query) {
  // Find the address
  if (strlen(query) > 100) {
    printf("Please limit your query to 100 characters\n\n");
    return;
  }
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; /* IPv4 only */
  hints.ai_socktype = SOCK_STREAM; /* TCP */
  int s;
  s = getaddrinfo(ip, port_string, &hints, &result);
  if (s != 0) {
    printf("s is not 0");
    return;
  }
  // Create the socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  // Connect to the server

  if (connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1) {
    printf("no connect");
    return;
  }

  char response[1000];
  int l = read(sock_fd, response, 999);
  response[l] = '\0';

  int r = CheckAck(response);
  if (r == -1) {
    printf("error\n");
    return;
  }
  printf("Connected to movie server\n");
  printf("\n");

  // Do the query-protocol
  char *buffer = query;
  write(sock_fd, buffer, strlen(buffer));

  char resp[1000];
  int len = read(sock_fd, resp, 999);
  resp[len] = '\0';

  SendAck(sock_fd);
  char res[1000];
  int le = read(sock_fd, res, 999);
  res[le] = '\0';
  while (strcmp(res, "GOODBYE") != 0) {
    SendAck(sock_fd);
    printf("%s\n", res);
    le = read(sock_fd,res,999);
    res[le] = '\0';
  }
  // Close the connection
  close(sock_fd);
  freeaddrinfo(result);
}

void RunPrompt() {
  char input[BUFFER_SIZE];

  while (1) {
    printf("Enter a term to search for, or q to quit: ");
    scanf("%s", input);

    printf("input was: %s\n", input);

    if (strlen(input) == 1) {
      if (input[0] == 'q') {
        printf("Thanks for playing! \n");
        return;
      }
    }
    printf("\n\n");
    RunQuery(input);
  }
}

// This function connects to the given IP/port to ensure 
// that it is up and running, before accepting queries from users.
// Returns 0 if can't connect; 1 if can. 
int CheckIpAddress(char *ip, char *port) {
  // Connect to the server
  // Listen for an ACK
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; /* IPv4 only */
  hints.ai_socktype = SOCK_STREAM; /* TCP */
  int s;
  s = getaddrinfo(ip, port, &hints, &result);
  if (s != 0) {
    return 0;
  }
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (connect(sock_fd, result->ai_addr, result->ai_addrlen) == -1) {
    printf("Connection Failed\n");
    return 0;
  }
  char resp[1000];
  int len = read(sock_fd, resp, 999);
  resp[len] = '\0';

  int r = CheckAck(resp);
  if (r == -1) {
    printf("cannot receive ACK");
    return 0;
  }
  // Send a goodbye
  s = SendGoodbye(sock_fd);
  // Close the connection
  close(sock_fd);
  freeaddrinfo(result);
  if (s == -1) {
    return 0;
  }
  return 1;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Incorrect number of arguments. \n");
    printf("Correct usage: ./queryclient [IP] [port]\n");
  } else {
    ip = argv[1];
    port_string = argv[2];
  }

  if (CheckIpAddress(ip, port_string)) {
    RunPrompt();
  }
  return 0;
}
