#include "validation.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
short tableValidation(char *table)
{
    if (!strcmp(table, ""))
        return 0;

    return 1;
}
short dateValidation(char *date)
{

    if (strlen(date)!=10)
        return 0;
for (int i = 0; i < strlen(date) - 1; i++)
    {
        if (isalpha(date[i]))
            return 0;
    }

        char dd[3];
        char mm[3];
        char year[5];
        strcpy(dd,strtok(date,"."));
        strcpy(mm,strtok(NULL,"."));
        strcpy(year,strtok(NULL,"."));

if(atoi(dd)<1||atoi(dd)>31)
return 0;
if(atoi(mm)<1||atoi(mm)>12 )
return 0;
if(atoi(year)<2018||atoi (year)>2019)
return 0;



    /*for (int i = 0; i < strlen(date) - 1; i++)
    {
        if (isalpha(date[i]))
            return 0;
    }*/
    return 1;
}

short couvertValidation(char *couvert)
{
    if (!strcmp(couvert, ""))
        return 0;
    if (!strncmp(couvert, "-",1))
        return 0;
    for (int j = 0; j < strlen(couvert) - 1; j++)
    {
        if (isalpha(couvert[j]))
            return 0;
    }
    return 1;
}
short totalValidation(char *total)
{
    if (!strcmp(total, ""))
        return 0;
    if (!strncmp(total,"-",1))
        return 0;

    for (int j = 0; j < strlen(total) - 1; j++)
    {
        if (isalpha(total[j]))
            return 0;
    }
    return 1;
}


short inputValidation(char *table, char *date, char *couvert, char *resID)
{
    if (!strcmp(table, ""))
        return 0;
    if (!strcmp(date, ""))
        return 0;
        if(strlen(date)<10)
        return 0;
        for (int i = 0; i < strlen(date) - 1; i++)
    {
        if (isalpha(date[i]))
            return 0;
    }
        char dd[3];
        char mm[3];
        char year[5];
        strcpy(dd,strtok(date,"."));
        strcpy(mm,strtok(NULL,"."));
        strcpy(year,strtok(NULL,"."));
printf("%s %s %s\n",dd,mm,year);
if(atoi(dd)<1||atoi(dd)>31)
return 0;
if(atoi(mm)<1||atoi(mm)>12 )
return 0;
if(atoi(year)<2018||atoi (year)>2019)
return 0;
    /*for (int i = 0; i < strlen(date) - 1; i++)
    {
        if (isalpha(date[i]))
            return 0;
    }*/


    if(atof(couvert)==0){return 0;}
    /*for (int j = 0; j < strlen(couvert) - 1; j++)
    {
        if (isalpha(couvert[j]))
            return 0;
    }*/
    if (!strcmp(resID, ""))
        return 0;

    return 1;
}



