#include "./NpcSeller.h"
#include "./../Managers/SpellManager.h"
#include "./../Managers/PlayerManager.h"
#include "./../Managers/TrapManager.h"

NpcSeller::NpcSeller(vector3df TPosition, vector3df TTScale, vector3df TRotation):Npc(){
	CreatePhysical(TPosition, TTScale, TRotation);
}

NpcSeller::~NpcSeller(){
}

void NpcSeller::CreatePhysical(vector3df TPosition, vector3df TScale, vector3df TRotation){
	GraphicEngine* engine = GraphicEngine::getInstance();
	clase = EENUM_NPC;

	gBody = engine->addObjMeshSceneNode("./../assets/modelos/npc.obj");
	gBody->setPosition(TPosition);
	gBody->setScale(TScale);
	gBody->setRotation(TRotation);
	gBody->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
	if (gBody) {
		gBody->setMaterialFlag(MATERIAL_FLAG::EMF_NORMALIZE_NORMALS, true);
        gBody->setMaterialTexture(0, "./../assets/textures/npc.png");
    }

	vector3df HalfExtents(TScale.X * 0.15f, TScale.Y * 0.35, TScale.Z * 0.15f);
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
		hp = (HumanPlayer*) p;
		hp->ToggleMenu(true);
		hp->SetAllInput(UP);
	}
}

void NpcSeller::Update(){
	MenuOption selected = NO_OPT;
	if (active){
		selected = GraphicEngine::getInstance()->ReadButtonPressed();
		switch(selected){

			// SELECT SPELL TO CHANGE
			case(SELLER_M_PS_2):{
				ChangeChangingSpell(1);
				break;
			}
			case(SELLER_M_PS_3):{
				ChangeChangingSpell(2);
				break;
			}
			case(SELLER_M_PS_4):{
				ChangeChangingSpell(3);
				break;
			}

			// CHANGE SPELL
			case(SELLER_SPEED):{
				ChangeSpell(SPELL_SPEED);
				break;
			}
			case(SELLER_DEFENSE):{
				ChangeSpell(SPELL_DEFENSE);
				break;
			}
			case(SELLER_INVISIBILITY):{
				ChangeSpell(SPELL_INVISIBILITY);
				break;
			}
			case(SELLER_UNTARGET):{
				ChangeSpell(SPELL_UNTARGET);
				break;
			}
			case(SELLER_FIRE):{
				ChangeSpell(SPELL_FIRE);
				break;
			}
			case(SELLER_POISON):{
				ChangeSpell(SPELL_POISON);
				break;
			}
			case(SELLER_THUNDER):{
				ChangeSpell(SPELL_THUNDER);
				break;
			}
			case(SELLER_TELEPORT):{
				ChangeSpell(SPELL_TELEPORT);
				break;
			}
			case(SELLER_CLEANSE):{
				ChangeSpell(SPELL_CLEANSE);
				break;
			}
			case(SELLER_WALL):{
				ChangeSpell(SPELL_WALL);
				break;
			}
			case(SELLER_DUMMY):{
				ChangeSpell(SPELL_DUMMY);
				break;
			}
			case(SELLER_TELEPORTBASE):{
				ChangeSpell(SPELL_TELEPORTBASE);
				break;
			}
			case(SELLER_BLIZZAR):{
				ChangeSpell(SPELL_BLIZZARD);
				break;
			}

			// CHANGE TRAP
			case(SELLER_DEATH_CLAWS):{
				ChangeTrap(TENUM_DEATH_CLAWS);
				break;
			}
			case(SELLER_SPIRITS):{
				ChangeTrap(TENUM_SPIRITS);
				break;
			}
			case(SELLER_SILENCE):{
				ChangeTrap(TENUM_SILENCE);
				break;
			}
			case(SELLER_TAXES):{
				ChangeTrap(TENUM_TAXES);
				break;
			}
			case(SELLER_DISTURBANCE):{
				ChangeTrap(TENUM_DISTURBANCE);
				break;
			}
			case(SELLER_EXPLOSIVE):{
				ChangeTrap(TENUM_EXPLOSIVE);
				break;
			}

			// ACCEPT
			case (SELLER_M_ACCEPT):{
				StopInteraction();
				break;
			}
			default:{
				break;
			}
		}
	}
}

void NpcSeller::StopInteraction(){
	MenuManager::GetInstance()->ClearMenu();
	hp->ToggleMenu(false);
	hp = NULL;
	active = false;
}

void NpcSeller::Draw(){
}

void NpcSeller::ChangeChangingSpell(int changingSpell){
	this->changingSpell = changingSpell;
}

void NpcSeller::ChangeTrap(TrapEnum tEnum){
	if(hp != NULL) TrapManager::GetInstance()->setPlayerTrap(hp, tEnum);
}

void NpcSeller::ChangeSpell(SPELLCODE sEnum){
	
}