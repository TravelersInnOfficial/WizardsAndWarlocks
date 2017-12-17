#ifndef LIST_H
#define LIST_H

#include <list>
#include <vector>
#include "NodeRecord.h"

class List{
public: 
    List();
    ~List();

    /**
     * @brief Returns the neares node
     * 
     * @param Node (Node to be checked)
     * @return Node* (Nearest node)
     */
    NodeRecord* smallestElement(Node*);

    /**
     * @brief Returns the list size
     * 
     * @return int (Size)
     */
    int size();

    /**
     * @brief Returns true if the node is in the list
     * 
     * @param Node* (The node we want to find)
     * @return true (The node is in the list)
     * @return false (The node is not in the list)
     */
    bool contains(Node*);

    /**
     * @brief Returns the NodeRecord from the node given
     * 
     * @param Node* (Node we want to find)
     * @return NodeRecord* (NodeRecord from the node given)
     */
    NodeRecord* find(Node*);

    /**
     * @brief Adds the nodeRecord to the list
     * 
     * @param NodeRecord (nodeRecord to be added)
     */
    void add(NodeRecord*);

    /**
     * @brief Removes a nodeRecord from the list
     * 
     * @param NodeRecord (nodeRecord to be removed)
     */
    void remove(NodeRecord*);

    /**
     * @brief Returns a vector of connections from the given node
     * 
     * @param Node (Node to analize)
     * @return std::list<Connection*> (Connections from the node) 
     */
    std::vector<Connection*> *getNodeConnections(Node*);

private:
    std::list<NodeRecord*> *m_list;
};

#endif