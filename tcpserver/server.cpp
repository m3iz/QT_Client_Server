#include "server.h"
#include "ui_server.h"

#include <QtNetwork>

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    connect(&tcpServer, SIGNAL(newConnection()),
            this, SLOT(acceptConnection()));

    ui->imageLabel->show();
}

Server::~Server()
{
    delete ui;
}

void Server::start()
{
    if (!tcpServer.listen(QHostAddress::LocalHost, ui->portLineEdit->text().toInt())) {
        qDebug() << tcpServer.errorString();
        close();
        return;
    }

    bytesReceived = 0;
    totalBytes = 0;
    imageSize = 0;

    ui->serverStatusLabel->setText(tr("Listening"));

}

void Server::acceptConnection()
{
    tcpServerConnection = tcpServer.nextPendingConnection();

    connect(tcpServerConnection, SIGNAL(readyRead()),
            this, SLOT(updateServerProgress()));

    ui->serverStatusLabel->setText(tr("Connection"));
}


void Server::updateServerProgress()
{
    QDataStream in(tcpServerConnection);
    in.setVersion(QDataStream::Qt_5_6);

    if (bytesReceived <= sizeof(qint64)*2) {
        if((tcpServerConnection->bytesAvailable() >= sizeof(qint64)*2)
                && (imageSize == 0)) {
            in >> totalBytes  >> imageSize;
            bytesReceived += sizeof(qint64) * 2;

            if(imageSize == 0){
                  ui->serverStatusLabel->setText(tr("Picture is empty!"));
            }
        }
        if((tcpServerConnection->bytesAvailable() >= imageSize)
                && (imageSize != 0)) {

            in >> imageContent;

            ui->serverStatusLabel->setText(tr("Receiving file"));

            QImage imageData = getImage(imageContent);

            QPixmap resImage = QPixmap::fromImage(imageData);
            QPixmap* imgPointer = &resImage;
            imgPointer->scaled(ui->imageLabel->size(), Qt::IgnoreAspectRatio);

            ui->imageLabel->setScaledContents(true);
            ui->imageLabel->setPixmap(*imgPointer);

            bytesReceived += imageSize;

            if(bytesReceived == totalBytes){
                 ui->serverStatusLabel->setText(tr("File received"));
                 totalBytes = 0;
                 bytesReceived = 0;
                 imageSize = 0;
            }

         }
     }
}

QImage Server::getImage(const QString &data)
{
    QByteArray imageData = QByteArray::fromBase64(data.toLatin1());
    QImage image;
    image.loadFromData(imageData);
    return image;
}

void Server::on_startButton_clicked()
{
        start();
}
