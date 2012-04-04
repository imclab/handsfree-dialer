VERSION = 0.2.5
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

