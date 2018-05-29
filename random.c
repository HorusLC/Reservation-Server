/*short applyChanges(char *resid, double totalsum)
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
}*/


/*void setTotalSum(int sock)
{
    char resid[RESID];
    char totalSUM[COUVERT];
    float totalSum;
    char smallBuf[SMALLBUF];
    read(sock, resid, RESID);
    read(sock, totalSUM, COUVERT);
    totalSum = atof(totalSUM);
    if (applyChanges(resid, totalSum))
    {
        strcpy(smallBuf, "Sum Added\n");
    }
    else
        strcpy(smallBuf, "No such ID\n");
    send(sock, smallBuf, SMALLBUF, 0);
}*/

/*short isReservationAccepted(char *table, char *date)
{

    Reservation *r;
    for (r = head; r != NULL; r = r->next)
    {
        if (!(strcmp(date, r->date)))
        {
            if (!strcmp(r->tableID, table))
            {

                return 0;
            }
        }
    }
    return 1;
}*/
/*void showReservations()
{
    Reservation *temp;
    for (temp = head; temp != NULL; temp = temp->next)
    {
        printf("%s\t%s\t%f\t%s\n", temp->tableID, temp->date, temp->couvert, temp->resID);
    }
}*/
/* searchReservations(char *date){
Reservation *temp;
    for (temp = head; temp != NULL; temp = temp->next)
    {
        printf("%s\t%s\t%f\n", temp->tableID, temp->date, temp->couvert);
    }
*/
/*
void checkDate(int sock)
{
    char date[DATE];
    read(sock, date, DATE);
    char responseBuffer[BIGBUF];
    strcpy(responseBuffer, "The Taken tables are: ");
    Reservation *temp;
    for (temp = head; temp != NULL; temp = temp->next)
    {
        if (!strcmp(temp->date, date))
        {
            strcat(responseBuffer, temp->tableID);
            strcat(responseBuffer, " ,");
        }
    }
    write(sock, responseBuffer, BIGBUF);
}
*/
//Reservation *getLastNode()
/*
void addReservation(int sock)
{
    char buffer[MIDBUF];
    char table[TABLE];
    char date[DATE];
    char couvert[COUVERT];
    char rsrvID[RESID];
    char response[BIGBUF];
    short result1;
    
memset(response,0,BIGBUF);
    read(sock, response, BIGBUF);
    if(strlen(response)==0){
            printf("Client Disconnected");
        return;}
    printf("%ld\n", strlen(response));
    //strcpy(table,"21");
    strcpy(table, strtok(response, "*"));
    strcpy(date, strtok(NULL, "*"));
    strcpy(couvert, strtok(NULL, "*"));
    strcpy(rsrvID, strtok(NULL, "*"));

    printf("%s\n %s\n%s\n%s\n", table, date, couvert, rsrvID);
    //result1 =
        result1 = isReservationAccepted(table, date);
        
    if (result1 == 1)
    {
        Reservation *temp = (Reservation *)malloc(sizeof(Reservation));
        strcpy(temp->date, date);
        strcpy(temp->resID, rsrvID);
        strcpy(temp->tableID, table);
        temp->couvert = atof(couvert);
        temp->finalSum = 0;
        if (head == NULL)
        {
            head = temp;
            temp->next = NULL;
            strcpy(buffer, "Successful Reservation");
        }
        else
        {
            Reservation *r;
            for (r = head; r->next != NULL; r = r->next)
                ;
            r->next = temp;
            temp->next = NULL;
            strcpy(buffer, "Successful Reservation\n");
        }
    }
    else if (result1 == 0)
    {
        strcpy(buffer, "Table has already been taken on this date!\n");
    }
    send(sock, buffer, MIDBUF, 0);
}
*/

/*

void addReservation(int sock,Reservation *head)
{
    char buffer[MIDBUF];
    char table[TABLE];
    char date[DATE];
    char couvert[COUVERT];
    char rsrvID[RESID];
    char response[BIGBUF];
    short result1;
    
memset(response,0,BIGBUF);
    read(sock, response, BIGBUF);
    if(strlen(response)==0){
            printf("Client Disconnected");
            
        return;}
    printf("%ld\n", strlen(response));
    //strcpy(table,"21");
    strcpy(table, strtok(response, "*"));
    strcpy(date, strtok(NULL, "*"));
    strcpy(couvert, strtok(NULL, "*"));
    strcpy(rsrvID, strtok(NULL, "*"));

    printf("%s\n %s\n%s\n%s\n", table, date, couvert, rsrvID);
    //result1 =
        result1 = isReservationAccepted(table, date,head);
        
    if (result1 == 1)
    {
        Reservation *temp = (Reservation *)malloc(sizeof(Reservation));
        strcpy(temp->date, date);
        strcpy(temp->resID, rsrvID);
        strcpy(temp->tableID, table);
        temp->couvert = atof(couvert);
        temp->finalSum = 0;
        if (head == NULL)
        {
            head = temp;
            temp->next = NULL;
            strcpy(buffer, "Successful Reservation");
        }
        else
        {
            Reservation *r;
            for (r = head; r->next != NULL; r = r->next)
                ;
            r->next = temp;
            temp->next = NULL;
            strcpy(buffer, "Successful Reservation\n");
        }
    }
    else if (result1 == 0)
    {
        strcpy(buffer, "Table has already been taken on this date!\n");
    }
    send(sock, buffer, MIDBUF, 0);
}*/