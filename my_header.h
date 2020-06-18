#define BUFFER_SIZE 1024
#define PORT argv[1] //portNumber passed as a CLI
#define MAX_CONNECTIONS 5	
#define EVER ;;

#ifndef INC_HEADER
#define INC_HEADER
#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <ctype.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#endif 

struct sockaddr_in populate_socket(struct sockaddr_in serv_addr, int port) {

	memset(&serv_addr, 0, sizeof(serv_addr)); //initilize structur fields to 0.
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	serv_addr.sin_port = htons(port);

	return serv_addr;
}


//------------------------------------------------------------------------------------------------------------------------
//Linked List

//Structure for the linked List.
typedef struct Ports {
    int portNum;
    struct Ports* next;
} port;


//null == empty list.
port *head = NULL;


//returns first item of the list.
port *get_head() {
    return head;
}

//first port of the list is set
void set_head(port *new_head) {
    head = new_head;
}



//returns last port or null if empty.
port *find_last() {
	if (get_head() == NULL) {
        	return NULL; //return null if list empty.
	}

	port *curr = get_head(), *last = get_head();

	while (curr != NULL) {
		if (curr->next != NULL) {
			last = curr->next;
		}

		curr = curr->next;
	}

    return last;
}

//Adds a port to the end of the list.
void add_port(port *new_port)
{
	//will only add to the list if port is > that 0
	if(new_port->portNum > 0){	
		if (get_head() == NULL) {
			set_head(new_port);
		}
		else {
			find_last()->next = new_port;
		}
	}
}


//returns total port number in the list.
int get_port_total() {
	int total = 0;
	port *curr = get_head();

	if(get_head() == NULL) {
		return total;
	}	
	else {
		while(curr) {
			total++;
			curr = curr->next;
		}
		return total;
	}
}


//populates the Linked 
void populate_list(int value) {
	port *p = (port*)malloc(sizeof(port));
	p->portNum = value;
	add_port(p);
}


//------------------------------------------------------------------------------------------------------------------------

//returns true only if unsigned int.
int isNumber(char number[]) {
    int i = 0;

    //return false if negative number
 	if (number[0] == '-') {
        	return 0;
	}

	for (; number[i] != 0; i++) {
		//if (number[i] > '9' || number[i] < '0')
		if (!isdigit(number[i])) {
	    		return 0; //false
		}
	}
	return 1; //true
}

typedef struct {
	char fileTitle[50];
 	int nsfd; //newsockfd
	int portNum;
}wrapper;



pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *log_thread(void *arg) {

	//Date-time  //<time.h>
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);

	
	wrapper *args = (wrapper*) arg;	
	char fileName[50] = {0};
	strcpy(fileName, args->fileTitle);  //setting filename
	int newsockfd = args->nsfd;
	int portNum = args->portNum;
		
	FILE *log;

	//printf("DAHAL:\n");
	//printf("Name: %s\n", fileName);
	//printf("newsockfd value from THREAD: %d\n", newsockfd );

	pthread_mutex_lock(&mutex);

	log = fopen(fileName, "a"); //if no filename inputted hits.log else custom name. (Creating file)
	if(log == NULL) {
		fprintf(stderr, "Error: Can't open Log!\n");
		//return 8;
	}

	
	#ifdef LITE
	printf("Connection attemp on Honeypot on port %d.\nAttemp registered on: Date-Time: %s\n\n", portNum, asctime(tm));
	#endif
	

	#ifdef FULL
	//Full version
	//Add to file
	fprintf(log, "Connection attemp on Honeypot on port %d.\nAttemp registered on: Date-Time: %s\n\n", portNum, asctime(tm)); 
	#endif
	
	fclose(log);
	pthread_mutex_unlock(&mutex);


	free(args);
	//
	close(newsockfd);
	pthread_exit(NULL);
	

}


