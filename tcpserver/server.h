#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QTcpServer>

class QFile;
class QTcpSocket;

namespace Ui {
    class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private:
    Ui::Server *ui;

    QTcpServer tcpServer;
    QTcpSocket *tcpServerConnection;
    qint64 bytesReceived;
    qint64 totalBytes;
    QString fileName;
    QFile *localFile;
    qint64 fileNameSize;
    QImage image;
    qint64 imageSize;
    QString imageContent;

private slots:
    void start();
    void acceptConnection();
    void updateServerProgress();

    QImage getImage(const QString &);

    void on_startButton_clicked();
};


#endif // SERVER_H
