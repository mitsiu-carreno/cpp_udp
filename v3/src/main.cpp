#include <iostream>
#include <thread>   // std::thread
#include <string>

#include <chrono>

// Socket
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>


void ListenConnections(){
  std::this_thread::sleep_for(std::chrono::seconds(10));
  std::cout << "Wake!\n";
}

bool GetUserInterruption(){
  std::string input;
  while(true){
    std::cout << "Press q to quit: ";
    std::getline(std::cin, input);
    if(input == "q"){
      return true;
    }
  }
}

int main(){

  // Non-blocking socket
  int sock_fd;
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
  sl.l_onoff = 1;
  sl.l_linger = 0;

  if(setsockopt(sock_fd, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl)) < 0){
    perror("Error linger");
    exit(EXIT_FAILURE);  
  }

  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;
  if(setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0){
    perror("Error timeout");
    exit(EXIT_FAILURE);
  }


  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(8080);
  if(bind(sock_fd, reinterpret_cast<const struct sockaddr *>(&server_addr), sizeof(server_addr)) < 0){
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  sockaddr_in client_addr;
  socklen_t client_length = sizeof(client_addr);
  char buffer[1024];

  printf("Litening connections on port 8080\n");
  printf("1");
  memset(&buffer, 0, 1024);
  printf("2");

  /*
  fd_set s;
  FD_ZERO(&s);
  FD_SET(sock_fd, &s);
  printf("3");
  struct timeval timeout;
  timeout.tv_sec = 1;
  timeout.tv_usec = 0;
  printf("pre");
  int retval = select(sock_fd +1, &s, &s, &s, &timeout);
  printf("begin");
  if(retval == -1){
    printf("timeout");
  }
  
  if(retval > 0){
    printf("case 2");
    */
    int bytes_in = recvfrom(sock_fd, buffer, 1024, MSG_WAITALL, reinterpret_cast<struct sockaddr *>(&client_addr), &client_length);
    if(bytes_in == -1){
      printf("Error receiving from client");
    }
  //}



  close(sock_fd);
  shutdown(sock_fd, SHUT_RDWR);

  // End non-blocking socket

  std::thread listenConnections(ListenConnections);  // spawn new thread that calls ListenConnections();
  std::thread getUserInterruption(GetUserInterruption);  // spawn new thread that calls GetUserInterruption()

  std::cout << "main, 1, and 2 now execute concurrently..\n";

  // synchronize threads
  listenConnections.join();   // Pauses until ListenConnections finishes
  getUserInterruption.join(); // Pauses until GetUserInterruption finishes

  std::cout << "1 and 2 completed\n";

  return 0;
}
