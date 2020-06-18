#include <stdio.h>
#include <string.h>
#include <stdlib.h>  

//sockets
#include <sys/types.h>
#include <sys/socket.h>

#include <netdb.h>
#include <unistd.h>

#include <ctype.h>

#include <sys/stat.h>
#include <errno.h>

#include "my_header.h"

int main(int argc, char *argv[]) {
	

	int sockfd = 0, range_start = 0, range_end = 0, range_size = 0, port_counter = 0;
	struct sockaddr_in serv_addr;
	FILE *log;

	if (argc <= 3) {
		fprintf(stderr,"Error - Format must be: <port range start> <port range end> <filename> (Without any extention)\n");
		return 1;
	}
	else if (argc > 4) {
		fprintf(stderr,"Error: Too many arguments entered.\n");
		return 2;
	}

	
	

	//if port range is passed, iterate the range.	
	else if(argc == 4){	
		range_start = atoi(PORT);
		range_end = atoi(argv[2]);
		range_size = range_end - range_start;


		if(isNumber(argv[1]) == 0 || isNumber(argv[2]) == 0) {
			fprintf(stderr,"Error - Arguments 1 and 2 must be port numbers and greater than 0.\n");	
			return 3;
		}
		else if(range_start == 0 || range_end == 0) {
			fprintf(stderr,"Error - Port numbers must be greater than 0.\n");	
			return 4;
		}

		

		#ifdef DEBUG
		printf("start: %d\n end: %d\n size: %d\n", range_start, range_end, range_size);
		#endif
		port_counter = range_start;

		
		log = fopen(strcat(argv[3], ".csv"), "a");
		if(log == NULL) {
			fprintf(stderr, "Error: Can't open Log!\n");
			return 5;
		}

		//Checks if log is empty.
		struct stat stat_record;
		if(stat(argv[3], &stat_record))
		 	printf("%s", strerror(errno));
			
		 else if(stat_record.st_size < 1){ //kienet <= 1 imma ma bdejtc naraha tamel sens.
		 	//if file is empty, enter port and status on top of csv
			fprintf(log, "Port\tConnection Status\n");



			while(port_counter != (range_start + range_size+1)) {


				
				//Create socket()
				sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				if (sockfd < 0)
				{
					fprintf(stderr, "socket() failed\n");
					return 6;
				}

				//populate_socket 
				serv_addr = populate_socket(serv_addr, port_counter);

				
				//connect()
				if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
				{
					//printf(stderr, "connect() failed\n");
					fprintf(stderr, "Port %d : Connect() failed\n", port_counter);
					fprintf(log, "%d\tfailed\n",port_counter);
					
				}

				else{
					//inzidu gol linked list.
					populate_list(port_counter);
					printf("Port %d : Connected Successfully!\n", port_counter);
					fprintf(log, "%d\tSuccess\n",port_counter);
				}
				
				port_counter++;
			}
			
			//prints total connected ports.
			printf("\nTotal ports connected Successfully: %d\n", get_port_total());



		}
		 else {
			//if not empty
		 	while(port_counter != (range_start + range_size+1)) {


				
				//Create socket()
				sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

				if (sockfd < 0)
				{
					fprintf(stderr, "socket() failed\n");
					return 6;
				}

				//populate_socket 
				serv_addr = populate_socket(serv_addr, port_counter);

				
				//connect()
				if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
				{
					//printf(stderr, "connect() failed\n");
					fprintf(stderr, "Port %d : Connect() failed\n", port_counter);
					fprintf(log, "%d\tfailed\n",port_counter);
					
				}

				else{
					//inzidu gol linked list.
					populate_list(port_counter);
					printf("Port %d : Connected Successfully!\n", port_counter);
					fprintf(log, "%d\tSuccess\n",port_counter);
				}
				
				port_counter++;
			}
			
			//prints total connected ports.
			printf("\nTotal ports connected Successfully: %d\n", get_port_total());
		}







		


	}
	

	fclose(log);
	close(sockfd);
	return 0;
}