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

file-formats.depends = calliperutil
tst-keyvaluesparser.depends = file-formats calliperutil
rendersystem.depends = calliperutil containers profiling
model.depends = rendersystem calliperutil profiling
model-loaders.depends = model rendersystem calliperutil file-formats dep-vtflib
user-interface.depends = rendersystem calliperutil model file-formats model-loaders dep-vtflib profiling
sb-rendersystem-test.depends = calliperutil rendersystem containers
tst-keyvaluesparser.depends = file-formats calliperutil
app-vpkinfo.depends = file-formats calliperutil
app-vmfviewer.depends = calliperutil rendersystem model file-formats model-loaders dep-vtflib user-interface profiling
app-calliper.depends = calliperutil rendersystem model file-formats model-loaders dep-vtflib user-interface profiling
