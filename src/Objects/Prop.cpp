#include "Prop.h"
#include <GraphicEngine/GraphicEngine.h>

Prop::Prop(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string model, std::string tex){
    GraphicEngine* engine = GraphicEngine::getInstance();
	
	if(model != ""){
        // Create an Irrlicht model
        graphBody = engine->addObjMeshSceneNode(model, TPosition, TRotation, TScale);
        graphBody->setMaterialTexture(0, tex);
        
        graphBody->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
        graphBody->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        graphBody->setAutomaticCulling();

    }else{
        graphBody = engine->addCube2Scene(TPosition, TRotation, TScale);
        graphBody->setMaterialTexture(0, tex);
    }
}

Prop::~Prop(){
    delete graphBody;
}