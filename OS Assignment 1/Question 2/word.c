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
    if (argv[2] == NULL && strcmp(argv[1], "-n") != 0 && strcmp(argv[1], "-d") != 0){
                FILE *fptr;
                if (argv[1] == NULL){
                    printf("word: Too few argumens\n");
                    
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
                        
                    }  
                    else {
                        printf("File Doesn't Exist\n");
                        
                    } 
                }  
            }
            else if (strcmp(argv[1], "-n") == 0){
                FILE *fptr;
                if (argv[2] == NULL){
                    printf("word: Too few arguments\n");
                    
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
                        
                    }          
                    else {
                        printf("File Doesn't Exist\n");
                        
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
                        
                    }      

                    if (words1>words2){
                        printf("File 1 has more words, and difference in words of both files is: %d\n", words1-words2);
                        
                    }
                    else if (words2>words1){
                        printf("File 2 has more words, and difference in works of both files is: %d\n", words2-words1);
                        
                    }
                    else {
                        printf("Both files have same no. of words, therefore difference is: 0 (zero)\n");
                        
                    }
                }
            }
            else {
                printf("%s: command not found in word\n", argv[1]);
                
            }
    return 0;
}