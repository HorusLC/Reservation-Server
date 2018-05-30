#ifndef RES_H_  
#define RES_H_
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
typedef struct Res
{
    char date[DATE];
    char tableID[TABLE];
    char resID[RESID];
    double finalSum;
    double couvert;
    struct Res *next;
} Reservation;
void calculateDailyIncome(Reservation *,char *,char*);
_Bool getDate(int,char *);
Reservation *loadReservations(const char *);
_Bool saveReservations(Reservation*,char *);
short getDeletionID(int ,char *);
Reservation *deleteReservation(Reservation *,char *,char *);
short parseTotalSum(int ,char *,char *);
void addReservation(int,Reservation *);
void showReservations(Reservation *);
short isReservationAccepted(char *, char *,char *,Reservation *,char *);
void checkDate(Reservation *,char *,char *);
void setTotalSum(int,char*,char*,Reservation *,char *);
short applyChanges(char *, double,Reservation *);
Reservation *parseData(int);
Reservation *addRes(Reservation *,Reservation *,char *);
short getDeletionID(int,char *);


#endif 