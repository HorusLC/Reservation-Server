#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "validation.h"
#define PORT 8081
#define COUVERT 5
#define DATE 15
#define TABLE 5
#define RESID 15
#define SMALLBUF 15
#define MIDBUF 256
#define BIGBUF 512

void makeMsg(char *, char *, char *,char *, char *);

void makeMsg(char *_date, char *_table, char *_couvert, char *_resid, char *_msg)
{
    strcat(_resid, "*");
    strcat(_table, "*");
    strcat(_date, "*");
    strcat(_couvert, "*");
    strcpy(_msg,_table);
    strcat(_msg,_date);
    strcat(_msg,_couvert);
    strcat(_msg,_resid);
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in address;
    int sock = 0, valread = 0;
    struct sockaddr_in serv_addr;
    char clientResponse[SMALLBUF];
    char table[TABLE];
    char date[DATE];
    char resId[RESID];
    char couvert[COUVERT];
    char serverResponse[MIDBUF];
    char bigClientResp[BIGBUF];
    char totalSUM[COUVERT];
    char smallBuf[SMALLBUF];
    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                   sizeof(timeout)) < 0)
    {

        perror("setsockopt failed\n");
        return -1;
    }

    while (1)
    {
        memset(serverResponse, 0, MIDBUF);
        memset(clientResponse, 0, SMALLBUF);
        memset(bigClientResp, 0, BIGBUF);
        valread=0;
        printf("\nTABLES:\n For 5 people - 1, 3,12\n For 6 people - 2, 4, 6, 8\n For 10 people - 5, 7, 9, 10, 11\n");
        printf("-----MENU-----\n 1.Check date\n 2. Add new Reservation\n 3. Add Total Sum for existing Reservation\n 4. Delete a Reservation\n 5.Quit\n 6.Backup all Reservations\n 7,Check daily Income for Specified date\n");
        scanf("%s", clientResponse);
        send(sock, clientResponse, SMALLBUF, 0);    
        if (!strcmp(clientResponse, "5"))
        {
            shutdown(sock, SHUT_RDWR);
            close(sock);
            break;
        }
        else if (!strcmp(clientResponse, "2"))
        {
            char tempDate[DATE];
            short result;
            do
            {
                memset(table, 0, TABLE);
                memset(date, 0, DATE);
                memset(couvert, 0, COUVERT);
                memset(resId, 0, RESID);
                memset(tempDate, 0, DATE);
                printf("enter table number\n");
                scanf("%s", table);
                printf("enter date\n");
                scanf("%s", date);
                printf("enter couvert\n");
                scanf("%s", couvert);
                printf("enter Reservation ID - phone number, personal ID number, etc. \n");
                scanf("%s", resId);
                strcpy(tempDate, date);
                result = inputValidation(table, tempDate, couvert, resId);
                if (result == 0)
                {
                    printf("incorrect data input\nPlease try again. \n");
                   
                }
            } while (result == 0);
            makeMsg(date,table,couvert,resId,bigClientResp);
            printf("%s", bigClientResp);
            send(sock, bigClientResp, BIGBUF, 0);
            valread = read(sock, serverResponse, MIDBUF);
            printf("%s\n", serverResponse);
            // memsetserverResponse, 0, MIDBUF);
            if (valread < 1)
            {
                printf("Server is not responding\n");
                break;
            }
        }
        else if (!strcmp(clientResponse, "1"))
        {
            char tempdate[DATE];
            short res;
            do
            {
                printf("enter date to check\n");
                scanf("%s", date);
                strcpy(tempdate, date);
                res = dateValidation(tempdate);
                if (res == 0)
                    printf("Invalid date\n Please try again\n");
            } while (res == 0);

            send(sock, date, DATE, 0);
            valread = read(sock, serverResponse, MIDBUF);
            printf("%d\n",valread);
            printf("%s\n", serverResponse);
            if (valread < 1)
            {
                printf("Server is not responding\n");
                break;
            }
        }
        else if (!strcmp(clientResponse, "3"))
        {
            short result;
            printf("enter Reservation ID \n");
            scanf("%s", resId);
            strcat(resId, "*");
            do
            {
                printf("enter total Sum amount \n");
                scanf("%s", totalSUM);
                result = totalValidation(totalSUM);
                if (result == 0)
                {
                    printf("Entered sum is not accepted\nPlease try again\n");
                }
            } while (result == 0);
            strcat(totalSUM, "*");
            strcpy(bigClientResp, resId);
            strcat(bigClientResp, totalSUM);
            printf("%s\n", bigClientResp);
            send(sock, bigClientResp, BIGBUF, 0);
            valread = read(sock, serverResponse, MIDBUF);
            printf("%s\n", serverResponse);
            if (valread < 1)
            {
                printf("Server is not responding\n");
                break;
            }
        }

        else if (!strcmp(clientResponse, "4"))
        {
            printf("enter ID of reservation that is to be deleted\n");
            scanf("%s", resId);
            send(sock, resId, RESID, 0);
            valread = read(sock, serverResponse, MIDBUF);
            if (valread < 1)
            {
                printf("Server is not responding\n");
                break;
            }
            printf("%s\n", serverResponse);
        }
        else if (!strcmp(clientResponse, "6"))
        {
            valread = read(sock, serverResponse, MIDBUF);
            if (valread < 1)
            {
                printf("Server is not responding\n");
                break;
            }
            printf("%s\n", serverResponse);
        }
        else if (!strcmp(clientResponse, "7"))
        {
            char tempdate[DATE];
            short result;
            do
            {
                memset(tempdate, 0, DATE);
                memset(date, 0, DATE);
                printf("enter date to check daily income\n");
                scanf("%s", date);
                strcpy(tempdate, date);
                result = dateValidation(tempdate);
                if (result == 0)
                {
                    printf("Date is not valid.\nPlease try again\n");
                }
            } while (result == 0);
            printf("%s date\n", date);
            send(sock, date, DATE, 0);
            valread = read(sock, serverResponse, MIDBUF);
            printf("%s\n", serverResponse);
            if (valread < 1)
            {
                printf("Server is not responding\n");
                break;
            }
        }
    }

    if (valread < 1)
    {
        shutdown(sock, SHUT_RDWR);
        close(sock);
        printf("disconnected\n");
    }

    return 0;
}