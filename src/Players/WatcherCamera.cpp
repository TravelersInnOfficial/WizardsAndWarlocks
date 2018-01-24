#include "WatcherCamera.h"

float lastDistance = 0;

WatcherCamera::WatcherCamera(vector3df position, vector3df lookat){
	// Graphic engine
	GraphicEngine* engine = GraphicEngine::getInstance();
	p_Camera = engine->addCameraSceneNode(position, lookat);

	// intialize variables
	YAngle = 0;
	XZAngle = 0;
	colliding = false;
	cam_distance = 2;
	engine->SetCursorPosition(vector2di(engine->GetScreenWidth()/2, engine->GetScreenHeight()/2));
	lastCursorPosition = engine->GetCursorPosition();

	//Bullet Physics
	//p_BtBody = NULL;
	p_BtBody = new BT_Body();
	position.Y+=1;
	p_BtBody->CreateBox(position, vector3df(0.25, 0.25, 0.25), 1, 0, vector3df(0,0,0), C_CAMERA, cameraCW);
	p_BtBody->SetGravity(vector3df(0.0f,0.0f,0.0f));
	p_BtBody->AssignPointer(this);
	clase = EENUM_CAMERA;
}

WatcherCamera::~WatcherCamera(){
	if(p_Camera!=NULL){
		p_Camera->Erase();
		p_Camera = NULL;
	}
	if(p_BtBody!=NULL){
		p_BtBody->Erase();
		delete p_BtBody;
		p_BtBody = NULL;
	}
}

void WatcherCamera::SetPosition(vector3df position){
	p_Camera->setPosition(position);
}

void WatcherCamera::SetTarget(vector3df lookat){
	p_Camera->setTarget(lookat);
}

void WatcherCamera::UpdateCamera(vector3df target){
	p_BtBody->Update();
	p_BtBody->SetLinearVelocity(vector3df(0,0,0));
	target.Y+=0.5;
	p_Camera->setTarget(target);
	
	float tx, ty, tz;
	tx = ty = tz = 0;

	// Horizontal rotation
	//ty = cam_distance * sin(XZAngle/2 * M_PI /180);
	//tz = cam_distance * cos(XZAngle/2 * M_PI /180);
	//// Vertical rotation
	//tx = cam_distance * sin(YAngle * M_PI /180);     // ßy=90 -> tx = x+2
	//tz *= cos(YAngle * M_PI /180);                    // ßy= 0 -> tz = z-2

	tx = sin(YAngle * M_PI /360) * cos(XZAngle/2 * (M_PI /100)) * cam_distance;
	ty = sin(XZAngle/2 * (M_PI /100)) * cam_distance;
	tz = cos(YAngle * M_PI /360) * cos(XZAngle/2 * (M_PI /100)) * cam_distance;
	
	//ty = cam_distance * sin(XZAngle * (M_PI/50));
	//tx = cam_distance * sin(YAngle * M_PI /180);     // ßy=90 -> tx = x+2
	//tz = cam_distance * cos(YAngle * M_PI /180);     // ßy= 0 -> tz = z-2

	//std::cout<<"Cam Pos: "<< p_Camera->getPosition() << "| tx: " << tx <<"| ty: "<< ty <<"| tz: "<< tz<< "\n";
	//std::cout<<"BT_Body: "<< p_BtBody->GetPosition() << "| NEWPOS: "<< newPosition;
	
	vector2di mousePos = GraphicEngine::getInstance()->GetCursorPosition();
	float mouseOffset = (mousePos - lastCursorPosition).length();

	if(false && mouseOffset<0.5) p_BtBody->SetLinearVelocity(vector3df(0.0f, 0.0f, 0.0f));
	
	else{
		vector3df newPosition(target.X + tx, target.Y + ty, target.Z + tz);

		vector3df camPos = p_BtBody->GetPosition();
		float angle = (float) atan2(camPos.X - target.X, camPos.Z - target.Z);
		angle = angle * 360/M_PI;
		angle = angle<0? angle+=720: angle;


		float diffAngle = 3.0f;
		if(abs(YAngle-angle)<diffAngle){
			//std::cout<<"Minimo"<<std::endl;
		}
		if((YAngle-angle)>diffAngle){
			newPosition.X = target.X + sin((angle+diffAngle) * M_PI /360) * cos(XZAngle/2 * (M_PI /100)) * cam_distance;
			newPosition.Z = target.Z + cos((angle+diffAngle) * M_PI /360) * cos(XZAngle/2 * (M_PI /100)) * cam_distance;
		}
		else if((YAngle-angle)<-diffAngle){
			newPosition.X = target.X + sin((angle-diffAngle) * M_PI /360) * cos(XZAngle/2 * (M_PI /100)) * cam_distance;
			newPosition.Z = target.Z + cos((angle-diffAngle) * M_PI /360) * cos(XZAngle/2 * (M_PI /100)) * cam_distance;
		}


		float distance = (newPosition-camPos).length();
		//std::cout<<"Distance queremos:" << distance << " Distancia tenemos:"<<(p_BtBody->GetPosition()-target).length()<<std::endl;
		//XZANGLE: " << XZAngle << "\n";
	
		//std::cout<<"Distance: "<<distance<<" Angulo NewP: "<<YAngle<<" Angulo Cam: "<<angle<<std::endl;

		newPosition = newPosition - p_BtBody->GetPosition();
		newPosition.normalize();
		p_BtBody->ApplyCentralImpulse(newPosition*3);
		p_Camera->setPosition(p_BtBody->GetPosition());
	}



	if(colliding && cam_distance>=1){
		//std::cout<<"COLISIONOOOO \n";
		//cam_distance-= 0.2;
	}
	else if (cam_distance<=2){
		//std::cout<<"AHORA NOOOOO \n";
		//cam_distance+= 0.2;
	}
	colliding = false;
	UpdateAngles();
	checkMaxVelocity();
}

void WatcherCamera::Contact(void* punt, EntityEnum tipo){
	if(tipo == EENUM_FLOOR){
		colliding = true;
	}
}
/*
void Block::UpdatePosShape(){
	physBody->Update();
	vector3df pos = physBody->GetPosition();
	graphBody->setPosition(pos);
}*/

void WatcherCamera::UpdateAngles(){
	GraphicEngine* engine = GraphicEngine::getInstance();
	int screenW = engine->GetScreenWidth();
	int screenH = engine->GetScreenHeight();
	
	vector2di mousePos = engine->GetCursorPosition();


	if(mousePos.X<=0 || mousePos.X>=screenW){
		if(mousePos.X<=0){
			mousePos.X = screenW;
			lastCursorPosition.X = lastCursorPosition.X>=screenW? mousePos.X-1 : mousePos.X+1;
		}
		else{
			mousePos.X = 0;
			lastCursorPosition.X = lastCursorPosition.X>=screenW? mousePos.X-1 : mousePos.X+1;            
		}
		engine->SetCursorPosition(mousePos);
	}
	else if(mousePos.Y<=0 || mousePos.Y>=screenH){
		if (mousePos.Y<=0){
			mousePos.Y = screenH;
			lastCursorPosition.Y = lastCursorPosition.Y>=screenH? mousePos.Y-1 : mousePos.Y+1;
		}
		else{
			mousePos.Y = 0;
			lastCursorPosition.Y = lastCursorPosition.Y>=screenH? mousePos.Y-1 : mousePos.Y+1;            
		}
		engine->SetCursorPosition(mousePos);
	}

	if(!(mousePos == lastCursorPosition)){
		int yDiff = mousePos.Y - lastCursorPosition.Y;
		yDiff = yDiff>0 ? 3 : yDiff<0? -3 : 0;
		int xDiff = mousePos.X - lastCursorPosition.X;
		xDiff = xDiff>0 ? 3 : xDiff<0? -3 : 0;

		YAngle += xDiff;
		YAngle = YAngle>=720? 0: YAngle<0? 720: YAngle;

		float maxAngle = 40; //[0, 100]
		if(XZAngle-yDiff >-maxAngle && XZAngle-yDiff < maxAngle){
			XZAngle -= yDiff;
			//XZAngle = XZAngle>=720? 0: XZAngle<0? 720: XZAngle;
			//XZAngle = XZAngle>=360? XZAngle-720: XZAngle; 
		}

		//std::cout<<"Last Mouse: "<< lastCursorPosition <<"| Mouse position: "<< mousePos <<"| Y: "<< YAngle<<"| XZ: "<< XZAngle<< "\n";
		lastCursorPosition = mousePos;
	}
}

void WatcherCamera::checkMaxVelocity(){
	float max_velocity = 5.0f;
	
	vector3df velocity = p_BtBody->GetLinearVelocity();
	vector3df auxVelocity(velocity.X, velocity.Y, velocity.Z);
	float speed = auxVelocity.length();

	if(speed > max_velocity) {
		auxVelocity *= max_velocity/speed;
		p_BtBody->SetLinearVelocity(auxVelocity);
	}
}