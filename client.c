#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char *argv[]){
	int BUFFSIZE = 500;
	char mess[BUFFSIZE];
	char response[BUFFSIZE];
	int HANDLESIZE = 11;
	char clientName[11];


	int sock, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;

	if (argc < 3) {
		fprintf(stderr,"Please Run %s Followed By The Server IP And Port Number\n", argv[0]); exit(1);
	}

	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	portNumber = atoi(argv[2]);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverHostInfo = gethostbyname(argv[1]);

	if (serverHostInfo == NULL) {
		fprintf(stderr, "No Such Host\n");
		exit(0);
	}

	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0){
		printf("%s \n", "An Error Occured");
		exit(1);
	}
	int yes = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));


	if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
		printf("%s \n", "An Error Occured");
		exit(1);
	}

	printf("%s\n", "Enter Your Name: ");
	fgets(clientName, HANDLESIZE, stdin);
	strtok(clientName, "\n");


	while (1){
		bzero(mess, BUFFSIZE);
		bzero(response, BUFFSIZE);
		printf("Your Message: ");
		fgets(response, BUFFSIZE, stdin);
		strtok(response, "\n");

		if (strcmp(response, "quit") == 0){
			write(sock, clientName, HANDLESIZE);
			write(sock, response, BUFFSIZE);
			printf("Terminating Connection\n");
			break;
		}

		if (strcmp(response, "quit") != 0){
			write(sock, clientName, HANDLESIZE);
			write(sock, response, BUFFSIZE);
			printf("Waiting response from Server...\n");
		}

		read(sock, mess, BUFFSIZE);
		strtok(mess, "\n");
		if (strcmp(mess, "quit") == 0){
			printf("Connection is Terminated By Server\n");
			break;
		}

		printf("%s\n", mess);
	}
	close(sock);
	return 0;
}
