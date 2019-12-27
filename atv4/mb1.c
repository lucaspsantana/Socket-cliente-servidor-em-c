#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<arpa/inet.h>


int main(int argc, char* const argv[]){
    
    int socket_id;
    struct sockaddr_in servidorAddr;
    unsigned short servidorPorta;
    int length;
    char *IP_servidor;
    char *mensagem;
    
    
    IP_servidor = argv[1];
    servidorPorta = atoi(argv[2]);
    mensagem = argv[3];
    
    socket_id=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socket_id<0){
        fprintf(stderr, "Erro na criação do socket!\n");
        exit(0);
    }
    
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
    
    close(socket_id);
    return 0;
       
}

