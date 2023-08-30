#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<time.h>


int main ()
{
    int rc = fork();
    if (rc == 0){
        wait(NULL);
    }
    if (rc == 0){
        printf("C) Child is having ID: %d\n", getpid());
        printf("D) My parent ID is: %d\n", getppid());
    }
    else if (rc<0){
        printf("Fork Failed\n");
    }
    else{
        printf("A) Parent (P) is having ID: %d\n", getpid());
        wait(NULL);
        printf("B) ID of P\'s Child is: %d\n", rc);
    }
    
    return 0;
}