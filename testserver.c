#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);
void receiveAndSaveFile(int clnt_sock, const char* filename);

int main(int argc, char*argv[])
{
    size_t file_size;
    int serv_sock, clnt_sock_1, clnt_sock_2;
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

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
    {
        error_handling("bind() error");
    }
    if(listen(serv_sock,5)==-1){
        error_handling("listen() error");
    }
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    if(clnt_sock==-1){ 
        error_handling("accept() error");
    }

    // Receive and save the first file
    receiveAndSaveFile(clnt_sock, "ReceiveBusan.csv");

    // Receive and save the second file
    receiveAndSaveFile(clnt_sock, "ReceiveChungbuk.csv");

    close(serv_sock);
    close(clnt_sock);

    printf("Files received and saved successfully\n");

    return 0;
}

void receiveAndSaveFile(int clnt_sock, const char* filename) 
{
    // Receive the file content from the client
    char buffer[1024];
    ssize_t bytesRead;

    FILE*file = fopen(filename, "wt");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while ((bytesRead = recv(clnt_sock, buffer, sizeof(buffer), 0)) > 0) 
    {
        fwrite(buffer, 1, bytesRead, file);
    }
    fclose(file);
}


void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
