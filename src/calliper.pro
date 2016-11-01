TEMPLATE = subdirs

SUBDIRS += \
	application \
    renderer \
    renderer-sandbox \
    model \
    calliperutil \
    vmfviewer \
    serialisation \
    vpkinfo

renderer-sandbox.depends = renderer calliperutil model serialisation
model.depends = renderer calliperutil
renderer.depends = calliperutil
vmfviewer.depends = calliperutil renderer model serialisation
serialisation.depends = model renderer calliperutil
