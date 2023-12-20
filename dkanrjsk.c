#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define MAX_LINE_LENGTH 1024
#define MAX_CLNT 12 //클라이언트 12명

void * handle_clnt(void*arg);
void receiveAndSaveFile(int clnt_sock, const char* filename);
void changeData(const char *filename);
void error_handling(char *message);
void *send_msg(void*arg);

int clnt_cnt = 1;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char*argv[])
{
    size_t file_size;
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_addr_size;
    int clco=0;
    clnt_addr_size=sizeof(clnt_addr);
    pthread_t t_id, th_id;

    if(argc!=2){
        printf("Usage : %s <port>\n",argv[0]);
        exit(1);
    }

    pthread_mutex_init(&mutx, NULL);
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1)
    {
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

    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);
        pthread_create(&th_id, NULL, send_msg, (void*)&clnt_sock);
        pthread_detach(th_id);

        printf("Connected client IP : %s \n", inet_ntoa(clnt_addr.sin_addr));
        
    }
    close(serv_sock);
    close(clnt_sock);

    printf("Files received and saved successfully\n");

    return 0;
}

void * handle_clnt(void*arg) //클라이언트한테 받은 자료를 지역별로 저장
{
    int clnt_sock = *((int*)arg);
    int str_len = 0, i;
    char msg[BUF_SIZE];
    int num;

    char clientInfo[1];  
    ssize_t bytesRead = recv(clnt_sock, clientInfo, 1, 0);
    if(bytesRead==-1){
        error_handling("read error");
    }
    
    if(clientInfo[0] == 'A')
    {
        num = 0;
    }
    if(clientInfo[0] == 'B')
    {
        num = 1;
    }
    if(clientInfo[0] == 'C')
    {
        num = 2;
    }
    if(clientInfo[0] == 'D')
    {
        num = 3;
    }
    if(clientInfo[0] == 'E')
    {
        num = 4;
    }
    if(clientInfo[0] == 'F')
    {
        num = 5;
    }
    if(clientInfo[0] == 'G')
    {
        num = 6;
    }
    if(clientInfo[0] == 'H')
    {
        num = 7;
    }
    if(clientInfo[0] == 'I')
    {
        num = 8;
    }
    if(clientInfo[0] == 'J')
    {
        num = 9;
    }
    if(clientInfo[0] == 'K')
    {
        num = 10;
    }
    
    switch(num) //클라이언트한테 숫자 신호 오면 어떤거 읽어들일지 정하는 스위치문 
    {
    case 0:
        receiveAndSaveFile(clnt_sock, "ReceiveBusan.csv");
        break;
    case 1:
        receiveAndSaveFile(clnt_sock, "ReceiveChungbuk.csv");
        break;
    case 2:
        receiveAndSaveFile(clnt_sock, "ReceiveDaegu.csv");
        break;
    case 3:
        receiveAndSaveFile(clnt_sock, "ReceiveDaejeon.csv");
        break;
    case 4:
        receiveAndSaveFile(clnt_sock, "ReceiveGangwon.csv");
        break;
    case 5:
        receiveAndSaveFile(clnt_sock, "ReceiveGwangju.csv");
        break;
    case 6:
        receiveAndSaveFile(clnt_sock, "ReceiveGyeongi.csv");
        break;
    case 7:
        receiveAndSaveFile(clnt_sock, "ReceiveJeju.csv");
        break;
    case 8:
        receiveAndSaveFile(clnt_sock, "ReceiveJeonju.csv");
        break;
    case 9:
        receiveAndSaveFile(clnt_sock, "ReceiveSeoul.csv");
        break;
    case 10:
        receiveAndSaveFile(clnt_sock, "ReceiveSejong.csv");
        break;
    }

    pthread_mutex_lock(&mutx);
    for(i = 0; i<clnt_cnt; i++)
    {
        if(clnt_sock == clnt_socks[i])
        {
            while(i++<clnt_cnt-1)
            {
                clnt_socks[i]=clnt_socks[i+1];
            }
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);
    return NULL;
}

void receiveAndSaveFile(int clnt_sock, const char* filename) //c클라이언트한테 파일 받고 저장하기까지
{
    char buffer[1024];
    ssize_t bytesRead;
    char str_tmp[1024];
    int b[7];
    int cnt;

    FILE * file = fopen(filename, "wt");
    if (!file) 
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    while ((bytesRead = recv(clnt_sock, buffer, sizeof(buffer), 0)) > 0) 
    {
        fwrite(buffer, 1, bytesRead, file);
    }
    fclose(file);
}

void changeData(const char *filename) //클라이언트한테 받은 자료 한줄씩 문자열로 출력
{
    FILE * file = fopen(filename, "r");

    if(file == NULL)
    {
        printf("파일을 열 수 없습니다 \n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    while(fgets(line, MAX_LINE_LENGTH, file)!=NULL)
    {
        printf("읽은 한줄 : %s", line);
    }
    fclose(file);
}

void *send_msg(void*arg) //클라이언트가 요청한 문자열 전송
{
    int clnt_sock = *((int*)arg);
    // char line[MAX_LINE_LENGTH];
    // int i;
    // char *token;
    int PyclientInfo; //받을 배열
    recv(clnt_sock, &PyclientInfo, sizeof(int), 0);
    printf("%d 여기는 리시브 후인거시애오\n",PyclientInfo);
    // char *rest = (char *)malloc(strlen(line) +1);
    // strcpy(rest, line);

    // 데이터 수신
    // size_t bytesRead2 = recv(clnt_sock, PyclientInfo, sizeof(PyclientInfo) , 0);
    // if (bytesRead2 < 0) 
    // {
    //     printf("데이터 수신 실패\n");
    //     close(clnt_sock);
    // }
    // for(i = 0; i<6; i++)
    // {
    //     token = strtok_r(rest, ",", &rest);
    //     if(token != NULL)
    //     {
    //         if(i==0) // 날짜
    //         {
    //             int data = atoi(token);

    //             if(data == PyclientInfo[0])
    //             {
    //                 send(clnt_sock,line,strlen(line),0);
    //             }
    //         }
    //     }
    // }
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}