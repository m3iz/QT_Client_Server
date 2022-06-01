#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>

class QFile;
class QTcpSocket;

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private:
    Ui::Client *ui;

    QTcpSocket *tcpClient;
    qint64 totalBytes;
    qint64 dataSize;
    QString fileName;
    QImage image;
    QString currentImageName;
    QFile *localFile;
    QByteArray dataOut;

    volatile bool isReady;

private slots:

    void connectServer();
    void tcpConnected();
    void tcpDisconnected();
    void openFile();
    void send();
    void startTransfer();

    QByteArray getImageData(const QImage&);

    void on_openButton_clicked();
    void on_sendButton_clicked();
    void on_connectButton_clicked();
signals:
    void buildConnected();
};

#endif // CLIENT_H
