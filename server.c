#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


void doprocessing (int newsockfd) {
  if(fork()!=0)
    return;
   int n;
   char buffer[256];
   bzero(buffer,256);
   //n = read( newsockfd,buffer,255 );//analog send and recv 
   n = recv(newsockfd,buffer,255,0);
   if (n < 0) {
     perror("ERROR reading from socket");
     exit(1);
   }
   printf("Here is the message: %s\n",buffer);
   char msg[] = "Hello\n";
   printf("Got a connection; writing 'hello' then closing.\n");
   send(newsockfd, msg, sizeof(msg), 0);
   close(newsockfd);
   exit(0);
}

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;//ipv4,sockaddr_in6 for ipv6, unix socket = sockaddr_un, socakaddr_storage for any of it, sockaddr* 
   int  n, pid;
   
   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0); //без блока O_NONBLOCK, EWOULDBLOCK 
   
   if (sockfd < 0) {
      perror("ERROR opening socket");
      exit(1);
   }
   
   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5001;   
   serv_addr.sin_family = AF_INET; // 
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);
   
   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) { //
      perror("ERROR on binding");
      exit(1);
   }
      
   /* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   */
   
   listen(sockfd,5);// 5 - number of expected connection
   clilen = sizeof(cli_addr);
   

for (;;) {
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd == -1) {
      if (errno == EWOULDBLOCK) {
	 printf("No pending connections; sleeping for one second.\n");
        sleep(1);
      } else {
        perror("error when accepting connection");
        exit(1);
      }
    } else {
      doprocessing(newsockfd);
    }
  }
      
   return 0;
}
