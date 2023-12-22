#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 10000
#define MAX_LINE_LENGTH 1024
#define MAX_CLNT 12 //클라이언트 12명

void * handle_clnt(void*arg);
void receiveAndSaveFile(int clnt_sock, const char* filename);
void PrintData(int clnt_sock, const char *filename, char *year, char *pdtnum);
void error_handling(char *message);
void send_msg(int clnt_sock, char* buffer);

int clnt_cnt = 1;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;
int is22made[11]={0,};

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
        printf("Connected client IP : %s \n", inet_ntoa(clnt_addr.sin_addr));
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);

    close(serv_sock);
    close(clnt_sock);

    printf("Files received and saved successfully\n");

    return 0;
}

void * handle_clnt(void*arg) //클라이언트한테 받은 자료를 지역별로 저장
{
    int clnt_sock = *((int*)arg);
    int str_len = 0, i;
    int num=-1;

    char clientInfo[1];  
    ssize_t bytesRead = recv(clnt_sock, clientInfo, 1, 0);
    if(bytesRead==-1){
        error_handling("read error");
    }
    if(clientInfo[0] == 'P')
    {
        num = 20;
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
    case 20:
        receiveAndSaveFile(clnt_sock, "P");
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
    int b[7];
    int cnt;

    FILE * file = fopen(filename, "wt");
    if (!file) 
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    if(strcmp(filename,"P")==0){
        bytesRead = recv(clnt_sock,buffer,sizeof(buffer),0);
        if(bytesRead<0)
        {
            error_handling("receive error임!!!!!!!\n");
        }
        send_msg(clnt_sock,buffer);
    }
    else{
        while ((bytesRead = recv(clnt_sock, buffer, sizeof(buffer), 0)) > 0) 
        {
            fwrite(buffer, 1, bytesRead, file);
        }
    }
    fclose(file);
}

void PrintData(int clnt_sock, const char *filename, char *year, char *pdtnum) //C클라이언트한테 받은 자료 문자열로 출력(파이썬 클라이언트한테 보내게)
{
    char *token;
    int tmpyear; //파일의 첫번째를 저장하는 값
    ssize_t sendByte=0;
    FILE * file = fopen(filename, "r");
    char line[MAX_LINE_LENGTH];
    char tmpStr[1024];
    if(file == NULL)
    {
        printf("%s 파일을 열 수 없습니다 \n",filename);
        return;
    }
    else{
        printf("%s 파일을 열었습니다 \n",filename);
    }
    fread(line,1,MAX_LINE_LENGTH,file);
    char *rest = (char *)malloc(strlen(line) +1);
    strcpy(rest,line);

    while(sizeof(rest)>sendByte){
        for(int i=0; i<6; i++)
        {
            token=strtok_r(rest, ",", &rest);
            if(token != NULL)
            {
                if(i==0)
                {
                    tmpyear = atoi(token);
                    
                }
                if(i==5)
                {
                    if(strcmp(token,pdtnum)==0)
                    {
                        if(tmpyear%(atoi(year)*10000)<1300)
                        {   
                            while(fgets(tmpStr, MAX_LINE_LENGTH, file) != NULL)
                            {        
                                ssize_t bytesend;
                                bytesend = send(clnt_sock,tmpStr,strlen(tmpStr),0);
                                sendByte += bytesend;
                                if(bytesend<0)
                                {
                                    error_handling("파일 전송 안됨\n");
                                    close(clnt_sock);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fclose(file);
}

void send_msg(int clnt_sock, char* buffer) //파이썬 클라이언트가 요청한 문자열 전송
{
    char year[5];
    char local[5];
    char pdtnum[5];
    int i;
    char *token;
    
    char *rest = (char *)malloc(strlen(buffer) +1);
    strcpy(rest, buffer);
    for(i=20;i<31;i++){
        is22made[i]=0;
    }

    for(i=0; i<3; i++) //클라이언트한테 받은 데이터 날짜, 지역, 상품코드로 나누기(token)
    {
        token=strtok_r(rest, ",", &rest);
        if(token != NULL)
        {


            if(i==0) //날짜
            {
                strcpy(year,token);
            }
            if(i==1) //상품코드
            {
                strcpy(pdtnum,token);
            }
            if(i==2) //지역코드
            {
                strcpy(local,token);
                if(strcmp(year,"2022")==0) //22년일때는 C클라이언트한테 요청하고 받아서 문자열로 출력
                {
                    // send(clnt_socks[MAX_CLNT],local,sizeof(local),0);
                    if(strcmp(local,"20")==0&&is22made[0]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Busan.csv");
                        is22made[0]=1;
                    }
                    else if(strcmp(local,"21")==0&&is22made[1]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Chungbuk.csv");
                        is22made[1]=1;
                    }
                    else if(strcmp(local,"22")==0&&is22made[2]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Daegu.csv");
                        is22made[2]=1;
                    }
                    else if(strcmp(local,"23")==0&&is22made[3]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Daejeon.csv");
                        is22made[3]=1;
                    }
                    else if(strcmp(local,"24")==0&&is22made[4]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Gangwon.csv");
                        is22made[4]=1;
                    }
                    else if(strcmp(local,"25")==0&&is22made[5]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Gwangju.csv");
                        is22made[5]=1;
                    }
                    else if(strcmp(local,"26")==0&&is22made[6]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Gyeongi.csv");
                        is22made[6]=1;
                    }
                    else if(strcmp(local,"27")==0&&is22made[7]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Jeju.csv");
                        is22made[7]=1;
                    }
                    else if(strcmp(local,"28")==0&&is22made[8]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Jeonju.csv");
                        is22made[8]=1;
                    }
                    else if(strcmp(local,"29")==0&&is22made[9]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Seoul.csv");
                        is22made[9]=1;
                    }
                    else if(strcmp(local,"30")==0&&is22made[10]==0){
                        send(clnt_sock,local,sizeof(local),0);
                        receiveAndSaveFile(clnt_sock,"Receive22Sejong.csv");
                        is22made[10]=1;
                    }
                }

                //13~14년도 파일
                if(strcmp(local, "01")==0){
                    PrintData(clnt_sock, "ReceiveBusan.csv", year,pdtnum);
                }
                else if(strcmp(local, "02")==0){
                    PrintData(clnt_sock, "ReceiveChungbuk.csv", year,pdtnum);
                }
                else if(strcmp(local, "03")==0){
                    PrintData(clnt_sock, "ReceiveDaegu.csv", year,pdtnum);
                }
                else if(strcmp(local, "04")==0){
                    PrintData(clnt_sock, "ReceiveDaejeon.csv", year,pdtnum);
                }
                else if(strcmp(local, "05")==0){
                    PrintData(clnt_sock, "ReceiveGangwon.csv", year,pdtnum);
                }
                else if(strcmp(local, "06")==0){
                    PrintData(clnt_sock, "ReceiveGwangju.csv", year,pdtnum);
                }
                else if(strcmp(local, "07")==0){
                    PrintData(clnt_sock, "ReceiveGyeongi.csv", year,pdtnum);
                }
                else if(strcmp(local, "08")==0){
                    PrintData(clnt_sock, "ReceiveJeju.csv", year,pdtnum);
                }
                else if(strcmp(local, "09")==0){
                    PrintData(clnt_sock, "ReceiveJeonju.csv", year,pdtnum);
                }
                else if(strcmp(local, "10")==0){
                    PrintData(clnt_sock, "ReceiveSeoul.csv", year,pdtnum);
                }

                // //2022년도 파일
                else if(strcmp(local, "20")==0){
                    PrintData(clnt_sock, "Receive22Busan.csv", year,pdtnum);
                }
                else if(strcmp(local, "21")==0){
                    PrintData(clnt_sock, "Receive22Chungbuk.csv", year,pdtnum);
                }
                else if(strcmp(local, "22")==0){
                    PrintData(clnt_sock, "Receive22Daegu.csv", year,pdtnum);
                }
                else if(strcmp(local, "23")==0){
                    PrintData(clnt_sock, "Receive22Daejeon.csv", year,pdtnum);
                }
                else if(strcmp(local, "24")==0){
                    PrintData(clnt_sock, "Receive22Gangwon.csv", year,pdtnum);
                }
                else if(strcmp(local, "25")==0){
                    PrintData(clnt_sock, "Receive22Gwangju.csv", year,pdtnum);
                }
                else if(strcmp(local, "26")==0){
                    PrintData(clnt_sock, "Receive22Gyeongi.csv", year,pdtnum);
                }
                else if(strcmp(local, "27")==0){
                    PrintData(clnt_sock, "Receive22Jeju.csv", year,pdtnum);
                }
                else if(strcmp(local, "28")==0){
                    PrintData(clnt_sock, "Receive22Jeonju.csv", year,pdtnum);
                }
                else if(strcmp(local, "29")==0){ 
                    PrintData(clnt_sock, "Receive22Seoul.csv", year,pdtnum);
                }
                else if(strcmp(local, "30")==0){
                    PrintData(clnt_sock, "Receive22Sejong.csv", year,pdtnum);
                }
            }
        }
    }
    close(clnt_sock);
    return;
}

void error_handling(char *message) // 오류 핸들링
{
    fputs(message, stderr);
    fputc('\n',stderr);
    exit(1);
}
