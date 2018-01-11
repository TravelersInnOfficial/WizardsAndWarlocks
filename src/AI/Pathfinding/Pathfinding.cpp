#include "Pathfinding.h"
#include "Heuristic.h"

Pathfinding::Pathfinding(){
    m_startRecord = new NodeRecord();
    m_openList = new List();
    m_closedList = new List();
}

Pathfinding::~Pathfinding(){
    delete m_startRecord;
    delete m_openList;
    delete m_closedList;
}

std::list<Connection*> *Pathfinding::AStar(Graph* g, Node* StartNode , Node* EndNode){
    Heuristic* heur = new Heuristic(EndNode);
    float endNodeHeuristic = 0;

    //Initialize the record for the start node
    m_startRecord->m_node = StartNode;
    m_startRecord->m_connection = NULL;
    m_startRecord->m_costSoFar = 0;
    //m_startRecord->previous = 0;
    m_startRecord->m_heuristic = heur->estimate(StartNode);
    m_startRecord->m_estimatedTotalCost = m_startRecord->m_costSoFar + m_startRecord->m_heuristic;
    //the estimated total cost is the sum of the cost-so-fat plus the heuristic function 
    //(how far is the node from the goal node)

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

        //Find the smallest element in the open list (USING THE m_estimatedTotalCost)
        std::cout<<"Finding the smallest element in the open list... "<<std::endl;
        current = m_openList->smallestElement();

        //if(current->previous!=0)std::cout<<"previous node:" << current->previous->m_node->getRegionName()<<std::endl;
        //else std::cout<<"THERE'S NO PREVIOUS NODE ASSIGNED"<<std::endl;
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

            //If the node is closed we may have to skip, or remove it from the closed list
            if(m_closedList->contains(endNode)){ 
                //Here we find the record in the closed list corresponding to the endNode.
                endNodeRecord = m_closedList->find(endNode);

                //If we didn't find a shorter route, skip
                if(endNodeRecord->m_costSoFar <= endNodeCost) continue;

                //Otherwhise remove it from the closed list
                std::cout<<"$$$$removing "<<endNodeRecord->m_node->getRegionName()<<" from closed list$$$$"<<std::endl;
                m_closedList->remove(endNodeRecord);
                m_closedList->printListOfNodes();
                std::cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<<std::endl;

                //We can use the node's old cost values to calculate its heuristic without calling the
                //possibly expensive heuristic function
                //TODO:: endNodeHeuristic = endNodeRecord.cost - endNodeRecord.costSoFar
                //endNodeHeuristic = endNodeCost - endNodeRecord->m_costSoFar;
                endNodeHeuristic = heur->estimate(endNodeRecord->m_node);
            }
            //Skip if the node is open and we've not found a better route
            else if(m_openList->contains(endNode)){
                std::cout<<"!! The node with the region: "<< endNode->getRegionName()<<" was already in the OPEN list\n"<<std::endl;
                //Here we find the record in the open list corresponding to the endNode.
                endNodeRecord = m_openList->find(endNode);

                //If our route is no better, then skip
                if(endNodeRecord!=NULL && endNodeRecord->m_costSoFar <= endNodeCost){ 
                    std::cout<<"Checking the current costSoFar with the new founded..."<<std::endl;
                    std::cout<<" CURRENT COSTSOFAR "<< endNodeCost<< " // COSTSOFAR FOUND "<< endNodeRecord->m_costSoFar<<std::endl;
                    continue;
                }

                //We can use the node's old cost values to calculate its heuristic without calling the
                //possibly expensive heuristic function
                endNodeHeuristic = heur->estimate(endNodeRecord->m_node);
            } 
            else{
                std::cout<<"We have an unvisited node called: "<<endNode->getRegionName()<<std::endl;
                //Otherwise we know we’ve got an unvisited node, so make a record for it
                endNodeRecord = new NodeRecord();
                endNodeRecord->m_node = endNode;

                //We'll need to calculate the heuristic value using the function, since we dont have an
                //existing record to use
                endNodeHeuristic = heur->estimate(endNode);
            }

            //We’re here if we need to update the node. Update the cost and connection
            std::cout<<"********************UPDATING THE NODE*****************"<<std::endl;
            endNodeRecord->m_costSoFar = endNodeCost;
            endNodeRecord->m_connection = nodeConnections.at(i);
            //endNodeRecord->previous = current;
            endNodeRecord->m_heuristic = endNodeHeuristic;
            endNodeRecord->m_estimatedTotalCost = endNodeCost + endNodeHeuristic;

            std::cout<<"----Node: "<<endNodeRecord->m_node->getRegionName()<<"----"<<std::endl;
            std::cout<<"----CostSoFar: "<<endNodeRecord->m_costSoFar<<"----"<<std::endl;
            std::cout<<"----Connection: "<<endNodeRecord->m_connection->getFromNode()->getRegionName()<<" >> "<<endNodeRecord->m_connection->getToNode()->getRegionName()<<std::endl;
            //std::cout<<"----Previous: "<<endNodeRecord->previous->m_node->getRegionName()<<std::endl;
            std::cout<<"----Heuristic: "<<endNodeRecord->m_heuristic<<"----"<<std::endl;
            std::cout<<"----Estimated Total Cost: "<<endNodeRecord->m_estimatedTotalCost<<"----"<<std::endl;
            std::cout<<"************************************************************"<<std::endl;

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
        return NULL;
    }
    else{
        std::cout<<"!! FOUND THE PATH TO THE END NODE with cost "<< current->m_costSoFar<<std::endl;
        path = new std::list<Connection*>();
        std::cout<<"Compiling path..."<<std::endl;
        while(/*current->previous !=0 && */current->m_node!= StartNode){
            std::cout<<"Node: "<<current->m_node->getRegionName()<<std::endl;
            //if(current->previous != 0)std::cout<<"Previous: "<<current->previous->m_node->getRegionName()<<std::endl;
            //else std::cout<<"THERE'S NO PREVIOUS NODE"<<std::endl;
            path->push_back(current->m_connection);
            //current = current->previous;
            current = m_closedList->find(current->m_connection->getFromNode());

        }
        path->reverse();
        std::cout<<"FINAL OPEN LIST\n"<<std::endl;
        m_openList->printListOfNodes();
        std::cout<<"FINAL CLOSED LIST\n"<<std::endl;
        m_closedList->printListOfNodes();
        return path;
    }

}

bool Pathfinding::pointInTriangle(vector3df point, std::vector<vector3df> triangle){
    /*
    A = triangle[0]
    B = triangle[1]
    C = triangle[2]
    p = point
    Any point p where [B-A] cross [p-A] does not point in the same direction
    as [B-A] cross [C-A] isn't inside the triangle. 
    If the cross products do point in the same direction, then we need to test p with the other lines
    as well. If the point was on the same side of AB as C and is also on the same side
    of BC as A and on the same side of CA as B, then it is in the triangle.
    */
    if(sameSide(point, triangle[0], triangle[1], triangle[2]) 
    && sameSide(point, triangle[1], triangle[0], triangle[2])
    && sameSide(point, triangle[2], triangle[0], triangle[1])) return true;
    return false;
}

float Pathfinding::dotProduct(vector3df A, vector3df B){
    //A*B = A.X*B.X + A.Y*B.Y + A.Z*B.Z
    return (A.X*B.X + A.Y*B.Y + A.Z*B.Z);   
}

vector3df Pathfinding::crossProduct(vector3df A, vector3df B){
    //A = [A.X A.Y A.Z]
    //B = [B.X B.Y B.Z]
    //AxB = [A.X*B.Y - A.Y*B.X, A.Y*B.Z - A.Z*B.Y, A.Z*B.X - A.X*B.Z]
    return vector3df(A.X*B.Y - A.Y*B.X, A.Y*B.Z - A.Z*B.Y, A.Z*B.X - A.X*B.Z);
}

bool Pathfinding::sameSide(vector3df p1, vector3df p2, vector3df A, vector3df B){
    vector3df cp1 = crossProduct(vector3df(B.X-A.X,B.Y-A.Y,B.Z-A.Z),vector3df(p1.X-A.X,p1.Y-A.Y,p1.Z-A.Z));
    vector3df cp2 = crossProduct(vector3df(B.X-A.X,B.Y-A.Y,B.Z-A.Z),vector3df(p2.X-A.X,p2.Y-A.Y,p2.Z-A.Z));
    if(dotProduct(cp1,cp2) >= 0) return true;
    else return false;
}

std::list<Connection*> *Pathfinding::DijkstraPF(Graph* g, Node* StartNode, Node* EndNode){
    //Initialize the record for the start node
    m_startRecord->m_node = StartNode;
    m_startRecord->m_connection = NULL;
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

        //if(current->previous!=0)std::cout<<"previous node:" << current->previous->m_node->getRegionName()<<std::endl;
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
                if(endNodeRecord!=NULL && endNodeRecord->m_costSoFar <= endNodeCost){ 
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
        return NULL;
    }
    else{
        std::cout<<"!! FOUND THE PATH TO THE END NODE with cost "<< current->m_costSoFar<<std::endl;
        path = new std::list<Connection*>();
/*
        while(current->previous !=0 && current->m_node!= StartNode){
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