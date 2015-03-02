#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define PORT 8888   	//The port on which to listen for incoming data
#define BUFLEN 1024		//Max length of buffer
#define MESSAGESIZE 8	//size of the message
#define	PACKSIZE	15
#define HEADER		2
#define TRAILER		4 
int nextpacketnumber = 1;
void die(char *s)
{
    perror(s);
    exit(1);
}


int convertpacket(char message[], char packet[])
{
	if ((int)packet[0] == '9')
	{
		printf("Recieved last packet\n");
		return -1;
	}
	char buffer[MESSAGESIZE];
	unsigned int i, j;
	j = (int)packet[1] - '0';		// to hold the order of the packet
	for (i = 0; i<MESSAGESIZE; i++)
	{
		buffer[i] = packet[i+HEADER];
	}
	printf("%s\n", buffer);
	if (i = (int)buffer%2 ==(int)(packet[MESSAGESIZE+HEADER]-'0'))
		{
			//printf("Passes CRC check\n");
		}
	else
	{
		printf("CRC Fails\n");
		return j;
	}

	for (i = 0; i<MESSAGESIZE;i++)
	{
		message[i+j*MESSAGESIZE] = buffer[i];
	}

		//printf("%x\n", j);
	if (j > nextpacketnumber)
	{
		printf("Packet dropped, sending resend command\n");
		return j;
	}

	nextpacketnumber++;
	return 0;
}

void main()
{
    struct sockaddr_in si_me, si_other;
     
    int s, slen = sizeof(si_other) , recv_len;
    char buf[PACKSIZE];
    char message[BUFLEN];
	char i; 
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    // zero out the structure
    memset((char *) &si_me, 0, sizeof(si_me));
     
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s , (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
     
    //keep listening for data
	while (1)
	{	
	printf("Waiting for data...\n");    
		while(1)
    	{
			memset(buf,0,PACKSIZE);
	
    	    //fflush(stdout);
    	     
    	    //try to receive some data, this is a blocking call
    	    if ((recv_len = recvfrom(s, buf, PACKSIZE, 0, (struct sockaddr *) &si_other, &slen)) == -1)
    	    {
    	        die("recvfrom()");
    	    }
    	    if(i = convertpacket(message, buf) > 0)
			{	
				buf[MESSAGESIZE] = '1';  	   		
				if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
    	    	{
    	    	    	die("sendto()");
    	    	}
			}
			else if (i == -1)
			{
				buf[0] = 0;    	   		
				if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
    	    	{
    	    	    	die("sendto()");
    	    	}
			}
			else
			{	
    	    	//print details of the client/peer and the data received
    	    	//printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
				//printf("Recieved: Packet Number %x%x\n", (int)buf[0]-'0',(int)buf[1]-'0');
    	    	//printf("Data: %s\n" , buf);
    	     
    	    	//now reply the client with the same data
    	   		//if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
    	    	//{
    	    	 //   	die("sendto()");
    	    	//}
			}
    	}
	}
 
    close(s);
}
