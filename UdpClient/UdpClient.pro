QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    encryption/encryption.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    encryption/encryption.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -ladvapi32 -lws2_32 -luser32 -lcrypt32


win32: LIBS += -L$$PWD/OpenSSL/lib/ -llibcrypto

INCLUDEPATH += $$PWD/OpenSSL/include
DEPENDPATH += $$PWD/OpenSSL/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/OpenSSL/lib/libcrypto.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/OpenSSL/lib/liblibcrypto.a

win32: LIBS += -L$$PWD/OpenSSL/lib/ -llibssl

INCLUDEPATH += $$PWD/OpenSSL/include
DEPENDPATH += $$PWD/OpenSSL/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/OpenSSL/lib/libssl.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/OpenSSL/lib/liblibssl.a
