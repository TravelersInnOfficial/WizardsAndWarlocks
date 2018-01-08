#include "Heuristic.h"
#include <math.h>
Heuristic::Heuristic(){}
Heuristic::Heuristic(Node* goalNode){
    m_goalNode = goalNode;
}
Heuristic::~Heuristic(){
    delete m_goalNode;
}

float Heuristic::estimate(Node* n){
  float estimated_cost = 0;
  float X = n->getPosition().X - m_goalNode->getPosition().X; 
  float Z = n->getPosition().Z - m_goalNode->getPosition().Z;

  estimated_cost = sqrt((X*X) + (Z*Z));
  return estimated_cost;  
}