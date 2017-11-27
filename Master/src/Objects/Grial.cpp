#include "Grial.h"

Grial::Grial(){
	casting = false;

	startTime = 0.0f;
	timeCasting = 5.0f;
	currentCasting = 0.0f;


	CreateGrial();
}

Grial::~Grial(){}

void Grial::Update(){
	if(casting){
		casting = false;

		//float deltaTime = GraphicEngine::getInstance()->getTime() * 0.001;
		//timeCasting += currentTime - startTime;		// Le sumamos la diferencia entre la ultima vez que se llamo al metodo
		//startTime = currentTime;	
	}
	UpdatePosShape();
}

void Grial::Interact(Player* p){
	float deltaTime = GraphicEngine::getInstance()->getTime() * 0.001;
	casting = true;
}

void Grial::CreateGrial(){
	vector3df TPosition(-3,0,-3);
	vector3df TScale(1,1,1);
	vector3df TRotation(0,0,0);


	GraphicEngine* engine = GraphicEngine::getInstance();

	// Create an Irrlicht cube
	m_grialNode = engine->addCube2Scene(TScale);
	m_grialNode->setPosition(TPosition);
	m_grialNode->setScale(TScale);
	m_grialNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_grialNode) {
		m_grialNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_grialNode->setMaterialTexture(0, "./../assets/textures/pocion.png");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,1,0);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

void Grial::UpdatePosShape(){
	bt_body->Update();
    vector3df* pos = bt_body->GetPosition();
    m_grialNode->setPosition(*pos);
}