#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ADD 1
#define LIST 2
#define SEARCH 3
#define SAVE 4
#define QUIT 5

#define MAXLINELENGTH 100
#define EVENT_NUMBER 1000

struct event
{
    char* name; //name of events
    int day; 
    int month;
    int year;
    int cycle; //the recurring cycle for repeated events
    int importance; //to evaluate how important an event is.
};

struct event * event_list[EVENT_NUMBER];


void diaplay(void);
void go(void);
void do_option(int);
void loadevent(void);
void removeNewLine(char*);
struct event *createEvent(char*, int, int, int, int, int);
void addevent(void);
void day_increment(int*, int*, int*, int);
void searchevent(void);
int cmp_w_now(int, int, int, int);
void list_importance(struct event**, int);
void list_urgency(struct event**, int);
void listevents(void);
void savediary(void);
int leap_feb(int, int);

struct event *createEvent(char* name, int day, int month, int year, int cycle, int importance) 
{
    struct event *event = calloc(sizeof(struct event),1);
    event->name = calloc(sizeof(char), strlen(name) + 1);
    strcpy(event->name,name);
    
    event->day = day;
    
    event->month = month;
    
    event->year = year;
    
    event->cycle = cycle;
    
    event->importance = importance;
    return event;
}



void display()
{
    printf("ღ EVENT DIARY ღ\n");
    printf("%d. Add a new event\n", ADD);
    printf("%d. List the events\n", LIST);
    printf("%d. Search an event\n", SEARCH);
    printf("%d. Save the diary\n", SAVE);
    printf("%d. Quit\n", QUIT);
    printf("NOTE: enter '0' if you are using the diary for the first time or you'd like to reset your diary.\n");
}


void go()
{
    
    while(true)
    {
        display();
        char a[2];
        int option;
        scanf("%s",a);
        if (sscanf(a,"%d",&option))
        {
            if (option == QUIT)
            {
                printf("Would you like to save your edits? (y/n)\n");
                char a[2];
                scanf("%s",a);
                if(a[0] =='y')
                {
                    savediary();
                    break;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (option == 0) //reset the diary: clear the event file amd the event_list array
                {
                    FILE *clearFile = fopen("event","w");
                    fclose(clearFile);
                    memset(event_list, 0, sizeof(struct event*)*EVENT_NUMBER); 

                }
                else
                {
                    do_option(option);
                    printf("--------------------------------\n"); // look nicer
                }
            }
        
        }
        else
        {
            printf("\nSorry - don't recognise that option, try again\n");
        }
       
    }
    
}


void do_option(int option)
{
    switch (option)
    {
        case ADD : addevent(); break;
        case LIST : listevents(); break;
        case SEARCH : searchevent(); break;
        case SAVE : savediary(); break;
        default : printf("\nSorry - don't recognise that option, try again\n");
    }
    
}



void loadevent() 
{
    FILE *inputFile = fopen("event","r"); //read events from the event file
    int index = 0;
    char name[MAXLINELENGTH];
    int day;
    int month;
    int year;
    int cycle;
    int importance;
    while (index < EVENT_NUMBER-1)
    {
        char * input = fgets(name,MAXLINELENGTH-1,inputFile);
        fscanf(inputFile,"%d\n%d\n%d\n%d\n%d\n",&day,&month,&year,&cycle,&importance);
        if (input == NULL)
        {
            break;
        }
        
        if(cmp_w_now(day,month,year,0)) //discard any oudated event
        {
            continue;
        }
        else
        {
            removeNewLine(name);
            event_list[index] = createEvent(name, day, month, year, cycle, importance);
            index++;
        }
        
    }
  
    event_list[index] = NULL;
    fclose(inputFile);
    
}

void removeNewLine(char *buffer) //remove '\n' from the stdin to keep the display neat
{
    size_t length = strlen(buffer);
    if (length == 0)
    {
        return;
    }
    if (buffer[length-1] == '\n')
    {
        buffer[length-1] = '\0';
    }
}


void day_increment(int* dd, int* mm, int* yy, int increment) // return a new date based on a given date and an increment, this is used to record recurring events
{
    int day = *dd;
    int m = *mm;
    int year = *yy;
    int next = day + increment;
    int month_lenth[13] = {29,31,28,31,30,31,30,31,31,30,31,30,31};
    
    while (next > month_lenth[leap_feb(m,year)])
    {
        next = next - month_lenth[leap_feb(m,year)];
        m++;
        if(m==13)
        {
            year++;
            m=m-12;
        }
    }
    *dd = next;
    *mm = m;
    *yy = year;   
}

int leap_feb(int m, int year)
{
    if (m!=2)
    {
        return m;
    }
    else
    {
        int leap = 1; 
        if (year%4 == 0)
        {
            leap = 0;
        }
        m=m*leap;
        return m;
    }

}


void addevent()
{
    printf("\nAdd a new event\n");
    int index = -1;
    while(event_list[++index] != NULL);
    if (index > EVENT_NUMBER-2)
    {
        printf("Sorry, the diary is full\n");
        return;
    }
    char name[MAXLINELENGTH];
    char idk[2];
    printf("Enter the event name: ");
    fgets(idk,2,stdin);
    fgets(name,MAXLINELENGTH-1,stdin);
    removeNewLine(name);
    int day, month, year, cycle, period, importance;
    printf("Enter the date of the event (in the form: DD MM YYYY): ");
    scanf("%d %d %d", &day, &month, &year);
    printf("Is this a recurring event?\n");
    printf("If so, enter the recurring cycle of the event in days, if not, enter 0: ");
    scanf("%d", &cycle);
    if (cycle != 0)
    {
        printf("Does the event repeat\n1. in the week?\n2. in the month?\n3. in the year?\n");
        scanf("%d", &period);
    }
    printf("What is the rate of importance of the event? (enter number 1-5, where 1 stands the least important and 5 the most): ");
    scanf("%d", &importance);
    if (cycle == 0)
    {
        struct event *event = createEvent(name, day, month, year, cycle, importance);
        event_list[index] = event;
        event_list[index+1] = NULL;
    }
    else
    {
        int number_of_recurrence;
        if (period == 1)
        {
            number_of_recurrence = 7/cycle;
        }
        else
        {
            if (period == 2)
            {
                number_of_recurrence = 31/cycle;
            }
            else
            {
                if(period == 3)
                {
                    number_of_recurrence = 365/cycle;
                }
            }
            
        }
        for (int i = 0; i<number_of_recurrence; i++)
        {

            struct event *event = createEvent(name, day, month, year, cycle, importance);
            day_increment(&day,&month,&year,cycle);
            event_list[index] = event;
            index++;
            
        }
        event_list[index] = NULL;
        
    }
    
    
}

int cmp_w_now(int day, int month, int year, int len) //this function compares 2 dates, the first date is from the input, the second date is current date plus 'len' days.
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = gmtime(&timep);
    int today = p->tm_mday;
    int tomonth = 1+p->tm_mon;
    int toyear = 1900+p->tm_year;
    day_increment(&today,&tomonth,&toyear,len);
    if (((year == toyear)&&(month == tomonth)&&(day>=today))||((year == toyear)&&(month>tomonth))||(year>toyear))
    {
        
        return 0;
    }
    else
    {
        return 1;
    }

}


void listevents()
{
    if (event_list[0] == NULL)
    {
        printf("\nThere are yet no events.\n");
        return;
    }
    printf("list:\n1.events this week\n2.events this month\n3.all the events\n"); //one can choose to view all the events in a certain period
    struct event * list_to_display[EVENT_NUMBER];
    int choice;
    int i = 0;
    int j = 0;
    scanf("%d",&choice);
    if(choice==3)
    {
        while (event_list[i] != NULL)
        {
            list_to_display[i] = event_list[i]; 
            i++; 
        }
        list_to_display[i] = NULL;

    }
    else
    {
        if(choice==1)
        {
            while (event_list[j] != NULL)
            {
                if(cmp_w_now(event_list[j]->day,event_list[j]->month,event_list[j]->year,7))
                {
                    list_to_display[i] = event_list[j]; 
                    i++;
                }
                
                j++; 
            }
            list_to_display[i] = NULL;
        }
        else
        {
            if(choice==2)
            {
                int i =0;
                int j =0;
                while (event_list[j] != NULL)
                {
                    if(cmp_w_now(event_list[j]->day,event_list[j]->month,event_list[j]->year,31))
                    {
                        list_to_display[i] = event_list[j]; 
                        i++;
                    }
                
                    j++; 
                } 
                list_to_display[i] = NULL; 

            }
            else
            {
                printf("\nSorry - don't recognise that option, try again\n");
                return;
            }
            
        }
        
    }
    printf("list by\n1. urgency\n2. importance\n"); // one can choose to view the events in the order of urgency(sooner events appear first) or importance(based on importance rate).
    int list_refer;
    scanf("%d",&list_refer);
    if (list_refer==1)
    {
        list_urgency(list_to_display,i-1);
    }
    else
    {
        if(list_refer==2)
        {
            list_importance(list_to_display,i-1);
        }
        else
        {
            printf("\nSorry - don't recognise that option, try again\n");
            return;    
        }
        
    }
    int index = 0;
    while (list_to_display[index] != NULL)
    {
        printf("event: %s   date: %d/%d/%d\n", list_to_display[index]->name, list_to_display[index]->day,list_to_display[index]->month,list_to_display[index]->year);
        index++;
    }
}

void list_importance(struct event** event,int len) // function of bubble sort by importance rate
{
    for(int i = 0;i<len;i++)
    {
        for(int j = 0; j<len-i; j++)
        {
            struct event* t;
            if(event[j]->importance < event[j+1]->importance)
            {
                t = event[j];
                event[j] = event[j+1];
                event[j+1] = t;
            }
        }
    }
}

void list_urgency(struct event** event,int len) // function of bubble sort by urgency
{
    for(int i = 0;i<len;i++)
    {
        for(int j = 0; j<len-i; j++)
        {
            struct event* t;
            if (((event[j]->year == event[j+1]->year)&&(event[j]->month == event[j+1]->month)&&(event[j]->day>event[j+1]->day))||((event[j]->year == event[j+1]->year)&&(event[j]->month>event[j+1]->month))||(event[j]->year>event[j+1]->year))
            {
                t = event[j];
                event[j] = event[j+1];
                event[j+1] = t;
            }
        }
    }
}

void searchevent()
{
    printf("\n\nSearch for an event\n");
    char name[MAXLINELENGTH];
    char idk[2];
    printf("Enter a key word: ");
    fgets(idk,2,stdin);
    fgets(name,MAXLINELENGTH-1,stdin);
    removeNewLine(name);
    int index = 0;
    printf("Relevant events:\n");
    while (event_list[index] != NULL)
    {
        if ((!strcmp(event_list[index]->name, name))||(strstr(event_list[index]->name, name)))  //give all the events containing the key word
        {
            printf("event: %s   date: %d/%d/%d\n", event_list[index]->name, event_list[index]->day,event_list[index]->month,event_list[index]->year);
        }
        index++;
    }
    printf("...Above are all the results found\n");
}

void savediary() // event file will be refreshed
{
    printf("\nDiary saved\n");
    FILE *clearFile = fopen("event","w");
    fclose(clearFile);
    FILE *outputFile = fopen("event","w");
    int index = 0;
    while (event_list[index] != NULL)
    {
        fprintf(outputFile, "%s\n", event_list[index]->name);
        fprintf(outputFile, "%d\n", event_list[index]->day);  
        fprintf(outputFile, "%d\n", event_list[index]->month);  
        fprintf(outputFile, "%d\n", event_list[index]->year);    
        fprintf(outputFile, "%d\n", event_list[index]->cycle); 
        fprintf(outputFile, "%d\n", event_list[index]->importance); 
        index++;
    }
    fclose(outputFile);
}




int main(void)
{
    loadevent();
    go();
    
    return 0;
}
