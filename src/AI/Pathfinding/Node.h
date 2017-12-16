#ifndef NODE_h
#define NODE_h

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
    Node(int id);

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

private: 
    int m_ID;

};

#endif