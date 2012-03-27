TEMPLATE = subdirs
CONFIG += ordered

assets.files += *.png *.jpg
assets.path  += $${installPrefix}/usr/share/hfdialer/images

INSTALLS += assets
