TEMPLATE = subdirs

SUBDIRS += \
	app-calliper \
    renderer \
    renderer-sandbox \
    model \
    calliperutil \
    app-vmfviewer \
    app-vpkinfo \
    file-formats \
    model-loaders \
    dep-vtflib \
    dep-qvtf

renderer-sandbox.depends = renderer calliperutil model
model.depends = renderer calliperutil
renderer.depends = calliperutil
app-vmfviewer.depends = calliperutil renderer model file-formats model-loaders
app-vpkinfo.depends = file-formats calliperutil
file-formats.depends = calliperutil
model-loaders.depends = model renderer calliperutil
dep-qvtf.depends = dep-vtflib
