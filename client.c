#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


/*
Author: James Spaniak


Client side program to work with server for a multi-person chat room using socket programming.
Available commands:
/me
/nick
/exit
*/

//main function, takes in localhost and port as additional arguments to be used on connection.
int main(int argc, char **argv) {
    //initialize needed info
	int sockfd, portnum;
	struct sockaddr_in serv_addr;
	struct hostent *host_name;
	char msg[256];
	char *username = (char*)malloc(256*sizeof(char));
	char *message = (char*)malloc(256*sizeof(char));

    //initial running of program, get hostname and port
	if (argc<3) {
		printf("Usage: ./client <host_name> <port_number>\n");
		exit(0);
	}
	if ((host_name = gethostbyname(argv[1]))==NULL) {
		printf("Invalid hostname\n");
		exit(0);
	}
	portnum = atoi(argv[2]);

	if (portnum<1024 || portnum>65535) {
		printf("Port must be between 1024 and 65535\n");
		exit(0);
	}

    //create socket and set up needed values
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((char*)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portnum);
	bcopy((char*)host_name->h_addr_list[0], (char*)&serv_addr.sin_addr.s_addr, host_name->h_length);
    //connect to socket
	if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0) {
		perror("connect");
		exit(0);
	}
    //get first input
	printf("Connected to server... sending message now.\n");
	printf("Enter username: ");
	fgets(msg, sizeof(msg), stdin);
	printf("\n");

    //exit program with /exit
	if (strncmp("/exit", msg, 5) == 0) {
		printf("Exiting sockettalk client\n");
		exit(0);
	}

    //set username for rest of connection (can be changed with /nick as well)
	else {
		strncpy(username, msg, strlen(msg) - 1);
		printf("Setting nickname to: %s\n", username);
        msg[strlen(msg) - 1] = 0;
        strcat(msg, " has joined the server.");
		write(sockfd, msg, strlen(msg + 1));
	}

	while (1) {
        //depending on beginning of input send message to server or execute specific command
		printf("%s: ", username);
		fgets(msg, sizeof(msg), stdin);

        //exit
		if (strncmp("/exit", msg, 5) == 0)
			exit(0);

        // /me "laughing" ouputs: <username> laughing
		else if (strncmp(msg, "/me", 3) == 0) {
            strcpy(message, username);
			strcat(message, &msg[3]);
			write(sockfd, message, strlen(message + 1));
		}
        //changes user name
		else if (strncmp(msg, "/nick", 5) == 0) {
            message = "Changing nickname of ";
			strcat(message, username);
			printf("%s", message);
			strncpy(username, &msg[6], strlen(&msg[6]) - 1);
			printf(" to %s", username);
			strcat(message, " to ");
			strcat(message, username);
			write(sockfd, message, strlen(message));
		}
        //send message to server with no command
		else {
            strcpy(message, username);
			strcat(message, ": ");
			strcat(message, msg);
			write(sockfd, message, strlen(message + 1));
		}
	}

	write(sockfd, "\0", 1);
    //close connection and return
	close(sockfd);
	sleep(5000);
	return 0;
}