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

#define THISWEEK 1
#define THISMONTH 2
#define ALL 3
#define PASTEVENT 4

#define BY_IMPORTANCE 1
#define BY_URGENCY 2

#define MAXLINELENGTH 80
#define EVENT_NUMBER 1000

struct event
{
    char* name;
    int day;
    int month;
    int year;
    int cycle;
    int importance;
};

struct event * event_list[EVENT_NUMBER];


void diaplay(void);
void go(void);
void do_option(int);
void loadevent(void);
void removeNewLine(char*);
struct event *createEvent(char*, int, int, int, int, int);
char* readString(char*);
void addevent(void);
void day_increment(int*, int*, int*, int);
void searchevent(void);
int cmp_w_now(int, int, int, int);


void display()
{
    printf("\tღ EVENT DIARY ღ\n");
    printf("%d. Add a new event\n", ADD);
    printf("%d. List the events\n", LIST);
    printf("%d. Search an event\n", SEARCH);
    printf("%d. Save the diary\n", SAVE);
    printf("%d. Quit\n", QUIT);
}


void go()
{
    
    while(true)
    {
        display();
        int option;
        scanf("%d", &option);
        if (option == QUIT)
        {
            break;
        }
        else
        {
            do_option(option);
        }
    }
    
}





void do_option(int option)
{
    switch (option)
    {
        case ADD : addevent(); break;
        case LIST : printf("%d\n",option); break;
        case SEARCH : printf("%d\n",option); break;
        case SAVE : printf("%d\n",option); break;
        default : printf("\nSorry - don't recognise that option, try again\n");
    }
    
}



void loadevent()
{
    FILE *inputFile = fopen("/Users/doudou/Desktop/principleofprogramming/phonebooktest/event","r");
    int index = 0;
    char name[MAXLINELENGTH];
    int day;
    int month;
    int year;
    int cycle;
    int importance;
    while (index < EVENT_NUMBER-1)
    {
        fgets(name,MAXLINELENGTH-1,inputFile);
        char * input = name;
        fscanf(inputFile,"%d\n%d\n%d\n%d\n%d\n",&day,&month,&year,&cycle,&importance);
        if (input == NULL)
        {
            break;
        }
        removeNewLine(name);
        event_list[index] = createEvent(name, day, month, year, cycle, importance);
        index++;
    }
  
    event_list[index] = NULL;
    fclose(inputFile);
    
}

void removeNewLine(char *buffer)
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

char* readString(char* prompt)
{
    char buffer[MAXLINELENGTH];
    printf("%s", prompt);
    fgets(buffer, MAXLINELENGTH, stdin);
    size_t inputLength = strlen(buffer);
    char *input = calloc(sizeof(char), inputLength);
    strncpy(input, buffer, inputLength-1);
    input[inputLength] = '\0';
    return input;
}

void day_increment(int* dd, int* mm, int* yy, int increment)
{
    int day = *dd;
    int month = *mm;
    int year = *yy;
    int next = day + increment;
    int leap;
    int month_lenth[13] = {29,31,28,31,30,31,30,31,31,30,31,30,31};
    int m = month;
    if (year%4 == 0)
    {
        leap = 0;
    }
    else
    {
        leap = 1;
    }
    if (m==2)
    {
        m=m*leap;
    }

    
    while (next > month_lenth[m])
    {
        next = next - month_lenth[m];
        month++;
        m = month;
        if(m==13)
        {
            year++;
        }
        while(m>12)
        {
            m=m-12;
        }
        if (year%4 == 0)
        {
            leap = 0;
        }
        else
        {
            leap = 1;
        }
        if (m==2)
        {
            m=m*leap;
        }
    }
    while (month > 12)
    {
        month = month-12;
    }
    
    *dd = next;
    *mm = month;
    *yy = year;   
}

void addevent()
{
    printf("\nAdd a new event \n");
    int index = -1;
    int max;
    while(event_list[++index] != NULL);
    if (index > EVENT_NUMBER-2)
    {
        printf("Sorry, the phone book is full\n");
        return;
    }
    char *name = readString("Enter the event name: ");
    int day, month, year, cycle, period, importance;
    printf("Enter the date of the event (in the form: DD MM YY): ");
    scanf("%d %d %d", &day, &month, &year);
    printf("Is this a recurring event\n");
    printf("If so, enter the recurring cycle of the event in days, if not, enter 0: ");
    scanf("%d", &cycle);
    if (cycle != 0)
    {
        printf("Does the event repeat\n1. in the week?\n2. in the month?\n3. in the year?\n");
        scanf("%d", &period);
    }
    printf("What is the rate of importance of the event? (enter number 1-10, where 1 stands the least important and 10 the most)");
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
        event_list[index+1] = NULL;
        
    }
    
    
    free(name);
}

int cmp_w_now(int day, int month, int year, int len)
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
    printf("list:\n1.events this week\n2.events this month\n3.all the events\n");
    struct event * list_to_display[EVENT_NUMBER];
    int len;
    int choice;
    scanf("%d",&choice);
    if(choice==3)
    {
        int j = 0;
        while (event_list[j] != NULL)
        {
            list_to_display[j] = event_list[j]; 
            j++; 
        }

    }
    else
    {
        if(choice==1)
        {
            int i =0;
            int j =0;
            while (event_list[j] != NULL)
            {
                if(cmp_w_now(event_list[j]->day,event_list[j]->month,event_list[j]->year,7))
                {
                    list_to_display[i] = event_list[j]; 
                    i++;
                }
                
                j++; 
            }
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

            }
            else
            {
                printf("\nSorry - don't recognise that option, try again\n");
                return;
            }
            
        }
        
    }
    printf("list by\n1. urgency\n2. importance\n");
    
    



    int index = 0;
    while (list_to_display[index] != NULL)
    {
        printf("event: %s   date: %d/%d/%d\n", list_to_display[index]->name, list_to_display[index]->day,list_to_display[index]->month,list_to_display[index]->year);
        index++;
    }
}

void searchevent()
{
    printf("\n\nSearch for an event\n");
    char *name = readString("Enter the event's name or some key word: ");
    int index = 0;
    printf("Relevant events:\n");
    while (event_list[index] != NULL)
    {
        if ((!strcmp(event_list[index]->name, name))||(strstr(event_list[index]->name, name)))
        {
            printf("event: %s   date: %d/%d/%d\n", event_list[index]->name, event_list[index]->day,event_list[index]->month,event_list[index]->year);
        }
        index++;
    }
    printf("...Above are all the results found\n");
    free(name);
}





int main(void)
{
    loadevent();
    void go();
    
    return 0;
}


