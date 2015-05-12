#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

/************* UDP SERVER CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

/************ ASCII Conversion Table ************/
char ASCII [16][6] = {			
		{' ','!','"','#','$','%','&',''','(',')','*','+',',','-','.','/'}, //[x][0] - 2.x
		{'0','1','2','3','4','5','6','7','8','9',':',';','<','+','>','?'}, //[x][1] - 3.x
		{'@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O'}, //[x][2] - 4.x
		{'P','Q','R','S','T','U','V','W','X','Y','Z','[','\',']','^','_'}, //[x][3] - 5.x
		{'`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o'}, //[x][4] - 6.x
		{'p','q','r','s','t','u','v','w','x','y','z','{','|','}','~','¬'}  //[x][5] - 7.x
					 };

int main(){
  int udpSocket, nBytes;
  char buffer[1024];
  struct sockaddr_in serverAddr, clientAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size, client_addr_size;
  int i;

  /*Create UDP socket*/
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*Bind socket with address struct*/
  bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverStorage;

  while(1){
    /* Try to receive any incoming UDP datagram. Address and port of 
      requesting client will be stored on serverStorage variable */
    nBytes = recvfrom(udpSocket,buffer,1024,0,(struct sockaddr *)&serverStorage, &addr_size);

    /*Convert message received to uppercase*/
    for(i=0;i<nBytes-1;i++)
      buffer[i] = toupper(buffer[i]);

    /*Send uppercase message back to client, using serverStorage as the address*/
    sendto(udpSocket,buffer,nBytes,0,(struct sockaddr *)&serverStorage,addr_size);
  }

  return 0;
}
