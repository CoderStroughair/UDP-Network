#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"

#include<stdio.h> //printf
#include<string.h> //memset
#include<stdlib.h> //exit(0);
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define SERVER "127.0.0.1"
#define BUFLEN 1024		//Max length of buffer
#define PORT 8888		//The port on which to send data
#define MESSAGESIZE 8	//size of the message
#define	PACKSIZE	15
#define HEADER		2
#define TRAILER		4
#define droprate	0.2
#define corruptrate 0.2


time_t t;
int num_packets = BUFLEN/MESSAGESIZE;

void die(char *s)
{
    perror(s);
    exit(1);
}
 

void packetmaker(char packet[], char data[], int number)
{	
	//printf("packet No: %x\n", number+1);
	//printf("Num_packets: %x\n", num_packets);
	int i, num_2;
	packet[0] = '0';
	while (number > 15)
	{
		packet[0] += 1;
		number = number - 16;
	}
	if (packet[0] == 'a')
			packet[0] = 16 + '0';
	packet[1] = number + '0';
	for (i = HEADER; i < MESSAGESIZE+HEADER; i++)
		packet[i] = data[i-HEADER];
	if ((int)data%2 ==1)
		packet[MESSAGESIZE+HEADER] = '1';
	else 
		packet[MESSAGESIZE+HEADER] = '0';
	packet[MESSAGESIZE+HEADER+1] = '0';
}


int corrupter (char packet[])
{
	int random = (rand()%100);
	int i;
	if (random < 60)
		return 1;
	if (random > 80)
		{
			printf("CORRUPTED PACKET\n");
			packet[HEADER+4]+= 1;
		}
	else
		printf("PACKET LOST\n");
		return 0;
}


int main(void)
{
	srand((unsigned) time(0));
	char end_mess[BUFLEN] = "End Message";
	int curr_packet = 0;
	char packet[PACKSIZE];
	memset(packet,0, PACKSIZE);
	FILE *fp;
	int forloop;
	int size = MESSAGESIZE;		//amount of data in the packet
	struct sockaddr_in si_other;
    int s, i = 0, slen=sizeof(si_other);
    char buf[BUFLEN];				//recieving message
    char message[BUFLEN];			//holds all data
	fp = fopen("Ascii.txt", "r");
	fscanf(fp,"%s", message);		//reading in the data
	char datagram[PACKSIZE];
	memset(datagram,0, PACKSIZE);
    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
 
    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
     
    if (inet_aton(SERVER , &si_other.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

	if (!fork()){
		while (curr_packet < num_packets)		//splits into sender and resend reciever
		{
			for (forloop = 0;forloop<MESSAGESIZE;forloop++)	//adding data
				{
					packet[forloop] = message[forloop+curr_packet*8];
				}
			packetmaker(datagram, packet, curr_packet);
			//printf("%s\n", datagram);
			if (corrupter(datagram))		//if true, it sends the packet
			{
				if (sendto(s, datagram, strlen(datagram) , 0 , (struct sockaddr *) &si_other, slen)==-1)
		        {
    		        die("sendto()");
    		    }
			}
			curr_packet++;			
		}
		memset(datagram,0, PACKSIZE);
		datagram[0] = '9';
		//printf("%s\n", datagram);
		if (sendto(s, datagram, strlen(datagram) , 0 , (struct sockaddr *) &si_other, slen)==-1)
		        {
    		        die("sendto()");
    		    }
		printf("Sent Final Packet. Awaiting any further resend requests\n");
	}
	//add stuff in to deal when we get resend requests
	else
	{
		while (buf[0] !='9')
		{	
			if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
      		{
      			die("recvfrom()");
      		}
			printf("%s\n", buf);
			if (buf[MESSAGESIZE] == '1')
			{
				int messagenum = (buf[0]-'0')+(buf[1] - '0');
				printf("%x\n", messagenum);
			}
			//printf("%s\n", buf);
		}
		printf("Last Packet Confirmed. Exiting...\n");	
	} 
	
	close(s);
    return 0;
}


/*int packetmaker(char input[], int number)
{
	int compare = 8;	//2_1000 in decimal
	//printf("%x\n", compare);
	unsigned int packet = number; 	// packet now holds the header
	int data;		//converts the data into an integer.
	printf("Data as a string: %s\n", input);	
	int i,j;
	int stuffer = 2;			//bit stuffing only occurs in the data, not the header
	int count;
	for (i = 0; i<4;i++)				
	{	
		int data = input[i];
		for (j = 0; j < 4;j++)
		{
			packet = packet<<1;	//increments the packet to add new data
			if (compare && data ==1)
			packet++;;			//1 is added to the packet if there is a 1 in the place on the data	

			if (packet%2 != stuffer)//ie, this bit is the same as the previous 
				count++;
			else if (packet%2 ==1 )	//ie, if the last bit is a 1 and not the same as the previous
			{
				count = 0;
				stuffer = 0;
			}
			else					//ie, if the last bit is a 0 and not the same as the previous
			{
				count = 0;
				stuffer = 1;
			}
			if (count ==4)		//ie, the addition of this bit means there are 4 of the same digit in a row
			{
				packet = packet << 1;
				packet += stuffer;
				count = 0;
			}
			data = data << 1;
		}					

	}
	return packet;

}
		printf("%lu\n", packet);
		packet = packet<<1;		//bitshifted to accomodate new data;		
		if (compare && data ==1)
			packet++;;			//1 is added to the packet if there is a 1 in the place on the data	

		if (packet%2 != stuffer)//ie, this bit is the same as the previous 
			count++;
		else if (packet%2 ==1 )	//ie, if the last bit is a 1 and not the same as the previous
		{
			count = 0;
			stuffer = 0;
		}
		else					//ie, if the last bit is a 0 and not the same as the previous
		{
			count = 0;
			stuffer = 1;
		}
		if (count ==4)		//ie, the addition of this bit means there are 4 of the same digit in a row
		{
			packet = packet << 1;
			packet += stuffer;
			count = 0;
		}
*/

	
	//add in stuff to make the trailer and crc checker






/*datagram = (int)packet;
		printf("%x\n",datagram);
		    while(1)
  	  {	
			for (forloop = 0;forloop<PACKSIZE;forloop++)
			{
				packet[forloop] = message[i];
				i++;
			}
			printf("Sending '%s'\n", packet);
     	   //send the message
      	  if (sendto(s, packet, strlen(packet) , 0 , (struct sockaddr *) &si_other, slen)==-1)
      	  {
      	      die("sendto()");
      	  }
         
      	  //receive a reply and print it
      	  //clear the buffer by filling null, it might have previously received data
      	  memset(buf,0, BUFLEN);
      	  //try to receive some data, this is a blocking call
      	  if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen) == -1)
      	  {
      	      die("recvfrom()");
      	  }
      	   
      	  //puts(buf);
			printf("Confirmation from Server: %s\n" , buf);

  	  }	
 	*/
