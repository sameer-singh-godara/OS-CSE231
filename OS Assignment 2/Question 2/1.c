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
    double executionTimes[3];
    int policies[3];
    double execution_time1, execution_time2, execution_time3;
    
    FILE* file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    struct timespec start_time1, start_time2, start_time3, end_time1, end_time2, end_time3; // defining the datatype of start and end time of each processes
    pid1 = fork();// fork for process 1
    if (pid1==0){
        pid_t rc = fork(); // calling internal fork for caclulating time for completing the process easily
        clock_gettime(CLOCK_REALTIME, &start_time1);
        if (!rc){
            struct sched_param param;
            param.sched_priority = 0;
            policies[SCHED_OTHER] = SCHED_OTHER;
            
            int check_sched_set = sched_setscheduler(getpid(), SCHED_OTHER, &param);
            if (check_sched_set == -1){
                perror("sched_scheduler error in process 1\n");
                exit(1);
            }
            
            execl("./2", "2", NULL);
            
            perror("execl error in proces 1\n");
            exit(1);
        }
        // clock_gettime(CLOCK_REALTIME, &start_time1);
        wait(NULL);
        clock_gettime(CLOCK_REALTIME, &end_time1);
        
        execution_time1 = (end_time1.tv_sec - start_time1.tv_sec) + (double)(end_time1.tv_nsec - start_time1.tv_nsec)/1e9;
        executionTimes[SCHED_OTHER] = execution_time1;
        fprintf(file, "%d %lf\n", SCHED_OTHER, executionTimes[SCHED_OTHER]); // Printing output in output.txt
        printf("Starting time of OTHER : %lf\n", start_time1.tv_sec + (double)(start_time1.tv_nsec/1e9));
        printf("Ending time of OTHER : %lf\n", end_time1.tv_sec + (double)(end_time1.tv_nsec/1e9));
        printf("\033[1;31m");
        printf("Time of execution of OTHER : %lf\n\n", execution_time1);
        printf("\033[0;0m"); 

    }
    else if (pid1<0){
        perror("Process 1 Failed");
        exit(1);
    }
    else{
        // wait(NULL);
        pid2 = fork(); // fork for process 2
        if (!pid2 && pid1){
            pid_t rc = fork(); // calling internal fork for caclulating time for completing the process easily
            clock_gettime(CLOCK_REALTIME, &start_time2);
            if (!rc){
                struct sched_param param;
                param.sched_priority = 1;
                policies[SCHED_RR] = SCHED_RR;

                int check_sched_set = sched_setscheduler(getpid(), SCHED_RR, &param);
                if (check_sched_set == -1){
                    perror("sched_scheduler error in process 2\n");
                    exit(1);
                }
                
                execl("./2", "2", NULL);

                perror("execl error in proces 2\n");
                exit(1);
            }
            // clock_gettime(CLOCK_REALTIME, &start_time2);
            wait(NULL);
            clock_gettime(CLOCK_REALTIME, &end_time2);
            execution_time2 = (end_time2.tv_sec - start_time2.tv_sec) + (double)(end_time2.tv_nsec - start_time2.tv_nsec)/1e9;
            executionTimes[SCHED_RR] = execution_time2;
            fprintf(file, "%d %lf\n", SCHED_RR, executionTimes[SCHED_RR]); // Printing output in output.txt
            printf("Starting time of RR : %lf\n", start_time2.tv_sec + (double)(start_time2.tv_nsec/1e9));
            printf("Ending time of RR : %lf\n", end_time2.tv_sec + (double)(end_time2.tv_nsec/1e9));
            printf("\033[1;32m");
            printf("Time of execution of RR : %lf\n\n", execution_time2);
            printf("\033[0;0m"); 

        }
        else if (pid2<0){
            perror("Process 2 Failed\n");
            exit(1);
        }
        else{
            // wait(NULL);
            pid3 = fork(); // fork for process 3
            if (!pid3 && pid1 && pid2){
                pid_t rc = fork(); // calling internal fork for caclulating time for completing the process easily
                clock_gettime(CLOCK_REALTIME, &start_time3);
                if (!rc){
                    struct sched_param param;
                    param.sched_priority = 1;
    
                    policies[SCHED_FIFO] = SCHED_FIFO;
                    int check_sched_set = sched_setscheduler(getpid(), SCHED_FIFO, &param);
                    if (check_sched_set == -1){
                        perror("sched_scheduler error in process 3\n");
                        exit(1);
                    }
                    
                    execl("./2", "2", NULL);

                    perror("execl error in proces 3\n");
                    exit(1);
                }
                
                // clock_gettime(CLOCK_REALTIME, &start_time3);
                wait(NULL);
                clock_gettime(CLOCK_REALTIME, &end_time3);
                execution_time3 = (end_time3.tv_sec - start_time3.tv_sec) + (double)(end_time3.tv_nsec - start_time3.tv_nsec)/1e9;
                executionTimes[SCHED_FIFO] = execution_time3;
                fprintf(file, "%d %lf\n", SCHED_FIFO, executionTimes[SCHED_FIFO]); // Printing output in output.txt
                printf("Starting time of FIFO : %lf\n", start_time3.tv_sec + (double)(start_time3.tv_nsec/1e9));
                printf("Ending time of FIFO : %lf\n", end_time3.tv_sec + (double)(end_time3.tv_nsec/1e9));
                printf("\033[1;34m");
                printf("Time of execution of FIFO : %lf\n\n", execution_time3);
                printf("\033[0;0m"); 
            }
            else if (pid3<0){
                perror("Process 3 Failed");
                exit(1);
            }
            else{
                // wait(NULL);
            }
        }
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);
    
    

    
    fclose(file);


    
    return 0;
}