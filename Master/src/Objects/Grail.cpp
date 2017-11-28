#include "Grail.h"
#include "./../Game.h"

Grail::Grail(){
	casting = false;

	timeCasting = 0.0f;
	maxCasting = 5.0f;


	CreateGrail();
}

Grail::~Grail(){}

void Grail::Update(){
	if(casting){
		casting = false;
	}
	else{
		timeCasting = 0.0f;
	}
	UpdatePosShape();
}

void Grail::Interact(Player* p){
	float deltaTime = Game::GetInstance()->GetDeltaTime();
	timeCasting+=deltaTime;
	if(timeCasting>=maxCasting){
		std::cout<<"YOU WIN"<<std::endl;
		timeCasting=0.0f;
	}

	casting = true;
}

void Grail::CreateGrail(){
	vector3df TPosition(-3,0.5,-3);
	vector3df TScale(1,1,1);
	vector3df TRotation(0,0,0);


	GraphicEngine* engine = GraphicEngine::getInstance();

	// Create an Irrlicht cube
	m_grailNode = engine->addCube2Scene(TScale);
	m_grailNode->setPosition(TPosition);
	m_grailNode->setScale(TScale);
	m_grailNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);

	if (m_grailNode) {
		m_grailNode->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        m_grailNode->setMaterialTexture(0, "./../assets/textures/grail.png");
    }

	//Bullet Physics
	vector3df HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	bt_body = new BT_Body();
	bt_body->CreateBox(TPosition, HalfExtents,0,0);
	bt_body->Rotate(TRotation);
	bt_body->AssignPointer(this);
}

void Grail::UpdatePosShape(){
	bt_body->Update();
    vector3df pos = bt_body->GetPosition();
    m_grailNode->setPosition(pos);
}