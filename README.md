
Program: 
2. Write a diary program, organized by day, week, month and year. It should be possible to add appointments to any day and allow for repeated appointments. The program should also be able to display a week or month neatly. It should also be possible to search the diary. The data should be stored in one or more files. 

Description: 
The event diary with the following function: 
1. Add events by providing the event’s name, date, recurring cycle and importance rate.
2. List events of a week or a month, by urgency or importance.
    NOTE: the diary is updated (outdated events get discarded) every time you open the diary, which means if you append some events to the diary list with a past date (though people normally don’t do that), it will be removed from the diary the next time you run the program.
3. Search events by key word.
4. Save events in file (named event) so they can be reaccessed.

How to use:
Compile and run it in terminal.

Reference:
1. This program is written mostly based on and refers to the source code of phonebook provided on Moodle.
This function is copied from the phonebook code:
void removeNewLine(char*);

These functions are written by modifying those in the phonebook code:
void diaplay(void);
void go(void);
void do_option(int);
void loadevent(void);
struct event *createEvent(char*, int, int, int, int, int);
void addevent(void);
void searchevent(void);
void listevents(void);
void savediary(void);

2. https://www.bilibili.com/video/av8074534/?p=116 
   a video course for c learners, I watched from p90-162, where pointer and struct are covered. 

3. Refers to this cookie https://www.eefocus.com/xuefu2009/blog/10-04/188676_67757.html for the usage of types time_t and struct tm and function gtime().



                                                            

