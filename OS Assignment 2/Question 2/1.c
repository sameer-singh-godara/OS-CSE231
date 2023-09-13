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
#include<sched.h>


int main ()
{
    pid_t pid1;
    pid_t pid2;
    pid_t pid3;

    double execution_times[3];

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    pid1 = fork();
    if (pid1==0){
        struct sched_param param;
        param.sched_priority = 0;

        int check_sched_set = sched_setscheduler(0, SCHED_OTHER, &param);
        if (check_sched_set == -1){
            perror("sched_scheduler error in process 1\n");
            exit(1);
        }
        
        execl("./2", "2", NULL);
        
        perror("execl error in proces 1\n");
        exit(1);
    }
    else if (pid1<0){
        perror("Process 1 Failed");
        exit(1);
    }


    pid2 = fork();
    if (pid2==0){
        struct sched_param param;
        param.sched_priority = 1;

        int check_sched_set = sched_setscheduler(0, SCHED_RR, &param);
        if (check_sched_set == -1){
            perror("sched_scheduler error in process 2\n");
            exit(1);
        }
        
        execl("./2", "2", NULL);

        perror("execl error in proces 2\n");
        exit(1);
    }
    else if (pid2<0){
        perror("Process 2 Failed\n");
        exit(1);
    }

    pid3 = fork();
    if (pid3==0){
        struct sched_param param;
        param.sched_priority = 1;

        int check_sched_set = sched_setscheduler(0, SCHED_FIFO, &param);
        if (check_sched_set == -1){
            perror("sched_scheduler error in process 3\n");
            exit(1);
        }
        
        execl("./2", "2", NULL);

        perror("execl error in proces 3\n");
        exit(1);
    }
    else if (pid3<0){
        perror("Process 3 Failed");
        exit(1);
    }

    waitpid(pid1, NULL, 0);
    // printf("pid1\n");
    waitpid(pid2, NULL, 0);
    // printf("pid2\n");
    waitpid(pid3, NULL, 0);
    // printf("pid3\n");

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    

    return 0;
}