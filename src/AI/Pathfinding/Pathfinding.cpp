#include "Pathfinding.h"
#include "Heuristic.h"
#include "./../../Managers/ObjectManager.h"

#include <PhysicsEngine/BulletEngine.h>

#include <algorithm>

Pathfinding::Pathfinding(){
    heur = new Heuristic();
    m_openList = new List();
    m_closedList = new List();
    StartNode = new Node();
    EndNode = new Node;
}

Pathfinding::~Pathfinding(){
    delete m_openList;
    delete m_closedList;
    delete heur;
    delete StartNode;
    delete EndNode;
    m_path.clear();        // Se limpia el vector de Nodos
}

 vector3df Pathfinding::GetPosNode(int index){
    // Preparamos el vector3df a devolver
    vector3df output;

    // Miramos si el camino es superior a 1
    if(m_path.size() > 0){
        //Miramos si el index es mayor o igual al tamanyo del m_path
        if(index >= m_path.size()){
            // En este caso lo igualamos al ultimo puesto
            index = m_path.size()-1;
        }

        // Igualamos la posicion del array
        output = m_path[index]->getPosition();
    }
    return output;
 }

int Pathfinding::GetIndexNearestNode(vector3df pos, int start){
    // Creamos el valor el cual vamos a devolver
    int output = start;

    // Miramos que no se pase del tamanyo del camino
    if(start < m_path.size()){
        BulletEngine* f_engine = BulletEngine::GetInstance();
        // Recorremos todos nodos del vector en busca del nodo mas cercano
        int size = m_path.size();
        for(int i=start; i<size && i<=start+2; i++){
            vector3df nodePos = m_path[i]->getPosition();
            nodePos.Y += 0.5;
            void* object = f_engine->Raycast(pos, nodePos, C_WALL | C_FOUNTAIN | C_DOOR);
            
            // Hacemos un procesado inicial del puntero
            if(object!=nullptr){
                Entidad* entity = (Entidad*)object;
                if(entity->GetClase()==EENUM_DOOR){
                    Door* door = (Door*)entity;
                    if(!door->GetOpenState()){
                        // En el caso de que la puerte este cerrada puede ver a traves
                        object = nullptr;
                    }
                }
            }
            
            // En el caso de que no haya nada en medio avanzamos el output a i
            if(object == nullptr){
                output = i;
            }
            // En el caso de que no veamos el actual es que ha habido un error y debemos retroceder
            if(object!=nullptr && i == start && i!=0){
                output = start-1;
                break;
            }
        }
    // En el caso de que se pase, miramos que el camino sea superior a 1
    }else if(m_path.size() > 0){
        // Lo ponemos al ultimo
        output = m_path.size()-1;
        // En el caso de que no se pase lo ponemos a 0
    }else{
        output = 0;
    }

    // Devolvemos el valor
    std::cout<<"Size: "<<m_path.size()<<std::endl;
    std::cout<<output<<std::endl;
    return output;
}

void Pathfinding::ResetValues(){
    m_closedList->clear();              // Limpiamos la ClosedList
    m_openList->clear();                // Limpiamos la OpenList

    int size = m_connections.size();
    for(int i=0; i<size; i++){
        m_connections[i]->Unlink();
        delete m_connections[i];        // Eliminamos las conexiones creadas en esta clase que no se van a usar mas
    }
    m_connections.clear();              // Limpiamos las conexiones, puesto que han sido eliminadas
}

bool Pathfinding::AStar( vector3df from,vector3df to, vector3df firstC, vector3df secondC){
    if(m_path.size() > 0){ 
        float dirFinal = (to - m_path[m_path.size() - 1]->getPosition()).length();
        if(dirFinal <= 1){
          return false;         // Limpiamos el vector de nodos, no se han creado variables en este vector
        }
    }

    // Reseteamos el camino
    m_path.clear();

    // Put values
    StartNode->setData(-1,from);
    EndNode->setData(-2,to);
    Connection* randomConn = new Connection((from-to).length(),StartNode,EndNode);
    m_connections.push_back(randomConn);

    heur->setNode(EndNode);

    std::cout<<"Start"<<std::endl;

    float endNodeHeuristic = 0;
    //Add the nearest nodes to the open list  
    NavMesh* navmesh = ObjectManager::GetInstance()->GetNavMesh();
    if(navmesh!=nullptr){
        std::vector<Node*> nearests = navmesh->SearchNearestNodes(from, firstC, secondC);
        std::vector<Node*> finals = navmesh->SearchNearestNodes(to);

        if(nearests.size() != 0){
            int size = nearests.size();
            for(int i = 0; i<size; i++){
                NodeRecord* nr = new NodeRecord();
                vector3df c_to = nearests[i]->getPosition();
                float X = c_to.X - from.X;
                float Y = c_to.Y - from.Y;
                float Z = c_to.Z - from.Z;
                float cost = sqrt((X*X) + (Y*Y) + (Z*Z));
                nr->m_node = nearests[i];

                Connection* newConnection = new Connection(cost,StartNode,nearests[i]);
                nr->m_connection = newConnection;
                m_connections.push_back(newConnection);

                nr->m_costSoFar = 0;
                nr->m_heuristic = heur->estimate(nearests[i]);
                nr->m_estimatedTotalCost = nr->m_costSoFar + nr->m_heuristic;

                m_openList->add(nr);
            }
        }
        if(finals.size() == 0){
            ResetValues();  // Una vez ya se ha creado el camino ya podemos limpiar los datos extras que se han creado
            return false;
        }

        //TODO::check if the current node is inside the end triangle

        //Initialize the record for the start node
        NodeRecord* m_startRecord = new NodeRecord();
        m_startRecord->m_node = StartNode;
        m_startRecord->m_connection = nullptr;
        m_startRecord->m_costSoFar = 0;
        m_startRecord->m_heuristic = heur->estimate(StartNode);
        m_startRecord->m_estimatedTotalCost = m_startRecord->m_costSoFar + m_startRecord->m_heuristic;
        //the estimated total cost is the sum of the cost-so-fat plus the heuristic function 
        //(how far is the node from the goal node)

        //Initialize the open and closed lists
        //m_openList->add(m_startRecord);
        m_closedList->add(m_startRecord);

        //Iterate through processing each node
        NodeRecord* current = m_startRecord;
        Node* endNode = nullptr;
        float endNodeCost = 0;
        NodeRecord* endNodeRecord = new NodeRecord();
        std::vector<Connection*> nodeConnections;

        while(m_openList->size()>0){
            
            //Find the smallest element in the open list (USING THE m_estimatedTotalCost)
            current = m_openList->smallestElement();

             //If it is the goal node, then terminate
            int size = finals.size();
            for(unsigned i = 0;i<size;i++){
                if(current->m_node == finals[i]){
                        NodeRecord *nr = new NodeRecord();
                        nr->m_node = EndNode;

                        Connection* newConnection = new Connection(0,current->m_node,EndNode);
                        nr->m_connection = newConnection;
                        m_connections.push_back(newConnection);

                        nr->m_heuristic = 0;
                        nr->m_estimatedTotalCost = 0;
                        m_openList->add(nr);

                        break;
                }
            }

            if(current->m_node == EndNode){
                m_closedList->add(current);
                break;
            }
            //Otherwise get its outgoing connections
            nodeConnections = current->m_node->getOutgoingConnections();
            //Loop through each connection in turn
            if(nodeConnections.size()==0){ break;}

            for(int i = 0; i < nodeConnections.size();i++){
                //Get the cost estimate for the end node 
                endNode = nodeConnections[i]->getToNode();
                endNodeCost =  current->m_costSoFar + nodeConnections.at(i)->getCost();

                //If the node is closed we may have to skip, or remove it from the closed list
                if(m_closedList->contains(endNode)){ 
                    //Here we find the record in the closed list corresponding to the endNode.
                    endNodeRecord = m_closedList->find(endNode);

                    //If we didn't find a shorter route, skip
                    if(endNodeRecord->m_costSoFar <= endNodeCost){ 
                        continue;
                    }

                    //Otherwhise remove it from the closed list
                    //m_closedList->remove(endNodeRecord);

                    //We can use the node's old cost values to calculate its heuristic without calling the
                    //possibly expensive heuristic function
                    //endNodeHeuristic = endNodeRecord.cost - endNodeRecord.costSoFar
                    endNodeHeuristic = heur->estimate(endNodeRecord->m_node);
                }
                //Skip if the node is open and we've not found a better route
                else if(m_openList->contains(endNode)){
        
                    //Here we find the record in the open list corresponding to the endNode.
                    endNodeRecord = m_openList->find(endNode);

                    //If our route is no better, then skip
                    if(endNodeRecord != nullptr && endNodeRecord->m_costSoFar <= endNodeCost){ 
                        continue;
                    }

                    //We can use the node's old cost values to calculate its heuristic without calling the
                    //possibly expensive heuristic function
                    endNodeHeuristic = heur->estimate(endNodeRecord->m_node);
                } 
                else{
                    //Otherwise we know we’ve got an unvisited node, so make a record for it
                    endNodeRecord = new NodeRecord();
                    endNodeRecord->m_node = endNode;

                    //We'll need to calculate the heuristic value using the function, since we dont have an
                    //existing record to use
                    endNodeHeuristic = heur->estimate(endNode);
                }

                //We’re here if we need to update the node. Update the cost and connection
                endNodeRecord->m_costSoFar = endNodeCost;
                endNodeRecord->m_connection = nodeConnections.at(i);
                //endNodeRecord->previous = current;
                endNodeRecord->m_heuristic = endNodeHeuristic;
                endNodeRecord->m_estimatedTotalCost = endNodeCost + endNodeHeuristic;



                //And add it to the open list
                if(!m_openList->contains(endNode)){
                    m_openList->add(endNodeRecord);
                }
            }
            //We’ve finished looking at the connections for the current node, so add it to the closed list 
            //and remove it from the open list
            //ALREADY REMOVED WHEN GET SMALLESTELEMENT
            //m_openList->remove(current);
            m_closedList->add(current);
        }

        //We’re here if we’ve either found the goal, or if we’ve no more nodes to search, find which.
        std::cout<<"Por aqui"<<std::endl;
        if(current!=nullptr && current->m_connection!=nullptr){
            Node* currentNode = current->m_node;
            if(currentNode != EndNode) {
                std::cout<<EndNode->getPosition()<<std::endl;
                m_path.push_back(EndNode);
            }
            do{
                std::cout<<currentNode->getPosition()<<std::endl;
                m_path.push_back(currentNode);
                current = m_closedList->find(current->m_connection->getFromNode());
                currentNode = current->m_node;
            }while(current!=nullptr && current->m_node != StartNode);

            m_path.push_back(current->m_node);  // Anyadimos lo que seria el StartNode
            std::reverse(m_path.begin(), m_path.end());
            std::cout<<"finish"<<std::endl;
        }
    }

    ResetValues();  // Una vez ya se ha creado el camino ya podemos limpiar los datos extras que se han creado
    return true;
}


std::list<Connection*> *Pathfinding::DijkstraPF( Node* StartNode, Node* EndNode){
    //Initialize the record for the start node
    NodeRecord* m_startRecord = new NodeRecord();
    m_startRecord->m_node = StartNode;
    m_startRecord->m_connection = nullptr;
    m_startRecord->m_costSoFar = 0;
    //m_startRecord->previous = 0;

    //Initialize the open and closed lists
    m_openList->add(m_startRecord);
    
    //Iterate through processing each node
    NodeRecord* current = m_startRecord;

    Node* endNode = new Node();
    float endNodeCost = 0;
    NodeRecord* endNodeRecord = new NodeRecord();

    std::vector<Connection*> nodeConnections;

    std::list<Connection*> *path;

    int i = 0;

    while(m_openList->size()>0){
        std::cout<<"--------------------------->> ITERATION "<<i<<" <<---------------------------"<<std::endl;
        i++;

        std::cout<<"OPEN LIST SIZE::: "<<m_openList->size()<<std::endl;

        //Find the smallest element in the open list
        std::cout<<"Finding the smallest element in the open list... "<<std::endl;
        current = m_openList->smallestElement();

        //if(current->previous != 0)std::cout<<"previous node:" << current->previous->m_node->getRegionName()<<std::endl;
        std::cout<<"--------------------------- CURRENT NODE "<<current->m_node->getRegionName()<<" ---------------------------"<<std::endl;
        
         //If it is the goal node, then terminate
        if(current->m_node == EndNode){ 
            std::cout<<"!! FOUND THE GOAL NODE "<<std::endl;
            break;
        }

        //Otherwise get its outgoing connections
        nodeConnections = current->m_node->getOutgoingConnections();

        //Loop through each connection in turn
        std::cout<<"NODE CONNECTIONS SIZE: "<<nodeConnections.size()<<std::endl;
        if(nodeConnections.size()==0) break;
        
        for(int i = 0; i < nodeConnections.size();i++){
            std::cout<<"--checking connection "<<i<<"--"<<std::endl;
            //Get the cost estimate for the end node 
            endNode = nodeConnections.at(i)->getToNode();
            
            std::cout<<"This connection leads to: "<<endNode->getRegionName()<<std::endl;
            endNodeCost =  current->m_costSoFar + nodeConnections.at(i)->getCost();

            //Skip if the node is closed
            if(m_closedList->contains(endNode)){ 
                std::cout<<"!! The node with the region: "<< endNode->getRegionName()<<" was already in the CLOSED list\n"<<std::endl;
                continue;
            }
             //.. or if it is open and we’ve found a worse route
            else if(m_openList->contains(endNode)){
                std::cout<<"!! The node with the region: "<< endNode->getRegionName()<<" was already in the OPEN list\n"<<std::endl;
                //Here we find the record in the open list corresponding to the endNode.
                endNodeRecord = m_openList->find(endNode);
                if(endNodeRecord!=nullptr && endNodeRecord->m_costSoFar <= endNodeCost){ 
                    std::cout<<"Checking the current costSoFar with the new founded..."<<std::endl;
                    std::cout<<" CURRENT COSTSOFAR "<< endNodeCost<< " // COSTSOFAR FOUND "<< endNodeRecord->m_costSoFar<<std::endl;
                    continue;
                }
            } 
            else{
                std::cout<<"We have an unvisited node called: "<<endNode->getRegionName()<<std::endl;
                //Otherwise we know we’ve got an unvisited node, so make a record for it
                endNodeRecord = new NodeRecord();
                endNodeRecord->m_node = endNode;
            }

            //We’re here if we need to update the node. Update the cost and connection
            std::cout<<"----EndNodecost: "<<endNodeCost<<"----"<<std::endl;
            endNodeRecord->m_costSoFar = endNodeCost;
            endNodeRecord->m_connection = nodeConnections.at(i);
            //endNodeRecord->previous = current;

            //And add it to the open list
            if(!m_openList->contains(endNode)){
                std::cout<<"The open list doesnt contain the new node, so add it:: "<<endNode->getRegionName()<<" Cost:" <<endNodeCost<<" Connection From "<<endNodeRecord->m_connection->getFromNode()->getRegionName()<<" To "<<endNodeRecord->m_connection->getToNode()->getRegionName()<<std::endl;
                m_openList->add(endNodeRecord);
            }
        }
        //We’ve finished looking at the connections for the current node, so add it to the closed list 
        //and remove it from the open list
        std::cout<<"----remove from open list----"<<std::endl;
        //ALREADY REMOVED WHEN GET SMALLESTELEMENT
        //m_openList->remove(current);

        std::cout<<"----add in closed list----"<<std::endl;
        m_closedList->add(current);
    }
    //We’re here if we’ve either found the goal, or if we’ve no more nodes to search, find which.
    if(current->m_node != EndNode){
        //We’ve run out of nodes without finding the goal, so there’s no solution
        std::cout<<"!! COULDNT FIND THE PATH TO THE END NODE"<<std::endl;
        return nullptr;
    }
    else{
        std::cout<<"!! FOUND THE PATH TO THE END NODE with cost "<< current->m_costSoFar<<std::endl;
        path = new std::list<Connection*>();
    /*
        while(current->previous != 0 && current->m_node!= StartNode){
            path->push_back(current->m_connection);
            current = current->previous;
        }
    */
        path->reverse();
        std::cout<<"FINAL OPEN LIST\n"<<std::endl;
        m_openList->printListOfNodes();
        std::cout<<"FINAL CLOSED LIST\n"<<std::endl;
        m_closedList->printListOfNodes();
        return path;
    }
}
