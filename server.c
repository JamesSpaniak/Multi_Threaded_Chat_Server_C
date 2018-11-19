#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


/*
Author: James Spaniak

Server side connection demonstrating socket programming in a chat room using multi-threading.
When running; run with additional argument spcifying port for connection.

Enjoy.
*/


//destroy pid
void interrupt();
void interrupt() {
	printf("Server exiting.\n");
	exit(0);
}

int main(int argc, char **argv)
{
    int pid=0;
	int sockfd, newsockfd, portnum;
	int len;
	char buffer[256];
	/*char *username = (char*)malloc(256*sizeof(char));*/
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
  
	/*check args*/
	if (argc < 2) {
		printf("Usage: ./server <port_number>\n");
		exit(0);
	}
  
	portnum = atoi(argv[1]);
	
	if (portnum < 1024 || portnum > 65535) {
		printf("Port must be between 1024 and 65535\n");
		exit(0);
	}

	signal(SIGINT, interrupt);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
	{
		perror("bind");
		exit(0);
	}
	
	listen(sockfd, 5);


	while(1) {
        clilen = sizeof(cli_addr);
		if ((newsockfd=accept(sockfd, (struct sockaddr*)&cli_addr, &clilen)) < 0) {
			perror("accept");
			exit(0);
		}
		
		usleep(2000);
        pid = fork();

        if(pid < 0) {
            perror("Fork failed");
        }
        else if(pid==0) {
            while ((len = read(newsockfd, &buffer, 256)) > 0) {
                buffer[strlen(buffer)] = '\0';
                if (strcmp(buffer, "/") == 0) {
                    printf("Error: command %s not found.\n", buffer);
                    bzero((char*)&buffer, sizeof(buffer));
                }

                else {
                    printf("%s", buffer);
                    bzero((char*)&buffer, sizeof(buffer));
                    printf("%s\n", buffer);
                }
            }
        }
	}
    interrupt();
	return 0;
}