TARGET = dialer
TEMPLATE = app
QT += dbus declarative
CONFIG += qdbus mobility qt-mobility link_pkgconfig network
PKGCONFIG += libpulse-mainloop-glib
MOBILITY += contacts multimedia
MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen

DEFINES += DBUS_SERVICE_PATH=\\\"/com/${QMAKE_TARGET}\\\"
DEFINES += DBUS_SERVICE=\\\"com.${QMAKE_TARGET}\\\"

verbose {
    DEFINES += VERBOSE 
}

wayland {
    PKGCONFIG += mlite-wayland ofono-qt-wayland
}
xlib{
    PKGCONFIG += mlite-xlib ofono-qt-xlib
}
!xlib:!wayland{
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
	dbusdialeradapter.cpp \
	voicecall_interface.cpp \
	operator_interface.cpp \
	hfdialer_adaptor.cpp \
	manager_interface.cpp \
	modem_interface.cpp

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
	voicecall_interface.h \
	operator_interface.h \
	hfdialer_adaptor.h \
	manager_interface.h \
	modem_interface.h \
    qmlmainwindow.h \
    qmldialer.h \
    qmlcallitem.h \
    dbusdialeradapter.h



system(qdbusxml2cpp -p voicecall_interface dbus/org.ofono.voicecall.xml)
system(qdbusxml2cpp -p operator_interface dbus/org.ofono.operator.xml)
system(qdbusxml2cpp -a hfdialer_adaptor dbus/com.hfdialer.xml)
system(qdbusxml2cpp -i dbustypes.h -p manager_interface dbus/org.ofono.manager.xml)
system(qdbusxml2cpp -i dbustypes.h -p modem_interface dbus/org.ofono.modem.xml)

MAKE_CLEAN += $$OBJECTS_DIR/*.o
MAKE_DISTCLEAN += \
    $$MOC_DIR/* $$MOC_DIR \
    $$OBJECTS_DIR/* $$OBJECTS_DIR \
    $$MGEN_OUTDIR/* $$MGEN_OUTDIR \

# Install
INSTALLS += target
