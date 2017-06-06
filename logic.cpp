#include <stdio.h>
#include "logic.h"
#include <stdlib.h>
#include <time.h> 

int ship[10] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
	
	


void createBoard(int mas[11][11]){
	int i = 0;
	while(i != 10)
	{
		int x, y;
		printf("Iveskite koordinates laivo, kurio ilgis %d \n", ship[i]);
		scanf("%d %d", &x, &y);
		scanf("%c");
		if(x < 1 || x > 10 || y < 1 || y > 10 || (y + ship[i]) > 11)
		{
			printf("Neteisingos koordinates, koordinates turi buti tarp 1 ir 10 \n");
		}else{
			
			if(setShip(x,y,ship[i],mas) == 0){
			//	print(mas);
				i++;
			}
			else{
			  printf("Laivo negalima deti salia kito laivo! \n");
			}
		}	
	}	
	
	return;
}

void createBoardRandom(int mas[11][11]){
	int i = 0;
	switch(rand()%8){
	case 0 :					//okay
	setShip(1,1,ship[i],mas);
	setShip(3,1,ship[++i],mas);
	setShip(5,1,ship[++i],mas);
	setShip(7,1,ship[++i],mas);
	setShip(9,1,ship[++i],mas);
	setShip(9,5,ship[++i],mas);
	setShip(7,5,ship[++i],mas);
	setShip(5,5,ship[++i],mas);
	setShip(3,6,ship[++i],mas);
	setShip(1,6,ship[++i],mas);
	break;
	case 1 :					//okay
	setShip(6,6,ship[i],mas);
	setShip(9,4,ship[++i],mas);
	setShip(2,4,ship[++i],mas);
	setShip(9,9,ship[++i],mas);
	setShip(6,3,ship[++i],mas);
	setShip(2,1,ship[++i],mas);
	setShip(1,8,ship[++i],mas);
	setShip(2,10,ship[++i],mas);
	setShip(4,4,ship[++i],mas);
	setShip(4,7,ship[++i],mas);
	break;
	case 2 :					//okay
	setShip(4,4,ship[i],mas);
	setShip(1,3,ship[++i],mas); 
	setShip(7,2,ship[++i],mas);
	setShip(7,9,ship[++i],mas);
	setShip(4,1,ship[++i],mas);
	setShip(10,7,ship[++i],mas);
	setShip(10,2,ship[++i],mas);
	setShip(10,4,ship[++i],mas);
	setShip(3,10,ship[++i],mas);
	setShip(6,6,ship[++i],mas);
	break;
	case 3 :					//okay
	setShip(4,7,ship[i],mas);
	setShip(3,1,ship[++i],mas);
	setShip(5,1,ship[++i],mas);
	setShip(7,1,ship[++i],mas);
	setShip(9,1,ship[++i],mas);
	setShip(9,5,ship[++i],mas);
	setShip(7,5,ship[++i],mas);
	setShip(5,5,ship[++i],mas);
	setShip(3,5,ship[++i],mas);
	setShip(1,6,ship[++i],mas);
	break;
	case 4 :					//okay
	setShip(3,7,ship[i],mas);
	setShip(7,6,ship[++i],mas);
	setShip(10,6,ship[++i],mas);
	setShip(3,4,ship[++i],mas);
	setShip(6,3,ship[++i],mas);
	setShip(10,3,ship[++i],mas);
	setShip(7,1,ship[++i],mas);
	setShip(1,1,ship[++i],mas);
	setShip(5,6,ship[++i],mas);
	setShip(1,6,ship[++i],mas); 
	break;
	case 5 :
	setShip(5,7,ship[i],mas);	//okay
	setShip(1,5,ship[++i],mas);
	setShip(8,5,ship[++i],mas);
	setShip(4,1,ship[++i],mas);
	setShip(8,2,ship[++i],mas);
	setShip(5,4,ship[++i],mas);
	setShip(10,1,ship[++i],mas);
	setShip(10,9,ship[++i],mas);
	setShip(8,9,ship[++i],mas);
	setShip(1,10,ship[++i],mas);
	break;
	case 6 :					//okay
	setShip(10,7,ship[i],mas);
	setShip(3,6,ship[++i],mas);
	setShip(7,6,ship[++i],mas);
	setShip(3,3,ship[++i],mas);
	setShip(6,3,ship[++i],mas);
	setShip(10,4,ship[++i],mas);
	setShip(10,2,ship[++i],mas);
	setShip(6,10,ship[++i],mas);
	setShip(5,7,ship[++i],mas);
	setShip(3,10,ship[++i],mas);
	break;
	case 7 :					//okay
	setShip(1,1,ship[i],mas);
	setShip(1,8,ship[++i],mas);
	setShip(4,2,ship[++i],mas);
	setShip(4,6,ship[++i],mas);
	setShip(4,9,ship[++i],mas);
	setShip(7,7,ship[++i],mas);
	setShip(7,2,ship[++i],mas);
	setShip(7,4,ship[++i],mas);
	setShip(10,3,ship[++i],mas);
	setShip(10,10,ship[++i],mas);
	break;
	}
	/*
	while(i != 10)
	{
	//	print(mas);
	//	system("PAUSE");
		int x, y;
		srand(time(NULL));
		
		x = 1 + rand()%(10);
		y = 1 + rand()%(10);
		if(x < 1 || x > 10 || y < 1 || y > 10 || (y + ship[i]) > 11)
		{
			
		}else{
			
			if(setShip(x,y,ship[i],mas) == 0){
				i++;
				
			}
		}	
	}	
	*/
	return;
}

int number(int mas[11][11]){
	
	for (int i=1; i<11; ++i){
		
		mas[i][0] = i;
		mas[0][i] = i;
	}
	
	return 0;
}

int print(int mas[11][11], int mas1[11][11])
{
	printf("         Player board      |        Computer board\n");
	printf("     1 2 3 4 5 6 7 8 9 10  |     1 2 3 4 5 6 7 8 9 10\n");
	printf("----------------------------------------------------\n");
	for (int i=1; i<11; ++i){
		if(i == 10)
		printf("%d | ", i);
		else
		printf("%d  | ", i);
		for (int j = 1; j < 11; ++j){	
			printf("%d ", mas[i][j]);
		}
		printf("  |");
		if(i == 10)
			printf("%d | ", i);
			else
			printf("%d  | ", i);
		for (int j = 1; j < 11; ++j){
				
			printf("%d ", mas1[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}

int setZero(int mas[11][11])
{
	for (int i = 0; i <= 11; ++i){
		for (int j = 0; j <= 11; ++j)	
			mas[i][j] = 0;
	}
	
	return 0;
}

int setShip(int x, int y, int length, int mas[11][11]){
	
	if(check(x, y, length, mas) == 0){
		
		for(int i = 0; i < length; ++i){
		mas[x][y + i] = 1;
		}	
		return 0;
	}else{
		return -1;
	}

}

int check(int x, int y, int length, int mas[11][11]){
	
		for(int i = 0; i <= length + 1; ++i){
		
		if(mas[x][y -1 + i] != 0 || mas[x + 1][y -1 + i] != 0 || mas[x - 1][y -1 + i] != 0)
		return -1;
	}
	
	return 0;
}

int shoot(int x, int y, int mas[11][11]){
	
	if(mas[x][y] == 1){
		return 1;
	}
	else if(mas[x][y] == 0)
	{
		return 0;
	}else 
	return 2;
	
}



