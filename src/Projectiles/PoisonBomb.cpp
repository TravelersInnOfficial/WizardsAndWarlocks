#include "PoisonBomb.h"
#include "../Players/Player.h"
#include "../Managers/EffectManager.h"
#include "./../Managers/BulletManager.h"
#include <cmath>

PoisonBomb::PoisonBomb(vector3df pos, vector3df dir, int emi, float damageMult)
: Projectile(
    pos,                    // initial position of the projectile
    dir,                    // direction of the projectile
    emi,                    // player id 
    0.2f,                   // radius of the projectile
    15.0f,                  // speed of the projectile
    20 * damageMult,        // damage of the projectile
    25.0f,                  // max distance the projectile can travel
    "./../assets/textures/projectils/SPELL_POISON.png")        // texture of the projectile
{
    //bt_body->SetCollisionFlags("no_contact");
    bt_body->SetGravity(vector3df(0,-9.8,0));
    
    ready2Burst = false;
    bursted = false;
    ghostScale = 1;
}

PoisonBomb::~PoisonBomb(){
    if(bt_ghost != NULL){
    bt_ghost->Erase();
    delete bt_ghost;
        bt_ghost = NULL;
    }
}

/**
 * @brief Method called when collides with a player
 * 
 * @param p 
 */
void PoisonBomb::ContactAction(Player* p){
    if(!ready2Burst){
        if(!bursted)
            p->ChangeHP(-damage);
        EffectManager::GetInstance()->AddEffect(p, WEAK_POISONED);
    }
}

/**
 * @brief method called after colliding with something
 * 
 */
void PoisonBomb::ContactBehavior(){
    if(!ready2Burst && !bursted){
        ready2Burst = true;
    }
}

// update modified of projectile
void PoisonBomb::UpdatePosShape(){
    if (!bursted){
        bt_body->Update();
        vector3df pos = bt_body->GetPosition();
        m_ProjectileNode->setPosition(pos);

        float yvel = bt_body->GetLinearVelocity().Y;
        //std::cout<<"YVEL: " << yvel <<"\n";
        if (ready2Burst){
        
            if (fabs(yvel) < 0.000317574){
                // bomb is in the correct surface
                Burst();
                ready2Burst = false;
            }
            //else 
                //std::cout<<"NO COLLIDE: LY: "<< lastHeight << " Y: " << actualHeight << " DIST: " << heightDiff << " R,B: "<< ready2Burst << ", " << bursted << std::endl;
        }
    }
    else{
        // Gas bomb update
        ghostScale += 0.005;
        bt_ghost->SetScale(ghostScale);
        m_ProjectileNode->setScale(vector3df(ghostScale*2));

        if(ghostScale > 3.0f){
            BulletManager::GetInstance()->AddToDeleteProyecil(this);
        }
    }
}

void PoisonBomb::Burst(){
    //std::cout<<"BOOM, "  << actualHeight << " lastH: " << lastHeight << "S: " << heightDiff << " \n";

    // Set bursted to true
    bursted = true;

    // Stop physics body
    //bt_body->SetCollisionFlags("no_contact");
    bt_body->SetLinearVelocity(vector3df(0.f,0.f,0.f));
    bt_body->SetCollisionFlags("no_contact");

    // delete visual node
	m_ProjectileNode->Erase();
	delete m_ProjectileNode;
    
    // Cargamos el modelo mierder
	m_ProjectileNode = GraphicEngine::getInstance()->addObjMeshSceneNode("../assets/modelos/bomb.obj");
    m_ProjectileNode->setPosition(bt_body->GetPosition());
	m_ProjectileNode->setScale(vector3df(2,2,2));

	// Aplicamos Material unlit y Textura
	if (m_ProjectileNode) {
		m_ProjectileNode->setMaterialFlag(MATERIAL_FLAG::EMF_LIGHTING, false);
		m_ProjectileNode->setMaterialTexture(0, "./../assets/textures/projectils/SPELL_POISON.png");
	}

    bt_ghost = new BT_GhostObject();
    bt_ghost->CreateGhostBox(bt_body->GetPosition(), vector3df(0.f,0.f,0.f), vector3df(1.5f,1.5f,1.5f));
    bt_ghost->AssignPointer(this);

}