#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include "reservation.h"
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

Reservation *head = NULL;
pthread_mutex_t lock;
void *client_handler(void *socket_desc)
{

    int sock = *(int *)socket_desc;
    char menuResponse[SMALLBUF];
    char response[MIDBUF];
    short res;
    char resid[RESID];
    char totalSUM[COUVERT];
    char date[DATE];

    while (1)

    {
        memset(menuResponse, 0, SMALLBUF);
        memset(response, 0, MIDBUF);
        memset(date,0,DATE);
        res = read(sock, menuResponse, SMALLBUF);
        // printf("res is %d", res);
        if (res < 1)
        {
            shutdown(sock, SHUT_RDWR);
            close(sock);
            printf("client disconnected without choosing Menu option\n");
            break;
        }

        if (!(strcmp(menuResponse, "5")))
        {
            shutdown(sock, SHUT_RDWR);
            close(sock);

            break;
        }
        else if (!(strcmp(menuResponse, "1")))
        {
            if (getDate(sock, date))
            {
                pthread_mutex_lock(&lock);
                checkDate(head, response, date);
                pthread_mutex_unlock(&lock);
                send(sock, response, MIDBUF,0);
            }
            else
            {
                shutdown(sock, SHUT_RDWR);
                close(sock);
                printf("client disconnected while checking date\n");
                break;
            }
        }
        else if (!(strcmp(menuResponse, "2")))
        {
            Reservation *r;
            r = parseData(sock);
            if (r == NULL)
            {
                printf("client suddenly disconnected\n");
                shutdown(sock, SHUT_RDWR);
                close(sock);
                break;
            }
            else
            {
                pthread_mutex_lock(&lock);
                head = addRes(r, head, response);
                pthread_mutex_unlock(&lock);
                send(sock, response, MIDBUF, 0);

                showReservations(head);
            }
        }
        else if (!(strcmp(menuResponse, "3")))
        {
            res = parseTotalSum(sock, resid, totalSUM);

            if (res < 1)
            {
                shutdown(sock, SHUT_RDWR);
                close(sock);
                printf("client disconnected while entering total Sum\n");
                break;
            }
            pthread_mutex_lock(&lock);
            setTotalSum(sock, resid, totalSUM, head, response);
            pthread_mutex_unlock(&lock);
            send(sock, response, MIDBUF, 0);
            memset(totalSUM, 0, COUVERT);
        }
        else if (!(strcmp(menuResponse, "4")))
        {
            res = getDeletionID(sock, resid);
            if (res < 1)
            {
                shutdown(sock, SHUT_RDWR);
                close(sock);
                printf("client disconnected while trying to delete reservation\n");
                break;
            }
            pthread_mutex_lock(&lock);
            head = deleteReservation(head, resid, response);
            pthread_mutex_unlock(&lock);
            send(sock, response, MIDBUF, 0);
        }
        else if (!(strcmp(menuResponse, "6")))
        {
            pthread_mutex_lock(&lock);
            res = saveReservations(head, "ResList.txt");
            pthread_mutex_unlock(&lock);
            if (res == 1)
            {
                strcpy(response, "Successful save");
            }
            else
                strcpy(response, "Save failed, please try again");
            send(sock, response, MIDBUF, 0);
        }
         else if (!(strcmp(menuResponse, "7"))){

            if (getDate(sock, date))
            {
                pthread_mutex_lock(&lock);
               calculateDailyIncome(head,response,date);
                pthread_mutex_unlock(&lock);
                send(sock, response, MIDBUF,0);
            }
            else
            {
                shutdown(sock, SHUT_RDWR);
                close(sock);
                printf("client disconnected while checking daily income\n");
                break;
            }


         }



    }
}

int main(int argc, char const *argv[])
{
    int server_fd, client_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    struct timeval timeout;
    timeout.tv_sec = 30;
    timeout.tv_usec = 0;
    head = loadReservations("ResList.txt");
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    while (1)
    {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address,
                                    (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                       sizeof(timeout)) < 0)
            perror("setsockopt failed\n");

        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_handler, (void *)&client_socket) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        printf("new client connected  thread %lu , fd %d\n", thread_id, client_socket);
    }

    return 0;
}