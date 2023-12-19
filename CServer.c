#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char*argv[]){
    size_t file_size;
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_addr_size;
    clnt_addr_size=sizeof(clnt_addr);

    if(argc!=2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);

    if(serv_sock==-1){
        error_handling("socket() error");
    }
    
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
        error_handling("bind() error");
    }
    if(listen(serv_sock,5)==-1){
        error_handling("listen() error");
    }
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sock==-1){ 
        error_handling("accept() error");
    }

    
    FILE *file13 = fopen("Receive13.csv","wt");
    char buffer13[1024];
    ssize_t bytesRead13;
    while((bytesRead13=recv(clnt_sock,buffer13,sizeof(buffer13),0))>0){
        fwrite(buffer13,1,bytesRead13,file13);
    }
    fclose(file13);

    FILE *file14 = fopen("Receive14.csv","wt");
    char buffer14[1024];
    ssize_t bytesRead14;
    while((bytesRead14=recv(clnt_sock,buffer14,sizeof(buffer14),0))>0){
        fwrite(buffer14,1,bytesRead14,file14);
    }
    fclose(file14);
    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}