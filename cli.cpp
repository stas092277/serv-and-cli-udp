#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

int main(int argc, char **argv)
{

    int  sockfd, n, len;
    struct sockaddr_in servaddr, cliaddr;
    unsigned short port;

    if(argc != 6){
      std::cout << "Not enough parameters, need path and port\n";
      exit(1);
    }

    std::string command = std::string(argv[3]);
    if(command != "rm" && command != "touch"){
      std::cout << "Invalid command, use \'rm\' or \'touch\'\n";
      exit(-1);
    }

    std::string path = argv[4];
    std::string fName = argv[5];

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;

    port = atoi(argv[2]);
    if(port == 0){
      std::cout <<"Invalid port\n";
      exit(-1);
    }
    servaddr.sin_port = htons(port);

    if(inet_aton(argv[1], &servaddr.sin_addr) == 0){
      std::cout << "Invalid IP address\n";
      exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
      std::cout << "Can\'t create socket\n";
      exit(1);
    }

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family      = AF_INET;
    cliaddr.sin_port        = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
      std::cout << "Can\'t bind socket\n";
      close(sockfd);
      exit(1);
    }


    char sendline[1000];
    if(path == "/"){
      strncpy(sendline, (command + ' ' + fName).c_str(), 1000);
    } else {
      strncpy(sendline, (command + ' ' + path + '/'+ fName).c_str(), 1000);
    }

    if(sendto(sockfd, sendline, strlen(sendline)+1, 0, (struct sockaddr *) &servaddr,
    sizeof(servaddr)) < 0){
      std::cout << "Can\'t send request\n";
      close(sockfd);
      exit(1);
    }

    std::cout << " Send command:" << sendline << "\n";
    close(sockfd);
    return 0;
}
