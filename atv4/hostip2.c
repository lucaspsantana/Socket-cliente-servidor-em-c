#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>

int socket_id;
void sigint_gerenciador(int signum);
void print_client_message(int client_socket);
void end_server(void);


int main(int argc, char* const argv[]){
    
    unsigned short servidorPorta;
    struct sockaddr_in servidorAddr;
    
    servidorPorta = atoi(argv[1]);
    
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
        
        fprintf(stderr, "Conexão com cliente: %s\n", inet_ntoa(clienteAddr.sin_addr));
        
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

void print_client_message(int client_socket){
    int length;
    char* text;
    
    read(client_socket, &length, sizeof(length));
    text = (char*) malloc(length);
    read(client_socket, text, length);
    fprintf(stderr,"Rx: %s\n",text);
    
    if(!strcmp(text,"sair")){
        free(text);
        fprintf(stderr,"Cliente cancelou a conexão..\n");
        end_server();
    }
    
    free(text);
}