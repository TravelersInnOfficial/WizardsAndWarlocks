#include "./NpcSelector.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GAnimation.h>
#include <GraphicEngine/MenuManager.h>
#include "./../Players/HumanPlayer.h"
#include <PhysicsEngine/BT_Body.h>
#include <GUIEngine/GUIEngine.h>
#include <Assets.h>
#include <Menus.h>

NpcSelector::NpcSelector(vector3df TPosition, vector3df TTScale, vector3df TRotation):Npc(){
	CreateVisual(TPosition, TTScale, TRotation);
	CreatePhysical(TPosition, TTScale, TRotation);
	clase = EENUM_NPC;
}

NpcSelector::~NpcSelector(){
}

void NpcSelector::CreateVisual(vector3df TPosition, vector3df TScale, vector3df TRotation){
	// CREATE VISUAL BODY
	//gBody = g_engine->addObjMeshSceneNode("./../assets/modelos/npc.obj");
	gBody = GraphicEngine::getInstance()->addAnimatedMeshSceneNode(TPosition, TRotation, TScale);
	if (gBody != nullptr ){
		gBody->setMaterialTexture(0, "./../assets/textures/genie.png");

		// ADD ANIMATION
		gBody->SetPaths("idle", AGENIE_DEFAULT);
		
		// SET INITIAL LOOP
		gBody->SetAnimationLoop("idle");
	} 
	

}

void NpcSelector::CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation){
	vector3df HalfExtents(TScale.X * 1.2f, TScale.Y * 1.2f, TScale.Z * 1.2f);
	pBody = new BT_Body();
	pBody->CreateBox(TPosition, HalfExtents,0,0,vector3df(0,0,0), C_NPC, npcCW);
	pBody->SetRotation(TRotation);
	pBody->AssignPointer(this);
}

void NpcSelector::Interact(Player* p){
	if(p->IsPlayerOne()){
		GraphicEngine::getInstance()->InitReceiver();
		MenuManager::GetInstance()->CreateMenu(ALLIANCE_M);
		active = true;
		HumanPlayer* hp = (HumanPlayer*) p;
		hp->ToggleMenu(true);
		hp->SetAllInput(UP);
	}
}

void NpcSelector::ShowInteractInfo(){
	GUIEngine::GetInstance()->ShowEntityInfo("[E] Change Alliance");
}