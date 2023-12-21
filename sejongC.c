#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char *message);
void processCSVRow(const char *line, int *count, double *sum, double *max, double *min);

//세종세종
//실행금지(13,14년데이터가 없음)

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
    const char localName[] = "K";
    send(sock,localName,strlen(localName),0);

    close(sock);
    return 0;
}

void processCSVRow(const char *line, int *count, double *sum, double *max, double *min) {
    char *token;
    char *rest = (char *)malloc(strlen(line) + 1);
    strcpy(rest, line);

    // ','를 기준으로 토큰 분리
    for (int i = 0; i < 6; ++i) {
        token = strtok_r(rest, ",", &rest);
        if (token != NULL) {
            // 특정 열에 대한 처리
            if (i == 5) { // 예시: 3번째 열을 이용하여 처리
                double value = atof(token);

                // 최대값 갱신
                if (*count == 0 || value > *max) {
                    *max = value;
                }

                // 최소값 갱신
                if (*count == 0 || value < *min) {
                    *min = value;
                }

                // 평균 누적
                *sum += value;
                *count += 1;
            }
        } else {
            break;
        }
    }

}
void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}