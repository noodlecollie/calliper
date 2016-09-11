TEMPLATE = subdirs

SUBDIRS += \
	application \
    renderer \
    renderer-sandbox \
    model \
    calliperutil

renderer-sandbox.depends = renderer calliperutil
model.depends = renderer calliperutil
renderer.depends = calliperutil
