#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error_handling(char *message);
void processCSVRow(const char *line, int *count, double *sum, double *max, double *min);

//부산부산

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
    const char localName[] = "A";
    send(sock,localName,strlen(localName),0);
    FILE * file13 = fopen("13busan.csv","rt");
    
    char line[1024];

    int count = 0;
    double sum = 0.0;
    double max = 0.0; // 최대값 초기화
    double min = 0.0; // 최소값 초기화

    while (fgets(line, 1024, file13) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        processCSVRow(line, &count, &sum, &max, &min);
    }
    if (count > 0) {
        double average = sum / count;
        printf("13 max: %.2f\n", max);
        printf("13 min: %.2f\n", min);
        printf("13 avg: %.2f\n", average);
    }
    else {
        printf("데이터가 없습니다.\n");
    }

    fseek(file13,0,SEEK_END);
    long file_size13 = ftell(file13);
    fseek(file13,0,SEEK_SET);    
    
    char* file_buffer13 = (char*)malloc(file_size13+1);
    if(file_buffer13==NULL){
        error_handling("Buffer13 alloc fail");
    }
    
    fread(file_buffer13,1,file_size13,file13);
    size_t ramain;

    
    fclose(file13);
    ssize_t sentBytes13 = send(sock,file_buffer13,file_size13,0);

    if(sentBytes13==-1){
        error_handling("read() error!");
    }
    else{
        printf("13 send success\n");
    }
    free(file_buffer13);
    
    FILE * file14 = fopen("14busan.csv","rt");
    char line2 [1024];
    count = 0;
    sum = 0.0;
    min = 0.0;
    max = 0.0;
    while (fgets(line2, 1024, file13) != NULL) {
        size_t len = strlen(line2);
        if (len > 0 && line2[len - 1] == '\n') {
            line2[len - 1] = '\0';
        }

        processCSVRow(line2, &count, &sum, &max, &min);
    }
    if (count > 0) {
        double average = sum / count;
        printf("14max: %.2f\n", max);
        printf("14min: %.2f\n", min);
        printf("14avg: %.2f\n", average);
    }
    else {
        printf("데이터가 없습니다.\n");
    }

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
        printf("14 send success\n");
    }
    free(file_buffer14);
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