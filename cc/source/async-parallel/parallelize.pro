TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# Libs
BOOST=/home/zaqwes/work/third_party/boost_1_55_0
DETAILS=..
_3RD_PARTY=../3rdparty
ADOBE_SL=/home/zaqwes/work/third_party/source_release

# но лучше бы 11 убрать, т.к. не ясно нужно ли доопределять некоторые функции
# -std=c++11
# Opt. keys:
# http://publib.boulder.ibm.com/infocenter/comphelp/v7v91/index.jsp?topic=%2Fcom.ibm.xlf91a.doc%2Fxlfug%2Fhu00509.htm
# -std=c++11
QMAKE_CXXFLAGS += -O3  -Wall -Wno-deprecated -pg 

SOURCES += \
  pools_test.cc \

  
SOURCES += \
  ../launcher.cc \
  $$_3RD_PARTY/gmock-1.6.0/fused-src/gmock-gtest-all.cc \

INCLUDEPATH += \
  $$BOOST \
  $$DETAILS \
  $$DETAILS/.. \
  $$ADOBE_SL \
  $$_3RD_PARTY/gmock-1.6.0/fused-src/ \
  /home/zaqwes/work/third_party/threadpool-0_2_5-src/threadpool 

LIBS += -lpthread -lrt -ltbb
LIBS += -L$$BOOST/stage/lib -lboost_thread -lboost_system

OBJECTS_DIR = obj