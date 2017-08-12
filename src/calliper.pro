TEMPLATE = subdirs

SUBDIRS += \
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
    rendersystem \
    containers \
    sb-rendersystem-test \
    profiling

model.depends = rendersystem calliperutil
rendersystem.depends = calliperutil containers profiling
app-vmfviewer.depends = calliperutil rendersystem model file-formats model-loaders dep-vtflib user-interface
app-vpkinfo.depends = file-formats calliperutil
file-formats.depends = calliperutil
model-loaders.depends = model rendersystem calliperutil file-formats dep-vtflib
dep-qvtf.depends = dep-vtflib
tst-keyvaluesparser.depends = file-formats calliperutil
user-interface.depends = rendersystem calliperutil model file-formats model-loaders dep-vtflib
app-calliper.depends = calliperutil rendersystem model file-formats model-loaders dep-vtflib user-interface
sb-rendersystem-test.depends = calliperutil rendersystem containers
