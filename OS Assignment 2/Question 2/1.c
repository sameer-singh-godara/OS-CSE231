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

    double execution_time1, execution_time2, execution_time3;

    struct timespec start_time1, start_time2, start_time3, end_time1, end_time2, end_time3; // defining the datatype of start and end time of each processes
    pid1 = fork();// fork for process 1
    if (pid1==0){
        pid_t rc = fork(); // calling internal fork for caclulating time for completing the process easily
        if (!rc){
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
        clock_gettime(CLOCK_REALTIME, &start_time1);
        wait(NULL);
        clock_gettime(CLOCK_REALTIME, &end_time1);
        // calculating time for execution of OTHER process
        execution_time1 = (end_time1.tv_sec - start_time1.tv_sec) + (double)(end_time1.tv_nsec - start_time1.tv_nsec)/1e9;
        printf("OTHER time : %lf\n", execution_time1);

    }
    else if (pid1<0){
        perror("Process 1 Failed");
        exit(1);
    }
    else {
        pid2 = fork(); // fork for process 2
        if (pid2==0){
            pid_t rc = fork(); // calling internal fork for caclulating time for completing the process easily
            if (!rc){
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
            clock_gettime(CLOCK_REALTIME, &start_time2);
            wait(NULL);
            clock_gettime(CLOCK_REALTIME, &end_time2);
            execution_time2 = (end_time2.tv_sec - start_time2.tv_sec) + (double)(end_time2.tv_nsec - start_time2.tv_nsec)/1e9;
            printf("RR time : %lf\n", execution_time2);

        }
        else if (pid2<0){
            perror("Process 2 Failed\n");
            exit(1);
        }
        else {
            pid3 = fork(); // fork for process 3
            if (pid3==0){
                pid_t rc = fork(); // calling internal fork for caclulating time for completing the process easily
                if (!rc){
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
                clock_gettime(CLOCK_REALTIME, &start_time3);
                wait(NULL);
                clock_gettime(CLOCK_REALTIME, &end_time3);
                execution_time3 = (end_time3.tv_sec - start_time3.tv_sec) + (double)(end_time3.tv_nsec - start_time3.tv_nsec)/1e9;
                printf("FIFO time : %lf\n", execution_time3);
            }
            else if (pid3<0){
                perror("Process 3 Failed");
                exit(1);
            }
        }
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    
    return 0;
}