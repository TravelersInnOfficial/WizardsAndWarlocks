#ifndef GUIENUMS_H
#define GUIENUMS_H

enum GUIAssetType {
	GUI_LUASCRIPTS      = 1,
	GUI_IMAGESETS       = 2,
	GUI_FONTS           = 3,
	GUI_SCHEMES         = 4,
	GUI_LOOKNFEELS      = 5,
	GUI_LAYOUTS         = 6
};

enum GUIInputType {
	GUI_MOUSEPOS		= 0,
	GUI_MOUSELCLICK     = 1,
	GUI_MOUSERCLICK     = 2,
	GUI_KEYPRESSED		= 3
};

enum GUIEventType {
	GUI_MOUSECLICK		= 0
};

#endif