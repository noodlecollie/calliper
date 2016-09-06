TEMPLATE = subdirs

SUBDIRS += \
	application \
    renderer \
    renderer-sandbox \
    model \
    util

renderer-sandbox.depends = renderer util
model.depends = renderer util
renderer.depends = util
