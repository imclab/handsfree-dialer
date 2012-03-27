TEMPLATE = subdirs
CONFIG += ordered

sounds.files += *.wav
sounds.path  += $${installPrefix}/usr/share/hfdialer/sounds

INSTALLS += sounds
