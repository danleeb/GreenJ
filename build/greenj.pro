# ----------------
# Qt Project file
# ----------------

TEMPLATE = app
TARGET = GreenJ
QT += core gui webkit network
win32 {
	DESTDIR = ../bin/win32
	LIBDIR = ../lib/win32
	BUILDDIR = ../build/win32
	QT += qtmain phonon
	PJSIP_DIR = ../lib/win32/pjsip
	PJSIP_TARGET = i386-Win32-vc8-Release
}
unix {
	DESTDIR = ../bin/linux
	LIBDIR = ../lib/linux
	BUILDDIR = ../build/linux
	PJSIP_DIR = ../lib/linux/pjsip
	PJSIP_TARGET = i686-pc-linux-gnu
}
unix:!mac {
	QT += phonon
}
mac {
	DESTDIR = ../bin/mac
	LIBDIR = ../lib/mac
	BUILDDIR = ../build/mac
	PJSIP_DIR = ../lib/mac
	PJSIP_TARGET = i386-apple-darwin11.3.0
}
SOURCEDIR = ../src
RESOURCEDIR = ../res

#CONFIG += debug
CONFIG += ordered

#DEFINES += QT_LARGEFILE_SUPPORT
DEFINES += DEBUG

INCLUDEPATH += $$SOURCEDIR/GeneratedFiles \
    $$SOURCEDIR/GeneratedFiles/Debug \
    $$SOURCEDIR \
    $$PJSIP_DIR \
	$$PJSIP_DIR/pjmedia/include \
    $$PJSIP_DIR/pjsip/include \
    $$PJSIP_DIR/pjnath/include \
    $$PJSIP_DIR/pjmedia/include/pjmedia-codec \
    $$PJSIP_DIR/pjmedia/include/pjmedia-audiodev \
    $$PJSIP_DIR/pjmedia/include/pjmedia \
    $$PJSIP_DIR/pjlib-util/include \
    $$PJSIP_DIR/pjlib/include
unix: INCLUDEPATH += /usr/include/

LIBS += -L/usr/lib/ \
	-L$$LIBDIR/ \
	-L$$PJSIP_DIR/third_party/lib \
	-L$$PJSIP_DIR/pjsip/lib \
	-L$$PJSIP_DIR/pjnath/lib \
	-L$$PJSIP_DIR/pjmedia/lib \
	-L$$PJSIP_DIR/pjlib-util/lib \
	-L$$PJSIP_DIR/pjlib/lib \
	
unix: LIBS += -L/usr/lib/ \
	-lpjsua-$$PJSIP_TARGET \
	-lpjsip-ua-$$PJSIP_TARGET \
	-lpjsip-simple-$$PJSIP_TARGET \
	-lpjsip-$$PJSIP_TARGET \
	-lpjmedia-codec-$$PJSIP_TARGET \
	-lpjmedia-$$PJSIP_TARGET \
	-lpjmedia-audiodev-$$PJSIP_TARGET \
	-lpjnath-$$PJSIP_TARGET \
	-lpjlib-util-$$PJSIP_TARGET \
	-lresample-$$PJSIP_TARGET \
	-lmilenage-$$PJSIP_TARGET \
	-lsrtp-$$PJSIP_TARGET \
	-lgsmcodec-$$PJSIP_TARGET \
	-lspeex-$$PJSIP_TARGET \
	-lilbccodec-$$PJSIP_TARGET \
	-lg7221codec-$$PJSIP_TARGET \
	-lportaudio-$$PJSIP_TARGET  \
	-lpj-$$PJSIP_TARGET \
	-lm \
	-lpthread \
	#-luuid \
	#-lcrypto \
	#-lssl

unix:!mac LIBS += -lnsl -lrt -lasound

win32: LIBS += -lIphlpapi \
    -ldsound \
    -ldxguid \
    -lnetapi32 \
    -lmswsock \
    -lws2_32 \
    -lodbc32 \
    -lodbccp32 \
    -lole32 \
    -luser32 \
    -lgdi32 \
    -ladvapi32 \
    -lpjlib-$$PJSIP_TARGET \
    -lpjlib-util-$$PJSIP_TARGET \
    -lpjmedia-$$PJSIP_TARGET \
    -lpjmedia-codec-$$PJSIP_TARGET \
    -lpjmedia-audiodev-$$PJSIP_TARGET \
    -lpjnath-$$PJSIP_TARGET \
    -lpjsua-lib-$$PJSIP_TARGET \
    -lpjsip-ua-$$PJSIP_TARGET \
    -lpjsip-simple-$$PJSIP_TARGET \
    -lpjsip-core-$$PJSIP_TARGET \
    -llibilbccodec-$$PJSIP_TARGET \
    -llibgsmcodec-$$PJSIP_TARGET \
    -llibg7221codec-$$PJSIP_TARGET \
    -llibmilenage-$$PJSIP_TARGET \
    -llibportaudio-$$PJSIP_TARGET \
    -llibresample-$$PJSIP_TARGET \
    -llibspeex-$$PJSIP_TARGET \
    -llibsrtp-$$PJSIP_TARGET

mac: LIBS += -lssl -lcrypto -framework CoreAudio -framework AudioUnit -framework AudioToolbox -framework Cocoa

DEPENDPATH += $$SOURCEDIR
MOC_DIR += $$SOURCEDIR/GeneratedFiles/debug
OBJECTS_DIR += $$BUILDDIR
UI_DIR += $$SOURCEDIR/GeneratedFiles
RCC_DIR += $$SOURCEDIR/GeneratedFiles

HEADERS += $$SOURCEDIR/phone/api/Interface.h \
    $$SOURCEDIR/phone/api/Sip.h \
    $$SOURCEDIR/phone/Account.h \
    $$SOURCEDIR/phone/Call.h \
    $$SOURCEDIR/phone/Phone.h \
    $$SOURCEDIR/Config.h \
    $$SOURCEDIR/Gui.h \
    $$SOURCEDIR/JavascriptHandler.h \
    $$SOURCEDIR/LogHandler.h \ 
    $$SOURCEDIR/PrintHandler.h \
    $$SOURCEDIR/Sound.h \
    $$SOURCEDIR/WebPage.h
SOURCES += $$SOURCEDIR/main.cpp \
    $$SOURCEDIR/phone/api/Sip.cpp \
    $$SOURCEDIR/phone/Account.cpp \
    $$SOURCEDIR/phone/Call.cpp \
    $$SOURCEDIR/phone/Phone.cpp \
    $$SOURCEDIR/Config.cpp \
    $$SOURCEDIR/Gui.cpp \
    $$SOURCEDIR/JavascriptHandler.cpp \
    $$SOURCEDIR/LogHandler.cpp \
    $$SOURCEDIR/PrintHandler.cpp \
    $$SOURCEDIR/Sound.cpp
FORMS += $$SOURCEDIR/gui.ui
RESOURCES += $$RESOURCEDIR/gui.qrc

win32:RC_FILE = $$RESOURCEDIR/greenj.rc
mac:ICON = $$RESOURCEDIR/images/greenj.icns
