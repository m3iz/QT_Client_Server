#include "client.h"
#include "ui_client.h"

#include <QFileDialog>
#include <QtNetwork>

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    dataSize = 64 * 1024;
    totalBytes = 0;
    isReady = false;
    
    ui->sendButton->setEnabled(false);

    tcpClient = new QTcpSocket(this);

    connect(tcpClient, SIGNAL(connected()), this, SLOT(tcpConnected()));
    connect(this, SIGNAL(buildConnected()), this, SLOT(startTransfer()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(tcpDisconnected()));
}

Client::~Client()
{
    delete ui;
}

void Client::openFile()
{
    fileName = QFileDialog::getOpenFileName(this);
    
    if (!fileName.isEmpty()) {
        
        currentImageName = fileName.right(fileName.size()
                                                 - fileName.lastIndexOf('/')-1);

        ui->clientStatusLabel->setText(tr("Файл %1 успешно открыт!").arg(currentImageName));

        if(isReady == true){
            ui->sendButton->setEnabled(true);
        }
    }
}

void Client::send()
{
    if(!isReady){
        ui->clientStatusLabel->setText(tr("Пожалуйста, сначала подключитесь к серверу"));
        return;
    }else{
        emit buildConnected();
    }
}

void Client::connectServer()
{
    ui->clientStatusLabel->setText(tr("Соединение…"));

    tcpClient->connectToHost(ui->hostLineEdit->text(),
                             ui->portLineEdit->text().toInt());

    isReady = true;
}


void Client::startTransfer()
{
    QDataStream sendOut(&dataOut, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_6);

    QImage image(fileName);
    QString imageData = getImageData(image);

    sendOut << qint64(0) << qint64(0) << imageData;

    totalBytes += dataOut.size();
    sendOut.device()->seek(0);

    sendOut << totalBytes << qint64((dataOut.size() - sizeof(qint64)*2));

    tcpClient->write(dataOut);

    dataOut.resize(0);

    ui->clientStatusLabel->setText(tr("File %1 sent").arg(currentImageName));
    totalBytes = 0;
}

void Client::tcpConnected()
{
    isReady = true;
    ui->connectButton->setText(tr("Disconnect"));

    ui->clientStatusLabel->setText(tr("Connected"));
}

void Client::tcpDisconnected()
{
    isReady = false;
    tcpClient->abort();
    ui->connectButton->setText(tr("Connect"));

    ui->clientStatusLabel->setText(tr("Связь потеряна"));
}

QByteArray Client::getImageData(const QImage &image)
{
    QByteArray imageData;
    QBuffer buffer(&imageData);
    image.save(&buffer, "png");
    imageData = imageData.toBase64();
    
    return imageData;
}

void Client::on_openButton_clicked()
{
    ui->clientStatusLabel->setText(tr("Статус: Ожидание открытия файла！"));
    openFile();

}

void Client::on_sendButton_clicked()
{
    send();
}

void Client::on_connectButton_clicked()
{
        tcpClient->abort();
        connectServer();
}

