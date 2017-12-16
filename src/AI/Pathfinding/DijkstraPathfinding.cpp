#include "DijkstraPathfinding.h"
/*
def pathfindDijkstra(graph, start, end): 
*/
std::list<Connection*> *DijkstraPahtfinding::makeDijkstraPahtfinding(Graph* graph, Node* StartNode, Node* GoalNode){

/*
    # This structure is used to keep track of the information we need for each node 
        struct NodeRecord: 
            node 
            connection 
            costSoFar 
    
    # Initialize the record for the start node 
        startRecord = new NodeRecord() 
        startRecord.node = start 
        startRecord.connection = None 
        startRecord.costSoFar = 0 */
    m_startRecord = new NodeRecord();
    m_startRecord->m_node = StartNode;
    m_startRecord->m_connection = NULL;
    m_startRecord->m_costSoFar = 0;

    /*
    # Initialize the open and closed lists 
        open = PathfindingList() 
        open += startRecord 
        closed = PathfindingList() */
    m_openList = graph->getNodeList();
    m_openList->add(m_startRecord);
    m_closedList = graph->getNodeList();

    /*    
    # Iterate through processing each node 
        while length(open) > 0: */
    NodeRecord* current = new NodeRecord();
    while(m_openList->size()>0){

        /*
        # Find the smallest element in the open list 
            current = open.smallestElement() */
        current = m_openList->smallestElement();

        /*
        # If it is the goal node, then terminate 
            if current.node == goal: break */
        if(current->m_node == GoalNode) break;

        /* 
        # Otherwise get its outgoing connections 
            connections = graph.getConnections(current) */
        std::vector<Connection*> nodeConnections = graph->getNodeConnectionList(current->m_node);

        /*
        # Loop through each connection in turn 
            for connection in connections: */
        Node* endNode = new Node();
        float endNodeCost = 0;

        for(int i = 0; i< nodeConnections.size();i++){
        /*
            # Get the cost estimate for the end node 
                endNode = connection.getToNode()
                endNodeCost = current.costSoFar + connection.getCost() */
            endNode = nodeConnections.at(i)->getToNode();
            endNodeCost =  current->m_costSoFar + nodeConnections.at(i)->getCost();

        /*
            # Skip if the node is closed 
                if closed.contains(endNode): continue */
            if(m_closedList->contains(endNode)) continue;
                
            /*
            # .. or if it is open and we’ve found a worse route 
                else if open.contains(endNode): */
            else if(m_openList->contains(endNode)){

                /*
                # Here we find the record in the open list corresponding to the endNode. 
                    endNodeRecord = open.find(endNode) 
                    if endNodeRecord.cost <= endNodeCost: 
                        continue */
                NodeRecord* endNodeRecord = m_openList->find(endNode);
                if(endNodeRecord->m_costSoFar <= endNodeCost) continue;
            }   
            /*
            # Otherwise we know we’ve got an unvisited node, so make a record for it 
                else: endNodeRecord = new NodeRecord() 
                    endNodeRecord.node = endNode */
            else{
                NodeRecord* endNodeRecord = new NodeRecord();
                endNodeRecord->m_node = endNode;
            
                /*        
                # We’re here if we need to update the node. Update the cost and connection 
                    endNodeRecord.cost = endNodeCost 
                    endNodeRecord.connection = connection */
                endNodeRecord->m_costSoFar = endNodeCost;
                endNodeRecord->m_connection = nodeConnections.at(i);
                /*
                # And add it to the open list 
                    if not open.contains(endNode): 
                        open += endNodeRecord */
                if(!m_openList->contains(endNode)) m_openList->add(endNodeRecord);
            }
        }           
        /*
        # We’ve finished looking at the connections for the current node, so add it to the closed list 
        # and remove it from the open list 
            open -= current 
            closed += current */
        m_openList->remove(current);
        m_closedList->add(current);
    }
    /*
    # We’re here if we’ve either found the goal, or if we’ve no more nodes to search, find which.
        if current.node != goal:*/ 
    if(current->m_node != GoalNode){

        /*
        # We’ve run out of nodes without finding the goal, so there’s no solution
            return None 
        
        else: */
        return NULL;
    }
    else{
        /*
        # Compile the list of connections in the path 
            path = [] */
        std::list<Connection*> *path = new std::list<Connection*>();

        /*  
        # Work back along the path, accumulating connections 
            while current.node != start:
                path += current.connection 
                current = current.connection.getFromNode()*/
        while(current->m_node != StartNode){
            path->push_back(current->m_connection);
           // current = current->m_connection->getFromNode();
        }
        
        /*        
        # Reverse the path, and return it 
        return reverse(path) */
        path->reverse();
        return path;
    }
}