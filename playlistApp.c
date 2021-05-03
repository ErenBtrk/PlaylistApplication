/*
    Playlist application in C with data structures.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct n{
    char songName[50];
    struct n*next;
    struct n*prev;
}node;

//function prototypes
void printPlaylist( node * , char );
void insertSong( node ** ,  char [] , char );
void moveSong( node ** , char [] , char [] , char );
void removeSong( node ** , char []  );
void playSong( node ** , char , node ** );
void reversePlaylist( node**);





int main(){
    const char *functionNames[6]={"ReversePlaylist","PrintPlaylist","PlaySong",
                                                        "InsertSong","MoveSong","RemoveSong"};
    char buffer[75];
    char functionName [50];
    char mode;
    char songName1[50];
    char songName2[50];

    FILE *deletePtr;
    if( (deletePtr = fopen("output.txt","w")) == NULL){
        printf("Dosya acilamadi.\n");
    }
    else{
        fclose(deletePtr);
    }




    FILE *rPtr;

    if ( (rPtr = fopen("input.txt","r")) == NULL ){
        printf("Dosya acilamadi.\n");
    }
    else{
        node *root = NULL;
        node *playingSong = NULL;
        playingSong = (node*)malloc(sizeof(node));


        while( fgets(buffer,50,rPtr) != NULL ){

            sscanf(buffer,"%s\t",functionName);
            if( strstr(functionName , functionNames[0]) != NULL ){
                reversePlaylist(&root);
            }
            else if( strstr(functionName,functionNames[1] ) != NULL ){
                sscanf(buffer,"%s\t%c\t",functionName,&mode);
                printPlaylist(root,mode);
            }

            else if( strstr(functionName,functionNames[2] ) != NULL ){
                sscanf(buffer,"%s\t%c\t",functionName,&mode);
                playSong(&root,mode,&playingSong);

            }

            else if( strstr(functionName,functionNames[3] ) != NULL ){
                sscanf(buffer,"%s\t%c\t%s\t",functionName,&mode,songName1);
                insertSong(&root,songName1,mode);
            }
            else if( strstr(functionName,functionNames[4] ) != NULL ){
                sscanf(buffer,"%s\t%c\t%s\t%s\t",functionName,&mode,songName1,songName2);
                moveSong(&root,songName2,songName1,mode);
            }
            else if( strstr(functionName,functionNames[5] ) != NULL ){
                sscanf(buffer,"%s\t%s\t",functionName,songName1);
                char control[50]="";
                if(playingSong != NULL){
                    sprintf(control,"%s",playingSong->songName);
                }
                if( strcmp(control,songName1) == 0){
                    FILE *wPtr;
                        if((wPtr = fopen("output.txt","a")) == NULL){
                            printf("Dosya acilamadi.\n");
                        }
                        else{
                            fprintf(wPtr,"Cannot remove the playing song.\n*****\n");
                            fclose(wPtr);
                        }
                }
                else{
                    removeSong(&root,songName1);
                }


            }


        }

    }
    fclose(rPtr);

    return 0;
}

void printPlaylist(node *root , char mode){
    FILE *ptr;
    if( (ptr = fopen("output.txt","a") )  == NULL ){
        printf("Dosya acilamadi.\n");
    }
    else{

        switch ( mode ){
        case 'F':case 'f':
            if( root == NULL ){
                fprintf(ptr,"No songs to print.\n");
            }
            else{
                while ( root != NULL ){
                    fprintf( ptr , "%s\n" , root -> songName );
                    root = root -> next;
                }
            }
        break;

        case 'R':case 'r':
            if( root == NULL ){
                fprintf(ptr,"No songs to print.\n");
            }
            else{
                node *iter;
                iter = root;
                while( iter->next != NULL){
                    iter = iter->next;
                }
                while( iter != NULL ){
                    fprintf( ptr , "%s\n" , iter -> songName );
                    iter = iter -> prev;
                }
            }
        break;
        }
    }
    fprintf(ptr,"*****\n");
    fclose(ptr);

}

void insertSong(node** root,char name[],char mode){
    node* tempRoot;
    tempRoot = *root;
    switch( mode ){
    case 'H': case 'h':
        if( tempRoot == NULL ){
            tempRoot = (node*)malloc(sizeof(node));
            strcpy( tempRoot -> songName , name );
            tempRoot -> next = NULL;
            tempRoot -> prev = NULL;
            *root = tempRoot;
        }
        else{
            node* temp;
            temp = (node*)malloc(sizeof(node));
            strcpy( temp -> songName , name );
            temp -> next = tempRoot;
                if(temp->next != NULL)//ONEMLI
                    temp->next->prev = temp ;
            temp -> prev = NULL;
            *root = temp;
        }
        break;
    case 'T':case 't':
        if( tempRoot == NULL ){
            tempRoot = (node*)malloc(sizeof(node));
            strcpy( tempRoot -> songName , name );
            tempRoot -> next = NULL;
            tempRoot -> prev = NULL;
            *root = tempRoot;
        }
        else{
            node *iter;
            iter = tempRoot;
            while( iter -> next != NULL ){
                iter = iter -> next;
            }
            iter -> next = (node*)malloc(sizeof(node));
            strcpy( iter ->next -> songName , name );
            iter -> next -> next = NULL;
            iter -> next -> prev = iter;
            *root = tempRoot;
        }
        break;
    }
}

void moveSong(node** root,char refSong[],char movingSong[],char mode){
    node *tempRoot;
    tempRoot = (node*)malloc(sizeof(node));
    tempRoot = *root;
    node* iterMovSong;
    iterMovSong = (node*)malloc(sizeof(node));
    iterMovSong = *root;
    node * iterRefSong;
    iterRefSong = (node*)malloc(sizeof(node));
    iterRefSong = *root;

    while ( strcmp( iterMovSong->songName , movingSong) != 0 && iterMovSong->next != NULL ){
            iterMovSong = iterMovSong -> next;
    }
    while ( strcmp( iterRefSong->songName , refSong) != 0 && iterRefSong->next != NULL ){
            iterRefSong = iterRefSong -> next;
    }

    if(iterMovSong ->prev != NULL && iterMovSong ->next != NULL){
        iterMovSong->prev->next = iterMovSong->next;
        iterMovSong->next->prev = iterMovSong->prev;
    }
    if(iterMovSong ->prev == NULL && iterMovSong ->next != NULL){
        iterMovSong->next->prev = NULL;
    }
    if(iterMovSong ->prev != NULL && iterMovSong ->next == NULL){
        iterMovSong->prev->next = NULL;
    }

    switch( mode ){
    case 'B': case 'b':
        if( iterRefSong ->prev == NULL){
            iterMovSong->next = iterRefSong;
            iterMovSong->prev = NULL;
             iterRefSong -> prev = iterMovSong;
            *root = iterMovSong;
        }
        else{
            iterMovSong -> next = iterRefSong;
            iterMovSong -> prev = iterRefSong -> prev;
            iterRefSong ->prev->next = iterMovSong;
            iterRefSong -> prev = iterMovSong;
            *root = tempRoot;
        }

        break;
    case 'A': case 'a':
        if(iterRefSong -> next == NULL){
            iterMovSong -> next = NULL;
            iterMovSong -> prev = iterRefSong;
            iterRefSong->next = iterMovSong;
        }
        else{
            iterMovSong -> next = iterRefSong->next;
            iterMovSong -> prev = iterRefSong;
            iterRefSong->next->prev = iterMovSong;
            iterRefSong->next = iterMovSong;
        }

        *root = tempRoot;

        break;
    }
}

void removeSong(node** root,char name[]){
    node* tempRoot;
    tempRoot = *root;




    if( tempRoot == NULL ){
        *root = NULL;
    } // Listenin bos olmasi durumu
    else if( strcmp( tempRoot -> songName , name ) == 0 && tempRoot -> next == NULL ){
        node* temp;
        temp = tempRoot;
        free(temp);
        *root = NULL;
    }
    else if( strcmp( tempRoot -> songName , name ) == 0 && tempRoot -> next != NULL){
        node* temp;
        temp = tempRoot;
        tempRoot = tempRoot -> next;
        tempRoot -> prev = NULL;
        free(temp);
        *root = tempRoot;
    }
    else{
        node* iter;
        iter = tempRoot;
        while ( strcmp ( iter -> next -> songName , name ) != 0 && iter->next != NULL ){
            iter = iter -> next;
        }
        if( iter -> next == NULL ){ // Sarkinin bulunamamasi durumu
            printf("Sarki bulunamadi.\n");
        }
        node* temp;
        temp = iter -> next;
        iter -> next = iter -> next -> next ;
        free(temp);
        if( iter -> next != NULL ){
            iter -> next -> prev = iter;
        }
        *root = tempRoot;
    }

}

void playSong(node** root,char mode,node** playingSong){
    FILE *wPtr;

    node *tempRoot;
    tempRoot = *root;
    node *tempPlayingSong;
    tempPlayingSong = *playingSong;



    if( (wPtr = fopen("output.txt","a")) == NULL ){
        printf("Dosya acilamadi.\n");
    }
    else{
        if( tempRoot == NULL ){
            fprintf(wPtr,"No songs to play\n*****\n");
            *playingSong = tempRoot;
            fclose(wPtr);
            return;

        }
        else if( tempRoot ->next == NULL ){
            fprintf(wPtr,"Playing %s\n*****\n",tempRoot->songName);
            *playingSong = *root;
            fclose(wPtr);
            return;
        }

        else if( tempPlayingSong == NULL && tempRoot != NULL ){
            switch(mode){
            case 'N': case 'n':
                fprintf(wPtr,"Playing %s\n*****\n",tempRoot->songName);
                *playingSong = tempRoot;
                break;
            case 'P': case 'p':
                while(tempRoot->next != NULL){
                    tempRoot = tempRoot -> next;
                }
                fprintf(wPtr,"Playing %s\n*****\n",tempRoot->songName);
                *playingSong = tempRoot;
                break;
            }
            fclose(wPtr);
            return;
        }
        else if(tempPlayingSong != NULL && tempRoot != NULL){
            switch(mode){
                case 'N': case 'n':
                    if(tempPlayingSong ->next == NULL){
                        fprintf(wPtr,"Playing %s\n*****\n",tempRoot->songName);
                        *playingSong = tempRoot;
                    }
                    else{
                        tempPlayingSong = tempPlayingSong->next;
                        fprintf(wPtr,"Playing %s\n*****\n",tempPlayingSong->songName);
                        *playingSong = tempPlayingSong;
                    }
                    break;
                case 'P': case 'p':
                    if(tempPlayingSong ->prev == NULL){
                        while( tempRoot -> next != NULL){
                            tempRoot = tempRoot ->next;
                        }
                        fprintf(wPtr,"Playing %s\n*****\n",tempRoot->songName);
                        *playingSong = tempRoot;
                    }
                    else{
                        tempPlayingSong = tempPlayingSong->prev;
                        fprintf(wPtr,"Playing %s\n*****\n",tempPlayingSong->songName);
                        *playingSong = tempPlayingSong;
                    }
                    break;
            }
            fclose(wPtr);
            return;
        }

    }
    fclose(wPtr);
}
void reversePlaylist( node **root)
{
     if( *root == NULL ){
        return;
      }
      node* current;
      current = *root;

      node* temp = NULL;


    while( current != NULL ){
        temp=current->prev;
        current->prev=current->next;
        current->next=temp;
        current=current->prev;
    }
     if(temp != NULL )
        *root = temp->prev;

}

