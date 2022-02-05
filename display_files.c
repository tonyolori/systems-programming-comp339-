/*
 This program displays the names of all files in the current directory.
 
  Question
 You are required to write a program that will copy all files under the given folder to another one.
 In order to be able to complete this assignment, you have to research how to obtain the list of files in a directory in Linux. 
 You must copy each file in a separate process.
 Folder name should be obtained from the arguments. You must perform all error checks. Submit a single C file.
 */

#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

char *PATH= "/home/liveuser/Desktop/c/ch2/";

int main(void) {

  DIR *dr,*dw;  // r = dir read, w = dir write
  struct dirent *dir;
  dr = opendir(PATH);

  if (!dr){
    printf("no d?");
    perror("opendir() error");
    }//error checking 

      

  char data[1025]; // 1 b to the end
  int size;
  int fd,fdw;
  char file_path[200];
  

    while ((dir = readdir(dr)) != NULL) {

      // if the file name is a "." or a ".." then skip
      if(dir->d_name[0] == '.')
        continue;

      // execute "for child only" code at each iteration
      if(!fork()){

        //printf("%s\n", dir->d_name);
        strcpy(file_path,PATH);
        strcat(file_path,dir->d_name);
        //printf("\n%s",file_path);

        fd= open(file_path, O_RDONLY);
        //fdw= open()

        printf("%d",fd);
        if(fd == -1){
          perror("file opening error");
        }


        while( size= read(fd,data,1025)){
          write(0,data,size);
          data[size] = 0;// '/0 at the end'
          }


        close(fd);
        //close(fdw);

        return 0;
      }
      wait(NULL);

      
    }

   closedir(dr);
    

   
  return(0);
}
