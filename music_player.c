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

int mprev = 0;
int count = 0;
int posn = 0;
int open = 1;
int shuffle = 0;

void play(char *file)
{
    int next = 0;
    int prev = 0;
    int pause = 0;
    
    // Initialize SDL video and audio systems
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    TTF_Init();

    // Initialize SDL mixer
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    // Load audio files
    // Mix_Music *music = Mix_LoadMUS(MY_COOL_MP3);
    Mix_Music *backgroundSound = Mix_LoadMUS(file);

    // Create a SDL window
    SDL_Window *window = SDL_CreateWindow("Playing song", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,0);

    // Create a renderer (accelerated and in sync with the display refresh rate)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    //SDL_SetRenderDrawColor(renderer,0, 0, 0, 255);
	
    	 TTF_Font * font = TTF_OpenFont("AbyssinicaSIL-R.ttf", 20);
	 const char * error = TTF_GetError();
	 SDL_Color color = { 255, 255, 255 };
	 strtok(file,"/");
	 char *file_name = strtok(NULL,".");
	 SDL_Surface * surface = TTF_RenderText_Solid(font,
	  file_name, color);
	 SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer,
	  surface);

	 int texW = 0;
	 int texH = 0;
	 SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	 SDL_Rect dstrect = { 0, 0, texW, texH };

    // Start the background music
    Mix_PlayMusic(backgroundSound,0);

    bool running = true;
    SDL_Event event;
    while(running)
    {
        // Process events
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = false;
                open = 0;
            }
            else if(event.type == SDL_KEYDOWN)
            {
            	// Press "ESC" and quit
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                	running = false;
                	// Release resources
			//SDL_DestroyTexture(texture);
			//SDL_FreeSurface(surface);
			TTF_CloseFont(font);

			//SDL_DestroyRenderer(renderer);
			//SDL_DestroyWindow(window);
			Mix_FreeMusic(backgroundSound);
			Mix_CloseAudio();
			TTF_Quit();
			//SDL_Quit();
                }
                // Press "SPACE" and pause
                if(event.key.keysym.sym == SDLK_SPACE)
                {
                	if(pause==0)
                	{
				Mix_PauseMusic();
				pause = 1;
			}
			else
			{
				Mix_ResumeMusic();
				pause=0;
			}
                }
                // Press "S" and shuffle
                if(event.key.keysym.sym == SDLK_s)
                {
                	running = false;
                	if(shuffle==0)
                	{
				shuffle = 1;
			}
			else
			{
				shuffle = 0;
			}
                }
                // Press ">"
                if(event.key.keysym.sym == SDLK_RIGHT)
                {
                	running = false;
                }
                // Press ">"
                if(event.key.keysym.sym == SDLK_LEFT)
                {
                	running = false;
                	prev = 1;
                }
            }
        }

	SDL_RenderCopy(renderer, texture, NULL, &dstrect);

        // Draw

        // Show what was drawn
        SDL_RenderPresent(renderer);
    }

    // Release resources
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(backgroundSound);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
    
    sleep(1);
    
    mprev = prev;

}

/* User clicked the "Add List" button. */
void button_add_clicked( gpointer data )
{
	initialize(&ls);
	readFile(&ls,"data.txt");
	Node *start = ls.head;
	Node *end = ls.tail;
	int i=1;
	char snum[5];
	gtk_clist_clear( (GtkCList *) data);
        count = 0;
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
		count++;
	}
	sprintf(snum, "%d", i++);
	char *name = (end->sng)->name;
	char *artist = (end->sng)->artist;
	char *album = (end->sng)->album;
	char *length = (end->sng)->length;
	char* arr[5] = {snum,name,artist,album,length};
	gtk_clist_append((GtkCList *)data,arr);
	end = end->prev;
	count++;

    return;
}

/* User clicked the "Clear List" button. */
void button_clear_clicked( gpointer data )
{
    /* Clear the list using gtk_clist_clear. This is much faster than
     * calling gtk_clist_remove once for each row.
     */
    gtk_clist_clear( (GtkCList *) data);
    count = 0;

    return;
}

void shuffle1( gpointer data )
{
	if(shuffle ==0)
		shuffle = 1;
	else
		shuffle = 0;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the null-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

/* If we come here, then the user has selected a row in the list. */
void selection_made( GtkWidget      *clist,
                     gint            row,
                     gint            column,
                     GdkEventButton *event,
                     gpointer        data )
{
    gchar *text;
    open = 1;

    /* Get the text that is stored in the selected row and column
     * which was clicked in. We will receive it as a pointer in the
     * argument text.
     */
    gtk_clist_get_text(GTK_CLIST(clist), row, column, &text);

    /* Just prints some information about the selected row */
    g_print("You selected row %d. More specifically you clicked in "
            "column %d, and the text in this cell is %s\n\n",
            row, column, text);
    
    char *name = search(row);
    //gtk_clist_get_text(GTK_CLIST(clist), row, 1, &name);
    printf("%s",name); 
    posn = row;  
	
	char *c1 = concat("music/",name);
	play(concat(c1,".ogg"));
	
	if(mprev == 1)
	{
		if(posn==0)
		{
			mprev = 0;
			posn = count-1;
			selection_made(clist , posn , column , event , data);
			
		}
		else
		{
			mprev = 0;
			posn--;
			selection_made(clist , posn , column , event , data);
		}
	}
	else if(open && shuffle)
	{
		posn = random_num();
		selection_made(clist , posn , column , event , data);

	}
	else if(open)
	{
		if(posn==count-1)
		{
			posn = 0;
			selection_made(clist , posn , column , event , data);
		}
		else
		{
			posn++;
			selection_made(clist , posn , column , event , data);
		}
	}

    return;
}

int main( int    argc,
          gchar *argv[] )
{                                  
    GtkWidget *window;
    GtkWidget *vbox, *hbox;
    GtkWidget *scrolled_window, *clist;
    GtkWidget *button_add, *button_clear, *button_shuffle;    
    gchar *titles[5] = { "Sno", "Name\t\t\t\t\t\t\t\t\t\t\t\t\t\t","Artist\t\t\t\t","Album\t\t\t\t\t\t","Length" };

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
    button_clear = gtk_button_new_with_label("Clear List");
    button_shuffle = gtk_toggle_button_new_with_label ("Shuffle");

    gtk_box_pack_start(GTK_BOX(hbox), button_add, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_clear, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), button_shuffle, TRUE, TRUE, 0);

    /* Connect our callbacks to the three buttons */
    gtk_signal_connect_object(GTK_OBJECT(button_add), "clicked",
                              GTK_SIGNAL_FUNC(button_add_clicked),
                              (gpointer) clist);
    gtk_signal_connect_object(GTK_OBJECT(button_clear), "clicked",
                              GTK_SIGNAL_FUNC(button_clear_clicked),
                              (gpointer) clist);
    gtk_signal_connect_object(GTK_OBJECT(button_shuffle), "clicked",
                              GTK_SIGNAL_FUNC(shuffle1),
                              (gpointer) clist);

    gtk_widget_show(button_add);
    gtk_widget_show(button_clear);
    gtk_widget_show(button_shuffle);

    /* The interface is completely set up so we show the window and
     * enter the gtk_main loop.
     */
    gtk_widget_show(window);
    gtk_main();
    
    return(0);
}
