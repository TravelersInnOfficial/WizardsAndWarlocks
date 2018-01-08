#include "PoisonBomb.h"
#include "../Players/Player.h"
#include "../Managers/EffectManager.h"
#include "./../Managers/BulletManager.h"

PoisonBomb::PoisonBomb(vector3df pos, vector3df dir, int emi, float damageMult)
: Projectile(
    pos,                    // initial position of the projectile
    dir,                    // direction of the projectile
    emi,                    // player id 
    0.2f,                   // radius of the projectile
    15.0f,                  // speed of the projectile
    20 * damageMult,        // damage of the projectile
    20.0f,                  // max distance the projectile can travel
    "./../assets/textures/projectils/poisonbomb.png")        // texture of the projectile
{
    //bt_body->SetCollisionFlags("no_contact");
    bt_body->SetGravity(vector3df(0,-9.8,0));
    
    ready2Burst = false;
    bursted = false;
    lastHeight = -1;
    ghostScale = 1;
    framecap = 0;
}

PoisonBomb::~PoisonBomb(){
    bt_ghost->Erase();
    delete bt_ghost;
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
        lastHeight = bt_body->GetPosition().Y;
    }
}

void PoisonBomb::UpdatePosShape(){
    if (!bursted){
        bt_body->Update();
        vector3df pos = bt_body->GetPosition();
        m_ProjectileNode->setPosition(pos);

        float actualHeight = pos.Y;
        if (ready2Burst && framecap++>10){
        
            float heightDiff = actualHeight - lastHeight;
            if (heightDiff < 0.0005 && heightDiff >=0) {
                // bomb is in the correct surface
                Burst();
                ready2Burst = false;
            }
            //else 
            //    std::cout<<"LH: "<< lastHeight << " PY: " << actualHeight << " DIFF: " << heightDiff<<"\n";
            
            lastHeight = actualHeight;
            framecap = 0;
        }
    }
    else{
        // Gas bomb update
        ghostScale += 0.005;
        bt_ghost->SetScale(ghostScale);
        m_ProjectileNode->setScale(vector3df(ghostScale*1.7));

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
		m_ProjectileNode->setMaterialTexture(0, "./../assets/textures/projectils/clouds.jpeg");
	}

    bt_ghost = new BT_GhostObject();
    bt_ghost->CreateGhostBox(bt_body->GetPosition(), vector3df(0.f,0.f,0.f), vector3df(1.5f,1.5f,1.5f));
    bt_ghost->AssignPointer(this);

}