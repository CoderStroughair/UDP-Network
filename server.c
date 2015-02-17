/*
** server.c -- a stream socket server demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490" // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define MAXDATASIZE 1024 // max number of bytes we can get at once


void sigchld_handler(int s)
	{
	while(waitpid(-1, NULL, WNOHANG) > 0);
	}
		
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
	{
	if (sa->sa_family == AF_INET) 
		{
		return &(((struct sockaddr_in*)sa)->sin_addr);
		}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}
		
int main(void)
	{
	int numbytes;
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;	//servinfo points to the HEAD, p cycles through the options
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;

	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;			//holds where the current addrinfo is in the list
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) //loads up a struct with all the info about the host
		{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
		}
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) 
		{
		
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) //opens a socket with addrinfo p
			{
			perror("server: socket");
			continue;
			}
		
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) //set socket options int setsockopt(int socket, int level, int option_name,const void *option_value, socklen_t option_len);
			{
			perror("setsockopt");
			exit(1);
			}
 
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) 
			{
			close(sockfd);
			perror("server: bind");
			continue;
			}
		break;
		}
	if (p == NULL) 
		{
		fprintf(stderr, "server: failed to bind\n");
		return 2;
		}
	freeaddrinfo(servinfo); // all done with this structure
	if (listen(sockfd, BACKLOG) == -1) 
		{
		perror("listen");
		exit(1);
		}
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) 
		{
		perror("sigaction");
		exit(1);
		}
	printf("server: waiting for connections...\n");
	while(1) 
		{ // main accept() loop
		printf("server: waiting for connections...\n");
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) 
			{
			perror("accept");
			continue;
			}
		inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
		printf("server: got connection from %s\n", s);

		if (!fork())
		{
		char number[1024];
		printf("Type in a Message: ");
		scanf("%[^\n]%*c", number);
		if (send(new_fd, number, sizeof(number), 0) == -1)
			perror("send");

		//if ((numbytes = recv(new_fd, buf, MAXDATASIZE-1, 0)) == -1) 
			//perror("recv");

		//buf[numbytes] = '\0';
		//printf("Server: received '%s'\n",buf);
			close(new_fd);
			exit(0);
		}
		close(new_fd); // parent doesn't need this
		}
	return 0;
	}
