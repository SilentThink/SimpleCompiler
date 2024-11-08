QT       += core gui widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codegen.cpp \
    emurunning.cpp \
    header.cpp \
    lexer.cpp \
    main.cpp \
    optimize.cpp \
    parser.cpp \
    semanticanalysis.cpp

HEADERS += \
    codegen.h \
    emurunning.h \
    header.h \
    lexer.h \
    optimize.h \
    parser.h \
    semanticanalysis.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
