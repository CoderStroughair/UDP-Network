#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

/************* UDP CLIENT CODE *******************/
//Since the max value for an ASCII number is 111 1111, we will be taking each character to be 7 bits in length
//This means that the data length at minimum will be 7 bits/character x 8 characters = 56 bits.
//adding in bitstuffing, at max 56/5 = 11 bitstuffing bits, giving max length to be 67 bits.
//If we take our CRC value to be 101010, the checksum will be 3 bits long, giving total length as 72 bits->9 bytes
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;					//used for the Continuous RQ - Allows a Buffer to be implemented.
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;



/************ Variables for Sockets Initialised Outside the Main ************/
int clientSocket, portNum, nBytes;
char buffer[1024];
struct sockaddr_in serverAddr;
socklen_t addr_size;
  
void main(){

  /*Create UDP socket*/
  clientSocket = socket(PF_INET, SOCK_DGRAM, 0);					//setup the socket with an address structure and frame type

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;									//set the type of address
  serverAddr.sin_port = htons(7891);								//set the port number you'll be using
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");				//set the address you're sending to
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);	//Just something that needs to be done

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
}

int ChartoInt(char data[])				//take the characters in, and convert to binary. Taking in 8 bits at a time, so length of data is always 8.
{			
	char current;
	int result = 0;
	int i;								//initialise the variables: current holding the character we're currently on, and result holding the ongoing hexadecimal result
	for (i = 0; i < 8; i++)	
	{	
		current = data[i];				//give current the value of the next character
		result = result << 7;			//bitshift the value of result 7 places so that it can hold the next value
		result = result+ (int)current;	//add the integer value of current to result
	}
	return result;						//return the final value of the character stream.	
}











