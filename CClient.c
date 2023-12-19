#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;

    if(argc!=3){
        printf("Usage : %s <IP><port>\n",argv[0]);
        exit(1);
    }

    sock=socket(PF_INET,SOCK_STREAM,0);

    if(sock==-1){
        error_handling("socket() error!");
    }
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
        error_handling("connect() error!");
    }

    FILE * file13 = fopen("13busan.csv","rt");
    
    fseek(file13,0,SEEK_END);
    long file_size13 = ftell(file13);
    fseek(file13,0,SEEK_SET);    
    
    char* file_buffer13 = (char*)malloc(file_size13+1);
    if(file_buffer13==NULL){
        error_handling("Buffer13 alloc fail");
    }
    
    fread(file_buffer13,1,file_size13,file13);
    file_buffer13[file_size13]='\0';
    fclose(file13);
    ssize_t sentBytes13 = send(sock,file_buffer13,file_size13,0);

    if(sentBytes13==-1){
        error_handling("read() error!");
    }
    else{
        printf("13 send success");
    }
    free(file_buffer13);
    
    FILE * file14 = fopen("14busan.csv","rt");
    
    fseek(file14,0,SEEK_END);
    long file_size14 = ftell(file14);
    fseek(file14,0,SEEK_SET);
    
    char* file_buffer14 = (char*)malloc(file_size14+1);
    if(file_buffer14==NULL){
        error_handling("Buffer14 alloc fail");
    }
    
    fread(file_buffer14,1,file_size14,file14);
    file_buffer14[file_size14]='\0';
    fclose(file14);

    

    

    ssize_t sentBytes14 = send(sock,file_buffer14,file_size14,0);
    if(sentBytes14==-1){
        error_handling("read() error!");
    }

    else{
        printf("14 send success");
    }
    free(file_buffer14);
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}