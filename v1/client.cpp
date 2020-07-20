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
  const char *hello = "Hello from client";
  struct sockaddr_in serv_addr;

  // Creating socket file descriptor
  if( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  if(connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
    printf("Connect failed\n");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));

  // Filling server information
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = INADDR_ANY;

  int n; 
  socklen_t len = sizeof(serv_addr);

  sendto(sock_fd, (const char *)hello, strlen(hello), 0, (const struct sockaddr *)&serv_addr, sizeof(serv_addr));

  printf("Hello message sent.\n");

  n = recvfrom(sock_fd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&serv_addr, &len);

  buffer[n] = '\0';
  printf("Server: %s\n", buffer);

  close(sock_fd);
  return 0;
}
