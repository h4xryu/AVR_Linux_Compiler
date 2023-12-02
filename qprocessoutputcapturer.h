#ifndef QPROCESSOUTPUTCAPTURER_H
#define QPROCESSOUTPUTCAPTURER_H
#include<QTextBrowser>
#include<QObject>
#include<QProcess>

class QProcessOutputCapturer : public QObject
{
    Q_OBJECT
public:
    QProcessOutputCapturer(QProcess* pProcess, QTextBrowser* pTextBrowser)
              : QObject(pProcess),
                m_pTextBrowser(pTextBrowser)

            {
                QObject::connect(pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(slot_readyReadStandardOutput()));
            }

public slots:
        // this is called whenever the QProcess::readyReadStandardOutput() signal is fired
        void slot_readyReadStandardOutput() {
            QProcess* pProcess = qobject_cast<QProcess*>(sender());
            if (pProcess) {
                m_pTextBrowser->append(pProcess->readAllStandardOutput());
            }
        }

protected:
    QTextBrowser* m_pTextBrowser;
};



#endif // QPROCESSOUTPUTCAPTURER_H
