#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

/************* UDP CLIENT CODE *******************/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

/************ ASCII Conversion Table ************/
char ASCII [16][6] = {			
		{' ','!','"','#','$','%','&',''','(',')','*','+',',','-','.','/'}, //[x][0] - 2.x
		{'0','1','2','3','4','5','6','7','8','9',':',';','<','+','>','?'}, //[x][1] - 3.x
		{'@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O'}, //[x][2] - 4.x
		{'P','Q','R','S','T','U','V','W','X','Y','Z','[','\',']','^','_'}, //[x][3] - 5.x
		{'`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o'}, //[x][4] - 6.x
		{'p','q','r','s','t','u','v','w','x','y','z','{','|','}','~','¬'}  //[x][5] - 7.x
					 };
//Since the max value for an ASCII number is 111 1111, we will be taking each character to be 7 bits in length
//This means that the data length at minimum will be 7 bits/character x 8 characters = 56 bits.
//adding in bitstuffing, at max 56/5 = 11 bitstuffing bits, giving max length to be 67 bits.
//If we take our CRC value to be 101010, the checksum will be 3 bits long, giving total length as 72 bits->9 bytes

int main(){
  int clientSocket, portNum, nBytes;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  int clientSocket, portNum, nBytes;
  char buffer[1024];
  struct sockaddr_in serverAddr;
  socklen_t addr_size;
  

  /*Create UDP socket*/
  clientSocket = socket(PF_INET, SOCK_DGRAM, 0);		//setup the socket with an address structure and frame type

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;									//set the type of address
  serverAddr.sin_port = htons(7891);								//set the port number you'll be using
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");				//set the address you're sending to
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);	//fill 

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverAddr;

  while(1){
    printf("Type a sentence to send to server:\n");
    fgets(buffer,1024,stdin);
    printf("You typed: %s",buffer);

    nBytes = strlen(buffer) + 1;
    
    /*Send message to server*/
    sendto(clientSocket,buffer,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);

    /*Receive message from server*/
                nBytes = recvfrom(clientSocket,buffer,1024,0,NULL, NULL);

    printf("Received from server: %s\n",buffer);

  }

  return 0;
}
