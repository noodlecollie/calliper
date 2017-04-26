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
    user-interface \
    app-calliper \
    app-vpkbrowser \
    sb-frame-buffer-rendering \
    rendersystem

renderer-sandbox.depends = calliperutil renderer model file-formats model-loaders dep-vtflib user-interface
model.depends = renderer calliperutil
renderer.depends = calliperutil
app-vmfviewer.depends = calliperutil renderer model file-formats model-loaders dep-vtflib user-interface
app-vpkinfo.depends = file-formats calliperutil
file-formats.depends = calliperutil
model-loaders.depends = model renderer calliperutil file-formats dep-vtflib
dep-qvtf.depends = dep-vtflib
tst-keyvaluesparser.depends = file-formats calliperutil
user-interface.depends = renderer calliperutil model file-formats model-loaders dep-vtflib
app-calliper.depends = calliperutil renderer model file-formats model-loaders dep-vtflib user-interface
app-vpkbrowser.depends = calliperutil file-formats user-interface
sb-frame-buffer-rendering.depends = calliperutil renderer model file-formats model-loaders dep-vtflib user-interface
