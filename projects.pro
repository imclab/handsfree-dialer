include (common.pri)
TEMPLATE = subdirs
CONFIG += ordered 
SUBDIRS = src qml dialerassets sounds

#OTHER_FILES += dialer.service
OTHER_FILES += *.service *.desktop *.sh
M_INSTALL_BIN = /usr/bin
M_INSATLL_DATA = /usr/share

# Desktop
desktop_entry.files = dialer.desktop
desktop_entry.path += $$INSTALL_ROOT/usr/share/applications
desktop_entry.CONFIG += no_check_exist

# DBus service
dbus_service.files = dialer.service
dbus_service.path += $$INSTALL_ROOT/usr/share$$M_DBUS_SERVICES_DIR

# Documentation
documentation.files = AUTHORS ChangeLog LICENSE README TODO
documentation.path  = $$INSTALL_ROOT/usr/share/hfdialer/

INSTALLS += \
    desktop_entry \
    dbus_service \
    documentation \

