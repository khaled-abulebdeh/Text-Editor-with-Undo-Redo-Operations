
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define str_size 1000
#define maxArraySize 1000
int realArraySize=0;
typedef struct StackPtr Stack;
typedef struct StackPtr UndoStack;
typedef struct StackPtr RedoStack;
typedef struct StackElements StackNode;

typedef struct QueuePtr Queue;
typedef struct QueueElements QueueNode;

struct StackElements {
        StackNode* nextS;// ptr to next stackNode
        char str[str_size];//storing string
        int status;
        //inserted or deleted word // 2 means inserted // -2 means deleted
        int position;

};
struct StackPtr {//Implementation of the head of the stack & the Top
    StackNode* Top;
};
struct QueueElements {
    QueueNode* nextQ;
    char str[str_size];
};
struct QueuePtr {//Implementation of the head of the Queue & Rear & Front
    QueueNode* Rear;// pointer of the same Data type as the pointered node
    QueueNode* Front;

};

//other function
void myMenu(char myStr [str_size], Queue* Q,UndoStack* US, RedoStack* RS);
void loadText (char myStr [str_size]);
void printLoadedText (char myStr [str_size]);
int findIndexToAdd (char myStr [str_size], char knownStrPosition[str_size]);
void insertTextAfterPosition (char myStr [str_size], int index, char sent [str_size]);
void insertTextFirst(char myStr [str_size], char sent [str_size]);
void deleteTextAfterPosition (char myStr [str_size],int index, char sent [str_size]);
void userDeletionPush (char myStr [str_size], char sent [str_size], UndoStack* US);
int findIndexTodelete (char myStr [str_size], char knownStrPosition[str_size]);
void saveToFile(char myStr [str_size]);

//stack
StackNode* createStackNode();
Stack* createStackHead();
void pushToStack (char myStr [str_size], StackNode* mySNode,Stack* S);
void pushStr (char str [str_size], Stack* S);
StackNode* popWithReturn (Stack* S);
void popWithOutReturn (Stack* S);
void pushToRedoStack (char myStr[str_size],UndoStack* US, RedoStack* RS);
void pushToUndoStack (char myStr[str_size],UndoStack* US, RedoStack* RS);
int isEmptyStack (Stack* S);
void makeEmptyStack (Stack* S);
void printStacks (UndoStack* US, RedoStack* RS);

//Queue
QueueNode* createQueueNode ();
Queue* createQueueHead ();
void EnQueueOp (char str[str_size], Queue* Q);
QueueNode* DeQueueWithReturn (Queue* Q);
void DeQueueWithOutReturn (Queue* Q);
void EnQueueAddedLine (char addedStr [str_size], Queue* Q);
void DeQueueThenPushToUndoStack (char myStr [str_size],char addedStr [str_size], Queue* Q, UndoStack* US);
int isEmptyQueue (Queue* Q);
int isLastQueueNode (QueueNode* myQNode, Queue* Q);

int main()
{

    Queue* Q= createQueueHead();
    UndoStack* US= createQueueHead();
    RedoStack* RS= createQueueHead();
    char myStr [str_size]="";
    myMenu(myStr, Q, US,RS);

    return 0;

}

//other function
void myMenu(char myStr [str_size], Queue* Q,UndoStack* US, RedoStack* RS){
    char addedStr [str_size]= "";
    char deletedStr [str_size]= "";


    int var;
    int myChoice=-1;
    do {
        printf("\n----------------------Menu----------------------\n");
        printf("Enter Your Choice!\n");
        printf("1- Load the input file which contains the initial text\n");
        printf ("2- Print the loaded text\n");
        printf ("3- Insert strings to the text\n");
        printf ("4- Remove strings from the text\n");
        printf ("5- Perform Undo operation\n");
        printf ("6- Perform Redo operation\n");
        printf ("7- Print the Undo Stack and the Redo stack\n");
        printf ("8- Save the updated text to the output file\n");
        printf ("9- Exit\n");
        printf("-------------------------------------------------\n\n");



        scanf("%d", &myChoice);
        getchar();//the user will have time to write the string

        switch(myChoice){

        case 1:loadText(myStr);
            printf("Data was loaded from input file..\n");

        break;

        case 2:printLoadedText(myStr);
        break;

        case 3:
            makeEmptyStack(RS);
            printf("Enter string you want to add: ");
            strcpy(addedStr,"");//to ensure clearing
            gets(addedStr);
            EnQueueAddedLine(addedStr,Q);
            DeQueueThenPushToUndoStack (myStr,addedStr,Q,US);
            printf("\nOriginal text was updated, your string was added..\n");
        break;

        case 4:
            makeEmptyStack(RS);
            printf("Enter string you want to delete: ");
            gets(deletedStr);
            userDeletionPush(myStr,deletedStr,US);

        break;

        case 5:pushToRedoStack(myStr,US,RS);

        break;

        case 6:pushToUndoStack(myStr,US,RS);

        break;

        case 7:printStacks(US,RS);
        break;

        case 8:saveToFile(myStr);
            makeEmptyStack(US);//important
            makeEmptyStack(RS);


        break;

        //i chose to free stacks in exit case, user may exit without saving!, so in that case Stacks will not be freed
        case 9:printf("Are you sure you want to exit?\n");
        printf("1- Yes\n");
        printf("2- No\n");
        var=-1;
        scanf("%d", &var);
        if (var==1){
            printf("You are welcome to use our program later\n");
            free(US);
            free(RS);
            free(Q);
            exit (0);
        }
        else{
            printf("please, save your edits to file..\n");
            myChoice=-1;//to return the loop
        }
        break;

        //otherwise
        default: printf("Sorry, you may entered a wrong number, Try again\n");

        }
    }
    while (myChoice!=9);
}
void loadText (char myStr [str_size]){

    FILE *input;
    input= fopen("inputFile.txt","r");

    char myLine [str_size]="";//stores line here, then concatenate with myStr to read more than 1 line
    char * valid=fgets(myLine,str_size,input);

    while (valid!=NULL){

        strcat(myStr, myLine);

        int length= strlen(myStr);

        if (myStr[length-1]=='\n'){//to remove "new line" char
            myStr[length-1]=' ';
        }

        if (myStr[length-1]!=' '){//to add space if was not
            strcat(myStr, " ");
        }
        valid=fgets(myLine,str_size,input);
    }

    fclose(input);
}
void printLoadedText (char myStr [str_size]){
    if (strlen(myStr)==0){
        printf("Sorry, there is no text to print!!\n");
    }
    else
    puts(myStr);
}
int findIndexToAdd (char myStr [str_size], char knownStrPosition[str_size]){
    //we will use strchr function to find the position to add after it
    char tempArr [str_size]="";
    strcpy(tempArr, myStr);

    int index=-100;//not found yet

    int knownStrLength= strlen(knownStrPosition);//the length of the str position entered from the user

    char* subStr= strtok (tempArr, " ");//comp
    index=0;
    while (subStr!= NULL){//while there are words in the original text

        int subLength= strlen(subStr);
        index+= subLength +1;//1 for cut space

        //handle if i want to add after a word which has a special char after it

        //we clear special char in both (original str) with knownPositionStr to compare letters only
        char handleCommas [str_size]="";
        strcpy(handleCommas, subStr);

        //check the last index of original word
        if (handleCommas[subLength-1]==',' || handleCommas[subLength-1]==':'||handleCommas[subLength-1]=='.'
            ||handleCommas[subLength-1]=='?'||handleCommas[subLength-1]=='!'){

            //remove the special char
            handleCommas[subLength-1]='\0';

        }

        //check the last index of knownPositionStr word
        if (knownStrPosition[knownStrLength -1]==',' || knownStrPosition[knownStrLength -1]==':'||knownStrPosition[knownStrLength -1]=='.'
            ||knownStrPosition[knownStrLength -1]=='?'||knownStrPosition[knownStrLength -1]=='!'){

            //remove the special char
            knownStrPosition[ knownStrLength -1] ='\0';

        }

        //compare two clean strings
        if (strcmp(handleCommas, knownStrPosition)==0){
            return index;
        }

        subStr= strtok(NULL, " ");//check next word int the original text//increment of loop
    }

    //to reach here, word looking for was not found
    index=-100;
    return index;
}
void insertTextAfterPosition (char myStr [str_size], int index, char sent [str_size]){

    int textLength=strlen(myStr);

    char temp [str_size]="";// "" is important when use strcpy
    strncpy(temp, myStr,index);//store the first part of original text

    char *subStr;
    if (index<textLength){//to never go out of the indexes
        subStr = myStr + index ;//to store the last part of original text
        strcat (temp, sent);//concatenate first part with my sent
        strcat(temp," ");

        strcat (temp, subStr);// concatenate (first part & my sent) with last part

        if (temp[strlen(temp)-1]!= ' '){//if last letter of first part is not a space, add space
            strcat(temp, " ");
        }
        strcpy(myStr, temp);//edit the original string
    }
    else {
        if( temp[textLength-1]!= ' '){
            strcat(temp," ");
        }
        strcat (temp, sent);//concatenate first part with my sent
        strcat(temp," ");

        if (temp[strlen(temp)-1]!= ' '){//if last letter of first part is not a space, add space
            strcat(temp, " ");
        }
        strcpy(myStr, temp);//edit the original string
    }

}
void insertTextFirst(char myStr [str_size], char sent [str_size]){
    char temp [str_size]="";
    strcpy(temp,sent);
    strcat(temp, " ");//to put space after the entered sent
    strcat(temp, myStr);
    strcpy(myStr, temp);

}
void userDeletionPush (char myStr [str_size], char sent [str_size], UndoStack* US){

        int index= findIndexTodelete(myStr,sent);
        if (index==-100){
            printf("Sorry, you are trying to delete Non- Existed word/s..!\n");
            return;
        }

        //we push deleted line(word) to Stack

        //here we push

        //create a stackNode then push it to Stack
        StackNode* mySNode= createStackNode();
        mySNode->nextS=NULL;
        strcpy(mySNode->str, sent);
        mySNode->position= index;
        mySNode->status= -2;//here we delete
        pushToStack(myStr,mySNode,US);
        printf("\nOriginal text was updated, your string was deleted..\n");



}
void deleteTextAfterPosition (char myStr [str_size], int index, char sent [str_size]){

    //to kill spaces in string entered
    int lengthDeleted= strlen(sent);
    while (sent[lengthDeleted-1]==' '){
        sent[lengthDeleted-1]='\0';//to kill the space
        lengthDeleted--;
    }

    //to kill the space
    int textLength= strlen(myStr);
    while (myStr[textLength-1]==' '){
        myStr[textLength-1]='\0';
        textLength--;
    }

    char temp [str_size]="";// "" is important when use strcpy

    //special case, if we inserted last, index=-1, and we want to delete this word from last
    if (index== -1){
        strncpy(temp, myStr, textLength-lengthDeleted);//1 to skip space, 1 to reach char after space
        strcpy(myStr,temp);
        return;
    }
    strncpy(temp, myStr,index);//store the first part of original text

    char *subStr = myStr + index + lengthDeleted+1;//to store the last part of original text// +1 to skip space

    if ( (lengthDeleted + index+1) < textLength ){//to avoid store letters after my original array end
        strcat (temp, subStr);// concatenate (first part) with last part
    }

    strcpy(myStr, temp);//edit the original string
}
int findIndexTodelete (char myStr [str_size], char knownStrPosition[str_size]){
   char temp [str_size]="";
    strcpy(temp,myStr);

    int textLength= strlen(myStr);
    int knownLength= strlen(knownStrPosition);

    char lastInsertedChar = knownStrPosition[knownLength -1];
    //handle space after position
    if (!((lastInsertedChar >='a' && lastInsertedChar<='z')||(lastInsertedChar >='A' && lastInsertedChar<='Z')
                   || (lastInsertedChar >='0' && lastInsertedChar<='9'))){

        knownStrPosition[knownLength-1]='\0';
        knownLength-=1;
    }



    char* subStr = strchr(myStr,knownStrPosition[0]);

    int result=-100;
    int index=-100;

    while(subStr!=NULL){

        int remainingChar= strlen(subStr);
        result=strncmp(subStr,knownStrPosition, knownLength);
        index= textLength - remainingChar;
        char lastOriginalChar= myStr[index+knownLength];
        if (result==0 ) {
            if  (!((lastOriginalChar >='a' && lastOriginalChar<='z')||(lastOriginalChar >='A' && lastOriginalChar<='Z')
                   || (lastOriginalChar >='0' && lastOriginalChar<='9'))){
                //we check the char after it to ensure that we delete complete words,
                //if "This" in text, and we want to delete "is", we ensure that "This" will not change to "Th"
                return index;
            }

        }

        subStr=strchr(subStr,subStr[1]);//to skip first char, ahmed --->>>> hmed
        subStr=strchr(subStr,knownStrPosition[0]);//retry to find the sent
    }

    if (subStr==NULL){
        return -100;//was not found
    }

}
void saveToFile(char myStr [str_size]){

    if (strlen(myStr)==0){
        printf("Sorry, there is no text to save!!\n");
    }
    else {
        FILE* output;
        output= fopen("outputFile.txt", "w");
        fprintf(output,"%s", myStr);
        fclose(output);
        printf("Data was stored in File..\n");
    }
}


//Stack
StackNode* createStackNode(){
    StackNode* mySNode= (StackNode*)malloc(sizeof(StackNode));
    if (mySNode==NULL){
        printf("Out Of Memory!!!\n");
        exit(0);
    }
    mySNode->nextS=NULL;
    //initialize int??
    return mySNode;

}
Stack* createStackHead(){
    Stack* mySHead= (Stack*)malloc(sizeof(Stack));
    if (mySHead==NULL){
        printf("Out Of Memory!!!\n");
        exit(0);
    }
    mySHead->Top=NULL;
    return mySHead;
}
void pushToStack (char myStr [str_size], StackNode* mySNode,Stack* S){
    if (S==NULL){
        printf("Sorry, there is no Stack\n");
        return;
    }
    if (mySNode==NULL){
        printf("There is no node to add to Stack!!!\n");
        return;
    }
    mySNode->nextS= S->Top;
    S->Top= mySNode;

    //we want to edit original text
    if (mySNode->status==2){//add string to original text

        if (mySNode->position==0){//means add first
            insertTextFirst(myStr,mySNode->str);
        }
        else {//means add after a position
            insertTextAfterPosition(myStr, mySNode->position, mySNode->str);
        }

    }

    else if (mySNode->status== -2){//delete string from original text
        deleteTextAfterPosition(myStr,mySNode->position,mySNode->str);
    }

}
void pushStr (char str [str_size], Stack* S){//reCheck
    if (S==NULL){
        printf("Sorry, there is no Stack\n");
        return;
    }

    StackNode* mySNode= createStackNode();
    strcpy(mySNode->str, str);
    //positions?? status?? index??

    mySNode->nextS= S->Top;
    S->Top= mySNode;
}
void pushToRedoStack (char myStr[str_size],UndoStack* US, RedoStack* RS){

    //first we pop from US, then we push them into RS
    //we will use pushToStack function that pushes, and edits the original text
    if(!isEmptyStack(US)){
    StackNode* mySNode= popWithReturn(US);
    //we will converse the operation, insert to delete, and delete to insert
    mySNode->status *= -1;
    pushToStack(myStr,mySNode,RS);
    printf("\nOriginal text was updated..\n");

    }
    else{
        printf("Sorry, there are no edits to Undo ..!\n");
    }

}
void pushToUndoStack (char myStr[str_size],UndoStack* US, RedoStack* RS){

    //first we pop from RS, then we push them into US

    //we will use pushToStack function that pushes, and edits the original text
    if(!isEmptyStack(RS)){

    StackNode* mySNode= popWithReturn(RS);
    //we will converse the operation, insert to delete, and delete to insert
    mySNode->status *= -1;
    pushToStack(myStr,mySNode,US);
    printf("\nOriginal text was updated..\n");
    }
    else{
        printf("Sorry, there are no edits to Redo ..!\n");
    }
}
StackNode* popWithReturn (Stack* S){
    if (S==NULL){
        printf("Sorry, there is no Stack\n");
        return;
    }

    //pops and returns the whole node
    if (isEmptyStack(S)){
        printf("Sorry, it is in Empty Stack\n");
        return;
    }
    else {
        StackNode* mySNode= S->Top;
        S->Top= mySNode->nextS;
        return mySNode;//we do not free it, we return it
    }
}
void popWithOutReturn (Stack* S){
    if (S==NULL){
        printf("Sorry, there is no Stack\n");
        return;
    }

    //pops and frees the whole node
    if (isEmptyStack(S)){
        printf("Sorry, it is in Empty Stack\n");
        return;
    }

    else {

        StackNode* mySNode= S->Top;
        S->Top= mySNode->nextS;
        free (mySNode) ;//we free it
    }

}
void makeEmptyStack (Stack* S){

        if (S==NULL){
            printf("There is no Stack to empty!!!\n");
            return;
        }

        while (!isEmptyStack(S)){
            popWithOutReturn(S);
        }


}
void printStacks (UndoStack* US, RedoStack* RS){

    //we pop elements then push to temp stack

    Stack* tempS= createStackHead();


    //print Undo Stack
    if (isEmptyStack(US)){
        printf("\nSorry, Undo Stack is empty..\n");
    }
    else {
        printf("Undo Stack:\n");
        printf("----------------------------------\n");
        StackNode* mySNode;
        while (!isEmptyStack(US)){

            mySNode= popWithReturn(US);//pops node from US

            char arr [10]="";
            if (mySNode->status==+2){
                strcpy(arr,"inserted");
            }
            else{
                strcpy(arr,"deleted");
            }

            printf("%-15s\t%-8s\t%-15d\n",mySNode->str,arr,mySNode->position);

            //now we push to tempS
             mySNode->nextS= tempS->Top;
             tempS->Top= mySNode;

        }

        //now we return elements from tempS to US
        while (!isEmptyStack(tempS)){

            mySNode= popWithReturn(tempS);//pops node from tempS

            mySNode->nextS= US->Top;
            US->Top= mySNode;
        }
    }


    //print Redo Stack
    if (isEmptyStack(RS)){
        printf("\n\nSorry, Redo Stack is empty..\n");

    }
    else {
        printf("\n\nRedo Stack:\n");
        printf("-----------------------------------\n");

        StackNode* mySNode;
        while (!isEmptyStack(RS)){

            mySNode= popWithReturn(RS);//pops node from RS

            char arr [10]="";
            if (mySNode->status==+2){
                strcpy(arr,"inserted");
            }
            else{
                strcpy(arr,"deleted");
            }

            printf("%-15s\t%-8s\t%-15d\n",mySNode->str,arr,mySNode->position);

            //now we push to tempS
             mySNode->nextS= tempS->Top;
             tempS->Top= mySNode;

        }
        //now we return elements from tempS to RS
        while (!isEmptyStack(tempS)){

            mySNode= popWithReturn(tempS);//pops node from tempS

            mySNode->nextS= RS->Top;
            RS->Top= mySNode;
        }
    }
    printf("\n");
    free(tempS);//it is surly empty



}
int isEmptyStack (Stack* S){
    if (S==NULL){
        printf("Sorry, There is no stack!!!\n");
        return;
    }
    return S->Top==NULL;
}

//Queue
QueueNode* createQueueNode (){
    QueueNode* myQNode= (QueueNode*)malloc(sizeof(QueueNode));
    if (myQNode==NULL){
        printf("Out Of Memory!!!\n");
        exit(0);
    }
    myQNode->nextQ=NULL;
    return myQNode;
}
Queue* createQueueHead (){
    Queue* myQHead= (Queue*)malloc(sizeof(Queue));
    if (myQHead==NULL){
        printf("Out Of Memory!!!\n");
        exit(0);
    }
    myQHead->Front=NULL;
    myQHead->Rear=NULL;
    return myQHead;

}
void EnQueueOp (char str[str_size], Queue* Q){
    if (Q==NULL){
        printf("Sorry, there is no Queue\n");
        return;
    }

    QueueNode* myQNode= createQueueNode();
    strcpy(myQNode->str, str);//Setting String
    myQNode->nextQ=NULL;

    //no nodes in the Queue
    if (isEmptyQueue(Q)){
        Q->Front= myQNode;//pointer to first added node
        Q->Rear= myQNode;//pointer to node
    }


    //if there are nodes in the Queue
    else {
       //the last element in the Queue is Q->Rear
       Q->Rear  ->nextQ  = myQNode;//connect last node to me new node
       Q->Rear= myQNode;//Rear is directed to the new Enqueued node
    }
}
QueueNode* DeQueueWithReturn (Queue* Q){
    if (Q==NULL){
        printf("Sorry, there is no Queue\n");
        return;
    }
    if (isEmptyQueue(Q)){
        printf("Sorry, this is an Empty Queue!!!\n");
        return;
    }

    //means there are nodes in the Queue
    QueueNode* myQNode= Q->Front;
    Q->Front= myQNode->nextQ;//Head is directed to the 2nd node


    if (isEmptyQueue(Q)){//means that myQNode was the only node, and after Dequeuing it, the Queue is empty
        //means that Q->Front=NULL;
        Q->Rear=NULL;//to ensure the behavior of Rear pointer
    }

    return myQNode;
}
void DeQueueWithOutReturn (Queue* Q){
    if (Q==NULL){
        printf("Sorry, there is no Queue\n");
        return;
    }
    if (isEmptyQueue(Q)){
        printf("Sorry, this is an Empty Queue!!!\n");
        return;
    }

    //means there are nodes in the Queue
    QueueNode* myQNode= Q->Front;
    Q->Front= myQNode->nextQ;//Head is directed to the 2nd node


    if (isEmptyQueue(Q)){//means that myQNode was the only node, and after Dequeuing it, the Queue is empty
        //means that Q->Front=NULL;
        Q->Rear=NULL;//to ensure the behavior of Rear pointer
    }

    free(myQNode) ;
}
void EnQueueAddedLine (char addedStr [str_size], Queue* Q){
    int realSize=0;
    char words[maxArraySize][str_size];

    char *p= strtok(addedStr, " ");

    while (p!=NULL){

        strcpy( words[realSize], p );
        realSize++;
        p=strtok(NULL," ");//NULL is set where " " is found

    }

    for (int i=0; i<realSize; i++){
        EnQueueOp (words[i], Q);
    }
}
void DeQueueThenPushToUndoStack (char myStr [str_size],char addedStr [str_size], Queue* Q, UndoStack* US){
    if (Q==NULL || US==NULL){
        printf("Sorry, you may entered NULL thing!!\n");
        return;
    }
    if (isEmptyQueue(Q)){
        printf("Sorry, the Queue is empty!!\n");
        return;
    }


    //there are elements in the Queue
    //we need to push their names to the Undo Stack
    int startIndex;

    int choice;
    char knownPosition [str_size]="";//stores the word that i will add after it
    do {
        printf("\nWhere do you want to add this string?\n");
        printf("1)At the beginning of the original text\n");
        printf("2)After a known word in original text\n");


        scanf("%d", &choice);
        getchar();//the user will have time to write the string

        if (choice==1){//adds first
            startIndex=0;
        }
        else if (choice==2){
             printf("Enter a known word to add after it: ");
             strcpy(knownPosition,"");//to ensure clearing
            gets(knownPosition);
            startIndex= findIndexToAdd(myStr, knownPosition);
            if (startIndex== -100){//means knownsPosition str was not found in the original text
                printf("Sorry, known word you entered was not found in the original text!!\n");
                printf("please, try again\n");
                choice= -1;// to let the loop turns more time
            }
        }

        else {
            printf("Sorry, you may entered a wrong choice, please try again\n");
        }
    }
    while (choice!=1 && choice!=2  );

    char strToPush [str_size]="";//temp string to take the string stored in the front to use it to create nodeStack to push
    //now, we know where to add,

        if (startIndex ==0){//choice=1
        //means all words entered will be added at the beginning

        int varIndex1=0;//first cell inserted at the beginning, and other cells entered are after it

        while (!isEmptyQueue(Q)){

        strcpy(strToPush,Q->Front->str);//copy string of front, to my temp string then to use it to create StackNode
        DeQueueWithOutReturn(Q);

        //create a stackNode then push it to UndoStack
        StackNode* mySNode= createStackNode();
        mySNode->nextS=NULL;
        strcpy(mySNode->str, strToPush);
        mySNode->position= varIndex1;//all words in this case will be added at the beginnig, --->>> index= 0 always
        varIndex1+= strlen(strToPush)+1;// increment position by word length, & 1 for space
        mySNode->status=2;//here we add

        //now push to UndoStack
        pushToStack(myStr,mySNode,US);


        //the increment of the loop is done by EnQueue method
        }

        return;//exits
    }


    //to reach here, means we will add after a word
    //note we have the start index
    int varIndex2= startIndex;//to determine location of next words(after first word with start index)


    //we are about to Enqueue data then push it to Undo stack
    while (!isEmptyQueue(Q)){

        strcpy(strToPush,Q->Front->str);//copy string of front, to my temp string then to use it to create StackNode
        DeQueueWithOutReturn(Q);

        //create a stackNode then push it to UndoStack
        StackNode* mySNode= createStackNode();
        mySNode->nextS=NULL;
        strcpy(mySNode->str, strToPush);
        mySNode->position= varIndex2;
        varIndex2+= strlen(strToPush)+1;// increment position by word length, & 1 for space
        mySNode->status=2;//here we add

        //now push to UndoStack
        pushToStack(myStr,mySNode,US);
        //the increment of the loop is done by EnQueue method
    }

}
int isEmptyQueue (Queue* Q){
    if (Q==NULL){
        printf("Sorry, There is no Queue!!!\n");
        return;
    }
    return Q->Front==NULL;
}
int isLastQueueNode (QueueNode* myQNode, Queue* Q){
     if (Q==NULL){
        printf("Sorry, there is no Queue\n");
        return;
    }
     if (myQNode==NULL){
        printf("Sorry, this is NULL, not a Queue Node\n");
        return;
    }
    //things are right
    return myQNode->nextQ==NULL;
}


///done



