#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

//sockets
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
//#include <arpa/inet.h>  //nuzaha ghal inet_addr (same as netdb, different Lib)

#include <pthread.h> //for multi threading

#include <sys/stat.h>
#include <errno.h>

#include "my_header.h"




int main(int argc, char *argv[]) {
	
	int sockfd = 0, newsockfd = 0, isDefaultFileName = 1;
	pthread_t trd;

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
	
	else if (argc > 2) {
		fprintf(stderr,"Error: To many arguments entered.\n");
		return -2;
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
				isDefaultFileName = 1;
				//set defualt file name to hits.log.	

				#ifdef DEBUG
				assert(isDefaultFileName == 1);
				#endif
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
		#ifdef DEBUG
		assert(isDefaultFileName == 0);	
		#endif
	}
	
	else if (argc > 3) {
		fprintf(stderr,"Error: To many arguments entered.\n");
		return -2;
	}
	
	#endif //end FULL
		
	//creating socket
	sockfd =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//-1 returns an error.
	if (sockfd == -1) {
		fprintf(stderr, "ERROR: Failed to open socket\n");
		close(sockfd);
		return 4;
	}
	


	serv_addr = populate_socket(serv_addr, atoi(PORT)); //populating socket's ip and port.

	
	//bind()
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		fprintf(stderr, "ERROR: bind() method failed\n");
		fprintf(stderr, "Error code: %s\n", strerror(errno));
		close(sockfd);
		return 5;
	}


	//listen()
	if (listen(sockfd, MAX_CONNECTIONS) == -1) {
		fprintf(stderr, "ERROR: listen() method failed\n");
		fprintf(stderr, "Error code: %s\n", strerror(errno));
		close(sockfd);
		return 6;
	}
	else {
		printf("Honeypot Running...\nListening for connections...\n");
	}

	

	for(EVER) {

		//Accept()
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		if (newsockfd == -1 ) {
			fprintf(stderr, "ERROR: accept() method failed\n");
			close(sockfd);
			return 7;
		}
		else {
			printf("\nAccepting new connections...\n"); 

			if(isDefaultFileName) {
				//storing args in a structure to pass multiple args to the log_thread
				wrapper *default_file_name = malloc(sizeof(*default_file_name));
				strcpy(default_file_name->fileTitle, "hits.logs");
				default_file_name->nsfd = newsockfd;
				default_file_name->portNum = atoi(argv[1]);

				#ifdef DEBUG
				//printf("\nname : %s\n", default_file_name->fileTitle);
				#endif

				if (pthread_create(&trd, NULL, log_thread, default_file_name) != 0)
				{
					printf("ERROR: pthread_create() method failed (default file)\n");
					//close(sockfd);
					return 8;
				}
			

			}
			else {

				wrapper *custom_file_name = malloc(sizeof(*custom_file_name));
				strcpy(custom_file_name->fileTitle, argv[2]);
				custom_file_name->nsfd = newsockfd;
				custom_file_name->portNum = atoi(argv[1]);
				
				#ifdef DEBUG
				//printf("\nname : %s\n", custom_file_name->fileTitle);
				#endif
				
				if (pthread_create(&trd, NULL, log_thread, custom_file_name) != 0)
				{
					//fprintf(stderr, "ERROR: pthread_create() failed\n");
					printf("ERROR: pthread_create() method failed (custom file)\n");
					//close(sockfd);
					return 8;
				}
	
			}
		
		
				pthread_join(trd, NULL);

			
		}

	
	}

	return 0;
}