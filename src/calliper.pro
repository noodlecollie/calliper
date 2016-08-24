TEMPLATE = subdirs

SUBDIRS += \
	application \
    renderer \
    renderer-sandbox

renderer-sandbox.depends = renderer
