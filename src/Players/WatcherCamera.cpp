#include "WatcherCamera.h"

float lastDistance = 0;

WatcherCamera::WatcherCamera(vector3df position, vector3df lookat){
	// Graphic engine
	GraphicEngine* engine = GraphicEngine::getInstance();
	p_Camera = engine->addCameraSceneNode(position, lookat);

	// intialize variables
	YAngle = 0;
	XZAngle = 0;
	mouseVelocity = 6;
	colliding = false;
	cam_distance = 2;

	lastCursorPosition = engine->GetCursorPosition();
	ResetMousePos();

	//Bullet Physics
	//p_BtBody = NULL;
	p_BtBody = new BT_Body();
	position.Y+=1;
	p_BtBody->CreateBox(position, vector3df(0.5, 0.5, 0.5), 1, 0, vector3df(0,0,0), C_CAMERA, cameraCW);
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
	//p_BtBody->SetLinearVelocity(vector3df(0,0,0));
	target.Y+=0.5;
	p_Camera->setTarget(target);
	
	float tx, ty, tz;
	tx = ty = tz = 0;

	// Horizontal rotation & Vertical rotation
	tx = sin(YAngle * M_PI /180) * cos(XZAngle * (M_PI /180)) * cam_distance;
	ty = sin(XZAngle * (M_PI /180)) * cam_distance;
	tz = cos(YAngle * M_PI /180) * cos(XZAngle * (M_PI /180)) * cam_distance;

	vector3df newPosition(target.X + tx, target.Y + ty, target.Z + tz);

	vector3df camPos = p_BtBody->GetPosition();
	float angle = atan2(camPos.X - target.X, camPos.Z - target.Z);
	angle = angle * 180/M_PI;
	angle = angle<0? angle+=360: angle;

	float distance = (newPosition-camPos).length();
	//std::cout<<"Distance queremos:" << distance << " Distancia tenemos:"<<(p_BtBody->GetPosition()-target).length()<<std::endl;
	//std::cout<< "XZANGLE: " << XZAngle << "\n";

	//std::cout<<"Distance: "<<distance<<" Angulo NewP: "<<YAngle<<" Angulo Cam: "<<angle<<std::endl;
	if(distance>0.1f){
		float diffAngle = YAngle-angle;
		/*if(abs(diffAngle)>20.0f){
			newPosition.X = sin((angle+20.0f*sign(diffAngle)) * M_PI /180) * cos(XZAngle * (M_PI /180)) * cam_distance;
			newPosition.Z = cos((angle+20.0f*sign(diffAngle))* M_PI /180) * cos(XZAngle * (M_PI /180)) * cam_distance;
		}*/
		newPosition = newPosition - camPos;
		newPosition.normalize();
		p_BtBody->ApplyCentralImpulse(newPosition*10);
	}else{
		p_BtBody->SetLinearVelocity(vector3df(0,0,0));
	}
	p_Camera->setPosition(p_BtBody->GetPosition());


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

int WatcherCamera::sign(float value){
	if(value>=0) return 1;
	return -1;
}

void WatcherCamera::Contact(void* punt, EntityEnum tipo){
	if(tipo == EENUM_FLOOR){
		colliding = true;
	}
}

void WatcherCamera::UpdateAngles(){
	GraphicEngine* engine = GraphicEngine::getInstance();
	int screenW = engine->GetScreenWidth();
	int screenH = engine->GetScreenHeight();
	
	vector2di mousePos = engine->GetCursorPosition();

	// Reseteamos la posicion del raton al centro de la pantalla
	ResetMousePos();

/*
	// First of all, check the X axis
	if(mousePos.X<=0 || mousePos.X>=screenW){
		if(mousePos.X<=0){
			mousePos.X = screenW;
			lastCursorPosition.X = lastCursorPosition.X>=screenW? mousePos.X-1 : mousePos.X+1;
		}
		else{
			mousePos.X = 0;
			lastCursorPosition.X = lastCursorPosition.X>=screenW? mousePos.X-1 : mousePos.X+1;            
		}
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
	}
*/

	int yDiff = screenH/2 - mousePos.Y;
	if(abs(yDiff)>5){
		XZAngle += 5*sign(yDiff)*-1;
		XZAngle = XZAngle>60? 60: XZAngle<-60? -60: XZAngle;
	}
	
	//yDiff = yDiff>0 ? mouseVelocity	 : yDiff<0? -mouseVelocity : yDiff;
	//xDiff = xDiff>0 ? mouseVelocity : xDiff<0? -mouseVelocity : xDiff;

	int xDiff =screenW/2 - mousePos.X;
	if(abs(xDiff)>5){
		YAngle += 5*sign(xDiff)*-1;
		YAngle = YAngle>360? YAngle-360: YAngle<0? YAngle+360: YAngle;
	}

	/*float maxAngle = 90; //[0, 100]
	if(XZAngle-yDiff >-maxAngle && XZAngle-yDiff < maxAngle){
		XZAngle -= yDiff;
		//XZAngle = XZAngle>=720? 0: XZAngle<0? 720: XZAngle;
		//XZAngle = XZAngle>=360? XZAngle-720: XZAngle; 
	}*/

	//lastCursorPosition = mousePos;

}

void WatcherCamera::ResetMousePos(){
	GraphicEngine* engine = GraphicEngine::getInstance();
	engine->SetCursorPosition(vector2di(engine->GetScreenWidth()/2, engine->GetScreenHeight()/2));
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