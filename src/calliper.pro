TEMPLATE = subdirs

SUBDIRS += \
	application \
    renderer \
    renderer-sandbox \
    model \
    calliperutil \
    vmfviewer \
    vpkinfo \
    file-formats \
    map-importers \
    dep-vtflib \
    dep-qvtf

renderer-sandbox.depends = renderer calliperutil model
model.depends = renderer calliperutil
renderer.depends = calliperutil
vmfviewer.depends = calliperutil renderer model file-formats map-importers
vpkinfo.depends = file-formats calliperutil
file-formats.depends = calliperutil
map-importers.depends = model renderer calliperutil
dep-qvtf.depends = dep-vtflib
