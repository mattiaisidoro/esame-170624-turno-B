/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

unsigned char  encryptor(unsigned char x, unsigned char k){
//printf("encripto %x e %x:\n" , x , k); 
unsigned char e = x ^ k;
//printf("%x\n", e);
return e;
}



void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 1025;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
	while(1){//faccio si che si fermi solo con cntr C 

   newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
 	//ora ho in buffer il messaggio letto da client
	unsigned char primo = buffer[0] ; //sempre uguale a p
	unsigned char len = buffer[1];
	//ho quindi create la mia stringa nella quale mettero  byte da decodificaree
	unsigned char *str = calloc(len , sizeof(unsigned char));
	for(int i = 0 ; i < len ; i++){
	str[i]=buffer[i+2];
//	printf("Debug char : %c\n" , str[i]);
	}
//	printf("Debug %c e len : %u\n" , primo , len );
	// ho quindi char di byte in str che devo modificare
unsigned char *res = calloc(len+2 , sizeof(unsigned char));
// res sara char di byte modificati che trasmettero a  client
res[0]='E';
res[1]=len;
//iniziallizzo vettore k per encriptare
unsigned char *k= calloc(4, sizeof(unsigned char));
k[0]=0xDE;
k[1]=0xAD;
k[2]=0xBE;
k[3]=0xEF;

	for(int j =0 ; j < len ; j++)
	{
	unsigned char ris = encryptor(str[j], k[j%4]);
	res[j+2]=ris;
//	printf("%x\n", ris);
	}
n=write(newsockfd , res , len+2);



close(newsockfd);

}
return 0;

}
