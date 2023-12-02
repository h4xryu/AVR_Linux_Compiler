#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
#include <QThread>
#include <iostream>
#include "qprocessoutputcapturer.h"
#include <unistd.h>

using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serialPort(new QSerialPort(this))
{
    ui->setupUi(this);
    this->setWindowTitle("Linux AVR Compiler");
    this->setStyleSheet("background-color: #233039;");
    this->setWindowIcon(QIcon("/home/student/avr_linux_compiler/avr.png"));
    ui->textBrowser->setStyleSheet("QTextBrowser { background-color: #030812;}");
    ui->textBrowser->setTextColor(QColor(Qt::green));
    ui->centralwidget->setStyleSheet("color: #FFFFFF;");
    ui->btnCompile->setStyleSheet("color: #666666;");
    ui->btnFlash->setStyleSheet("color: #666666;");
    ui->comboBox->setStyleSheet("color: #FFFFFF;");
    ui->menubar->setStyleSheet("color: #FFFFFF;");
    ui->baudrateBox->setStyleSheet("color: #FFFFFF;");

    menuToolbarCreate();
    fillPortsInfo();
    ui->btnCompile->setEnabled(false);
    ui->btnFlash->setEnabled(false);
    boardBtn->setEnabled(false);
    tinyISPBtn->setEnabled(false);
    usb2ttlBtn->setEnabled(false);
    connect(ui->btnFlash, SIGNAL(clicked),this, SLOT(on_btnFlash_clicked));
    connect(ui->btnCompile, SIGNAL(clicked),this , SLOT(on_btnCompile_clicked));
    connect(fileBtn, SIGNAL(triggered()), this, SLOT(set_MCU()));

    connect(boardBtn, SIGNAL(triggered()), this, SLOT(set_boardAsArduino()));
    connect(tinyISPBtn, SIGNAL(triggered()), this, SLOT(set_USBtinyISP()));
    connect(usb2ttlBtn, SIGNAL(triggered()), this, SLOT(set_USB2TTL()));

    ui->textBrowser->append("Simple Linux AVR Compiler v1.0");
    ui->textBrowser->append("Select a MCU first.");


}

void MainWindow::openConsole(std::string currentPath){

    process = new QProcess(this);
    QString cmd = "ls";
    QStringList args;
    args << QString::fromStdString(currentPath);
    //std::system(("sudo cd "+currentPath).c_str());
    process->start(cmd, args);
    if (process->waitForStarted()){
        process->waitForFinished(-1);
        // 명령의 표준 출력 가져오기
        QByteArray result = process->readAllStandardOutput();

        // 명령의 표준 에러 가져오기
        QByteArray error = process->readAllStandardError();
        ui->textBrowser->setTextColor(QColor(Qt::green));
        ui->textBrowser->append(result);
        ui->textBrowser->setTextColor(QColor(Qt::red));
        ui->textBrowser->append(error);
        ui->textBrowser->setTextColor(QColor(Qt::green));

        process->exitCode();
    }
}

void MainWindow::consoleCommand(QString cmd, QStringList args){
    // QProcess를 사용하여 명령 실행
    process->start(cmd, args);

    if (process->waitForStarted()) {
       process->waitForFinished(-1); // 대기 시간을 -1로 설정하면 명령이 완료될 때까지 대기

       // 명령의 표준 출력 가져오기
       QByteArray result = process->readAllStandardOutput();

       // 명령의 표준 에러 가져오기
       QByteArray error = process->readAllStandardError();
\
       ui->textBrowser->setTextColor(QColor(Qt::green));
       ui->textBrowser->append(result);
       ui->textBrowser->setTextColor(QColor(Qt::red));
       ui->textBrowser->append(error);
       ui->textBrowser->setTextColor(QColor(Qt::green));

       // 명령의 종료 코드 가져오기
       int exitCode = process->exitCode();



   } else {
       qDebug() << "Failed to start the process.";
   }

    ui->textBrowser->append((cmd.toStdString()+" "+args[0].toStdString()+"... Done.").c_str());
}

void MainWindow::flashCommand(QString cmd, QStringList args){
    // QProcess를 사용하여 명령 실행
    process->start(cmd, args);

    if (process->waitForStarted()) {
       process->waitForFinished(-1); // 대기 시간을 -1로 설정하면 명령이 완료될 때까지 대기

       // 명령의 표준 출력 가져오기
       QByteArray result = process->readAllStandardOutput();

       // 명령의 표준 에러 가져오기
       QByteArray error = process->readAllStandardError();
\
       ui->textBrowser->setTextColor(QColor(Qt::green));
       ui->textBrowser->append(result);
       ui->textBrowser->append(error);

       // 명령의 종료 코드 가져오기
       int exitCode = process->exitCode();



   } else {
       qDebug() << "Failed to start the process.";
   }

}


void MainWindow::noprintCommand(QString cmd, QStringList args){
    // QProcess를 사용하여 명령 실행
    process->start(cmd, args);

    if (process->waitForStarted()) {
       process->waitForFinished(-1); // 대기 시간을 -1로 설정하면 명령이 완료될 때까지 대기



       // 명령의 종료 코드 가져오기
       int exitCode = process->exitCode();



   } else {
       qDebug() << "Failed to start the process.";
   }
    ui->textBrowser->append((cmd.toStdString()+" "+args[0].toStdString()+"... Done.").c_str());
}

void MainWindow::menuToolbarCreate()
{

    // 메뉴바에 도형 메뉴 추가작
        pGraphMenu = menuBar()->addMenu(tr("&Select MCU"));
        ToolBar = addToolBar(tr("Select Board"));
        ToolBar->setStyleSheet("color: #FFFFFF;");
        // 아이콘 설정
        fileBtn = new QAction(tr("&atmega328p"), this);
        stm32Btn = new QAction(tr("&STM32"), this);
        boardBtn = new QAction(tr("&Arduino"), this);
        tinyISPBtn = new QAction(tr("&USBtinyISP"), this);
        usb2ttlBtn = new QAction(tr("&USB2TTL"), this);
        // 메뉴바에 "그래프 시작 추가
        pGraphMenu->addAction(fileBtn);
        pGraphMenu->addAction(stm32Btn);
        ToolBar->addAction(boardBtn);
        ToolBar->addAction(tinyISPBtn);
        ToolBar->addAction(usb2ttlBtn);

}

void MainWindow::set_MCU(){
    //enable btns
    MCU_set_flag = 1;
    if(board_set_flag && MCU_set_flag) {
        ui->btnCompile->setEnabled(true);
        ui->btnCompile->setStyleSheet("color: #FFFFFF;");
    }
    boardBtn->setEnabled(true);
    tinyISPBtn->setEnabled(true);
    usb2ttlBtn->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->comboBox->setStyleSheet("color: #FFFFFF;");
    avrType = "atmega328p";
    flashType = "m328p";

    ui->textBrowser->clear();
    ui->textBrowser->append("Select avrdude programmer_type. (Arduino, ISP...)");

}

void MainWindow::set_USB2TTL(){
    board_set_flag = 1;
    boardNum = 1;
    if(board_set_flag && MCU_set_flag) {
        ui->btnCompile->setEnabled(true);
        ui->btnCompile->setStyleSheet("color: #FFFFFF;");
    }
    ui->comboBox->setEnabled(false);
    ui->comboBox->setStyleSheet("color: #666666;");
    boardName = "stk500";
    cpu_clock = "16000000";
}

void MainWindow::set_USBtinyISP(){
    board_set_flag = 1;
    boardNum = 2;
    if(board_set_flag && MCU_set_flag) {
        ui->btnCompile->setEnabled(true);
        ui->btnCompile->setStyleSheet("color: #FFFFFF;");
    }
    ui->comboBox->setEnabled(false);
    ui->comboBox->setStyleSheet("color: #666666;");
    boardName = "usbtiny";
    cpu_clock = "16000000";

}


void MainWindow::set_boardAsArduino(){
    board_set_flag = 1;
    boardNum = 3;
    if(board_set_flag && MCU_set_flag) {
        ui->btnCompile->setEnabled(true);
        ui->btnCompile->setStyleSheet("color: #FFFFFF;");
    }
    boardName = "Arduino";
    cpu_clock = "16000000";
}

void MainWindow::fillPortsInfo()
{
    ui->comboBox->clear();

    const auto infos = QSerialPortInfo::availablePorts(); // 시리얼 포트 정보 이용가능하도록
    for(const QSerialPortInfo &info : infos) {
        ui->comboBox->addItem(info.portName()); // 콤보박스에 시리얼포트 정보 받아온 이름을 붙여넣는다.
    }

    ui->baudrateBox->addItem("57600");
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
    }
}

void MainWindow::on_btnFlash_clicked(){
    ui->textBrowser->clear();
    ui->textBrowser->append("Uploading Firmware...");
    command = "avrdude";


    if(boardNum == 0 ){
         ui->textBrowser->append("Please select board before");
         return;
    }

    if(boardNum == 1){
        ui->textBrowser->append("USB to TTL isn't supported");
        return;
    }

    if(boardNum == 2){
        arguments.clear();
        QString arg = ("-p attiny2313 -c "+boardName+" -F -U flash:w:"+fileName+".flash.hex:i").c_str();
        arguments << arg.split(" ");
        flashCommand(command, arguments);
    }

    if(boardNum == 3){
        arguments.clear();
        QString arg = ("-p "+avrType+" -P /dev/"+ui->comboBox->currentText().toStdString()+" -c "+boardName+" -U flash:w:"+fileName+".flash.hex:i").c_str();
        arguments << arg.split(" ");
        flashCommand(command, arguments);
    }

    arguments.clear();
    ui->comboBox->setEnabled(true);
    fillPortsInfo();
    ui->comboBox->setStyleSheet("color: #FFFFFF;");

}

void MainWindow::on_btnCompile_clicked(){
    ui->textBrowser->clear();
    fillPortsInfo();
    c_file = QFileDialog::getOpenFileName(this, "Select a C file", "./","*.c");


    if(c_file != NULL){

        fileName = c_file.toStdString();
        fileName.pop_back();
        fileName.pop_back();
        currentPath = fileName;
        std::string extracted_tmp;
        std::string extracted_fileName;


        for(int i = 0; i < currentPath.size(); i++){ //extract only filename
            char tmp = currentPath[currentPath.size()-i-1];
            extracted_tmp.push_back(tmp);
            if(tmp == '/') break;
        }
        for(int i = extracted_tmp.size() -2; i >= 0; i--){
            extracted_fileName.push_back(extracted_tmp[i]);
        }

        onlyName = extracted_fileName;


        while(1){
            char tmp = currentPath[currentPath.size()-1];
            currentPath.pop_back();
            if(tmp == '/') break;
        }

        fileName = currentPath+"/"+onlyName;
        openConsole(currentPath);



        command = "rm";
        QString arg = (fileName+".elf "+fileName+".eeprom.hex "+fileName+".fuses.hex "+fileName+".flash.hex "+fileName+".o").c_str();
        arguments.clear();
        arguments << arg.split(" ");
        consoleCommand(command,arguments);


        command = "avr-gcc";
        arg = ("-g -DF_CPU="+cpu_clock+" -Wall -Os -Wextra -mmcu="+avrType+" -Wa,-ahlmns="+fileName+".lst -c -o "+fileName+".o"+" "+fileName+".c").c_str();
        arguments.clear();
        arguments << arg.split(" ");
        consoleCommand(command,arguments);


        arguments.clear();
        arg = ("-g -DF_CPU="+cpu_clock+" -Wall -Os -Wextra -mmcu="+avrType+" -o "+fileName+".elf"+" "+fileName+".o").c_str();
        arguments << arg.split(" ");
        noprintCommand(command,arguments);


        arguments.clear();
        command = "chmod";
        arg = ("a-x "+fileName+".elf 2>&1").c_str();
        arguments << arg.split(" ");
        noprintCommand(command,arguments);

        arguments.clear();
        arg = ("-g -DF_CPU="+cpu_clock+" -Wall -Os -Wextra -mmcu="+avrType+" -b "+ui->baudrateBox->currentText().toStdString()+" -v -U eeprom:w:"+fileName+"eeprom.hex").c_str();
        arguments << arg.split(" ");
        noprintCommand(command,arguments);


        command = "avr-objcopy";
        arguments.clear();
        arg = ("-j .text -j .data -O ihex "+fileName+".elf"+" "+fileName+".flash.hex").c_str();
        arguments << arg.split(" ");
        consoleCommand(command,arguments);
        //std::system(("avr-objcopy -j .text -j .data -O ihex "+fileName+".elf"+" "+fileName+".flash.hex").c_str());


        arguments.clear();
        arg = ("-j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 -O ihex "+fileName+".elf "+fileName+".eeprom.hex").c_str();
        arguments << arg.split(" ");
        flashCommand(command,arguments);
        std::system(("avr-objcopy -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 -O ihex "+fileName+".elf "+fileName+".eeprom.hex").c_str());

        arguments.clear();
        arg = (" -j .fuse -O ihex "+fileName+".elf "+fileName+".fuses.hex --change-section-lma .fuse=0").c_str();
        arguments << arg.split(" ");
        noprintCommand(command,arguments);
        //std::system(("avr-objcopy -j .fuse -O ihex "+fileName+".elf "+fileName+".fuses.hex --change-section-lma .fuse=0").c_str());

        ui->btnFlash->setEnabled(true);
        ui->btnFlash->setStyleSheet("color: #FFFFFF;");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}




