#include "DijkstraPathfinding.h"


std::vector<Connection*> DijkstraPahtfinding::makeDijkstraPahtfinding(Graph* graph, Node* StartNode, Node* GoalNode){
    //initialize the record for the start node
    m_startRecord = new NodeRecord();
    m_startRecord->m_node = StartNode;
    //m_startRecord->m_connection = none
    m_startRecord->m_costSoFar = 0;

    //initialize the open and closed list
    m_openList = graph->getNodeList();
    //m_openList += m_startRecord;
    m_closedList = graph->getNodeList();

    Node* currentNode = new Node();
    //iterate through processing each node
    while(m_openList.size()>0){
        //find the smallest element in the open list
        currentNode = getOpenListSmallestElement();

        //if it is the goal node, then terminate
        if(currentNode == GoalNode) break;

        //otherwise get its outgoing connections
        std::vector<Connection*> nodeConnections = graph->getNodeConnectionList(currentNode);

        //loop for each connection in turn
        Node* endNode = new Node();
        float endNodeCost = 0;
        for(int i = 0; i< nodeConnections.size();i++){
            //get the cost estimate for the end node
            endNode = nodeConnections.at(i)->getToNode();
            endNodeCost =  m_startRecord->m_costSoFar + nodeConnections.at(i)->getCost();
            //TODO:: PAGINA 210
        }
    }

    return m_path;
}

Node* DijkstraPahtfinding::getOpenListSmallestElement(){
    Node* smallestNode = new Node();

    // TODO::

    return smallestNode;
}