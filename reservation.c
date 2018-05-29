#include "reservation.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define PORT 8081
#define COUVERT 5
#define DATE 15
#define TABLE 5
#define RESID 15
#define SMALLBUF 15
#define MIDBUF 256
#define BIGBUF 512
#define CLIENT_DISC 2
#define OK 1

void calculateDailyIncome(Reservation *head, char *response, char *date)
{
    Reservation *r;
    double sum = 0;
    char sumArr[10];
    memset(sumArr, 0, 10);
    for (r = head; r != NULL; r = r->next)
    {
        if (!strcmp(r->date, date))
        {
            sum = sum + r->finalSum;
        }
    }
    snprintf(sumArr, 10, "%.2f", sum);
    strcpy(response, "the daily income for date: ");
    strcat(response, date);
    strcat(response, " is :");
    strcat(response, sumArr);
}

Reservation *loadReservations(char *fname)
{
    Reservation *head = NULL;
    char table[TABLE];
    char date[DATE];
    char resid[RESID];
    char couvert[COUVERT];
    char totalSum[COUVERT];
    int fd = open(fname, O_RDWR);

    if (fd == -1)
    {
        perror("Bad file\n");
        return NULL;
    }
    while (1)
    {
        if (read(fd, table, TABLE) == 0)
        {
            break;
            close(fd);
        }
        if (read(fd, date, DATE) < 0)
        {
            perror("error");

            return NULL;
        }
        if (read(fd, couvert, COUVERT) < 0)
        {
            perror("error");
            return NULL;
        }
        if (read(fd, totalSum, COUVERT) < 0)
        {
            perror("error");
            return NULL;
        }
        if (read(fd, resid, RESID) < 0)
        {
            perror("error");
            return NULL;
        }
        Reservation *t = (Reservation *)malloc(sizeof(Reservation));
        if (t == NULL)
        {
            return NULL;
        }
        strcpy(t->tableID, table);
        strcpy(t->date, date);
        strcpy(t->resID, resid);
        t->finalSum = atof(totalSum);
        t->couvert = atof(couvert);
        t->next = head;
        head = t;
    }

    return head;
}

_Bool saveReservations(Reservation *head, char *fname)
{
    Reservation *temp;
    char couvert[COUVERT];
    char totalSum[COUVERT];
    int fd = open(fname, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd == -1)
    {
        perror("Bad File\n");
        return 0;
    }

    /** if ((f=fopen(fname,"wb"))==NULL)
    {
        printf("I/O Error\n");
        return 0;
    }*/

    for (temp = head; temp != NULL; temp = temp->next)

    {
        printf("Table: %s\t Date:%s\tCouvert: %f\tFinal Sum: %f\tReservation ID: %s\n", temp->tableID, temp->date, temp->couvert, temp->finalSum, temp->resID);
        if (write(fd, temp->tableID, TABLE) < 1)
        {
            perror("Error while writing\n");
            close(fd);
            return 0;
        }

        if (write(fd, temp->date, DATE) < 1)
        {
            perror("Error while writing\n");
            close(fd);
            return 0;
        }
        snprintf(couvert, COUVERT, "%.2f", temp->couvert);
        if (write(fd, couvert, COUVERT) < 1)
        {
            perror("Error while writing\n");

            close(fd);
            return 0;
        }
        snprintf(totalSum, COUVERT, "%.2f", temp->finalSum);
        if (write(fd, totalSum, COUVERT) < 1)
        {
            perror("Error while writing\n");

            close(fd);
            return 0;
        }
        if (write(fd, temp->resID, RESID) < 1)
        {
            perror("Error while writing\n");

            close(fd);
            return 0;
        }
        memset(couvert, 0, COUVERT);
        memset(totalSum, 0, COUVERT);

        /*if(fwrite(&(temp->tableID),sizeof(char),1,f)!=1)
        {
            printf("Write Error\n");
            fclose(f);
            return 0;
        }*/
    }

    close(fd);
    return 1;
}

Reservation *deleteReservation(Reservation *head, char *resid, char *response)
{
    Reservation *p, *q;
    strcpy(response, "No reservation with that ID was found");
    for (p = head; p != NULL; q = p, p = p->next)
    {
        if (!strcmp(resid, p->resID))
        {
            memset(response, 0, MIDBUF);
            if (p == head)
            {
                q = head;
                head = p->next;
                free(q);

                strcpy(response, "Reservation deleted");
                break;
            }
            else
            {
                q->next = p->next;
                free(p);
                p = q;
                strcpy(response, "Reservation deleted");
                break;
            }
        }
    }
    return head;
}
short getDeletionID(int sock, char *resID)
{
    short res;
    res = read(sock, resID, RESID);
    return res;
}

short applyChanges(char *resid, double totalsum, Reservation *head)
{
    Reservation *r;
    for (r = head; r != NULL; r = r->next)
    {
        if (!strcmp(r->resID, resid))
        {
            r->finalSum = totalsum + (r->couvert);
            return 1;
        }
    }
    return 0;
}

_Bool getDate(int sock, char *date)
{
    if (read(sock, date, DATE) < 1)
    {
        return 0;
    }
    return 1;
}

void checkDate(Reservation *head, char *response, char *date)
{

    strcpy(response, "The Taken tables are: ");
    Reservation *temp;
    for (temp = head; temp != NULL; temp = temp->next)
    {
        if (!strcmp(temp->date, date))
        {
            strcat(response, temp->tableID);
            strcat(response, ", ");
        }
    }
}
short parseTotalSum(int sock, char *resid, char *totalSUM)
{
    char clientResponse[BIGBUF];
    short result;
    result = read(sock, clientResponse, BIGBUF);
    if (result > 0)
    {
        strcpy(resid, strtok(clientResponse, "*"));
        strcpy(totalSUM, strtok(NULL, "*"));
    }

    return result;
}

void setTotalSum(int sock, char *resid, char *totalSUM, Reservation *head, char *clientResponse)
{

    float totalSum;
    // char clientResponse[BIGBUF];

    totalSum = atof(totalSUM);
    if (applyChanges(resid, totalSum, head))
    {
        strcpy(clientResponse, "Total sum successuflly Added\n");
    }
    else
        strcpy(clientResponse, "No such ID\nTry again\n");
}

void showReservations(Reservation *head)
{
    Reservation *temp;
    for (temp = head; temp != NULL; temp = temp->next)
    {
        printf("Table: %s\t Date:%s\tCouvert: %f\tFinal Sum: %f\tReservation ID: %s\n", temp->tableID, temp->date, temp->couvert, temp->finalSum, temp->resID);
    }
}

short isReservationAccepted(char *table, char *date, char *resid, Reservation *head, char *response)
{

    Reservation *r;
    for (r = head; r != NULL; r = r->next)
    {
        if (!strcmp(r->resID, resid))
        {

            strcpy(response, "Reservation ID already exists");
            return 0;
        }
        if (!(strcmp(date, r->date)))
        {
            if (!strcmp(r->tableID, table))
            {
                strcpy(response, "Table already taken on this date");

                return 0;
            }
        }
    }
    return 1;
}

Reservation *parseData(int sock)
{
    char buffer[MIDBUF];
    char table[TABLE];
    char date[DATE];
    char couvert[COUVERT];
    char rsrvID[RESID];
    char response[BIGBUF];
    short result1;

    memset(response, 0, BIGBUF);
    read(sock, response, BIGBUF);
    if (strlen(response) == 0)
    {
        //printf("Client Disconnected");
        return NULL;
    }
    //printf("%ld\n", strlen(response));
    //strcpy(table,"21");
    strcpy(table, strtok(response, "*"));
    strcpy(date, strtok(NULL, "*"));
    strcpy(couvert, strtok(NULL, "*"));
    strcpy(rsrvID, strtok(NULL, "*"));
    Reservation *temp = (Reservation *)malloc(sizeof(Reservation));
    strcpy(temp->date, date);
    strcpy(temp->resID, rsrvID);
    strcpy(temp->tableID, table);
    temp->couvert = atof(couvert);
    temp->finalSum = 0;
    return temp;
}

Reservation *addRes(Reservation *temp, Reservation *head, char *response)
{
    short result = isReservationAccepted(temp->tableID, temp->date,temp->resID, head,response);

    if (result == 0)
    {

        free(temp);
    }
    else
    {
        if (head == NULL)
        {
            head = temp;
            temp->next = NULL;
            strcpy(response, "Successful Reservation\n");
        }
        else
        {
            Reservation *r;
            for (r = head; r->next != NULL; r = r->next)
                ;
            r->next = temp;
            temp->next = NULL;
            strcpy(response, "Successful Reservation\n");
        }
    }
    return head;
}
