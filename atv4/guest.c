#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <math.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>

float k=0, N0=0, N=1;
float x,y;

char mensagem[100] = "";

void* comunicacao(void* c,  char *IP_servidor, unsigned short servidorPorta){
	
    int socket_id;
    struct sockaddr_in servidorAddr;
    int length;
   
	while(1){
	socket_id=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socket_id<0){
    	fprintf(stderr, "Erro na criação do socket!\n");
    	exit(0);
	}
	//scanf("%s", &mensagem);
    
	memset(&servidorAddr, 0, sizeof(servidorAddr));
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_servidor);
	servidorAddr.sin_port = htons(servidorPorta);   
    
	if(connect(socket_id, (struct sockaddr*) &servidorAddr, sizeof(servidorAddr))<0){
    	fprintf(stderr, "Erro na conexão!\n");
    	exit(0);
	}
    
	length = strlen(mensagem) +1;
	write(socket_id, &length, sizeof(length));
	write(socket_id, mensagem, length);
    
	}
	close(socket_id);
}

void* calcularPosicao(void* a){
	//Definindo variáveis
    
	while(1){
    	//utilizando as funções cos, sin e pow da biblioteca math.h para calcular o cosseno, seno e potencia, respectivamente:
    	x = 0.5*pow(cos(0),2)*(pow(N,2)-pow(N0,2));
    	y = 0.5*pow(sin(0),2)*(pow(N,2)-pow(N0,2));
    	N0=N;
    	N = 1.07 * k;
    	k++;
   	 
    	sprintf(mensagem, "MB2: %f,%f\n", x,y);
    	printf("MB2: %f,%f\n", x,y);
  	 
    	usleep(90000);
   	 
	}
	return NULL;
}


int main(int argc, char* const argv[]){
    
    
	pthread_t th_id1;
	pthread_t th_id2;
    
	//char c = 'A';
	 
	 char *IP_servidor;
	unsigned short servidorPorta;

    
    void *c;
    IP_servidor = argv[1];
    servidorPorta = atoi(argv[2]);
    //mensagem = argv[3];
    
    
	printf("-------");
	pthread_create(&th_id1, NULL, comunicacao(c,IP_servidor,servidorPorta), NULL);
	pthread_create(&th_id2, NULL, &calcularPosicao, NULL);
    
 	while(1){
    	fputc('-',stderr);
    	usleep(50000);
	}
    
	return 0;
}
