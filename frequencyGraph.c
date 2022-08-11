/*
    CO222 project2

    E/17/190 S.N.Liyanage.
    23/06/2020

    HOW TO USE THE PROGRAM
    run ./progName [-l length] [-w | -c] [--scaled] filename1 filename2 ..
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//to store words or characters
typedef union _{
        char letter ;
        char word[255];
    }val;

//to store occurrence and data type
typedef struct __{
    int occurrence;
    val data;
    struct __ * next;
}alpha;

//function prototypes
void charsort();
void wordsort();
void printUsage();
int LongestWord();
char *removepunct(char *);
void printCharGraph(int scale);
int argumentCheck(int argc, char **argv);
void printWordGraph(int scale, int wordlen);


//global variables
char scaled[10]="--scaled";
char charflag[3]="-c";
char wordflag[3]="-w";
char length[3]="-l";
int scale,c,w;
int printLength=10;
int sum=0;
int *fileIndex;
char *simple;
char argv0[20];


alpha * head=NULL;


int main(int argc, char ** argv){
    strcpy(argv0,argv[0]);

    //a list to contain index of files
    fileIndex=(int*)malloc(sizeof(int)*argc);

    //if arguments are invalid
    if(argumentCheck(argc,argv)==0) return 0;


    if(c==1){

        //get the index number of files and read one by one
        for(int i=0; *(fileIndex+i)>0;i++){

            FILE *filePointer ;
            filePointer = fopen(argv[fileIndex[i]], "r") ;

            if ( filePointer == NULL ){
                printf( "Cannot open one or more given files\n" ) ;
                return 0;
            }
            else{

                char oneletter;

                //read one letter by letter
                while( fscanf(filePointer,"%c",&oneletter ) == 1 ){

                    int occure=0;
                    if(head==NULL){ //add first element
                        alpha *temp;
                        temp=(alpha *)malloc(sizeof(alpha));
                        //if letter is capital make it simple
                        if(oneletter<='Z' && oneletter>='A'){
                            temp->data.letter=oneletter-'A'+'a';
                            temp->occurrence=1;
                            head=temp;
                            head->next=NULL;
                            sum++;
                        }
                        if(oneletter<='z' && oneletter>='a'){
                            temp->data.letter=oneletter;
                            temp->occurrence=1;
                            head=temp;
                            head->next=NULL;
                            sum++;
                        }
                        if(oneletter<='9' && oneletter>='0'){
                            temp->data.letter=oneletter;
                            temp->occurrence=1;
                            head=temp;
                            head->next=NULL;
                            sum++;
                        }



                    //if the node is not the first element
                    }else{
                        alpha *p;
                        p=head;

                        //check whether the letter is already exists in linked list
                        while(p!=NULL){
                            if(p->data.letter==oneletter || p->data.letter==oneletter-'A'+'a'){
                                occure=1;
                                break;
                            }p=p->next;
                        }
                        //if letter is already there increase the occurrence
                        if(occure){
                            sum++;
                            p->occurrence=p->occurrence+1;

                        //if there's no such letter, create a new node for that letter
                        }else{
                            alpha *temp;
                            temp=(alpha *)malloc(sizeof(alpha));
                            if(oneletter<='Z' && oneletter>='A'){
                                temp->data.letter=oneletter-'A'+'a';
                                temp->occurrence=1;
                                temp->next=head;
                                head=temp;
                                sum++;
                            }if(oneletter<='z' && oneletter>='a'){
                                temp->data.letter=oneletter;
                                temp->occurrence=1;
                                temp->next=head;
                                head=temp;
                                sum++;
                            }
                            if(oneletter<='9' && oneletter>='0'){
                                temp->data.letter=oneletter;
                                temp->occurrence=1;
                                temp->next=head;
                                head=temp;
                                sum++;
                            }
                        }
                    }
                }
            }

        fclose(filePointer) ;
        }

        //if the files are empty
        if(sum==0){
            printf("No data to process\n");
            return 0;
        }
        //printf("%d\n",sum);
        //sort and print the graph
        charsort();
        printCharGraph(scale);





    }else{ //if w==1 or default option

        //get the index number of files and read one by one
        for(int i=0; *(fileIndex+i)>0;i++){

            FILE *filePointer ;
            filePointer = fopen(argv[fileIndex[i]], "r") ;

            if ( filePointer == NULL ){
                printf( "Cannot open one or more given files\n" ) ;
                return 0;
            }else{

                char oneword[255];

                //get word one by one
                while( fscanf(filePointer,"%s",oneword ) == 1 ){

                    //remove punctuation marks and turn the word in to a simple letter word
                    simple=removepunct(oneword);
                    int occure=0;
                    if(head==NULL){ //add first element
                        if(simple[0]!='\0'){
                            alpha *temp;
                            temp=(alpha *)malloc(sizeof(alpha));
                            strcpy(temp->data.word,simple);
                            temp->occurrence=1;
                            head=temp;
                            head->next=NULL;
                            sum++;

                        }

                    //if the node is not the first element
                    }else{

                         //check whether the word is already exists in linked list
                        alpha *p;
                        p=head;
                        while(p!=NULL){
                            if(strcmp(p->data.word,simple)==0){
                                occure=1;
                                break;
                            }p=p->next;
                        }

                        //if word is already there in linked list
                        if(occure){
                            sum++;
                            p->occurrence=p->occurrence+1;

                        //if there's no such word, create a new node for that word
                        }else{
                            if(simple[0]!='\0'){
                                alpha *temp;
                                temp=(alpha *)malloc(sizeof(alpha));

                                strcpy(temp->data.word,simple);
                                temp->occurrence=1;
                                temp->next=head;
                                head=temp;
                                sum++;
                            }
                        }
                    }
                }
            }
        fclose(filePointer) ;
        }

        //if the files are empty
        if(sum==0){
            printf("No data to process\n");
            return 0;
        }


        //sort the word linked list
        wordsort();
        //find the lengthiest word in given range
        int strlength;
        strlength=LongestWord();
        //print the graph
        printWordGraph(scale,strlength);



    }


    return 0;
}


//check the argument validity
int argumentCheck(int argc, char **argv){

    int index=0;

    for(int i=1; i<argc;i++){
        if(argv[i][0]=='-'){ //if the argument is a flag
            if(strcmp(scaled,argv[i])==0){
                scale=1;
            }else if(strcmp(charflag,argv[i])==0){
                c=1;
            }else if(strcmp(wordflag,argv[i])==0){
                w=1;
            }else if(strcmp(length,argv[i])==0){
                printLength=0;

                //if the -l is the last argument
                if(i==argc-1){
                    printf("Not enough options for [-l]\n");
                    printUsage();
                    return 0;

                //check whether the next argument next to -l is negative or not
                }else if(argv[i+1][0]=='-'){
                    for(int k=1; argv[i+1][k]!='\0';k++){
                        if(argv[i+1][k]<='9' && argv[i+1][k]>='0'){
                            printLength=printLength*10+argv[i+1][k]-'0';
                        }else{
                            printf("Invalid options for [-l]\n");
                            printUsage();
                            return 0;
                        }

                    }
                    //if argument is -0 after the -l
                    if(printLength==0) return 0;
                    //if print length is negative
                    printf("Invalid option(negative) for [-l]\n");
                    printUsage();
                    return 0;


                //check the argument next to the -l
                }else{
                    for(int k=0; argv[i+1][k]!='\0';k++){

                        if(argv[i+1][k]<='9' && argv[i+1][k]>='0'){
                            printLength=printLength*10+argv[i+1][k]-'0';
                        }else{
                            printf("Invalid options for [-l]\n");
                            printUsage();
                            return 0;
                        }
                    }i++; //if the option for -l is correct skip to the next argument
                }

                //if the print length is zero
                if(printLength==0) return 0;

            //for invalid flags
            }else{
                int zero=0; //check if there are zeros as a flag
                for(int j=1;argv[i][j]!='\0';j++){
                    if(argv[i][j]!='0') zero=1;
                }
                if(zero==0){ // -0 is considered as a file
                    *(fileIndex+index)=i;
                    index++;

                }else{ //invalid flags
                    printf("Invalid option [%s]\n",argv[i]);
                    printUsage();
                    return 0;
                }
            }
        //if the argument is not a flag
        }else{
            *(fileIndex+index)=i; //add the argument index to the list
            index++;
        }


    }

    if(w==1 && c==1) {
            printf("[-c] and [-w] cannot use together\n");
            printUsage();
            return 0;
    }

    //if the files are empty
    if(*fileIndex=='\0'){
        printf("No input files were given\n");
        printUsage();
        return 0;
    }


    return 1;

}

void printUsage(){
    printf("usage: %s [-l length] [-w | -c] [--scaled] filename1 filename2 ..\n",argv0);
}



void charsort(){
    alpha *q,*p;
    p=head;

    while(p->next!=NULL){
        q=p->next;
        while(q!=NULL){
            if((p->occurrence)<=(q->occurrence)){
                int tempInt;
                char tempChar;

                //swap the data
                tempInt=p->occurrence;
                tempChar=p->data.letter;
                p->occurrence=q->occurrence;
                p->data.letter=q->data.letter;
                q->occurrence=tempInt;
                q->data.letter=tempChar;

            }q=q->next;
        }p=p->next;
    }
}

void wordsort(){
    alpha *q,*p;
    p=head;
    while(p->next!=NULL){
        q=p->next;
        while(q!=NULL){
            if((p->occurrence)<=(q->occurrence)){
                int tempInt;
                char tempChar[255];

                //swap the data
                tempInt=p->occurrence;
                strcpy(tempChar,p->data.word);
                p->occurrence=q->occurrence;
                strcpy(p->data.word,q->data.word);
                q->occurrence=tempInt;
                strcpy(q->data.word,tempChar);

            }q=q->next;
        }p=p->next;
    }
}


//function to remove punctuations in a word and make it simple
char * removepunct(char *str){
    static char simple[255];

    //clear static char
    for(int i=0;simple[i]!='\0';i++) simple[i]='\0';

    int j=0;
    for(int i=0;str[i]!='\0';i++){

        if(str[i]<='Z' && str[i]>='A'){
            simple[j]=str[i]-'A'+'a';
            j++;
        }else if(str[i]<='z' && str[i]>='a'){
            simple[j]=str[i];
            j++;
        }else if(str[i]<='9' && str[i]>='0'){
            simple[j]=str[i];
            j++;
        }
    }
    return simple;

}


//find the longest word in the range of printing length
int LongestWord(){
    int length=0;
    int templength=0;
    alpha *p;
    p=head;

    for(int i=0;i<printLength && p!=NULL;i++){
        templength=0;
        while(p->data.word[templength]!='\0'){
            templength++;
        }
        p=p->next;
        if(templength>length){
            length=templength;
        }
    }
    return length;

}


void printCharGraph(int scale){
    alpha * p;
    p=head;

    int scaler;
    int firstPercentage;
    firstPercentage=head->occurrence/(sum*0.01);

    for(int i=0;i<printLength && p!=NULL;i++){
        float percentage;
        percentage=p->occurrence/(sum*0.01);

        if(scale==0 && head->occurrence<sum){
            scaler=(80-4-6)*percentage/100;
        }else if(scale==0 && head->occurrence==sum){   //if first percentage ==100
            scaler=(80-4-7)*percentage/100;
        }else if(scale==1 && firstPercentage<10.00){   //if first percentage < 10
            scaler=p->occurrence*(80-4-5)*1.0/head->occurrence;
        }else if(scale==1 && head->occurrence==sum){   //if first percentage == 100
            scaler=p->occurrence*(80-4-7)*1.0/head->occurrence;
        }else if(scale==1 && firstPercentage>=10.00){   //if first percentage >= 10
            scaler=p->occurrence*(80-4-6)*1.0/head->occurrence;
        }


        //first row of the pole
        printf("\n   \u2502");
        for(int j=0;j<scaler;j++){
            printf("\u2591");
        }

        //second row of the pole
        printf("\n %c \u2502",p->data.letter);
        for(int j=0;j<scaler;j++){
            printf("\u2591");
        }
        printf("%.2f%%",percentage);

        //third part of the pole
        printf("\n   \u2502");
        for(int j=0;j<scaler;j++){
            printf("\u2591");
        }

        printf("\n   \u2502");

        p=p->next;

    }

    //final row of the graph (x-axis)
    printf("\n   \u2514");
    for(int i=0;i<76;i++){
        printf("\u2500");
    }printf("\n");


}

void printWordGraph(int scale, int wordlen){
    alpha * p;
    p=head;

    int strlength;
    int scaler;
    int firstPercentage;
    firstPercentage = head->occurrence/(sum*0.01);

    for(int i=0;i<printLength && p!=NULL;i++){
        float percentage;

        percentage=p->occurrence/(sum*0.01);
        strlength=strlen(p->data.word);

        if(scale==0 && head->occurrence<sum){
            scaler=(80-3-wordlen-5)*percentage/100;
        }else if(scale==0 && head->occurrence==sum){   //if first percentage ==100
            scaler=(80-3-wordlen-7)*percentage/100;
        }else if(scale==1 && firstPercentage<10.00){   //if first percentage < 10
            scaler=p->occurrence*(80-3-wordlen-5)*1.0/head->occurrence;
        }else if(scale==1 && head->occurrence==sum){  //if first percentage == 100
            scaler=p->occurrence*(80-3-wordlen-7)*1.0/head->occurrence;
        }else if(scale==1 && firstPercentage>=10.00){  //if first percentage >= 10
            scaler=p->occurrence*(80-3-wordlen-6)*1.0/head->occurrence;
        }

        //first row of the pole
        printf("\n");
        for(int i=0;i<wordlen+2;i++) printf(" ");
        printf("\u2502");
        for(int j=0;j<scaler;j++){
            printf("\u2591");
        }

        //second row of the pole
        printf("\n ");
        printf("%s",p->data.word);
        for(int i=0;i<wordlen-strlength+1;i++) printf(" ");
        printf("\u2502");
        for(int j=0;j<scaler;j++){
            printf("\u2591");
        }
        printf("%.2f%%",percentage);

        //third row of the pole
        printf("\n");
        for(int i=0;i<wordlen+2;i++) printf(" ");
        printf("\u2502");
        for(int j=0;j<scaler;j++){
            printf("\u2591");
        }

        printf("\n");
        for(int i=0;i<wordlen+2;i++) printf(" ");
        printf("\u2502");

        p=p->next;

    }
     //final row of the graph (x-axis)
    printf("\n");
    for(int i=0;i<wordlen+2;i++) printf(" ");
    printf("\u2514");
    for(int i=0;i<77-wordlen;i++){
        printf("\u2500");
    }printf("\n");


}



