TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.cc

QMAKE_CXXFLAGS += -std=c++11 -O0

#DEFINES += _RELEASE
#DEFINES += _DEBUG

HEADERS += \
    main.h
