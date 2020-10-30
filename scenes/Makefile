.PHONY : all

#n.b. the '-y' sets autoexec scripts to 'on' so that driver expressions will work
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	BLENDER = /Applications/Blender/blender.app/Contents/MacOS/blender -y
else ifeq ($(UNAME_S),Linux)
	BLENDER = ../../blender-2.90.0-linux64/blender -y
else
	BLENDER = blender
endif

EXPORT_MESHES=export-meshes.py
EXPORT_WALKMESHES=export-walkmeshes.py
EXPORT_SCENE=export-scene.py

DIST=../dist

all : \
	$(DIST)/toxic-assets.pnct \
	$(DIST)/toxic-assets.scene \

$(DIST)/toxic-assets.pnct : toxic-assets.blend $(EXPORT_MESHES)
	$(BLENDER) --background --python $(EXPORT_MESHES) -- '$<':Platforms '$@'

$(DIST)/toxic-assets.scene : toxic-assets.blend $(EXPORT_SCENE)
	$(BLENDER) --background --python $(EXPORT_SCENE) -- '$<':Platforms '$@'
