TEMPLATE = subdirs
CONFIG += ordered

qml.files = *.qml javascripts
qml.path  = $${installPrefix}/usr/share/hfdialer/qml

INSTALLS += qml
