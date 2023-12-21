#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char *message);
void processCSVRow(const char *line, int *count, double *sum, double *max, double *min);

//��������
//�������(13,14�ⵥ���Ͱ� ����)

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

    // ','�� �������� ��ū �и�
    for (int i = 0; i < 6; ++i) {
        token = strtok_r(rest, ",", &rest);
        if (token != NULL) {
            // Ư�� ���� ���� ó��
            if (i == 5) { // ����: 3��° ���� �̿��Ͽ� ó��
                double value = atof(token);

                // �ִ밪 ����
                if (*count == 0 || value > *max) {
                    *max = value;
                }

                // �ּҰ� ����
                if (*count == 0 || value < *min) {
                    *min = value;
                }

                // ��� ����
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