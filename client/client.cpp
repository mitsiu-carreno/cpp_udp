#include <stdio.h>  //perror definition
#include <stdlib.h>   // EXIT_FAILURE definitionand htons functions
#include <unistd.h>   // close socket
//#include <sys/types.h>
#include <sys/socket.h> // sock() function and AF_INET and SOCK_DGRAM definition and shutdown function
#include <arpa/inet.h>  // struct sockaddr_in 
//#include <netinet/in.h>
#include <cstring>  // memset definition
#include <string>
#include <iostream>

void Continue(){
  std::string input;
  printf("Press enter to send message: ");
  // Flush cin
  std::cin.ignore(32767, '\n');

}

int main(int argc, char* argv[]){
  // argv[0] is program name
  if(argv[1] == NULL){
    printf("Agregue un string con la dirección del servidor ejemplo ./client/client \"192.168.100.12\" \"20777\" \"Hello kitty\"");
    exit(EXIT_FAILURE);
  }
  if(argv[2] == NULL){
    printf("Agregue un string con el puerto al que se desea acceder ejemplo ./client/client \"192.168.100.12\" \"20777\" \"Hello kitty\"");
    exit(EXIT_FAILURE);
  }
  if(argv[3] == NULL){
    printf("Agregue un string con el mensaje que desea enviar ejemplo ./client/client \"192.168.100.12\" \"20777\" \"Hello kitty\"");
    exit(EXIT_FAILURE);
  }
  //char server_address[] = {argv[1]};
  char * server_address = argv[1];
  int port {std::stoi(argv[2])};

  std::string s = argv[3];
  //int max_bytes_msg = 1024;

  // Start socket
  int sock_fd;

  // Create structure for the server
  sockaddr_in server_addr;
  // Bind socket to ip address and port
  server_addr.sin_family = AF_INET; // AF_INET = IPv4
  server_addr.sin_port = htons(port); // convert from little to big endian
  
  // convert from string to byte array
  inet_pton(AF_INET, reinterpret_cast<const char*>(&server_address), &server_addr.sin_addr);


  // Socket creation
  if( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  /*
  LEGACY CODE THAT SOMEHOW BREAKS THE CODE
   if(connect(sock_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) < 0){
    printf("Connect failed\n");
  }
  */


  
  Continue();
  
  // s.size()+1 cause we want to store the \0 at the end of the string
  // ==========================
  // Send a message to the socket
  // size_t sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen)
  // sockfd - file descriptr of socket
  // buff - application buffer containing the data to be sent
  // len - size of buf application buffer
  // flags - bitwise OR of flags to modify socket behaviour
  // dest_addr - structure containing address of destination
  // addrlen - size of dest_addr structure
  int send_status = sendto(sock_fd, s.c_str(), s.size()+1, 0, reinterpret_cast<const struct sockaddr*>(&server_addr), sizeof(server_addr));  

  if(send_status == -1){
    perror("Error while sending message");
  }

  // Close socket
  // Destroy a socket
  // Close a file descriptor
  close(sock_fd);

  // Shutdown socket in a flexible way to block communication in one or both directions
  // SHUT_RD - disable further receive operations
  // SHUT_WR - disable further send operations
  // SHUT_RDWR - disable further send and receive operations
  shutdown(sock_fd, SHUT_RDWR);

  return 0;
}
