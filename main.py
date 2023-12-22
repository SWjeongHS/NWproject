import sys
import socket
from PyQt5.QtWidgets import *
from PyQt5 import uic
import matplotlib
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
import numpy as np

form_class = uic.loadUiType("map.ui")[0]

City = {
    '부산': 0,
    '충북': 1,
    '대구': 2,
    '대전': 3,
    '강원': 4,
    '광주': 5,
    '경기': 6,
    '제주': 7,
    '전주': 8,
    '서울': 9
}

Item= {
    '방울토마토': 422,
    '미나리': 252,
    '찹쌀': 112,
    '콩': 141,
    '팥(적두)': 142,
    '고구마': 151,
    '감자': 152,
    '시금치': 213,
    '토마토': 225,
    '당근': 232,
    '마늘': 244,
    '양파': 245,
    '파': 246,
    '생강': 247,
    '건미역': 642,
    '굴': 644
}

class WindowClass(QMainWindow, form_class) :
    def __init__(self):
        super().__init__()
        self.HOST = '127.0.0.1'  # 서버의 IP 주소
        self.PORT = 9106  # 서버의 포트 번호
        # # 서버에 연결
        self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client_socket.connect((self.HOST, self.PORT))
        self.setupUi(self)
        self.message = []
        self.total = []
        self.total1 = []
        self.save = []
        self.stackedWidget.setCurrentIndex(0)

        # 버튼 및 콤보박스 초기화 및 이벤트 핸들러 연결
        self.initUIAndConnectEvents()

    def initUIAndConnectEvents(self):
        # 버튼 및 콤보박스 초기화
        self.initButtonsAndComboBox()

        # 첫 화면 넘어가기 버튼 이벤트 연결
        self.pushButton.clicked.connect(self.btn_main_exe)

        # 1 화면 연도 콤보박스 이벤트 연결
        self.comboBox.currentIndexChanged.connect(self.comboBoxClicked0)

        # 2 화면 품목 콤보박스 이벤트 연결
        self.comboBox_2.currentIndexChanged.connect(self.comboBox_1_Clicked)

        # 도시 버튼 이벤트 연결
        self.connectCityButtons()

    def initButtonsAndComboBox(self):
        # 콤보박스 비활성화
        self.comboBox_2.setDisabled(True)

        # 푸시버튼 비활성화
        buttons = [
            self.pushButton1, self.pushButton2, self.pushButton3,
            self.pushButton4, self.pushButton5, self.pushButton6,
            self.pushButton7, self.pushButton8, self.pushButton9,
            self.pushButton10
        ]
        for button in buttons:
            button.setDisabled(True)

    def connectCityButtons(self):
        # 도시 버튼 연결
        cities = [
            ('서울', self.pushButton1), ('경기도', self.pushButton2),
            ('대구', self.pushButton3), ('충북', self.pushButton4),
            ('대전', self.pushButton5), ('강원', self.pushButton6),
            ('광주', self.pushButton7), ('제주', self.pushButton8),
            ('전주', self.pushButton9), ('부산', self.pushButton10)
        ]
        for city_name, button in cities:
            button.clicked.connect(lambda ch=city_name: self.area_clicked(ch))

    def comboBoxClicked0(self):
        # 지도화면 연도 콤보박스
        selected_year = self.comboBox.currentText()

        if selected_year == '-':
            self.initButtonsAndComboBox()
            return

        self.activateCityButtons()
        self.message.append(selected_year)

    def activateCityButtons(self):
        # 도시 버튼 활성화
        buttons = [
            self.pushButton1, self.pushButton2, self.pushButton3,
            self.pushButton4, self.pushButton5, self.pushButton6,
            self.pushButton7, self.pushButton8, self.pushButton9,
            self.pushButton10
        ]
        for button in buttons:
            button.setEnabled(True)

    def btn_main_exe(self): # 메인화면 -> 지역화면
        self.stackedWidget.setCurrentIndex(1)

    def comboBoxClicked0(self):  # 지도화면 연도 콤보박스
        self.message = []   # 초기화
        a=self.comboBox.currentText()
        if a == '-':    # - 항목 선택 불가
            self.pushButton1.setDisabled(True)
            self.pushButton2.setDisabled(True)
            self.pushButton3.setDisabled(True)
            self.pushButton4.setDisabled(True)
            self.pushButton5.setDisabled(True)
            self.pushButton6.setDisabled(True)
            self.pushButton7.setDisabled(True)
            self.pushButton8.setDisabled(True)
            self.pushButton9.setDisabled(True)
            self.pushButton10.setDisabled(True)
            return
        self.pushButton1.setEnabled(True)
        self.pushButton2.setEnabled(True)
        self.pushButton3.setEnabled(True)
        self.pushButton4.setEnabled(True)
        self.pushButton5.setEnabled(True)
        self.pushButton6.setEnabled(True)
        self.pushButton7.setEnabled(True)
        self.pushButton8.setEnabled(True)
        self.pushButton9.setEnabled(True)
        self.pushButton10.setEnabled(True)
        self.message.append(a)

    def area_clicked(self, city_name):
        if len(self.message) == 2:
            self.message.pop()  # 두 번째 요소를 제거합니다.
        self.message.append(city_name)  # 선택한 도시를 리스트에 추가합니다.
        self.total = City[self.message[1]]  # 선택한 도시의 정보를 total 변수에 저장합니다.
        self.comboBox_2.setEnabled(True)

        # 도시 버튼들을 공통 함수에 연결합니다.
        self.pushButton1.clicked.connect(lambda: self.area_clicked('서울'))
        self.pushButton2.clicked.connect(lambda: self.area_clicked('경기도'))
        self.pushButton3.clicked.connect(lambda: self.area_clicked('대구'))
        self.pushButton4.clicked.connect(lambda: self.area_clicked('충북'))
        self.pushButton5.clicked.connect(lambda: self.area_clicked('대전'))
        self.pushButton6.clicked.connect(lambda: self.area_clicked('강원'))
        self.pushButton7.clicked.connect(lambda: self.area_clicked('광주'))
        self.pushButton8.clicked.connect(lambda: self.area_clicked('제주'))
        self.pushButton9.clicked.connect(lambda: self.area_clicked('전주'))
        self.pushButton10.clicked.connect(lambda: self.area_clicked('부산'))

    def comboBox_1_Clicked(self):  # 2 콤보박스 품목
        c=self.comboBox_2.currentText()
        if c=='-':
            self.pushButton1.setDisabled(True)
            self.pushButton2.setDisabled(True)
            self.pushButton3.setDisabled(True)
            self.pushButton4.setDisabled(True)
            self.pushButton5.setDisabled(True)
            self.pushButton6.setDisabled(True)
            self.pushButton7.setDisabled(True)
            self.pushButton8.setDisabled(True)
            self.pushButton9.setDisabled(True)
            self.pushButton10.setDisabled(True)
            return
        self.comboBox_2.setDisabled(True)

        if len(self.message) == 3:
            self.message.pop()
        self.message.append(c)
        self.total = City[self.message[1]] # 메시지 1의 정보를 토탈 리스트에 추가
        self.total1 = Item[self.message[2]] # 메시지 2의 정보를 토탈 리스트에 추가
        self.save = "{},{}".format(self.total,self.total1)  # 세이브 리스트에 토탈 정보 추가
        self.send=self.message[0]+ ',' + self.save  # 메시지와 토탈 send에 저장

        #데이터 송신
        self.client_socket.send(self.send.encode('utf-8'))
        self.client_socket.recv(1024)
        #
        # # 서버로부터 데이터 수신
        Data = self.client_socket.recv(1000000)

        with open('file.csv','wb') as f:
            while Data:
                print(Data)
                f.write(Data)
                Data = self.client_socket.recv(10000)

        # 데이터 불러오기
        data = pd.read_csv('file.csv')

        # 데이터 최대, 최소, 평균, 예상가격
        max_price = data['조사가격'].max()
        min_price = data['조사가격'].min()
        average_price = data['조사가격'].mean()
        rounded_average_price = int(average_price)  # 소수점 제거

        # 규격과 조사가격 추출
        specifications, prices = data['규격'], data['조사가격']

        # 그래프 그리기
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 6))

        # 가격 분포 히스토그램
        ax1.hist(prices, bins=20, color='skyblue', alpha=0.7)
        ax1.set_title('상품 가격 분포')
        ax1.set_xlabel('가격')
        ax1.set_ylabel('빈도')

        # 규격별 가격 박스 플롯
        boxplot_data = [data[data['규격'] == spec]['조사가격'] for spec in set(specifications)]
        ax2.boxplot(boxplot_data, labels=set(specifications), vert=False)
        ax2.set_title('규격별 상품 가격 분포')
        ax2.set_xlabel('가격')
        ax2.set_ylabel('규격')

        plt.tight_layout()
        plt.show()

        # 최대, 최소, 평균 가격 출력
        self.max_price_label.setText(f'최대 가격: {max_price}원')
        self.min_price_label.setText(f'최소 가격: {min_price}원')
        self.average_price_label.setText(f'평균 가격: {rounded_average_price}원')
        self.rounded_average_price_label.setText(f'예상 가격: {int(rounded_average_price * 1.2)}원')

        # 그래프 화면 종료 후 콤보박스 초기화
        self.comboBox.setCurrentIndex(0)
        self.comboBox_2.setCurrentIndex(0)
         # 종료 소켓
        self.closesock()
    #
    def closesock(self):
         self.client_socket.close()

if __name__ == "__main__" :
    app = QApplication(sys.argv)
    myWindow = WindowClass()
    myWindow.show()
    app.exec_()
