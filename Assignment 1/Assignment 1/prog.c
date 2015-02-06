#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>


#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */
#define HISTORY "history"
#define MAX_HISTORY_ENTRIES 10
/**
 * setup() reads in the next command line, separating it into distinct tokens
 * using whitespace as delimiters. setup() sets the args parameter as a
 * null-terminated string.
 */
int setup(char inputBuffer[], char *args[],int *background)
{
    int length,     /* # of characters in the command line */
    i,              /* loop index for accessing inputBuffer array */
    start,          /* index where beginning of next command parameter is */
    ct;             /* index of where to place the next parameter into args[] */
    
    ct = 0;
    
    /* read what the user enters on the command line */
    length = read(STDIN_FILENO, inputBuffer, MAX_LINE); //length of the input + 1
    
    start = -1;
    if (length == 0)
        exit(0); /* ^d was entered, end of user command stream */
    if (length < 0){
        perror("error reading the command");
        exit(-1); /* terminate with error code of -1 */
    }
    /* examine every character in the inputBuffer */
    for (i=0;i<length;i++) {
        switch (inputBuffer[i]){
            case ' ':
            case '\t' : /* argument separators */
                if(start != -1){
                    args[ct] = &inputBuffer[start]; /* set up pointer */
                    ct++;
                }
                inputBuffer[i] = '\0'; /* add a null char; make a C string */
                start = -1;
                break;
            case '\n': /* should be the final char examined */
                if (start != -1){
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL; /* no more arguments to this command */
                break;
            default : /* some other character */
                if (start == -1)
                    start = i;
                if (inputBuffer[i] == '&'){
                    *background = 1;
                    inputBuffer[i] = '\0';
                }
        }
    }
    args[ct] = NULL; /* just in case the input line was > 80 */
    return (ct+1);
}


/* the steps are:
 (1) fork a child process using fork()
 (2) the child process will invoke execvp()
 (3) if background == 0, the parent will wait,
 otherwise returns to the setup() function. */
void enterCommand(char *args[], int background, int numberArgs){
    int status;
    //start
    pid_t pid = fork();
    
    int argsArraySize;
    
    
    
    if(pid == 0){
        //in child
        printf("===================IN CHILD====================\n");
        //printf("args[0] = %s\n", args[0]);
        //printf("args[1] = %s\n", args[1]);
        
        char *options[numberArgs];
        int j = 0;
        while(j<numberArgs){
            options[j] = args[j];
            j++;
        }
        options[argsArraySize-1] = 0;
        //printf("args[0] = %s\n", args[0]);
        //printf("options = %s\n", options[1]);
        execvp(args[0],args);
        
        //printf("inside?");
        //
        printf("======================OUT CHILD==============\n");
        
    }
    else{
        //waitpid(pid, &status, WNOHANG);
        wait(&status);
    }
    
}

void printHistory(char *history[]){
    int i;
    
    for(i = 0; i < MAX_HISTORY_ENTRIES; i++){
        printf("history : %d --- %s \n", i, history[i]);
    }
    
    
}

void addToHistory(char *history[], char *cmd, int historyLength){
    //printf("==================  START addToHistory ====================== \n");
    int MaximumEntries = MAX_HISTORY_ENTRIES;
    int j;
    
    //if did not reach 35 yet
    if(historyLength < MAX_HISTORY_ENTRIES + 1){
        //printf("historyLength = %d\n ", historyLength);
        history[historyLength-1] = strdup(cmd);
        
    }
    //if maximum reached
    else{
        int i;
        
        for(i = MaximumEntries; i > 0; i--){
            history[i] = history[i-1];
            
        }
        history[0] = strdup(cmd);
        
    }
    //printHistory(history);
    //printf("==================  END addToHistory ====================== \n");
    
    
}


char * buildCmd(char cmd[], char *args[], int numberOfCmd, int background){
    
    strcpy(cmd,"");
    
    int i = 0;
    //printf("==================  START buildCmd ====================== \n");
    //printf("%d \n", numberOfCmd);
    //printf("args[0] in buildCmd = %s \n", args[0]);
    //concatenate the arguments
    for(i = 0; i < numberOfCmd; i++){
        if(args[i] != 0){
            //printf("args[%d] is = %s \n", i, args[i]);
            strcat(cmd, args[i]);
            strcat(cmd, " ");
        }
    }
    
    //if a command is run in the background, it will end with & as mentionned in instructions
    if(background == 0){
        //printf("background is == 0 \n");
        cmd[strlen(cmd) - 1] = '&';
    }
    
    //printf("%s", cmd);
    //printf("buildCmd returns cmd =  %s\n", cmd);
    //printf("==================  END buildCmd ====================== \n");
    return cmd;
    
    
}

//If the input is a built-in command, return the "command"
//else return false "false"
char *builtInCommands(char *cmd){
    
    char *builtCommand;
    
    
    if(strcmp(cmd, "cd")==0){
        builtCommand = "cdCmd";
    }else if(strcmp(cmd, "pwd")==0){
        builtCommand = "pwdCmd";
    }else if(strcmp(cmd, "exit")==0){
        builtCommand = "exitCmd";
    }else if(strcmp(cmd, "history")==0){
        builtCommand = "historyCmd";
    }
    //else if(strcmp(cmd,"r") ==0 || strcmp(cmd,"r\n") == 0){
    //    builtCommand = "rCmd";
    //}
    else{
        builtCommand = "false";
    }
    return builtCommand;
}

int main(void)
{
    char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE/+1]; /* command line (of 80) has max of 40 arguments */
    char *history[10];
    //int numberOfCmd = 0;
    char cmd[MAX_LINE]; //
    char *buildCmdTest;
    int historyLength = 0;
    
    //HISTORY
    //    if(strcmp(args[0], HISTORY) == 0){
    //        printf("you selected history\n");
    //    }
    
    
    
    while (1){ /* Program terminates normally inside setup */
        background = 0;
        printf(" COMMAND->\n");
        int numberArgs = setup(inputBuffer,args,&background); /* get next command, returns the index of next element*/
        //printf("index of next argument = %d \n ",numberArgs);
        
        //if user enters nothing, then reprompt him
        if(numberArgs == -1){
            continue;
        }
        
        
        // HERE
        if(args[0] != 0){
            buildCmdTest = buildCmd(cmd, args, numberArgs, background);
            //printf("print return of build cmd test = %s \n", buildCmdTest);
            historyLength++;
            addToHistory(history, buildCmdTest, historyLength);
            
        }else{
            continue;
        }
        
        // ERROR!!!

        
        /* the steps are:
         (1) fork a child process using fork()
         (2) the child process will invoke execvp()
         (3) if background == 0, the parent will wait,
         otherwise returns to the setup() function. */
        
        //printf("is system call? %d \n",strcmp(builtInCommands(args[0]),"false"));
        
        
        
        
        
        
        
        
        // Step : if the user inputs a built-in command such as history, exit, cd, pwd
        if(strcmp(builtInCommands(args[0]),"false") != 0){
            if(strcmp(builtInCommands(args[0]), "cdCmd") == 0){
                chdir(args[1]);
            }else  if(strcmp(builtInCommands(args[0]), "exitCmd") == 0){
                exit(0);
            } else if(strcmp(builtInCommands(args[0]), "historyCmd") == 0){
                printHistory(history);
            } else if(strcmp(builtInCommands(args[0]), "pwdCmd") == 0){
                char cwd[1024];
                if(getcwd(cwd, sizeof(cwd)) != NULL){
                    fprintf(stdout, "presend working directory : %s\n", cwd);
                }else {
                    printf("error in pwd");
                }
                
            }else{
                enterCommand(args, numberArgs, background);
                }
            
        }
        
        /*
        if(strcmp(builtInCommands(args[0]),"rCmd") == 0){
            if(args[1] != 0){
                printf("inside r?");
                //printf("history feature, no 2nd argument\n");
                //printf("most recent command = %s\n", history[0]);
                strcpy(cmd, history[0]);
                printf("%sls\n", cmd);
                
                
                historyLength++;
                addToHistory(history, cmd, historyLength);
                
                char recentTemp[MAX_LINE + 1];
                strcpy(recentTemp, cmd);
                strcat(recentTemp, "\n");
                
                //strcpy(recentTemp, "ls\n");
                numberArgs = setup(recentTemp,args,&background);
                //execvp(history[0], history);
                //enterCommand(history, numberArgs, background);
                //execvp("ls", args);
                printHistory(history);
            }
            
        }
        */
        
        //Step : R x for history feature
        
        
        
        //char *test[10];
        //char *command = args[0];
        
        //char *string2 = "hello";
        //char *string = strdup(string2);
        //printf("%s",strdup(string2));
        //printf("arg 0 is\n");
        //printf("%s", *args);
        /*
         if(strcmp(args[0], HISTORY)==0){
         printf("success");
         
         }
         */
        //buildCmd(*cmd, *args, numberOfCmd, background);
        //addToHistory(history, *args, numberOfCmd);
        
        
    }
}

//gcc /Users/Tuan/Desktop/McGill/ECSE\ 427/ECSE427-OS/Assignment\ 1/Assignment\ 1/prog.c
///Users/Tuan/a.out ;