#include <iostream>
#include <string.h>
#include <cstdlib>

using namespace std;
string packet();

struct addrinfo {					//Preps Socket address Structures for future use
 int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
 int ai_family; // AF_INET, AF_INET6, AF_UNSPEC
 int ai_socktype; // SOCK_STREAM, SOCK_DGRAM
 int ai_protocol; // use 0 for "any"
 size_t ai_addrlen; // size of ai_addr in bytes
 struct sockaddr *ai_addr; // struct sockaddr_in or _in6
 char *ai_canonname; // full canonical hostname
 struct addrinfo *ai_next; // linked list, next node
};

struct sockaddr {
 unsigned short sa_family; // address family, AF_xxx
 char sa_data[14]; // 14 bytes of protocol address
}; 

// (IPv4 only--see struct sockaddr_in6 for IPv6)
struct sockaddr_in {
 short int sin_family; // Address family, AF_INET
 unsigned short int sin_port; // Port number
 struct in_addr sin_addr; // Internet address
 unsigned char sin_zero[8]; // Same size as struct sockaddr
};

// (IPv4 only--see struct in6_addr for IPv6)
// Internet address (a structure for historical reasons)
struct in_addr {
 uint32_t s_addr; // that's a 32-bit int (4 bytes)
};

int main(){

	string data = packet();	



	return 0;

}



string packet(){

	string data;
	for (int i = 0; i< 1024; i++){
				
		int a = rand()%74+48;
		char b = a;
		data+= b;
		cout << data << endl;
	}
	return data;
}

void MessageSend(string message)
{
    int sd, ret;
    struct sockaddr_in server;
    struct in_addr ipv4addr;
    struct hostent *hp;

    sd = socket(AF_INET,SOCK_DGRAM,0);
    server.sin_family = AF_INET;

    inet_pton(AF_INET, HOST, &ipv4addr);
    hp = gethostbyaddr(&ipv4addr, sizeof ipv4addr, AF_INET);
    //hp = gethostbyname(HOST);

    bcopy(hp->h_addr, &(server.sin_addr.s_addr), hp->h_length);
    server.sin_port = htons(PORT);

    connect(sd, (const sockaddr *)&server, sizeof(server));
    send(sd, (char *)message.c_str(), strlen((char *)message.c_str()), 0);
}
