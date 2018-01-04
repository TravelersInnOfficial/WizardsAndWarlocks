#include "Heuristic.h"

Heuristic::Heuristic(){}
Heuristic::Heuristic(Node* goalNode){
    m_goalNode = goalNode;
}
Heuristic::~Heuristic(){
    delete m_goalNode;
}

float Heuristic::estimate(Node* n){
  float estimated_cost = 0;


  return estimated_cost;  
}