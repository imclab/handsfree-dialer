TARGET = dialer
TEMPLATE = app
QT += dbus declarative
CONFIG += qdbus mobility qt-mobility link_pkgconfig network
PKGCONFIG += libpulse-mainloop-glib
MOBILITY += contacts multimedia
MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen

DEFINES += DBUS_SERVICE_PATH=\\\"/com/tizen/${QMAKE_TARGET}\\\"
DEFINES += DBUS_SERVICE=\\\"com.tizen.${QMAKE_TARGET}\\\"

if (verbose) {
    DEFINES += VERBOSE 
}

if (wayland) {
    PKGCONFIG += mlite-wayland ofono-qt-wayland
} else if (xlib) {
    PKGCONFIG += mlite-xlib ofono-qt-xlib
} else {
    PKGCONFIG += mlite ofono-qt
}

target.path += $$INSTALL_ROOT/usr/bin 

SOURCES += main.cpp \
    dialercontext.cpp \
    dialerapplication.cpp \
    managerproxy.cpp \
    modemproxy.cpp \
    networkproxy.cpp \
    callitem.cpp \
    callproxy.cpp \
    callmanager.cpp \
    dbustypes.cpp \
    pacontrol.cpp \
    qmlmainwindow.cpp \
    qmldialer.cpp \
    qmlcallitem.cpp \
    dbusdialeradapter.cpp

HEADERS += \
    common.h \
    dialercontext.h \
    dialerapplication.h \
    managerproxy.h \
    modemproxy.h \
    networkproxy.h \
    callitem.h \ 
    callproxy.h \
    callmanager.h \
    dbustypes.h \
    pacontrol.h \
    $$MODEL_HEADERS \
    $$STYLE_HEADERS \
    $$DBUS_INTERFACE_HEADERS \
    $$DBUS_ADAPTOR_HEADERS \
    qmlmainwindow.h \
    qmldialer.h \
    qmlcallitem.h \
    dbusdialeradapter.h

DBUS_ADAPTORS += dbus/com.tizen.hfdialer.xml

DBUS_INTERFACES += \
    dbus/org.ofono.voicecall.xml \
    dbus/org.ofono.manager.xml \
    dbus/org.ofono.modem.xml \
    dbus/org.ofono.operator.xml \

    system(qdbusxml2cpp -a dialer_adaptor.h: dbus/com.tizen.hfdialer.xml)
    system(qdbusxml2cpp -i dbustypes.h -p manager_interface.h: dbus/org.ofono.manager.xml)
    system(qdbusxml2cpp -i dbustypes.h -p modem_interface.h: dbus/org.ofono.modem.xml)

MAKE_CLEAN += $$OBJECTS_DIR/*.o
MAKE_DISTCLEAN += \
    $$MOC_DIR/* $$MOC_DIR \
    $$OBJECTS_DIR/* $$OBJECTS_DIR \
    $$MGEN_OUTDIR/* $$MGEN_OUTDIR \

# Install
INSTALLS += target
