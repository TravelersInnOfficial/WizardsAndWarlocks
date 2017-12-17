#ifndef NODE_h
#define NODE_h

#include <iostream>
#include <vector>

class Connection;

class Node{
public:

    /**
     * @brief Node Constructor
     * 
     */
    Node();

    /**
     * @brief Creates a new Node
     * 
     * @param id (Node ID)
     */
    Node(int, std::string);

    /**
     * @brief Node destructor
     * 
     */
    ~Node();
    
    /**
     * @brief Returns the node ID
     * 
     * @return int nodeID
     */
    int getNodeID();

    /**
     * @brief Returns the region name
     * 
     * @return std::string 
     */
    std::string getRegionName();

    /**
     * @brief Adds a connection to the node
     * 
     * @param Connection (Connection for the node)
     */
    void addConnection(Connection*);

    /**
     * @brief Returns the node list of outgoing connections
     * 
     * @return std::vector<Connection*> 
     */
    std::vector<Connection*> getOutgoingConnections();

private: 
    int m_ID;
    std::string m_RegionName;
    std::vector<Connection*> m_connections;
};

#endif