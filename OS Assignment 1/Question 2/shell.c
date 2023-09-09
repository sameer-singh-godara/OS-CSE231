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
	char working_directory[1000];
	char execution[1000];
	char input_from_user[1000];
    printf("\033[1;31m"); 
	printf("Welcome to the ");
	printf("Sameer Singh Godara Shell (ssgs)\n");
    getcwd(working_directory, sizeof(working_directory));
    while (1) {
        // initial part of shell
        printf("\033[1;34m"); 
		printf("ssgs >");
        printf("\033[1;32m");
		printf(" %s", getcwd(execution, sizeof(execution)));
        printf("\033[0;0m"); 
		printf("$ ");
		fgets(input_from_user, 1000, stdin);
		
        if (!strcmp(input_from_user, "exit\n")){
            break;
        }
        if (input_from_user[strlen(input_from_user)-2] == ' ') {
			input_from_user[strlen(input_from_user)-2] = '\n';
			input_from_user[strlen(input_from_user)-1] = '\0';
		}
        if (input_from_user[strlen(input_from_user)-1] == '\n') {
			input_from_user[strlen(input_from_user)-1] = '\0';
		}


        int numValues = 1; // Start with 1 to account for the last value
        for (int i = 0; i < strlen(input_from_user); i++) {
            if (input_from_user[i] == ' ') {
                numValues++;
            }
        }

        // Allocate memory for an array of string pointers
        char **list_of_user_input = (char **)calloc(numValues, sizeof(char *));

        // Split input_form_user into separate strings and store in the argv
        char *token = strtok(input_from_user, " ");
        int index = 0;
        while (token != NULL) {
            list_of_user_input[index] = (char *)calloc(strlen(token), sizeof(char));
            strcpy(list_of_user_input[index], token);
            token = strtok(NULL, " ");
            index++;
        }
        
        char **argv = malloc((index + 1) * sizeof(char *));
        
        // Copy the original arguments to the new array
        for (int i = 0; i < index; i++) {
            argv[i] = list_of_user_input[i];
        }
        free(list_of_user_input);
        // Add a NULL pointer at the end of the new array
        argv[index] = NULL;

        

        
        // ------------Commands------------

        // internal commands

        if (!strcmp(argv[0], "word")){
            if (argv[2] == NULL && strcmp(argv[1], "-n") != 0 && strcmp(argv[1], "-d") != 0){
                FILE *fptr;
                if (argv[1] == NULL){
                    printf("word: Too few argumens\n");
                    continue;
                }
                else {
                    fptr = fopen(argv[1], "r");
                    if (fptr != NULL){
                        char ch;
                        int words = 0;
                        ch = fgetc(fptr);
                        int is_pointer_inside_word = 0;
                        while(ch != EOF){
                            if (ch == ' '){
                                if (is_pointer_inside_word){
                                    is_pointer_inside_word = 0;
                                    words++;
                                }
                            }
                            else {
                                is_pointer_inside_word = 1;
                            }
                            
                            // printf("%c", ch);
                            ch = fgetc(fptr);
                        }
                        if (is_pointer_inside_word){
                            words++;
                        }

                        printf("No. of Words: %d\n", words);
                        fclose(fptr);
                        continue;
                    }  
                    else {
                        printf("File Doesn't Exist\n");
                        continue;
                    } 
                }  
            }
            else if (strcmp(argv[1], "-n") == 0){
                FILE *fptr;
                if (argv[2] == NULL){
                    printf("word: Too few arguments\n");
                    continue;
                }
                else {
                    fptr = fopen(argv[2], "r");
                    if (fptr != NULL){
                        char ch;
                        int words = 0;
                        ch = fgetc(fptr);
                        int is_pointer_inside_word = 0;
                        while(ch != EOF){
                            if (ch == ' ' || ch == '\n'){
                                if (is_pointer_inside_word){
                                    is_pointer_inside_word = 0;
                                    words++;
                                }
                            }
                            else {
                                is_pointer_inside_word = 1;
                            }
                            
                            // printf("%c", ch);
                            ch = fgetc(fptr);
                        }
                        if (is_pointer_inside_word){
                            words++;
                        }
                        printf("No. of Words: %d\n", words);
                        fclose(fptr);
                        continue;
                    }          
                    else {
                        printf("File Doesn't Exist\n");
                        continue;
                    }      
                }
            }    
            else if (strcmp(argv[1], "-d") == 0)
            {
                int words1;
                int words2;
                FILE *fptr1; 
                if (argv[2] == NULL || argv[3] == NULL){
                    printf("word: Too few arguments\n");
                    continue;
                }
                else {
                    fptr1 = fopen(argv[2], "r");
                    if (fptr1 != NULL){
                        char ch1;
                        words1 = 0;
                        ch1 = fgetc(fptr1);
                        int is_pointer_inside_word1 = 0;
                        while(ch1 != EOF){
                            if (ch1 == ' '){
                                if (is_pointer_inside_word1){
                                    is_pointer_inside_word1 = 0;
                                    words1++;
                                }
                            }
                            else {
                                is_pointer_inside_word1 = 1;
                            }
                            
                            // printf("%c", ch1);
                            ch1 = fgetc(fptr1);
                        }
                        if (is_pointer_inside_word1){
                            words1++;
                        }
                        // printf("with command -n: %d\n", words1);
                        fclose(fptr1);
                        
                    }  
                    else {
                        printf("File 1 Doesn't Exist\n");
                        continue;
                    }      

                    FILE *fptr2; 
                    fptr2 = fopen(argv[3], "r");
                    if (fptr2 != NULL){
                        char ch2;
                        words2 = 0;
                        ch2 = fgetc(fptr2);
                        int is_pointer_inside_word2 = 0;
                        while(ch2 != EOF){
                            if (ch2 == ' '){
                                if (is_pointer_inside_word2){
                                    is_pointer_inside_word2 = 0;
                                    words2++;
                                }
                            }
                            else {
                                is_pointer_inside_word2 = 1;
                            }
                            
                            // printf("%c", ch2);
                            ch2 = fgetc(fptr2);
                        }
                        if (is_pointer_inside_word2){
                            words2++;
                        }
                        // printf("with command -n: %d\n", words2);
                        fclose(fptr2);
                    }  
                    else {
                        printf("File 2 Doesn't Exist\n");
                        continue;
                    }      

                    if (words1>words2){
                        printf("File 1 has more words, and difference in words of both files is: %d\n", words1-words2);
                        continue;
                    }
                    else if (words2>words1){
                        printf("File 2 has more words, and difference in works of both files is: %d\n", words2-words1);
                        continue;
                    }
                    else {
                        printf("Both files have same no. of words, therefore difference is: 0 (zero)\n");
                        continue;
                    }
                }
            }
            else {
                printf("%s: command not found in word\n", argv[1]);
                continue;
            }
        }

        // external commands

        else if (!strcmp(argv[0], "dir")){
            if (argv[1]==NULL){
                printf("dir: Too few arguments\n");
                continue;
            }
            int rc = fork();
            if (rc == 0){
                strcat(working_directory, "/dir");
                execv(working_directory, argv);
                return 0;
            }
            else {
                wait(0);
                if (argv[2] == NULL && strcmp(argv[1], "-r") != 0 && strcmp(argv[1], "-v") != 0){
                    if (argv[1] == NULL){
                    }
                    else {
                        char* directory_name = argv[1];
                        chdir(directory_name);
                    }
                }
                else if (strcmp(argv[1], "-r") == 0){
                    if (argv[2] == NULL){
                    }
                    else {                    
                        char* directory_name = argv[2];
                        chdir(directory_name);
                        
                    }
                }
                else if (strcmp(argv[1], "-v") == 0) {
                     if (argv[2] == NULL){
                    }
                    else {
                        char* directory_name = argv[2];
                        chdir(argv[2]);
                    }                         
                }
                continue;
            }
        }
        else if (!strcmp(argv[0], "date")){
            if (argv[1]==NULL){
                printf("date: Too few arguments\n");
                continue;
            }
            int rc = fork();
            if (rc == 0){
                strcat(working_directory, "/date");
                execv(working_directory, argv);
                return 0;
            }
            else {
                wait(0);
                continue;
            }
        }

        free(argv);
    }
    printf("You Have Successfully Exited the Sameer Singh Godara Shell\n");
    return 0;
}
