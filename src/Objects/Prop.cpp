#include "Prop.h"
#include "../GraphicEngine/GraphicEngine.h"

Prop::Prop(vector3df TPosition, vector3df TRotation, vector3df TScale, std::string model, std::string tex){
    GraphicEngine* engine = GraphicEngine::getInstance();
	
	if(model != ""){
        float TMass = 0;
        clase = EENUM_FLOOR;

        // Create an Irrlicht model
        graphBody = engine->addObjMeshSceneNode(model, TPosition, TRotation, TScale);
        graphBody->setMaterialTexture(0, tex);
        
        graphBody->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
        graphBody->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        graphBody->setAutomaticCulling();
        graphBody->setTriangleSelector();

        /*
        //Bullet Physics
        vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
        physBody = new BT_Body();
        physBody->CreateBox(TPosition, HalfExtents,TMass,0, vector3df(0,0,0), C_WALL, wallCW);
        physBody->Rotate(TRotation);
        physBody->AssignPointer(this);
        */
        
    }else{
        cout<<"ERROR: No model passed to prop \n"; 
    }
}

Prop::~Prop(){
    delete graphBody;
    delete physBody;
}


void Prop::Update(){
	UpdatePosShape();
}

void Prop::UpdatePosShape(){
	physBody->Update();
    vector3df pos = physBody->GetPosition();
    graphBody->setPosition(pos);
}

vector3df Prop::GetPosition(){
	return physBody->GetPosition();
}