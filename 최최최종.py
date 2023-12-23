import sys
from PyQt5.QtWidgets import *
from PyQt5 import uic
from socket import *
import pandas as pd

# farm.ui 파일을 로드하여 UI 클래스 생성
form_class = uic.loadUiType("farm.ui")[0]

# 연도, 지역, 상품 정보 설정
year = {
    2013: "2013,",
    2014: "2014,",
    2022: "2022,"
}

City = {
    '부산': "01",
    '충북': "02",
    '대구': "03",
    '대전': "04",
    '강원': "05",
    '광주': "06",
    '경기': "07",
    '제주': "08",
    '전주': "09",
    '서울': "10",
}

# 2022년일땐 지역코드 변경
if 2022 in year:
    City = ({
        '부산': '20',
        '충북': '21',
        '대구': '22',
        '대전': '23',
        '강원': '24',
        '광주': '25',
        '경기': '26',
        '제주': '27',
        '전주': '28',
        '서울': '29,',
        '세종': '30',
    })

Item = {
    '방울토마토': "422,",
    '미나리': "252,",
    '찹쌀': "112,",
    '콩': "141,",
    '팥(적두)': "142,",
    '고구마': "151,",
    '감자': "152,",
    '시금치': "213,",
    '토마토': "225,",
    '당근': "232,",
    '마늘': "244,",
    '양파': "245,",
    '파': "246,",
    '생강': "247,",
    '건미역': "642,",
    '굴': "644,"
}

class WindowClass(QMainWindow, form_class):

    def __init__(self):
        super().__init__()
        self.setupUi(self)

        # 콤보박스 값 변경 시 함수 연결
        self.yearcombo.currentIndexChanged.connect(self.yearsel)
        self.localcombo.currentIndexChanged.connect(self.localsel)
        self.itemcombo.currentIndexChanged.connect(self.itemsel)
        self.checkbtn.clicked.connect(self.checkbtnfu)

        # 데이터를 저장할 DataFrame 초기화
        self.df = pd.DataFrame()

    # 연도 선택 콤보박스 값 변경 시 호출되는 함수
    def yearsel(self):
        self.a = year.get(int(self.yearcombo.currentText()), "")

    # 도시 선택 콤보박스 값 변경 시 호출되는 함수
    def localsel(self):
        self.c = City.get(self.localcombo.currentText(), "")

    # 작물 선택 콤보박스 값 변경 시 호출되는 함수
    def itemsel(self):
        self.b = Item.get(self.itemcombo.currentText(), "")

    # 확인 버튼 클릭 시 호출되는 함수
    def checkbtnfu(self):
        # 서버에 전송할 데이터 생성
        sendserver = f'{self.a}{self.b}{self.c}'

        # 소켓 통신 정보 설정
        class SocketInfo():
            HOST = "10.10.20.100"
            PORT = 9192
            BUFSIZE = 10000000000
            ADDR = (HOST, PORT)

        # 소켓 생성 및 서버에 연결달
        csock = socket(AF_INET, SOCK_STREAM)
        csock.connect(SocketInfo.ADDR)
        print("연결 성공")

        # 자신이 파이썬이라는 정보 전달
        to_server = "P"
        right_method = to_server.encode("UTF-8")
        sent = csock.send(right_method)
        print("전송 데이터: {}, 바이트 길이: {}, 바이트 내용: {}".format(to_server, len(right_method), right_method))

        # 콤보박스로 선택한 데이터 전달
        to_server = sendserver
        right_method = to_server.encode("UTF-8")
        print("전송 데이터: {}, 바이트 길이: {}, 바이트 내용: {}".format(to_server, len(right_method), right_method))
        csock.send(right_method)

        while 1:
            # 서버로부터 데이터 수신
            commend = csock.recv(SocketInfo.BUFSIZE)
            data = commend.decode("UTF-8")
            print(data)

            # 수신된 데이터를 DataFrame에 추가
            new_df = pd.DataFrame({self.b: [data]})
            self.df = pd.concat([self.df, new_df], ignore_index=True)

            # 데이터를 파일로 저장
            self.df.to_csv('sample.txt', index=False)
            self.df.to_csv('sample_tab.txt', sep='\t', index=False)
            self.df.to_csv('sample_noidx.txt', sep='\t', index=False)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWindow = WindowClass()
    myWindow.show()
    app.exec_()
