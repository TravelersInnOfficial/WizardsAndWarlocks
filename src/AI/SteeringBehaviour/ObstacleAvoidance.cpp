#include "ObstacleAvoidance.h"
#include "./../../PhysicsEngine/BulletEngine.h"

ObstacleAvoidance::ObstacleAvoidance(){
	avoidDistance = 2.0f;
	lookAHead = 2.5f;
	lookAHead2 = 1.0f;	
}

ObstacleAvoidance::~ObstacleAvoidance(){}

//Igual lo mejor sería hacer los tres raycast y hacer caso al que este más cerca
SteeringOutput ObstacleAvoidance::GetSteering(Kinematic cKin){
	Kinematic tKin;
	cKin.velocity.Y = 0;
	float angle = atan2(cKin.velocity.X, cKin.velocity.Z);
	// -------------------------------------------------------------------Primer RayCast |
	vector3df rayVector = cKin.velocity;
	rayVector.normalize();
	rayVector = rayVector * lookAHead;

	vector3df point_1(std::numeric_limits<float>::max(),0,0);
	vector3df normal_1(0,0,0);
	BulletEngine::GetInstance()->Raycast(cKin.position, cKin.position+rayVector, &point_1, &normal_1);
	//Caso de que se haya alterado al valor, es decir se ha encontrado un punto


	// --------------------------------------------------------------------Segundo RayCast ->
	rayVector = vector3df(sin(angle-M_PI/4), 0, cos(angle-M_PI/4));
	rayVector = rayVector * lookAHead2;

	vector3df point_2(std::numeric_limits<float>::max(), 0, 0);
	vector3df normal_2(0,0,0);

	BulletEngine::GetInstance()->Raycast(cKin.position, cKin.position+rayVector, &point_2, &normal_2);
	//Caso de que se haya alterado al valor, es decir se ha encontrado un punto
	
	// -------------------------------------------------------------------Tercer Raycast <- 
	rayVector = vector3df(sin(angle+M_PI/4), 0, cos(angle+M_PI/4));
	rayVector = rayVector * lookAHead2;

	vector3df point_3(std::numeric_limits<float>::max(), 0, 0);
	vector3df normal_3(0,0,0);

	BulletEngine::GetInstance()->Raycast(cKin.position, cKin.position+rayVector, &point_3, &normal_3);
	//Caso de que se haya alterado al valor, es decir se ha encontrado un punto

	// -------------------------------------------------------------------Resolucion
	if(point_1.X == std::numeric_limits<float>::max() && point_2.X == std::numeric_limits<float>::max() && point_3.X == std::numeric_limits<float>::max()){
		SteeringOutput output;
		return output;
	}
	vector3df position_final(0,0,0);
	int goodPoints = 0;
	if(point_1.X != std::numeric_limits<float>::max()){
		goodPoints++;
		normal_1 = vector3df(sin(normal_1.Y),0, cos(normal_1.Y));
		position_final = position_final + point_1 + normal_1 * avoidDistance;
	}
	if(point_2.X != std::numeric_limits<float>::max()){
		goodPoints++;
		normal_2 = vector3df(sin(normal_2.Y),0, cos(normal_2.Y));
		position_final = position_final + point_2 + normal_2 * avoidDistance;
	}
	if(point_3.X != std::numeric_limits<float>::max()){
		goodPoints++;
		normal_3 = vector3df(sin(normal_3.Y),0, cos(normal_3.Y));
		position_final = position_final + point_3 + normal_3 * avoidDistance;
	}
	position_final = position_final / goodPoints;
	tKin.position = position_final;
	return Seek::GetSteering(cKin, tKin);
}

/*
SteeringOutput ObstacleAvoidance::GetSteering(Kinematic cKin){
	cKin.velocity.Y = 0;
	// -------------------------------------------------------------------Primer RayCast |
	vector3df rayVector = cKin.velocity;
	rayVector.normalize();
	rayVector = rayVector * lookAHead;

	vector3df point(std::numeric_limits<float>::max(),0,0);
	vector3df normal(0,0,0);
	GraphicEngine::getInstance()->Raycast(cKin.position, cKin.position+rayVector, &point, &normal);
	//Caso de que se haya alterado al valor, es decir se ha encontrado un punto
	if(!(point.X == std::numeric_limits<float>::max())){		
		Kinematic tKin;

		normal = vector3df(sin(normal.Y),0, cos(normal.Y));
		tKin.position = point + normal * avoidDistance;

		return Seek::GetSteering(cKin, tKin);
	}
	float angle = atan2(cKin.velocity.X, cKin.velocity.Z);
	// --------------------------------------------------------------------Segundo RayCast ->
	rayVector = vector3df(sin(angle-M_PI/4), 0, cos(angle-M_PI/4));
	rayVector = rayVector * lookAHead2;

	GraphicEngine::getInstance()->Raycast(cKin.position, cKin.position+rayVector, &point, &normal);
	//Caso de que se haya alterado al valor, es decir se ha encontrado un punto
	if(!(point.X == std::numeric_limits<float>::max())){		
		Kinematic tKin;

		normal = vector3df(sin(normal.Y),0, cos(normal.Y));
		tKin.position = point + normal * avoidDistance;

		return Seek::GetSteering(cKin, tKin);
	}
	// -------------------------------------------------------------------Tercer Raycast <- 
	rayVector = vector3df(sin(angle+M_PI/4), 0, cos(angle+M_PI/4));
	rayVector = rayVector * lookAHead2;

	GraphicEngine::getInstance()->Raycast(cKin.position, cKin.position+rayVector, &point, &normal);
	//Caso de que se haya alterado al valor, es decir se ha encontrado un punto
	if(!(point.X == std::numeric_limits<float>::max())){		
		Kinematic tKin;

		normal = vector3df(sin(normal.Y),0, cos(normal.Y));
		tKin.position = point + normal * avoidDistance;

		return Seek::GetSteering(cKin, tKin);
	}

	SteeringOutput output;
	return output;
}
*/