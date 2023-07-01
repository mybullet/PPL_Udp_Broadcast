#pragma

//openssl genpkey -algorithm RSA -out private_key.pem -aes256   qwer1234
//openssl rsa -pubout -in private_key.pem -out public_key.pem   qwer1234

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#include <QByteArray>
#include <QFile>

bool handleErrors();
bool encryptData(QByteArray& data, const QString& publicKeyPath, const QString& passPhrase);
bool decryptData(QByteArray& data, const QString& privateKeyPath, const QString& passPhrase);


