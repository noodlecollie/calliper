TEMPLATE = subdirs

SUBDIRS += \
	application \
    renderer \
    renderer-sandbox \
    model \
    calliperutil \
    vmfviewer \
    serialisation

renderer-sandbox.depends = renderer calliperutil model
model.depends = renderer calliperutil
renderer.depends = calliperutil
vmfviewer.depends = calliperutil renderer model
serialisation.depends = model renderer calliperutil
