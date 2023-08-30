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



int main (int argc, char *argv[])
{
    if (argv[2] == NULL && strcmp(argv[1], "-d") != 0 && strcmp(argv[1], "-R") != 0){
        if (argv[1] == NULL){
            printf("date: Too few arguments\n");
        }
        else {
            struct stat fileStatus;
            if (stat(argv[1], &fileStatus) == 0) {      
                printf("Last Modified Date and Time: %s", ctime(&fileStatus.st_mtime));
            }
            else {
                printf("File \'%s\' doesn\'t exist\n", argv[1]);
            }
        }
    }
    else if (strcmp(argv[1], "-R") == 0) {
        if (argv[2] == NULL){
            printf("date: Too few arguments\n");
        }
        else {
            struct stat fileStatus;
            if (stat(argv[2], &fileStatus) == 0) {
                // Retrieve the last modified time from the file_stat structure
                time_t modified_time = fileStatus.st_mtime;
                struct tm *RFC5322 = localtime(&modified_time);

                // Format and print the last modified date and time
                char Final_Modified_Time_in_RFC5322 [50];
                strftime(Final_Modified_Time_in_RFC5322, sizeof(Final_Modified_Time_in_RFC5322), "%a, %d %b %Y %H:%M:%S %z", RFC5322);
                printf("Last Modified Date and Time in RFC 5322 format: %s\n", Final_Modified_Time_in_RFC5322);
            } 
            else {
                printf("File \'%s\' doesn\'t exist\n", argv[2]);
            }
        }
    }
    else if (strcmp(argv[1], "-d") == 0) {
        if (argv[2] == NULL){
            printf("date: Too few arguments\n");
        }
        else {
                struct stat fileStatus;
            if (stat(argv[2], &fileStatus) == 0) {
                // Retrieve the last modified time from the file_stat structure
                time_t modified_time = fileStatus.st_mtime;
                struct tm *string = localtime(&modified_time);

                // Format and print the last modified date and time
                char Final_Modified_Time_in_STRING [50];
                strftime(Final_Modified_Time_in_STRING, sizeof(Final_Modified_Time_in_STRING), "%dth %b %Y, %H:%M:%S", string);
                printf("Last Modified Date and Time in STRING format: %s\n", Final_Modified_Time_in_STRING);
            } 
            else {
                printf("File \'%s\' doesn\'t exist\n", argv[2]);
            }
        }
    }
    else {
        printf("%s: command not found in date\n", argv[1]);
    }
    return 0;
}