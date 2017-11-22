#include "Proyectil.h"
#include "ControlProyectil.h"

Proyectil::Proyectil(float dirX, float dirY, float dirZ, float r, float v){
    direction.push_back(dirX);
    direction.push_back(dirY);
    direction.push_back(dirZ);
    NormalizeDir();
    radio = r;
    velocity = v;
    clase = "proyectil";
}

Proyectil::~Proyectil(){

    //SI pongo los delete la cosa peta
    //delete m_playerRigidBody;
    //delete m_playerShape;

    //delete m_playerMesh;
    //delete m_playerNode;
}

void Proyectil::CreateProyectil(btDiscreteDynamicsWorld* dynamicsWorld, irr::scene::ISceneManager* sceneManager, irr::IrrlichtDevice *device, irr::video::IVideoDriver *driver){
    // CREATES THE PLAYER SHAPE (A BOX IN THIS CASE)
	//m_playerShape = new btSphereShape(radio);

    // 0.7 = Relacion radio con medio lado de un cuadrado inciso en un circulo
    const btVector3 HalfExtents(0.7*radio,0.7*radio,0.7*radio);
    m_playerShape = new btBoxShape(HalfExtents);

	// CREATE INITIAL MOTION STATE OF THE PLAYER
	// Con el 1 en offset contrarrestamos el -1 del suelo
    btDefaultMotionState* playerMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 1, 0)));

    // CREATE PLAYER INFO
    playerMass = 1;
    btVector3 playerInertia(0, 0, 0);
    m_playerShape->calculateLocalInertia(playerMass, playerInertia);
    btRigidBody::btRigidBodyConstructionInfo playerRigidBodyCI(playerMass, playerMotionState, m_playerShape, playerInertia);

    // ADD PLAYER TO THE WORLD
    m_playerRigidBody = new btRigidBody(playerRigidBodyCI);
    dynamicsWorld->addRigidBody(m_playerRigidBody);

     //Le metemos dentro un puntero a el mismo
    m_playerRigidBody->setUserPointer(this);

    // IRRLICHT
    // Cargamos la esfera
    m_playerNode = sceneManager->addSphereSceneNode(radio, 16);

    if (m_playerNode){
        m_playerNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        m_playerNode->setMaterialTexture( 0, driver->getTexture("./assets/textures/proyectil.jpg"));
    }

    //LE DESACTIVA LAS FUERZA
    m_playerRigidBody->setGravity(btVector3(0, 0, 0));

    //DESACTIVA EL GIRO
    m_playerRigidBody->setAngularFactor( btVector3(0, 0, 0));

}

void Proyectil::Update(){
    btVector3 vel(direction[0], direction[1], direction[2]);
    vel = vel * velocity;
    m_playerRigidBody->setLinearVelocity(vel);
    
    UpdatePosShape();
}

void Proyectil::UpdatePosShape(){

    btTransform trans;
    m_playerRigidBody->getMotionState()->getWorldTransform(trans);

    float m_posX = trans.getOrigin().getX();
    float m_posY = trans.getOrigin().getY();
    float m_posZ = trans.getOrigin().getZ();

    irr::core::vector3df pos(m_posX, m_posY, m_posZ);
    m_playerNode->setPosition(pos);
}

void Proyectil::Contact(void* punt, std::string tipo){
    if(tipo.compare("player")==0){
        ControlProyectil* c = ControlProyectil::GetInstance();
        c->AddToDeleteProyecil(this);
        cout << "ups colisionaste wey"<<endl;
    }
}

btRigidBody* Proyectil::GetBody(){
    return m_playerRigidBody;
 }

irr::scene::ISceneNode* Proyectil::GetShape(){
    return m_playerNode;
}

void Proyectil::NormalizeDir(){
    float length = sqrt(pow(direction[0], 2) + pow(direction[1], 2) + pow(direction[2],2));
    direction[0] = direction[0]/length;
    direction[1] = direction[1]/length;
    direction[2] = direction[2]/length;
}