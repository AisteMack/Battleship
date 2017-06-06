#ifndef Header
#define Header

int number(int mas[11][11]);
int print(int mas[11][11], int mas1[11][11]);
int check(int x, int y, int length, int mas[11][11]);
int setShip(int x, int y, int length, int mas[11][11]);
int shoot(int x, int y, int mas[11][11]);
void createBoard(int mas[11][11]);
void createBoardRandom(int mas[11][11]);
int setZero(int mas[11][11]);

#endif
