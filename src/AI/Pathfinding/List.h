#ifndef LIST_H
#define LIST_H

#include <vector>
class Node;
struct NodeRecord;

template<typename T>
class vector3d;
typedef vector3d<float> vector3df;

class List{
public: 
    List();
    ~List();

    /**
     * @brief Clear the list of nodes
     * 
     */
    void clear();

    /**
     * @brief Returns the nearest node
     * 
     * @param Node (Node to be checked)
     * @return NodeRecord* (Nearest node and connection)
     */
    NodeRecord* smallestElement();

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

    int getIndexNearestNode(vector3df pos, int start = 0);

    vector3df getPosNode(int index);

private:
    std::vector<NodeRecord*> m_list;

    //min heap functions
    NodeRecord* getMin();
    void        insertNode(NodeRecord*);
    void        swapNodes(int, int);
    void        heapify(int);
    int         parentNodeIndex(int);
    int         leftNodeIndex(int);
    int         rightNodeIndex(int);
    
};

#endif