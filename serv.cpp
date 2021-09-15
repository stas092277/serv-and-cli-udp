#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <string>


int main(int argc, char **argv)
{
    int port;
    int  sockfd, clilen, n;
    char line[1000];
    std::string path;
    struct sockaddr_in servaddr, cliaddr;

    if(argc != 3){
       std::cout << "Not enough parameters, need path and port\n";
       exit(1);
    }

    port = atoi(argv[2]);
    if(port == 0){
       std::cout << "Invalid port\n";
       exit(1);
    }

    path = std::string(argv[1]);
    if (chdir(path.c_str()) == -1) {
       std::cout << "Invalid path\n";
       exit(1);
    }
    system("pwd");


    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_port        = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
       std::cout << "Can\'t create socket\n";
       exit(1);
    }

    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
       std::cout << "Can\'t bind socket\n";
       close(sockfd);
       exit(1);
    }


    while(1) {

       clilen = sizeof(cliaddr);

       if( ( n = recvfrom(sockfd, line, 999, 0, (struct sockaddr *) &cliaddr,
       (socklen_t *) &clilen)) < 0){
          std::cout << "Can\'t receive request\n";
          close(sockfd);
          exit(1);
       }

       std::cout << "Get command: " << line <<  " from: " << inet_ntoa(cliaddr.sin_addr)<<"\n";

       system(line);
    }
    return 0;
}
