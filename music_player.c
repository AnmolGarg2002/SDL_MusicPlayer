#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <gtk/gtk.h>
#include <string.h>
#include "Header1.h"

static const int width = 1000;
static const int height = 50;

List ls;

int random_num()
{
    int r =rand()%ls.number_of_nodes;
    return r;
}

void initialize(List *p)
{
    p->head=NULL;
    p->tail=NULL;
    p->number_of_nodes=0;
}

void insert_head(List* p, Song *s)
{
    Node *temp;
    temp=(Node*)malloc(sizeof(Node));
    temp->sng=s;
    temp->prev=temp->next=NULL;
    
    
    if(p->head==NULL)
        p->head=p->tail=temp;
    
    else
    {
        temp->next=p->head;
        temp->prev=p->tail;
        p->head->prev=temp;
        p->tail->next=temp;
        p->head=temp;
    }
    p->number_of_nodes++;
}

void readFile(List *p,char* fileName)
{
    FILE* fp;
    fp=fopen(fileName, "r");
    if (fp==NULL)
    {
        printf("Unable to open file\n");
        exit(0);
    }
    int i=0;
    char str[1024];
    while (fgets(str,1024,fp)!=NULL)
    {
        Song *sng;
        sng=(Song*)malloc(sizeof(Song));
        sng->name=strdup(strtok(str,","));
        sng->artist=strdup(strtok(NULL,","));
        sng->album=strdup(strtok(NULL,","));
        sng->length=strdup(strtok(NULL,","));
        sng->index=i;
        insert_head(p,sng);
        i++;
    }
    fclose(fp);
}

char* search(int posn)
{
	Node *end = ls.tail;
	int no = ls.number_of_nodes;
	int i = 0;
	char *ch;
	while(i<no)
	{
		if(posn == i)
		{
			ch = end->sng->name; 
			printf("%s %d %d\n",ch,posn,i);
			return(ch);
		}
		else
		{
			end = end->prev;
		}
		i++;
	}
	return(NULL);
}	
