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


int deleteDirectoryRecursively(const char *directoryPath) {// written this function with the help of chat gpt
    struct dirent *entry;
    DIR *dir = opendir(directoryPath);

    if (!dir) {
        // perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "%s/%s", directoryPath, entry->d_name);

        if (entry->d_type == DT_DIR) {
            deleteDirectoryRecursively(filepath);
        } else {
            if (remove(filepath) != 0) {
                // perror("remove");
                return 1;
            }
        }
    }

    closedir(dir);
    
    if (rmdir(directoryPath) != 0) {
        // perror("rmdir");
        return 1;
    }
    return 0;
}


int main (int argc, char *argv[])
{
    char directoryPath[1000];
    getcwd(directoryPath, sizeof(directoryPath));
    if (argv[2] == NULL && strcmp(argv[1], "-r") != 0 && strcmp(argv[1], "-v") != 0){
        if (argv[1] == NULL){
            printf("dir: Too few arguments\n");
        }
        else {
            char* directory_name = argv[1];
            int check_dic_created_or_not;
            
            DIR* dir = opendir(directory_name);
            if (dir){
                printf("Directory \'%s\' Exists\n", directory_name);
            }
            else {
                check_dic_created_or_not = mkdir(directory_name, 0777);
                
                if (!check_dic_created_or_not){
                    printf("Directory \'%s\' Created\n", directory_name);
                    // chdir(directory_name);
                }
                else {
                    printf("Unable to create Directory \'%s\'\n", directory_name);
                }
                chdir(directory_name);
            }
        }
    }
    else if (strcmp(argv[1], "-r") == 0){
        if (argv[2] == NULL){
            printf("dir: Too few arguments\n");
        }
        else {
            char* directory_name = argv[2];
            int check_dic_created_or_not;
            
            DIR* dir = opendir(directory_name);
            if (dir){
                printf("Directory \'%s\' Exists\n", directory_name);
                int check_dic_removed_or_not;
                char temporary_path[1000];
                getcwd(temporary_path, sizeof(temporary_path));
                chdir(directory_name);
                // printf("%s\n", getcwd(temporary_path, sizeof(temporary_path)));
                check_dic_removed_or_not = deleteDirectoryRecursively(getcwd(temporary_path, sizeof(temporary_path)));
                chdir("..");
                if (!check_dic_removed_or_not){
                    printf("Directory \'%s\' removed successfully\n", directory_name);

                }
                else {
                    printf("Directory \'%s\' not removed\n", directory_name);
                }

                // printf("%s\n", getcwd(temporary_path, sizeof(temporary_path)));
                check_dic_created_or_not = mkdir(directory_name, 0777);
                if (!check_dic_created_or_not){
                    printf("Directory \'%s\' Created\n", directory_name);
                    chdir(directory_name);

                }
                else {
                    printf("Unable to create Directory \'%s\'\n", directory_name);
                }
            }
            else {
                printf("Directory \'%s\' Doesn\'t Exists\n", directory_name);
                check_dic_created_or_not = mkdir(directory_name, 0777);
                if (!check_dic_created_or_not){
                    printf("Directory \'%s\' Created\n", directory_name);
                    chdir(directory_name);
                }
                else {
                    printf("Unable to create Directory \'%s\'\n", directory_name);
                }
            }
        }
    }
    else if (strcmp(argv[1], "-v") == 0){
        if (argv[2] == NULL){
            printf("dir: Too few arguments\n");
        }
        else {
            char* directory_name = argv[2];
            int check_dic_created_or_not;

            
            DIR* dir = opendir(directory_name);
            printf("Checking Whether Directory \'%s\' Exists or Not\n", directory_name);
            if (dir){
                printf("Directory \'%s\' Exists\n", directory_name);
                chdir(directory_name);

                printf("Creating Directory %s\n", directory_name);
                
                check_dic_created_or_not = mkdir(directory_name, 0777);
                
                if (!check_dic_created_or_not){
                    printf("Directory \'%s\' Created\n", directory_name);
                    chdir(directory_name);

                }
                else {
                    printf("Unable to create Directory \'%s\'\n", directory_name);
                }

            }
            else {
                printf("Directory \'%s\' doesn\'t exist\n", directory_name);
                printf("Creating Directory %s\n", directory_name);

                check_dic_created_or_not = mkdir(directory_name, 0777);
                
                if (!check_dic_created_or_not){
                    printf("Directory \'%s\' Created\n", directory_name);
                    chdir(directory_name);

                }
                else {
                    printf("Unable to create Directory \'%s\'\n", directory_name);
                }
            }
        }
    }
    else {
        printf("%s: command not found in dir\n", argv[1]);
    }
    return 0;
}