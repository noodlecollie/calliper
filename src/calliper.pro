TEMPLATE = subdirs

SUBDIRS += \
    renderer \
    renderer-sandbox \
    model \
    calliperutil \
    app-vmfviewer \
    app-vpkinfo \
    file-formats \
    model-loaders \
    dep-vtflib \
    tst-keyvaluesparser \
    high-level-convenience \
    app-calliper \
    app-resize-test

renderer-sandbox.depends = calliperutil renderer model file-formats model-loaders dep-vtflib high-level-convenience
model.depends = renderer calliperutil
renderer.depends = calliperutil
app-vmfviewer.depends = calliperutil renderer model file-formats model-loaders dep-vtflib high-level-convenience
app-vpkinfo.depends = file-formats calliperutil
file-formats.depends = calliperutil
model-loaders.depends = model renderer calliperutil file-formats dep-vtflib
dep-qvtf.depends = dep-vtflib
tst-keyvaluesparser.depends = file-formats calliperutil
high-level-convenience.depends = renderer calliperutil model file-formats model-loaders dep-vtflib
