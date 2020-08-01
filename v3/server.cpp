#include <stdio.h>  // perror definition
#include <stdlib.h> // EXIT_FAILURE definition and htons functions
#include <unistd.h> // close socket
//#include <sys/types.h>
#include <sys/socket.h> // sock() function and AF_INET and SOCK_DGRAM definition and shutdown function
#include <arpa/inet.h>  // struct sockaddr_in
//#include <netinet/in.h>
#include <cstring>  // memset required it

namespace constants{
  int kPort {8080};
  int kMaxBytesMsg {1024}; 
}

int main(){

  int sock_fd;
  int enable = 1;

  // Start socket
  // Create an unbound socket in the specified domain
  // int socket(int domain, int type, int protocol)
  // domain - specifies the communication domain (AF_INET for IPv4 || AF_INET6 for IPv6)
  // type - type of socket to be created (SOCK_STREAM for TCP || SOCK_DGRAM for UDP)
  // protocol - protocol to be used by socket (0 means default protocol for the address family)
  if( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  // Enable port reuse
  if( setsockopt(sock_fd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) < 0){
    perror("socket SO_REUSEPORT option failed");
    exit(EXIT_FAILURE);
  }
  // Enable address reuse
  if( setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
    perror("socket SO_REUSEADDR option failed");
    exit(EXIT_FAILURE);
  }

  
  // SO_LINGER controls the action taken when unsent messages are qued and a close(2) is performed
  // The system waits only for the duration of linger interval to drain the data.
  struct linger sl;
  sl.l_onoff = 1; // Non-zero value enables linger option in kernel
  sl.l_linger = 0;  // timeout interval in seconds

  if( setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl)) < 0){
    perror("Error linger");
    exit(EXIT_FAILURE);
  }

  // Create structure for server_addr;
  struct sockaddr_in server_addr;
  // Bind socket to ip address and port
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(constants::kPort);   // convert from little to big endian

  // Bind socket (just an int, with an address and a port, so any traffic to that address and port 
  // gets handled to the socket)
  // reinterpret_cast<> intended for low level casts that yield implelentation-dependent
  // (i.e unportable) results e.g. casting a pointer to an int 
  // ===========================
  // Assigns address to the unbound socket
  // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
  // sockdf - file descriptor of socket to be binded
  // addr - structure in which address to be binded to is specified
  // addrlen - size of addr structure
  if( bind(sock_fd, reinterpret_cast<const struct sockaddr *>(&server_addr), sizeof(server_addr)) < 0 ){
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  // Create structure for client
  sockaddr_in client_addr;
  socklen_t client_length = sizeof(client_addr);
  char buffer [constants::kMaxBytesMsg];

  printf("Listening connections on port %d\n", constants::kPort);

  // Enter loop
  while(true){
    // Clean memory
    memset(&client_addr, 0, client_length);
    memset(&buffer, 0, constants::kMaxBytesMsg);
    
    // Wait for  message
    // size_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)
    // Receive a message from the socket
    // sockfd - file descriptor of socket
    // buf - application buffer in which to receive data
    // len - size of buf application buffer
    // flags - bitwise or of flags to modify socket behaviour
    // src_addr - structure containing source address is returned
    // addrlen - variable in which size of src_addr structure is returned
    int bytes_in = recvfrom(sock_fd, buffer, constants::kMaxBytesMsg, MSG_WAITALL, reinterpret_cast<struct sockaddr *>(&client_addr), &client_length);
    if(bytes_in == -1){
      printf("Error receiving from client");
      continue;
    }

    // legacy, end buffer probably handled by memset(zero)
    //buffer[bytes_in] = '\0';
    
    // Display message and client info
    char client_ip[256];
    memset(&client_ip, 0, 256);

    // ntop = number to pointer to string     // pton = pointer to string to number
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
