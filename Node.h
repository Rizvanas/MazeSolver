#ifndef MAZESOLVER_NODE_H
#define MAZESOLVER_NODE_H
#include <memory>

class Node {
private:
    size_t x, y;
    int neighborCount;
    bool visited;
    bool rootNode, endNode;
    bool deadEnd;

    std::weak_ptr<Node> leftNeighbor;
    std::weak_ptr<Node> topNeighbor;
    std::weak_ptr<Node> rightNeighbor;
    std::weak_ptr<Node> bottomNeighbor;
    std::weak_ptr<Node> prevNode;

    int leftNeighborDist;
    int rightNeighborDist;
    int bottomNeighborDist;
    int topNeighborDist;
    int tentativeDist;
public:
    int getTentativeDist() const;
    void setTentativeDist(int tentativeDist);
    bool isDeadEnd() const;
    void setDeadEnd(bool deadEnd);

    Node() = default;
    bool operator==(const Node& rhs);

    int getLeftNeighborDist() const;
    int getRightNeighborDist() const;
    int getBottomNeighborDist() const;
    int getTopNeighborDist() const;

    Node(size_t x, size_t y);
    ~Node() = default;

    size_t getX() const;
    size_t getY() const;
    bool isVisited() const;
    bool isRootNode() const;
    bool isEndNode() const;

    void setX(size_t x);
    void setY(size_t y);
    void setVisited(bool visited);
    void setRootNode(bool rootNode);
    void setEndNode(bool endNode);

    void setPrevNode(std::weak_ptr<Node> prevNode);
    void setLeftNeighbor(std::shared_ptr<Node> node);
    void setRightNeighbor(std::shared_ptr<Node> node);
    void setTopNeighbor(std::shared_ptr<Node> node);
    void setBottomNeighbor(std::shared_ptr<Node> node);

    std::shared_ptr<Node> getLeftNeighbor();
    std::shared_ptr<Node> getRightNeighbor();
    std::shared_ptr<Node> getTopNeighbor();
    std::shared_ptr<Node> getBottomNeighbor();
    std::shared_ptr<Node> getPrevNode();

    int getNeighborCount();

    void setLeftNeighborDist(int leftNeighborDist);
    void setRightNeighborDist(int rightNeighborDist);
    void setBottomNeighborDist(int bottomNeighborDist);
    void setTopNeighborDist(int topNeighborDist);
};

#endif //MAZESOLVER_NODE_H