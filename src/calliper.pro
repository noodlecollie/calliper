TEMPLATE = subdirs

SUBDIRS += \
	application \
    renderer \
    renderer-sandbox \
    model

renderer-sandbox.depends = renderer
model.depends = renderer
