/*
In this assignment you are required to follow the Unix domain server and client example done in the classroom.
After this point, you need to setup the server in a way that it will process the data sent by the client and send back the processed data.
You must read at least two set of data, and send back at least a piece of information. For instance, client might send two numbers and the server will send the summation back.
This is only an example, please do something a little more complicated.
I do not want to see same operation over and over again in the assignments.
You can use scanf and printf with dup2 to obtain numbers.
Alternatively, you could read the text and use sscanf and sprintf.
You could also perform text based operations. It is up to you.
*/
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>

int main(){
	//**** create a socket
	int sock = socket(AF_UNIX, SOCK_STREAM, 0);

	//****Bind to a file
	struct sockaddr_un addr = {AF_UNIX, "ls.tmp"};

	int err = connect(sock,(struct sockaddr*)&addr, sizeof(addr));

	if(err < 0){
		perror("Connect");
		return -1;
	}


	
	//recive some data
	char sizetoreceive;
	recv(sock,&sizetoreceive,1,0);
	char incoming;
	while(recv(sock,&incoming,1,0)> 0 ){
		write(1,&incoming,1);
		sizetoreceive--;
		if(sizetoreceive == 0)
			break;
	}
	write(1,"\n",1);

	//send some data
	char datatosend[] = "Hello back at you";
	int size = strlen(datatosend);
	int sent = 0;
	while(size > sent){
		sent+= send(sock,datatosend+sent,size-sent,0);
	}


	//close communication socket
	close(sock);

	return 0;
}
