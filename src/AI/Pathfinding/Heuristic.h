#ifndef HEURISTIC_H
#define HEURISTIC_H

#include "Node.h"

class Heuristic{
    public:
    //Generates an estimated cost to reach the goal from the given node
    Heuristic();
    Heuristic(Node*);
    ~Heuristic();

    /**
     * @brief Generates an estimated cost to reach the goal from the given node
     * 
     * @param Node (node given)
     * @return float (estimated cost)
     */
    float estimate(Node*);

    void setNode(Node*);

    private:
    Node* m_goalNode;
};

#endif