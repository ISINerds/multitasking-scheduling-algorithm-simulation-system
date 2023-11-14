#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This C program will be :
// 1- parsing the processes file
// 2- Generate a static table of a convenient format of the Process details

struct Process {
    char name[10];
    int arrivalTime ;
    int ExecTime;
    int priority;
};

//A Function that extracts the maximum number of processes from the configuration file.
int Max_nb_processes() {

    FILE* fPointer = fopen ("../../config.conf","r");
    char content[100] ;

    //const char *delimiter = "=";
    //char *token;
    int value;

    if (fPointer != NULL ) {
            printf("Lecture of conf file is successfull ! \n");
            fgets(content,100,fPointer);

        // Find the position of "=" in the string
        if (sscanf(content, "%*[^=]=%d", &value) == 1) {
            return value;
        }
        else {
        return -1;
        }

    }
    
    else {
        printf("ERROR opening the conf file ! \n");
        return -1;

    }
    fclose(fPointer);

}


//The function that tokenize a string and return a Process struct.
struct Process tokenize (char ch[255],char del[]) {
    ch[strcspn(ch, "\n")] = '\0';

    //Variables of process details
    char ProcessName[10]="";
    char arrivalTime[10]="";
    char ExecTime[10]="";
    char priority[10] ="";
    int ind=0; int indAT=0; int indEX=0; int indPR=0;

    for (int i=0;i<strlen(ch);i++) {
        if (ind==0){
            if (ch[i]==':'){
                ind++;
                continue;
            }
            else {
                ProcessName[i]=ch[i];
                }
        }
        else if (ind==1) {
            if (ch[i]==del[0]){
                ind++;
                continue;
            }
            else {
            arrivalTime[indAT]=ch[i];
            indAT++;
            }
        }
        else if (ind==2) {
            if (ch[i]==del[0]){
                ind++;
                continue;
            }
            else {
            ExecTime[indEX]=ch[i];
            indEX++;
            }
        }
        else if (ind==3) {
            priority[indPR]=ch[i];
            indPR++;
            }
        }

        struct Process P;
        strcpy(P.name, ProcessName);
        P.arrivalTime = atoi(arrivalTime);
        P.ExecTime = atoi(ExecTime);
        P.priority = atoi(priority);

        return P;
    }


int main(){
    // Choosing the seperator character.
    char sep[]=";";

    //Getting the size of the table of processes
    int sizeTable=Max_nb_processes();

    FILE* fPointer = fopen ("test.txt","r");
    char content[255] ;

    //Number of lines
    int line=1;
    //Position of the table
    int i;

    struct Process processes[sizeTable];

    if (fPointer != NULL ) {
            printf("Lecture is successfull ! \n");
            while(!feof(fPointer)){
                if (line==1){
                    fgets(content,255,fPointer);
                    sizeTable = atoi(content);
                    struct Process processes[sizeTable];
                    printf("Size of the table is : %d \n",sizeTable);
                    i=0;
                    line++;
                    // We skip the second line \n ????
                    fgets(content,255,fPointer);
                    continue;
                    }

                fgets(content,255,fPointer);
                struct Process p = tokenize(content,sep);
                processes[i]=p;
                i++;
                line++;
            }
            //Testinng part
            printf("%d",processes[0].priority);

    }
    else {
        printf("Error opening the processes file");
        return -1;
    }

    fclose(fPointer);

    return 0;
}

