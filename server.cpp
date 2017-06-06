#include <stdio.h>
#include <time.h> 
#include <winsock.h>
#include <stdint.h>
#include <stdlib.h>
#include "logic.h"

#define PORT 13301
#define BUFFER_LENGTH 1024
#define MAX_CLIENTS 5


bool checkingRecv(int received_len, int connected_socket);

int main(int argc, char *argv[]) {
	int err;
	
	WSADATA wsaData;
	err = WSAStartup(0x0202, &wsaData);
	if (err != 0) {                                  
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }


	char buffer[BUFFER_LENGTH];
	int port;
	
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int client_address_len = sizeof(client_address);

	int listening_socket;
	int connected_socket;

	int received_len;
	int sent_len;
	
	int activity;
	fd_set readfds;
	
	//table for clients sockets
	struct Clients {
	 
	int connected_socket;
	int serverBoard[11][11];
	int playerBoard[11][11];
	int playerHitCounter;
	int serverHitCounter;
	 
	};
	 
	Clients clients[MAX_CLIENTS];

	if (argc == 2) {
        port = atoi(argv[1]);
		if ((port < 1) || (port > 65535)){
			port = PORT;
		}
    }
    else{
        port = PORT;
    }
	
	 for(int i = 0; i < MAX_CLIENTS; i++)
	{
		clients[i].connected_socket = 0;
	}

	listening_socket = socket(
		AF_INET,        //IPv4
		SOCK_STREAM,    //two-way socket
		IPPROTO_TCP     //TCP
	);

	if (listening_socket < 0) {
		printf("Socket creation failed.\n");
		exit(1);
	}

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY; //any interface

	if (bind(listening_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
		printf("Binding socket failed.\n");
		exit(1);
	}

	if (listen(listening_socket, 5) < 0) {
		printf("Listening of socket failed.\n");
		exit(1);
	}

	printf("Listening on %d...\n", port);
	
	while (1) {
		
			memset(&client_address, 0, sizeof(client_address));
			
			FD_ZERO(&readfds);							//clear the socket fd set
													
			FD_SET(listening_socket, &readfds);			//add listening socket to fd set
			
			for(int i = 0; i < MAX_CLIENTS; i++) {
					if(clients[i].connected_socket > 0)
					FD_SET(clients[i].connected_socket, &readfds);
			}
			
			activity = select(
				0, 			
				&readfds,	
				NULL,	
				NULL,		
				NULL		
			);
			
			if(activity == SOCKET_ERROR) {
				printf("Select call failed with error code : %d" , WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			
			
			int client_address_len = sizeof(client_address);
			
			
			
			if(FD_ISSET(listening_socket, &readfds)){
				connected_socket = accept(listening_socket, (struct sockaddr *) &client_address, &client_address_len);
				printf("Client connected.\n");
				//add socket to fd set
				for(int i = 0; i < MAX_CLIENTS; i++) {
					if(clients[i].connected_socket == 0)
					{	
						
						clients[i].connected_socket = connected_socket;
						setZero(clients[i].serverBoard);
						createBoardRandom(clients[i].serverBoard);
						setZero(clients[i].playerBoard);
						createBoardRandom(clients[i].playerBoard);
						clients[i].serverHitCounter = 0;
						clients[i].playerHitCounter = 0;
						
						print(clients[i].playerBoard, clients[i].serverBoard);
						
						int playerBoard[10];
						for(int u = 1; u < 11; u++){
							for(int k = 0; k < sizeof(clients[i].playerBoard[u]); k++){
								playerBoard[k] = htonl(clients[i].playerBoard[u][k+1]);
							}	
							
							send(connected_socket, (char *) playerBoard, sizeof(playerBoard), 0);
						}
						
						break;
					}
				}
			}
			
			//check other connections
			for(int i = 0; i < MAX_CLIENTS; i++){
				
				connected_socket = clients[i].connected_socket;
	
				if(FD_ISSET(connected_socket, &readfds)) {
					
					int coordinate_converted[2];
					int coordinate[2]={0};
					char* info_buffer;
					
					memset(&coordinate_converted, 0, sizeof(coordinate_converted));
					received_len = recv(connected_socket, (char *) coordinate_converted, sizeof(coordinate_converted), 0); //gaunam x ir y coordinateerdinates
					if(!checkingRecv(received_len, connected_socket)){
						closesocket(connected_socket);
						clients[i].connected_socket = 0;
						break;
					}
					
					printf("Socket %d. Client: %d. Server hit count: %d \n", i, connected_socket, clients[i].serverHitCounter);
					printf("Socket %d. Client: %d. Player hit count: %d \n", i, connected_socket, clients[i].playerHitCounter);
					
					for (int j = 0; j < sizeof(coordinate_converted); j++)
					coordinate[j] = ntohl(coordinate_converted[j]);
	  	 
					printf("Socket %d. Client: %d. Received: %d %d. \n", i, connected_socket, coordinate[0], coordinate[1]);
					
					
					switch (shoot(coordinate[0], coordinate[1], clients[i].serverBoard)){
					case 0 :
						info_buffer = "F";
						clients[i].serverBoard[coordinate[0]][coordinate[1]] = 3;
					 	break;				
					case 1 :
						info_buffer = "T";
						clients[i].serverHitCounter++;
						clients[i].serverBoard[coordinate[0]][coordinate[1]] = 2;
						if(clients[i].serverHitCounter == 4)
						info_buffer = "W";
						break;
					case 2 :
						info_buffer = "S";
						break;
					}
					
					sent_len = send(connected_socket, info_buffer, 1, 0);
					if(info_buffer != "W"){
	
					
					
					while(1){
						srand(time(NULL));
					
						coordinate[0] = 1 + rand()%(10);
						coordinate[1] = 1 + rand()%(10);
						if(clients[i].playerBoard[coordinate[0]][coordinate[1]] == 0 || clients[i].playerBoard[coordinate[0]][coordinate[1]] == 1)
						break;	
					}
					
					for (int u = 0; u < sizeof(coordinate); u++)
					coordinate_converted[u] = htonl(coordinate[u]);
				 
					sent_len = send(connected_socket, (char *) coordinate_converted, sizeof(coordinate_converted), 0);
	
					
					printf("Socket %d. Client: %d. Sent: %c %d %d ", i, connected_socket, info_buffer[0], coordinate[0], coordinate[1]);
					
					switch (shoot(coordinate[0], coordinate[1], clients[i].playerBoard)){
					case 0 :
						info_buffer = "F";
						clients[i].playerBoard[coordinate[0]][coordinate[1]] = 3;
					 	break;				
					case 1 :
						info_buffer = "T";
						clients[i].playerHitCounter++;
						clients[i].playerBoard[coordinate[0]][coordinate[1]] = 2;
						if(clients[i].playerHitCounter == 4)
						info_buffer = "W";
						break;
					case 2 :
						info_buffer = "S";
						break;
					}
					printf("%c \n", info_buffer[0]);
					
					sent_len = send(connected_socket, info_buffer, 1, 0);
					
					}
					if(info_buffer == "W"){
						int d, dd;
					recv(connected_socket, (char *)dd, sizeof(dd), 0);
					
					d = ntohl(dd);
					printf("DD %d", d);	
						if(d == 1){
							printf("AS viduje \n");
						clients[i].connected_socket = connected_socket;
						setZero(clients[i].serverBoard);
						createBoardRandom(clients[i].serverBoard);
						setZero(clients[i].playerBoard);
						createBoardRandom(clients[i].playerBoard);
						clients[i].serverHitCounter = 0;
						clients[i].playerHitCounter = 0;
						
						print(clients[i].playerBoard, clients[i].serverBoard);
						
						int playerBoard[10];
						for(int u = 1; u < 11; u++){
							for(int k = 0; k < sizeof(clients[i].playerBoard[u]); k++){
								playerBoard[k] = htonl(clients[i].playerBoard[u][k+1]);
							}	
							
							send(connected_socket, (char *) playerBoard, sizeof(playerBoard), 0);
						}
						}
						else{
						closesocket(connected_socket);
						clients[i].connected_socket = 0;
						break;	
						}
						
						
					}
				}
			}
			
		
	}
	
	return 0;
}

bool checkingRecv(int received_len, int connected_socket){
	if( received_len == SOCKET_ERROR){
		int error_code = WSAGetLastError();
		
		if(error_code == WSAECONNRESET){
			printf("Client %d disconnected unexpectedly\n", connected_socket); //Somebody disconnected , get his details and print
			return false;
		}
		else{
			printf("recv from client %d failed with error code : %d \n" , connected_socket, error_code);
			return false;
		}
	} 
	
	if(received_len == 0){
		
		printf("Client %d disconnected.\n", connected_socket);
		return false;
	}
	return true;
}
