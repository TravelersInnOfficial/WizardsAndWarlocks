#ifndef OVERLAYCODES_H
#define OVERLAYCODES_H

enum OverlayCodes{
	BLOOD		= 0,
	HITLANDED	= 1,
	PARALYZED	= 3,
	FROZEN		= 4,
	FIRE		= 5,
	POISION		= 6
};

static std::string OverlayPath[] = {
	"./../assets/textures/overlays/BLOOD.png",
	"./../assets/textures/overlays/HITLANDED.png",
	"./../assets/textures/overlays/PARALYZED.png",
	"./../assets/textures/overlays/FROZEN.png",
	"./../assets/textures/overlays/FIRE.png",
	"./../assets/textures/overlays/POISION.png"
};

#endif
