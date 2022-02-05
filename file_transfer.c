/*
Implement a file transfer system using UDP.
For this task first you should implement char peer system done in the class. 
Modify this program so that a file can be sent. Ask a port number and filename. 
Then send the file in 100 byte packages to the other side. Every package should have starting point of the data. 
When you get data, you should seek to the location that is specified at the start of the data. 
You can assume that you are working on the same system. 
Ignore any missing data packages. 
Save to fixed filename or if you want transfer filename first.

You can use lseek as follows:

lseek(fd, start, SEEK_SET);


*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int pid;
int sock;

void interrupt(int sig){
	close(sock);

	if(pid){
		kill(pid,SIGINT);//parent will kill child
		printf("exiting\n",SIGINT);
	}
	
	exit(0);
}

int main(int argc, char *argv[]){

	if (argc != 2){
		printf("usage: %s portnumber \n", argv[0]);
		return 1;
	}

	signal(SIGINT,&interrupt);
	signal(SIGTERM,&interrupt);


	//**** create a socket
 	sock = socket(AF_INET, SOCK_DGRAM, 0);

	//****Bind to a file
	struct sockaddr_in addr ={
		AF_INET,
		htons(atoi(argv[1])),//port number
		INADDR_ANY
	};

	int res = bind(sock, (struct sockaddr*)&addr,
					sizeof(addr));

	if(res<0){
		perror("bind");
		return -1;
	}

	pid = fork();

	if(pid){
			char buf[100];
			int chars = 0;
			char c;
			int portnumber;
			char file[30];
			int choice;
			int start;

			scanf("%d",&portnumber);

			while(1){
				printf("will you send or receive?\n enter 1 to send, 2 to receive \n");
				scanf("%d",&choice);

				if(choice!= 2 && choice!=1)
					printf("wrong choice, try again\n");
				else 
					break;
			}



			if(choice== 1){
					printf("enter file name to send over: ");
			
					scanf("%s",file);
		
					int fd=open(file,O_RDONLY);
		
					if(fd<0){
						printf("file does not exist\n");
						raise(SIGTERM);
					}

					printf("Select seek offset: ");
					scanf("%d",&start);

					
			
		
					struct sockaddr_in target ={
					AF_INET,
					htons(portnumber),//port number
					inet_addr("127.0.01")
					};
		
					while((chars =  read(fd,&buf,100) ) > 0){
							sendto(sock,buf,chars,0,(struct sockaddr*)&target,
								sizeof(target));

						lseek(fd, start, SEEK_CUR);// every sent packaage is offset
		
					}
					close(fd);
				}
			wait(NULL);


	}
	else{

		struct sockaddr_in incoming;
		int lenincoming=sizeof(incoming);

		char buf[101];
		int chars = 0;

		int fdw = creat("newfile.txt",0640);

		while( (chars = recvfrom(sock,&buf, 100, 0,
			(struct sockaddr*)&incoming,&lenincoming)) >= 0){

			write(1,buf,chars);

			write(fdw,&buf,lenincoming);


			


		}

	}
	
	return 0;
}
