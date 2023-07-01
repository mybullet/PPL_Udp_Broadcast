
#include "mainwindow.h"

#include <QApplication>
#include <QUdpSocket>
#include <QNetworkInterface>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include "./encryption/encryption.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();

    // 从文件中读取公钥和私钥的字符串表示
    QString publicKeyPath = QCoreApplication::applicationDirPath() + "/key/public_key.pem";
    QString privateKeyPath = QCoreApplication::applicationDirPath() + "/key/private_key.pem";

    QUdpSocket socket;
    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&]() {
        QStringList ipList;

        // 获取本机所有 IPv4 地址
        QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
        foreach (const QNetworkInterface &interface, interfaces) {
            if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
                !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
                QList<QNetworkAddressEntry> entries = interface.addressEntries();
                foreach (const QNetworkAddressEntry &entry, entries) {
                    if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                        ipList.append(entry.ip().toString());
                    }
                }
            }
        }

        // 使用 "-" 将 IP 地址串起来
        QString message = ipList.join("-");

        QByteArray originalData = message.toUtf8();

        // 加密数据
        encryptData(originalData, publicKeyPath, "qwer1234");

        // 发送广播消息
        socket.writeDatagram(originalData, QHostAddress::Broadcast, 1234);
    });

    timer.start(2000);

//    // 原始数据
//    QByteArray originalData = "Hello, World!123456789";

//    // 加密数据
//    encryptData(originalData, publicKeyPath, "qwer1234");

//    qDebug() << "Encrypted data:" << originalData;

//    // 解密数据
//    decryptData(originalData, privateKeyPath, "qwer1234");

//    qDebug() << "Decrypted data:" << originalData;


    return a.exec();
}
