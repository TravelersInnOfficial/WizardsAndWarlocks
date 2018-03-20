#include "Light.h"
#include "GraphicEngine.h"
#include <TravelersOcularEngine/src/TOcularEngine/TOcularEngine.h>

Light::Light(vector3df TPosition, vector3df Color, float range){
	SceneManager* sm = VideoDriver::GetInstance()->GetSceneManager();
	toe::core::TOEvector3df position = toe::core::TOEvector3df(TPosition.X, TPosition.Y, TPosition.Z);
	toe::core::TOEvector3df rotation = toe::core::TOEvector3df(0,0,0);
	toe::core::TOEvector4df color = toe::core::TOEvector4df(Color.X, Color.Y, Color.Z, 1);
	
	float attenuation = 0.1f - (range / 100.0f); // Attenuation: 0.1 TO 0 |||| Range: 0 to 10
	if(attenuation < 0) attenuation = 0.0f;
	if(attenuation > 1) attenuation = 0.1f;

    privateNode = (TFNode*)sm->AddLight(position, rotation, color, attenuation);
}

Light::~Light(){
	VideoDriver::GetInstance()->GetSceneManager()->DeleteLight((TFLight*)privateNode);
}

void Light::SetActiveState(bool active){
	TFLight* light = (TFLight*)privateNode;
	light->SetActive(active);
}

void Light::ChangeColor(vector3df color){
	toe::core::TOEvector4df toecolor = toe::core::TOEvector4df(color.X, color.Y, color.Z, 1);
	TFLight* light = (TFLight*)privateNode;
	light->SetColor(toecolor);
}

void Light::ChangeAttenuation(float attenuation){
	TFLight* light = (TFLight*)privateNode;
	light->SetAttenuation(attenuation);
}