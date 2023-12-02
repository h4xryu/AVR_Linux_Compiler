#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QVector>
#include <QQueue>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QMenuBar>
#include <QToolBar>
#include <QInputDialog>
#include <cstring>
#include <QFileDialog>
#include <QDir>
#include <QObject>
#include <QProcess>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QString portName;
    char *device;
    QQueue<QVector<double>> data;
    QByteArray ba;
    QMenu *pGraphMenu;
    QToolBar *ToolBar;
    QAction *stm32Btn;
    QAction *fileBtn;
    QAction *boardBtn;
    QAction *tinyISPBtn;
    QAction *usb2ttlBtn;
    std::string fileName;
    QAction *openBtn;
    QString c_file;
    std::string avrType;
    std::string flashType;
    std::string onlyName;
    std::string boardName;
    std::string currentPath;
    std::string cpu_clock;
    int MCU_set_flag = 0;
    int board_set_flag = 0;
    QProcess *process;
    QStringList arguments;
    QString command;
    int boardNum = 0;
    void breakFlag();
    bool flag = 0;
    void menuToolbarCreate();
    void openConsole(std::string currentPath);
    void consoleCommand(QString cmd, QStringList args);
    void flashCommand(QString cmd, QStringList args);
    void noprintCommand(QString cmd, QStringList args);




    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void fillPortsInfo();


    QSerialPort* m_serialPort = nullptr;
private slots:
    void handleError(QSerialPort::SerialPortError error);
    void set_boardAsArduino();
    void set_USB2TTL();
    void on_btnFlash_clicked();
    void on_btnCompile_clicked();
    void set_MCU();
    void set_USBtinyISP();
};
#endif // MAINWINDOW_H
