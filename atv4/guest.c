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

void* comunicacao(void* c){
	int socket_id;
	struct sockaddr name;
	char* socket_name;
	socket_name = "/tmp/socket1";

	int length;
	
	//memset(&name, 0, sizeof(name));
    socket_id = socket(PF_LOCAL,SOCK_STREAM,0);
    name.sa_family = AF_LOCAL;
    strcpy(name.sa_data,socket_name);
    connect(socket_id, &name, sizeof(name));

	length = strlen(mensagem) +1;
	write(socket_id, &length, sizeof(length));
	write(socket_id, mensagem, length);
   
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
   	 
    	sprintf(mensagem, "MB1: %f,%f\n", x,y);
    	printf("MB1: %f,%f\n", x,y);
  	 
    	usleep(90000);
   	 
	}
	return NULL;
}

int main(){
    
	pthread_t th_id1;
	pthread_t th_id2;
    
    
	pthread_create(&th_id1, NULL, &comunicacao, NULL);
	pthread_create(&th_id2, NULL, &calcularPosicao, NULL);
    
 	while(1){
    	usleep(50000);
	}
    
	return 0;
}
