#ifndef CONNECTION_H
#define CONNECTION_H

class Node;

class Connection{
public:

    /**
     * @brief Connection constructor
     * 
     */
    Connection();

    /**
     * @brief 
     * 
     * @param float cost (Connection cost)
     * @param Node FromNode (Connection start node)
     * @param Node ToNode (Connection end node)
     */
    Connection(float, Node*, Node*);

    /**
     * @brief Connection Destructor
     * 
     */
    ~Connection();

    /**
     * @brief Returns the non-negative cost of the Connection
     * 
     * @return float connectionCost
     */
    float getCost();

    /**
     * @brief returns the node that this connection came from
     * 
     * @return Node* fromNode
     */
    Node* getFromNode();

    /**
     * @brief Returns the node that this connection leads to 
     * 
     * @return Node* toNode
     */
    Node* getToNode();

    void Unlink();

private:
    float m_cost;
    Node* m_FromNode;
    Node* m_ToNode;
};

#endif