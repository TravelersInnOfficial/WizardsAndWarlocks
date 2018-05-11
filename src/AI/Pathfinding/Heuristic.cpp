#include "Heuristic.h"

#include "Node.h"
#include <math.h>

Heuristic::Heuristic(){}
Heuristic::Heuristic(Node* goalNode){
    m_goalNode = goalNode;
}
Heuristic::~Heuristic(){
}

void Heuristic::setNode(Node* goalNode){
	m_goalNode = goalNode;
}

float Heuristic::estimate(Node* n){
  float estimated_cost = 0;
  float X = n->getPosition().X - m_goalNode->getPosition().X; 
  float Y = n->getPosition().Y - m_goalNode->getPosition().Y; 
  float Z = n->getPosition().Z - m_goalNode->getPosition().Z;

  estimated_cost = sqrt((X*X) + (Y*Y) + (Z*Z));
  return estimated_cost;  
}