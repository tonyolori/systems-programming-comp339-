/*
Using pipes and executing ls, obtain the filenames in a directory and only display the files that start with a character that the user request. 
You must use two sets of pipes.
First one should be tied to ls, second will tie to filtering system, third will get the filtered data and display on the screen.

While second child that filters the data reads data from the pipe, you will have trouble splitting it. To solve it you can dup2 standard input and use scanf to help you.
*/

#include <unistd.h>
#include <wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(){

	int fd[2];
	int fd2[2];
	char data1;
	char data2;
	char c;
	pipe(fd);
	pipe(fd2);

	
	

	write(1,"Enter the letter you would like to filter with\n",47);
	scanf("%c",&c);
	


	if(!fork()){
		//child
		close(fd2[0]);// close reading end for pipe 2
		close(fd2[1]);// close writing end for pipe 2
		


		dup2(fd[1],1);

		execlp("ls","ls",NULL);

		close(fd[1]);// close writing end for pipe 1
		close(fd[0]);// close reading end for pipe 1


		return 0;
	}


	else if(!fork()){
		//child

		close(fd[1]);//close writing end for pipe 1
		close(fd2[0]); //close reading end for pipe 2


		while(read(fd[0], &data1,1) > 0){//read from ls and filter
			if(data1 == c){
				do{

					if(data1=='\n'){
						write(fd2[1],&data1,1);//write newline to pipe 2
						break;
					}

					write(fd2[1],&data1,1);//write data to pipe 2

				}while(read(fd[0], &data1,1));
			}

			else{// read until a newline comes up

				while(read(fd[0], &data1,1) > 0){

					if(data1 =='\n'){
						break;
					}
				}
			}
		}

		close(fd[0]);//close reading end for pipe 1
		close(fd2[1]);// close writing end for pipe 2

	
		
		return 0;
	}



	
	close(fd2[1]);// close writing end for pipe 2
	close(fd[0]);//close reading end for pipe 1
	close(fd[1]);// close writing end for pipe 1

	wait(NULL);
	wait(NULL);
	while(read(fd2[0], &data1,1) > 0){
		write(1,&data1,1);//write data to screen
	}
	
	
	close(fd2[0]);//close reading end for pipe 2
	

	
		
	
	
	
	return 0;
}

