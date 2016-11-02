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
    map-importers

renderer-sandbox.depends = renderer calliperutil model
model.depends = renderer calliperutil
renderer.depends = calliperutil
vmfviewer.depends = calliperutil renderer model file-formats map-importers
vpkinfo.depends = file-formats
file-formats.depends = calliperutil
map-importers.depends = model renderer calliperutil
