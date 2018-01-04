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
     * @return NodeRecord* (Nearest node and connection)
     */
    NodeRecord* smallestElement(NodeRecord*);

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
     * @brief Prints in the terminal the actual list of nodes
     * 
     */
    void printListOfNodes();

    int getNodeRecordPosition(Node*);

private:
    std::vector<NodeRecord*> m_list;

    //min heap functions
    void insertNode(NodeRecord*);
    NodeRecord* getMin();
    void swapNodes(NodeRecord*, NodeRecord*);
    int parentNodeIndex(int);
    int leftNodeIndex(int);
    int rightNodeIndex(int);
    void heapify(int);
    
};

#endif