VERSION = 0.2.6
CONFIG += link_pkgconfig network opengl

TEMPLATE = subdirs
CONFIG += ordered 
SUBDIRS = src qml dialerassets sounds

OTHER_FILES += *.service *.desktop *.sh

# Desktop
desktop_entry.files = dialer.desktop
desktop_entry.path += $$INSTALL_ROOT/usr/share/applications
desktop_entry.CONFIG += no_check_exist

# DBus service
dbus_service.files = dialer.service
dbus_service.path += $$INSTALL_ROOT/usr/share/dbus-1/services

# Documentation
documentation.files = AUTHORS ChangeLog LICENSE README TODO
documentation.path  = $$INSTALL_ROOT/usr/share/hfdialer/

INSTALLS += \
    desktop_entry \
    dbus_service \
    documentation \

PROJECT_NAME = hfdialer

dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION} &&
dist.commands += git clone . $${PROJECT_NAME}-$${VERSION} &&
dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION}/.git &&
dist.commands += tar jcpvf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION}
QMAKE_EXTRA_TARGETS += dist
