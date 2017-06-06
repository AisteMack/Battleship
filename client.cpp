#include <stdio.h>
#include <winsock.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <ctype.h>
#include "logic.h"

#define PORT 13301
#define BUFFER_LENGTH 1024
#define HOST "127.0.0.1"

using namespace std;

bool checkingRecv(int received_len);

int main(int argc, char *argv[]) {
	int err;
	
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	if (err != 0) {                                  
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

	int curr_socket;
	struct sockaddr_in servaddr;
	int received_len;

	char* send_buffer;
	char receive_buffer[BUFFER_LENGTH];
	int port;
	char* host;
	if (argc == 3) {
              port = atoi(argv[1]);
			  if ((port < 1) || (port > 65535)){
				  port = PORT;
			  }
			  host = argv[2];
          }
          else if(argc == 2){
               port = atoi(argv[1]);
			   if ((port < 1) || (port > 65535)){
				  port = PORT;
				}
			   host = HOST;
          }
          else{
               port = PORT;
			   host = HOST;
          }
  
	curr_socket = socket(
		AF_INET,				 // address family: The Internet Protocol version 4 (IPv4);
		SOCK_STREAM, 			//type specification: two-way;
		0     					//protocol: service provider will choose the protocol to use.
	);  
	if (curr_socket < 0) {
		printf("Socket creation failed.\n");
		exit(1);
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;				
	servaddr.sin_port = htons(port); 			//htons converts a u_short from host to TCP/IP network byte order
	servaddr.sin_addr.s_addr = inet_addr(host);	//inet_addr function converts a string containing an IPv4 dotted-decimal address into a proper address

	if(connect(curr_socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("Socket connection failed.\n");
		exit(1);
	}
	
	int PlayerBoard[10];
	
	while(1){
	
	int playerBoard[11][11] = {0};
	
	
	int coordinate[2] = {0};
	for(int i = 1; i < 11; i++){
		
		received_len = recv(curr_socket, (char *) PlayerBoard, sizeof(PlayerBoard), 0);
		if(checkingRecv(received_len) == false){
		 exit(1);
		}
		
		for (int j = 0; j < sizeof(PlayerBoard); j++)
		playerBoard[i][j+1] = ntohl(PlayerBoard[j]);
	}
	
	int serverBoard[11][11] = {0};
	print(playerBoard, serverBoard);
	
	while(1){	
	
			printf("Enter X and Y  \n");
			while(1){
				cin >> coordinate[0]
				>> coordinate[1];
				if(cin.fail()){
					string s;
					cin.clear();
					cin >> s;
					printf("Neteisingi duomenys. Veskite naujus\n ");
					continue;
				}
				if(coordinate[0] < 1 || coordinate[0] > 10 ||coordinate[1] < 1 || coordinate[1] > 10){
				
					printf("False coordinates! enter a new number from 1 to 10 \n ");
					continue;
					}
				else
				break;
			}
			
			int x = coordinate[0];
			int y = coordinate[1];
			int coordinate_converted[2];
			for (int i = 0; i < sizeof(coordinate); i ++)
		  	 coordinate_converted[i] = htonl(coordinate[i]);
		  	 
		  	send(curr_socket, (char *) coordinate_converted, sizeof(coordinate_converted), 0);  //Siunciam x ir y koordinates
			
			memset(&receive_buffer, 0, BUFFER_LENGTH);
			received_len = recv(curr_socket, receive_buffer, BUFFER_LENGTH, 0);   //Gaunate ar pataiekme i laiva ar ne
			if(checkingRecv(received_len) == false){
				exit(1);
			}
			int d;
			switch(receive_buffer[0]){
				case 'T' :								//pataikem i laiva
				serverBoard[x][y] = 2;
				break;
				case 'W' :
				printf("You won! ");
				break;
				case 'F' :								//nepataikem i laiva
				serverBoard[x][y]	= 3;
				break;
			}
			if(receive_buffer[0] != 'W'){
			
			memset(&coordinate_converted, 0, sizeof(coordinate_converted));
			received_len = recv(curr_socket, (char *) coordinate_converted, sizeof(coordinate_converted), 0);	//Gaunam x ir y koordinates
			if(checkingRecv(received_len) == false){
				exit(1);
			}
			for (int i = 0; i < sizeof(coordinate_converted); i++)
		  	coordinate[i] = ntohl(coordinate_converted[i]);
		  	
			memset(&receive_buffer, 0, BUFFER_LENGTH);
		  	received_len = recv(curr_socket, receive_buffer, BUFFER_LENGTH, 0);   //Gaunate ar pataikeme i laiva ar ne
			if(checkingRecv(received_len) == false){
				exit(1);
			}
			
			printf("Server shot  %d, %d \n", coordinate[0], coordinate[1]);
			
			switch(receive_buffer[0]){
				case 'T' :
				playerBoard[coordinate[0]][coordinate[1]]	= 2;			
				break;
				case 'W' :													
				printf("Computer won! ");
				printf("Pasirinkite: \n1. Naujas zaidimas. \n2. Iseiti\n");
				scanf("%d", &d);
				if(d == 2){
				closesocket(curr_socket);
				printf("Disconnected.\n");
				exit(1);
				}
				
				break;
				case 'F' :													
				playerBoard[coordinate[0]][coordinate[1]]	= 3;
				break;
			}
		  	
		  	print(playerBoard, serverBoard);
			}
			if(receive_buffer[0] == 'W'){
				int d;
				int dd;
				printf("Pasirinkite: \n1. Naujas zaidimas. \n2. Iseiti\n");
				scanf("%d", &d);
		  		dd = ntohl(d);
				send(curr_socket,(char *) &dd, sizeof(dd), 0);
				if(d == 2){
					closesocket(curr_socket);
					printf("Disconnected.\n");
					exit(1);
				}else
				break;
				
			}
		}  	
	}

	return 0;
}

bool checkingRecv(int received_len){
	if( received_len == SOCKET_ERROR){
		
		int error_code = WSAGetLastError();
		
		if(error_code == WSAECONNRESET){
			printf("Server disconnected unexpectedly\n"); //Somebody disconnected , get his details and print
			return false;
		}	
		else{
			printf("recv failed with error code : %d" , error_code);
			return false;
		}
		if(received_len == 0){
		printf("Server disconnected.\n");
		}
	}
	return true;
}




