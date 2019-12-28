#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>
#include <math.h>

int socket_id;
void sigint_gerenciador(int signum);
void print_client_message(int client_socket);
void end_server(void);


int main(int argc, char* const argv[]){
    
	unsigned short servidorPorta;
	struct sockaddr_in servidorAddr;
    
	servidorPorta = 8080;
    
	signal(SIGINT, sigint_gerenciador);
    
	socket_id = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socket_id < 0){
    	fprintf(stderr, "Erro na criação do socket!\n");
    	exit(0);
	}
    
	memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(servidorPorta);
    
	if(bind(socket_id,(struct sockaddr*) &servidorAddr, sizeof(servidorAddr))<0){
    	fprintf(stderr, "Erro na conexão!\n");
    	exit(0);
	}
	if(listen(socket_id,10)<0){
    	fprintf(stderr,"Erro !\n");
    	exit(0);
	}
    
	while(1){
    	int socketCliente;
    	struct sockaddr_in clienteAddr;
    	unsigned int clienteLength;
   	 
    	clienteLength = sizeof(clienteAddr);
    	if((socketCliente = accept(socket_id, (struct sockaddr*) &clienteAddr, &clienteLength))<0){
        	fprintf(stderr, "Falha na recepção!\n");
    	}
   	 
    	fprintf(stderr, "%s\n", inet_ntoa(clienteAddr.sin_addr));
   	 
    	print_client_message(socketCliente);
    	close(socketCliente);
	}
    
	return 0;
}

void sigint_gerenciador(int signum){
	fprintf(stderr, "\nForçando a finalização da conexão...\n");
	end_server();
}

void end_server(void){
	close(socket_id);
	exit(0);
}

int distancia(float x1, float y1, float x2, float y2){
    float distancia = sqrt( pow(x1 - x2, 2) + pow(y1 - y2, 2) );
	if(distancia < 1){
    	return 1;
	}else{
    	return 0;
	}
   	 
}

void print_client_message(int client_socket){
	int length;
	char *text, *token, *MB;
	float x,y;
	float posMB1[2],posMB2[2],posMB3[2], posMB4[2];
	read(client_socket, &length, sizeof(length));
	text = (char*) malloc(length);
	read(client_socket, text, length);
	//Imprimindo mensagem do guest:
	fprintf(stderr,"%s\n",text);
	//Verificando conteúdo da mensagem para calcular distância
	char* data = text;
	//Pegando informação de qual o MB e salvando na variável MB:
	token = strtok(data, ":");
	MB = token;
	//Salvando o conteúdo x na variável "x":
	token = strtok(NULL, " ,");
	//utilizando a função atof para transformar a cadeia de caracteres em float
	x = atof(token);
	//Salvando o conteúdo y na variável "y":
	token = strtok(NULL, ",");
	y = atof(token);
    
	//Verificando qual o MB para salvar sua nova posição e depois calcular a distância euclidiana:
	if(strcmp(MB, "MB1")==0){
  	posMB1[0]= x;
  	posMB1[1]= y;
  	int distMB1MB2 = distancia(posMB1[0], posMB1[1], posMB2[0], posMB2[1]);
  	int distMB1MB3 = distancia(posMB1[0], posMB1[1], posMB3[0], posMB3[1]);
  	if(distMB1MB2 == 1 || distMB1MB3 == 1){
    	printf("Colisão iminente!\n");
    	end_server();
  	}
	}else if(strcmp(MB, "MB2")==0){
  	posMB2[0]= x;
  	posMB2[1]= y;
  	int distMB1MB2 = distancia(posMB1[0], posMB1[1], posMB2[0], posMB2[1]);
  	int distMB2MB3 = distancia(posMB2[0], posMB2[1], posMB3[0], posMB3[1]);
  	if(distMB1MB2 == 1 || distMB2MB3 == 1){
    	printf("Colisão iminente!\n");
    	end_server();
  	}
	}else if(strcmp(MB, "MB3")==0){
  	posMB3[0]= x;
  	posMB3[1]= y;
  	int distMB1MB3 = distancia(posMB1[0], posMB1[1], posMB3[0], posMB3[1]);
  	int distMB2MB3 = distancia(posMB2[0], posMB2[1], posMB3[0], posMB3[1]);
  	if(distMB1MB3 == 1 || distMB2MB3 == 1){
    	printf("Colisão iminente!\n");
    	end_server();
  	}
      }else if(strcmp(MB, "MB4")==0){
  	posMB4[0]= x;
  	posMB4[1]= y;
  	int distMB1MB4 = distancia(posMB1[0], posMB4[1], posMB4[0], posMB4[1]);
  	int distMB2MB4 = distancia(posMB2[0], posMB2[1], posMB4[0], posMB4[1]);
  	int distMB3MB4 = distancia(posMB3[0], posMB3[1], posMB4[0], posMB4[1]);
  	if(distMB1MB4 == 1 || distMB2MB4 == 1 || distMB3MB4 == 1){
    	printf("Colisão iminente!\n");
    	end_server();
  	}
	}
    
	if(!strcmp(text,"sair")){
    	free(text);
    	fprintf(stderr,"Cliente cancelou a conexão..\n");
    	//end_server();
	}
    
	free(text);
}
