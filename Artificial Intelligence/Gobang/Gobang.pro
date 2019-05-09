TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        alphaBetaSearch.cpp \
        display.cpp \
        evaluate.cpp \
        gobang.cpp \
        main.cpp \
        operate.cpp

HEADERS += \
    gobang.h
