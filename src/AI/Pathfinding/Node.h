#ifndef NODE_h
#define NODE_h

#include <iostream>
#include <vector>
#include <vector3d.h>

class Connection;

class Node{
public:

    /**
     * @brief Node Constructor
     * 
     */
    Node();

    /**
     * @brief Creates a new node
     * 
     * @param int (ID)
     * @param vector3df (Position)
     */
    Node(int, vector3df);

    void setData(int, vector3df);

    void eraseConnection(Node* n);

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

    /**
     * @brief Returns the node position
     * 
     * @return vector3df (position)
     */
    vector3df getPosition();

private: 
    int m_ID;
    std::string m_RegionName;
    std::vector<Connection*> m_connections;
    vector3df m_position;
};

#endif