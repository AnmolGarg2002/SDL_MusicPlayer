#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <gtk/gtk.h>
#include <string.h>
#include "Header1.h"

static const int width = 200;
static const int height = 50;

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
    fgets(str,1024,fp);
    while (fgets(str,1024,fp)!=NULL)
    {
        Song *sng;
        sng=(Song*)malloc(sizeof(Song));
        sng->name=strdup(strtok(str,","));
        sng->artist=strdup(strtok(NULL,","));
        sng->album=strdup(strtok(NULL,","));
        sng->length=strdup(strtok(NULL,","));
        sng->index=i;
        //printf("%s\n",sng->name);
        insert_head(p,sng);
        i++;
    }
    fclose(fp);
}


FILE *fp;

/* User clicked the "Add List" button. */
void button_add_clicked( gpointer data )
{
                           
  /*DIR *dir;
  struct dirent *ent;
  if ((dir = opendir ("music")) != NULL) 
  {
  
       Here we do the actual adding of the text. It's done once for
      each row.
     
     	int i=1;
	char snum[5];
	while ((ent = readdir (dir)) != NULL)
	 {
		  sprintf(snum, "%d", i++);
		  char* arr[4] = {snum,ent->d_name,"Anmol","2:54"};
		  if(strcmp(arr[1],"..") && strcmp(arr[1],"."))
		  {
		  	gtk_clist_append((GtkCList *)data,arr);
		  }
		  else
		  {
		  	i--;
		  }
	 }
	 closedir (dir);
   }*/
   
    gtk_clist_clear( (GtkCList *) data);
    
   	List ls;
	initialize(&ls);
	readFile(&ls,"Music.csv");
	Node *start = ls.head;
	Node *end = ls.tail;
	int i=1;
	char snum[5];
	while(end!=ls.head)
	{
		sprintf(snum, "%d", i++);
		char *name = (end->sng)->name;
		char *artist = (end->sng)->artist;
		char *album = (end->sng)->album;
		char *length = (end->sng)->length;
		length = strtok(length,"\n");
		//printf("%s",length);
		char* arr[5] = {snum,name,artist,album,length};
		gtk_clist_append((GtkCList *)data,arr);
		end = end->prev;
	}
	sprintf(snum, "%d", i++);
	char *name = (end->sng)->name;
	char *artist = (end->sng)->artist;
	char *album = (end->sng)->album;
	char *length = (end->sng)->length;
	char* arr[5] = {snum,name,artist,album,length};
	gtk_clist_append((GtkCList *)data,arr);
	end = end->prev;

    return;
}

/* User clicked the "Clear List" button. */
void button_clear_clicked( gpointer data )
{
    /* Clear the list using gtk_clist_clear. This is much faster than
     * calling gtk_clist_remove once for each row.
     */
    gtk_clist_clear( (GtkCList *) data);

    return;
}

/* If we come here, then the user has selected a row in the list. */
void selection_made( GtkWidget      *clist,
                     gint            row,
                     gint            column,
                     GdkEventButton *event,
                     gpointer        data )
{
    gchar *name;
    gchar *artist;
    gchar *album;
    gchar *length;

    /* Get the text that is stored in the selected row and column
     * which was clicked in. We will receive it as a pointer in the
     * argument text.
     */
    gtk_clist_get_text(GTK_CLIST(clist), row, 1, &name);
    gtk_clist_get_text(GTK_CLIST(clist), row, 2, &artist);
    gtk_clist_get_text(GTK_CLIST(clist), row, 3, &album);
    gtk_clist_get_text(GTK_CLIST(clist), row, 4, &length);
    
    /* Just prints some information about the selected row */
    g_print("%s,%s,%s,%s,\n",name,artist,album,length);
    fprintf(fp,"%s,%s,%s,%s,\n",name,artist,album,length);

    return;
}

int main( int    argc,
          gchar *argv[] )
{       
    fp = fopen("data.txt","w");
                           
    GtkWidget *window;
    GtkWidget *vbox, *hbox;
    GtkWidget *scrolled_window, *clist;
    GtkWidget *button_add, *done;    
    gchar *titles[5] = { "Sno", "Name\t\t\t\t\t\t\t\t\t\t\t\t\t\t","Artist\t\t\t\t","Album\t\t\t\t","Length" };

    gtk_init(&argc, &argv);
    
    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_widget_set_usize(GTK_WIDGET(window), 1500, 500);

    gtk_window_set_title(GTK_WINDOW(window), "Music Player");
    gtk_signal_connect(GTK_OBJECT(window),
                       "destroy",
                       GTK_SIGNAL_FUNC(gtk_main_quit),
                       NULL);
    
    vbox=gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);
    
    /* Create a scrolled window to pack the CList widget into */
    scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

    gtk_box_pack_start(GTK_BOX(vbox), scrolled_window, TRUE, TRUE, 0);
    gtk_widget_show (scrolled_window);

    /* Create the CList. For this example we use 2 columns */
    clist = gtk_clist_new_with_titles( 5, titles);

    /* When a selection is made, we want to know about it. The callback
     * used is selection_made, and its code can be found further down */
    gtk_signal_connect(GTK_OBJECT(clist), "select_row",
                       GTK_SIGNAL_FUNC(selection_made),
                       NULL);

    /* It isn't necessary to shadow the border, but it looks nice :) */
    gtk_clist_set_shadow_type (GTK_CLIST(clist), GTK_SHADOW_OUT);

    /* What however is important, is that we set the column widths as
     * they will never be right otherwise. Note that the columns are
     * numbered from 0 and up (to 1 in this case).
     */
    gtk_clist_set_column_width (GTK_CLIST(clist), 0, 150);

    /* Add the CList widget to the vertical box and show it. */
    gtk_container_add(GTK_CONTAINER(scrolled_window), clist);
    gtk_widget_show(clist);

    /* Create the buttons and add them to the window. See the button
     * tutorial for more examples and comments on this.
     */
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, TRUE, 0);
    gtk_widget_show(hbox);

    button_add = gtk_button_new_with_label("Add List");
    done = gtk_button_new_with_label("DONE");

    gtk_box_pack_start(GTK_BOX(hbox), button_add, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), done, TRUE, TRUE, 0);

    /* Connect our callbacks to the three buttons */
    gtk_signal_connect_object(GTK_OBJECT(button_add), "clicked",
                              GTK_SIGNAL_FUNC(button_add_clicked),
                              (gpointer) clist);
    gtk_signal_connect_object(GTK_OBJECT(done), "clicked",
                              GTK_SIGNAL_FUNC(gtk_main_quit),
                              (gpointer) clist);

    gtk_widget_show(button_add);
    gtk_widget_show(done);

    /* The interface is completely set up so we show the window and
     * enter the gtk_main loop.
     */
    gtk_widget_show(window);
    gtk_main();
    return(0);
}
