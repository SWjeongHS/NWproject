#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char *message);
void processCSVRow(const char *line, int (*arr)[4], char (*name)[30]) ;

//��������
//�������(13,14�ⵥ���Ͱ� ����)

int main(int argc, char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    int infoArr22[1000][4]={0,}; // 1000 == 100~999ǰ���ȣ, 4 == 0->count, 1->min, 2->max, 3->sum
    char nameArr22[1000][30]; // 1000 == 100~999ǰ���ȣ, 30 == ǰ��� ����
    double avgArr22[1000]={0.0};
    int count = 0;
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

    FILE * file22 = fopen("22sejong.csv","rt");
    char line3 [1024];
    while (fgets(line3, 1024, file22) != NULL) {
        size_t len = strlen(line3);
        if (len > 0 && line3[len - 1] == '\n') {
            line3[len - 1] = '\0';
        }
        processCSVRow(line3, infoArr22, nameArr22);
    }
    count = 0;
    while(count<1000){
        if(infoArr22[count][0]!=0){
            avgArr22[count]=infoArr22[count][3]/infoArr22[count][0];
        }
        count++;
    }

    fseek(file22,0,SEEK_END);
    long file_size22 = ftell(file22);
    fseek(file22,0,SEEK_SET);
    
    char* file_buffer22 = (char*)malloc(file_size22+1);
    if(file_buffer22==NULL){
        error_handling("Buffer22 alloc fail");
    }
    
    fread(file_buffer22,1,file_size22,file22);
    file_buffer22[file_size22]='\0';
    fclose(file22);

    char requestData[1024];    
    while(1){
        requestData[0]='\0';
        if(recv(sock,requestData,sizeof(requestData),0)>0){
            if(strcmp(requestData,"30")==0){
                ssize_t sentBytes22 = send(sock,file_buffer22,file_size22,0);
                if(sentBytes22==-1){
                    error_handling("read() error!");
                }
                else{
                    printf("22 send success\n");
                }
            }
        }
    }
    free(file_buffer22);
    close(sock);
    return 0;
}

void processCSVRow(const char *line, int (*arr)[4], char (*name)[30]) {
    char *token;
    char *PDTname;//���� ��ǰ��
    int PDTprice;//���� ����
    char *rest = (char *)malloc(strlen(line) + 1);
    int num;
    strcpy(rest, line);

    // ','�� �з�
    for (int i = 0; i < 6; ++i) {
        token = strtok_r(rest, ",", &rest);
        if (token != NULL) {
            if (i == 2){
                PDTname = token;
            }
            if (i == 4){
                PDTprice = atoi(token);
            }
            if (i == 5) { // ǰ���ȣ�϶�
                num = atoi(token); // num == ǰ���ȣ

                if(name[num][0]=='\0'){//�̸��� ������ ����
                    strcpy(name[num],PDTname);
                }
                // ������ ����
                if (arr[num][0] == 0 || PDTprice > arr[num][2]) {
                    arr[num][2] = PDTprice;
                }

                // �ְ� ����

                if (arr[num][0] == 0 || PDTprice < arr[num][1]) {
                    arr[num][1] = PDTprice;
                }

                arr[num][3] += PDTprice;
                arr[num][0]++;
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