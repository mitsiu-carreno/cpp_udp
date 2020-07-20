#include <stdio.h>  // perror definition
#include <stdlib.h> // EXIT_FAILURE definition and htons functions
#include <unistd.h> // close socket
//#include <sys/types.h>
#include <sys/socket.h> // sock() function and AF_INET and SOCK_DGRAM definition and shutdown function
#include <arpa/inet.h>  // struct sockaddr_in
//#include <netinet/in.h>
#include <cstring>  // memset required it

int main(){
  int port = 8080;
  int max_bytes_msg = 1024;

  // Start socket
  int sock_fd;
  struct sockaddr_in server_addr;
  int enable = 1;

  if( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  if( setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0){
    perror("socket SO_REUSEPORT option failed");
    exit(EXIT_FAILURE);
  }
  if( setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
    perror("socket SO_REUSEADDR option failed");
    exit(EXIT_FAILURE);
  }

  struct linger sl;
  sl.l_onoff = 1; // Non-zero value enables linger option in kernel
  sl.l_linger = 0;  // timeout interval in seconds
  if( setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl)) < 0){
    perror("Error linger");
    exit(EXIT_FAILURE);
  }

  // Bind socket to ip address and port
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);   // convert from little to big endian

  // reinterpret_cast<> intended for low level casts that yield implelentation-dependent
  // (i.e unportable) results e.g. casting a pointer to an int 
  if( bind(sock_fd, reinterpret_cast<const struct sockaddr *>(&server_addr), sizeof(server_addr)) < 0 ){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  sockaddr_in client_addr;
  socklen_t client_length = sizeof(client_addr);
  char buffer [max_bytes_msg];

  printf("Listening connections on port %d\n", port);

  // Enter loop
  while(true){
    // Clean memory
    memset(&client_addr, 0, client_length);
    memset(&buffer, 0, max_bytes_msg);
    
    // Wait for message
    int bytes_in = recvfrom(sock_fd, buffer, max_bytes_msg, MSG_WAITALL, reinterpret_cast<struct sockaddr *>(&client_addr), &client_length);
    if(bytes_in == -1){
      printf("Error receiving from client");
      continue;
    }

    // Display message and client info
    char client_ip[256];
    memset(&client_ip, 0, 256);

    // client_addr.sin_addr store ip address as bytes, we turn it into a string "127.0.0.1"
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, 256);  
    printf("Msg received from %s : %s", client_ip, buffer);
  
  printf("Closing socket");
  
  // Close socket 
  // Actually destroy a socket
  close(sock_fd);


  // Shutdown socket a flexible way to block communication in one or both directions 
  // SHUT_RD - disable further receive operations
  // SHUT_WR - disable futher send operations
  // SHUT_RDWR - disable further send and receive operations
  shutdown(sock_fd, SHUT_RDWR);

  }

  return 0;  
}
