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

void factorial(int num, int rc){
    int a = 1;
    for (int i = 1; i<=num; i++){
        a = a * i; 
    }
    if (rc == 0){
        printf("Factorial Function is called by Child and Factorial of 4 is: %d\n", a);
    }
    else if (rc > 0) {
        printf("Factorial Function is called by Parent and Factorial of 4 is: %d\n", a);
    }
}

void fibonacci(int num, int rc){
    if (rc == 0){
        int a = 0;
        int b = 1;
        int c;
        printf("Fibonacci Series Function is called by Child and the series till %dth term is: %d %d", num, a, b);
        for (int i = 3; i<=num; i++){
            c = a;
            a = b;
            b = b+c;
            printf("%d ", b);
        }
        printf("\n");
    }
    if (rc > 0){
        int a = 0;
        int b = 1;
        int c;
        printf("Fibonacci Series Function is called by Parent and the series till %dth term is: %d %d", num, a, b);
        for (int i = 3; i<=num; i++){
            c = a;
            a = b;
            b = b+c;
            printf("%d ", b);
        }
        printf("\n");
    }
}

int main ()
{
    int rc = fork();
    if (rc==0){
        wait(NULL);
    }
    if (rc<0){
        printf("Fork Failed\n");
    }
    else if (rc == 0){
        factorial(4, rc);
    }
    else {
        fibonacci(16, rc);
    }

    return 0;
}