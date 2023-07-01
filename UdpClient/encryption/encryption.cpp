#include "encryption.h"


bool handleErrors()
{
    ERR_print_errors_fp(stderr);
    // 处理错误，例如抛出异常或返回错误码
    // ...
    return false;
}

bool encryptData(QByteArray& data, const QString& publicKeyPath, const QString& passPhrase)
{
    // 读取公钥文件
    FILE* publicKeyFile = fopen(qPrintable(publicKeyPath), "r");
    if (!publicKeyFile) {
        // 处理文件打开失败的情况
        // ...
        return handleErrors();
    }

    // 使用密码短语加载公钥
    EVP_PKEY* publicKey = PEM_read_PUBKEY(publicKeyFile, NULL, NULL, (void*)passPhrase.toStdString().c_str());
    fclose(publicKeyFile);
    if (!publicKey) {
        // 处理公钥加载失败的情况
        // ...
        return handleErrors();
    }

    // 创建 RSA 加密上下文
    RSA* rsa = EVP_PKEY_get1_RSA(publicKey);
    if (!rsa) {
        // 处理 RSA 对象获取失败的情况
        // ...
        EVP_PKEY_free(publicKey);
        return handleErrors();
    }

    // 加密数据
    int dataSize = data.size();
    int encryptedSize = RSA_size(rsa);
    unsigned char* encryptedData = new unsigned char[encryptedSize];

    int result = RSA_public_encrypt(dataSize, reinterpret_cast<const unsigned char*>(data.constData()), encryptedData, rsa, RSA_PKCS1_PADDING);
    if (result == -1) {
        // 处理加密失败的情况
        // ...
        RSA_free(rsa);
        EVP_PKEY_free(publicKey);
        delete[] encryptedData;
        return handleErrors();
    }

    // 更新数据为加密后的数据
    data = QByteArray(reinterpret_cast<char*>(encryptedData), encryptedSize);

    // 释放资源
    RSA_free(rsa);
    EVP_PKEY_free(publicKey);
    delete[] encryptedData;
    return true;
}

bool decryptData(QByteArray& data, const QString& privateKeyPath, const QString& passPhrase)
{
    // 读取私钥文件
    FILE* privateKeyFile = fopen(qPrintable(privateKeyPath), "r");
    if (!privateKeyFile) {
        // 处理文件打开失败的情况
        // ...
        return handleErrors();
    }

    // 使用密码短语加载私钥
    EVP_PKEY* privateKey = PEM_read_PrivateKey(privateKeyFile, NULL, NULL, (void*)passPhrase.toStdString().c_str());
    fclose(privateKeyFile);
    if (!privateKey) {
        // 处理私钥加载失败的情况
        // ...
        return handleErrors();
    }

    // 创建 RSA 解密上下文
    RSA* rsa = EVP_PKEY_get1_RSA(privateKey);
    if (!rsa) {
        // 处理 RSA 对象获取失败的情况
        // ...
        EVP_PKEY_free(privateKey);
        return handleErrors();
    }

    // 解密数据
    int dataSize = data.size();
    int decryptedSize = RSA_size(rsa);
    unsigned char* decryptedData = new unsigned char[decryptedSize];

    int result = RSA_private_decrypt(dataSize, reinterpret_cast<const unsigned char*>(data.constData()), decryptedData, rsa, RSA_PKCS1_PADDING);
    if (result == -1) {
        // 处理解密失败的情况
        // ...
        RSA_free(rsa);
        EVP_PKEY_free(privateKey);
        delete[] decryptedData;
        return handleErrors();
    }

    // 更新数据为解密后的数据
    data = QByteArray(reinterpret_cast<char*>(decryptedData), result);

    // 释放资源
    RSA_free(rsa);
    EVP_PKEY_free(privateKey);
    delete[] decryptedData;
    return true;
}
