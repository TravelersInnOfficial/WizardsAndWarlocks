#include "./NpcSeller.h"

#include <GraphicEngine/GraphicEngine.h>
#include <GraphicEngine/GAnimation.h>
#include <GraphicEngine/MenuManager.h>
#include "./../Players/HumanPlayer.h"
#include <PhysicsEngine/BT_Body.h>
#include <GUIEngine/GUIEngine.h>
#include <Assets.h>
#include <Menus.h>

NpcSeller::NpcSeller(vector3df TPosition, vector3df TTScale, vector3df TRotation):Npc(){
	CreateVisual(TPosition, TTScale, TRotation);
	CreatePhysical(TPosition, TTScale, TRotation);
	clase = EENUM_NPC;
}

NpcSeller::~NpcSeller(){
}

void NpcSeller::CreateVisual(vector3df TPosition, vector3df TScale, vector3df TRotation){
	// CREATE VISUAL BODY
	gBody = GraphicEngine::getInstance()->addAnimatedMeshSceneNode(TPosition, TRotation, TScale);
	if (gBody != nullptr){
		gBody->setMaterialTexture(0, TEXTUREMAP[TEXTURE_NPC_TARANTULA]);

		// ADD ANIMATION
		gBody->SetPaths("idle", ATARANTULA_DEFAULT);
		
		// SET INITIAL LOOP
		gBody->SetAnimationLoop("idle");
	} 

}

void NpcSeller::CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation){
	vector3df HalfExtents(TScale.X * 1.5, TScale.Y * 1.2, TScale.Z * 1.5);
	pBody = new BT_Body();
	pBody->CreateBox(TPosition, HalfExtents,0,0,vector3df(0,0,0), C_NPC, npcCW);
	pBody->SetRotation(TRotation);
	pBody->AssignPointer(this);
}

void NpcSeller::Interact(Player* p){
	if(p->IsPlayerOne()){
		GraphicEngine::getInstance()->InitReceiver();
		MenuManager::GetInstance()->CreateMenu(SELLER_M);
		active = true;
		HumanPlayer* hp = (HumanPlayer*) p;
		hp->ToggleMenu(true);
		hp->SetAllInput(UP);
	}
}

void NpcSeller::ShowInteractInfo(Player* p){
	GUIEngine::GetInstance()->ShowEntityInfo("[E] Shop");
}
