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
    this->setStyleSheet("background-color: #637089;");


    ui->textBrowser->setStyleSheet("QTextBrowser { background-color: black;}");
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
    connect(ui->btnFlash, SIGNAL(clicked),this, SLOT(on_btnFlash_clicked));
    connect(ui->btnCompile, SIGNAL(clicked),this , SLOT(on_btnCompile_clicked));
    connect(fileBtn, SIGNAL(triggered()), this, SLOT(set_MCU()));
    connect(boardBtn, SIGNAL(triggered()), this, SLOT(set_boardAsArduino()));

}

void MainWindow::openConsole(std::string currentPath){

    process = new QProcess(this);
    QString cmd = "cd";
    QStringList args;
    args << QString::fromStdString(currentPath) << "||  pwd";
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

void MainWindow::menuToolbarCreate()
{
    // 메뉴바에 도형 메뉴 추가작
        pGraphMenu = menuBar()->addMenu(tr("&Select Board"));
        ToolBar = addToolBar(tr("Select Board"));
        ToolBar->setStyleSheet("color: #FFFFFF;");
        // 아이콘 설정
        fileBtn = new QAction(tr("&atmega328p"), this);
        boardBtn = new QAction(tr("&arduino"), this);

        // 메뉴바에 "그래프 시작 추가
        pGraphMenu->addAction(fileBtn);
        ToolBar->addAction(boardBtn);

}

void MainWindow::set_MCU(){
    //enable btns
    MCU_set_flag = 1;
    if(board_set_flag && MCU_set_flag) {
        ui->btnCompile->setEnabled(true);
        ui->btnCompile->setStyleSheet("color: #FFFFFF;");
    }
    avrType = "atmega328p";
    flashType = "m328p";
}

void MainWindow::set_boardAsArduino(){
    board_set_flag = 1;
    if(board_set_flag && MCU_set_flag) {
        ui->btnCompile->setEnabled(true);
        ui->btnCompile->setStyleSheet("color: #FFFFFF;");
    }
    boardName = "Arduino";
}

void MainWindow::fillPortsInfo()
{
    ui->comboBox->clear();

    const auto infos = QSerialPortInfo::availablePorts(); // 시리얼 포트 정보 이용가능하도록
    for(const QSerialPortInfo &info : infos) {
        ui->comboBox->addItem(info.portName()); // 콤보박스에 시리얼포트 정보 받아온 이름을 붙여넣는다.
    }

    ui->baudrateBox->addItem("57600");
    ui->baudrateBox->addItem("115200");
    ui->baudrateBox->addItem("230400");
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
    arguments.clear();
    arguments << ("-p"+flashType).c_str() << ("-P/dev/"+ui->comboBox->currentText().toStdString()).c_str() <<("-c"+boardName).c_str() << "-U" << ("flash:w:"+fileName+".flash.hex:i").c_str();
    flashCommand(command, arguments);
    arguments.clear();

}

void MainWindow::on_btnCompile_clicked(){
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
        command = "avr-gcc";
        QString arg = ("-g -DF_CPU=160000 -Wall -Os -Wextra -mmcu="+avrType+" -Wa,-ahlmns="+fileName+".lst -c -o "+fileName+".o "+fileName+".c").c_str();
        arguments << arg.split(" ");
        consoleCommand(command,arguments);

        arguments.clear();
        arg = ("-g -DF_CPU=160000 -Wall -Os -Wextra -mmcu="+avrType+" -o -o "+fileName+".elf "+fileName+".o").c_str();
        arguments << arg.split(" ");
        flashCommand(command,arguments);
        std::system(("chmod a-x "+fileName+".elf 2>&1").c_str());
        command = "avr-objcopy";
        arguments.clear();
        arg = ("-j .text -j .data -O ihex "+fileName+".elf"+" "+fileName+".flash.hex").c_str();
        arguments << arg.split(" ");
        consoleCommand(command,arguments);
        //std::system(("avr-objcopy -j .text -j .data -O ihex "+fileName+".elf "+fileName+".flash.hex").c_str());
        //arguments.clear();
        //arguments << "-j .text" << "-j .eeprom" << "--set-section-flags=.eeprom=" << "\"alloc,load\" --change-section-lma .eeprom=0" << "-O" << "ihex" << (fileName+".elf").c_str() << (fileName+".eeprom.hex").c_str();
        //consoleCommand(command,arguments);
        std::system(("avr-objcopy -j .eeprom --set-section-flags=.eeprom=\"alloc,load\" --change-section-lma .eeprom=0 -O ihex "+fileName+".elf "+fileName+".eeprom.hex").c_str());
        //arguments.clear();
        //arguments << "-j .fuse" << "-O" << "ihex" << (fileName+".elf").c_str() <<  (fileName+".fuses.hex").c_str() << "--change-section-lma .fuse=0";
        //consoleCommand(command,arguments);
        std::system(("avr-objcopy -j .fuse -O ihex "+fileName+".elf "+fileName+".fuses.hex --change-section-lma .fuse=0").c_str());
        ui->btnFlash->setEnabled(true);
        ui->btnFlash->setStyleSheet("color: #FFFFFF;");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


