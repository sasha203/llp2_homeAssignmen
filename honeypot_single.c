#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

//sockets
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
//#include <arpa/inet.h>  //nuzaha ghal inet_addr (same as netdb, different Lib)

#include <sys/stat.h>
#include <errno.h>


#include "my_header.h"

int main(int argc, char *argv[]) {
	
	int sockfd = 0, newsockfd = 0, attempts = 0, isDefaultFileName = 1;
	FILE *log;

	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	#ifdef LITE
	if (argc < 2) {
		fprintf(stderr,"Error: No port number entered, Please enter a port number.\n");
		return 1;
	}
	else if (argc == 2) {
		if(isNumber(argv[1]) == 0) {
			fprintf(stderr,"Error: Port number must be a number and greater than 0.\n");
			return 2;
		}
	}
	
	#endif //end LITE

	#ifdef FULL 

	if (argc < 2) {
		//fprintf(stderr,"Error: No port number entered, Please enter a port number.\n");
		fprintf(stderr,"Error - Format should be: <port> <filename> (Without any extention)\n");
		return 1;
	}
	else if(argc == 2) {
		//if 1 cla is number(port)
		if(isNumber(argv[1]) == 1) {
			//if not file name is inputted.
			if(argv[2] == NULL) {
				fprintf(stderr,"Warning: missing <filename> command line argument, defaulting to hits.log\n");
				
				//set defualt file name to hits.log.
				log = fopen("hits.log", "a");
			}
				
		}
		else {
			//if it's a file name.
			fprintf(stderr,"Error - Format should be: <port> <filename> (Without any extention)\n");
			return 2;
		}
	}

	else if (argc == 3) {
		//if 1st arg(port) is not a number
		if(isNumber(argv[1]) == 0) {
			fprintf(stderr,"Error - Arguments 1 must be a port number and greater than 0.\n");	
			return 3;
		}

		isDefaultFileName = 0; //is not the default fileName·	
	}
		
	#endif //end FULL
		
	//creating socket
	sockfd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//-1 returns an error.
	if (sockfd == -1) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		return 4;
	}
	


	serv_addr = populate_socket(serv_addr, atoi(PORT)); //populating socket's ip and port.

	
	//bind()
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		fprintf(stderr, "ERROR: bind() method failed\n");
		fprintf(stderr, "Error code: %s\n", strerror(errno));
		return 5;
	}


	//listen()
	if (listen(sockfd, MAX_CONNECTIONS) == -1) {
		fprintf(stderr, "ERROR: listen() method failed\n");
		fprintf(stderr, "Error code: %s\n", strerror(errno));
		return 6;
	}
	else {
		printf("Honeypot Running...\nListening for connections...\n");
	}

	
	

	for(EVER) {

		//date time
		time_t t = time(NULL);
    		struct tm *tm = localtime(&t);


		//Accept()
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd == -1 ) {
			fprintf(stderr, "ERROR: accept() method failed\n");
			return 7; ////////
		}
		else {

		
			if(isDefaultFileName) {
				log = fopen("hits.log", "a"); //if no filename inputted.
			}
			else {
				log = fopen(argv[2], "a");
			}
		

			if(log == NULL) {
				fprintf(stderr, "Error: Can't open Log!\n");
				return 8;
			}


			attempts++; //will increament with each connection to the honeypot.

			#ifdef LITE
			printf("Attempt number %d to connect to the Honeypot on Port %s.\nAttemp registered on: Date-Time: %s\n\n", attempts, PORT, asctime(tm));
			#endif

			#ifdef FULL
			//Full version
			//Add to file
			fprintf(log, "Attempt number %d to connect to the Honeypot on Port %s.\nAttemp registered on: Date-Time: %s\n\n", attempts, PORT, asctime(tm));
			#endif
		}


		//memset(buffer, 0, BUFFER_SIZE);
		
		

		fclose(log);
		close(newsockfd);
	
	}


		
		
	

	return 0;
}