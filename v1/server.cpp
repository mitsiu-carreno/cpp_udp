#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

int main(){
  int sock_fd;
  char buffer[MAXLINE];
  const char *hello = "Hello from server";
  struct sockaddr_in serv_addr, cli_addr;

  // Create an unbound socket in the specified domain
  // int socket(int domain, int type, int protocol)
  // domain - specifies the communication domain (AF_INET for IPv4 || AF_INET6 for IPv6)
  // type - type of socket to be created (SOCK_STREAM for TCP || SOCK_DGRAM for UDP)
  // protocol - protocol to be used by socket (0 means default protocol for the address family)
  if( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  memset(&cli_addr, 0, sizeof(cli_addr));

  // Filling server information
  serv_addr.sin_family = AF_INET; // IPv4
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT);

  // Assigns address to the unbound socket
  // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
  // sockdf - file descriptor of socket to be binded
  // addr - structure in which address to be binded to is specified
  // addrlen - size of addr structure
  if( bind(sock_fd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0 ){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  
  printf("Listening connections on port %d\n", PORT);

  int n;

  socklen_t len = sizeof(cli_addr);   // len is value/resuslt

  // size_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
  // Receive a message from the socket
  // sockfd - file descriptor of socket
  // buf - application buffer in which to receive data
  // len - size of buf application buffer
  // flags - bitwise or of flags to modify socket behaviour
  // src_addr - structure containing source address is returned
  // addrlen - variable in which size of src_addr structure is returned
  n = recvfrom(sock_fd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cli_addr, &len);

  buffer[n] = '\0';
  printf("Client: %s\n", buffer);
  
  // Send a message to the socket
  // size_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
  // sockfd - file descriptr of socket
  // buff - application buffer containing the data to be sent
  // len - size of buf application buffer
  // flags - bitwise OR of flags to modify socket behaviour
  // dest_addr - structure containing address of destination
  // addrlen - size of dest_addr structure
  sendto(sock_fd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *)&cli_addr, len);
  printf("Hello message sent.\n");

  // Close a file descriptor
  // int close(int sockfd)
  // sockfd - File descriptor of socket

  return 0;
}
