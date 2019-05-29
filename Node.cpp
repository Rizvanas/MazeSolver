#include <utility>


#include "Node.h"

Node::Node(size_t x, size_t y) : x{x}, y{y}, neighborCount{0},
								 visited{false}, rootNode{false}, endNode{false}, deadEnd{false},
								 leftNeighborDist{-1}, rightNeighborDist{-1},
								 bottomNeighborDist{-1}, topNeighborDist{-1},
								 tentativeDist{-1} {}

void Node::setX(size_t x) {
    Node::x = x;
}

void Node::setY(size_t y) {
    Node::y = y;
}

void Node::setLeftNeighborDist(int leftNeighborDist) {
    Node::leftNeighborDist = leftNeighborDist;
}

void Node::setRightNeighborDist(int rightNeighborDist) {
    Node::rightNeighborDist = rightNeighborDist;
}

void Node::setBottomNeighborDist(int bottomNeighborDist) {
    Node::bottomNeighborDist = bottomNeighborDist;
}

void Node::setTopNeighborDist(int topNeighborDist) {
    Node::topNeighborDist = topNeighborDist;
}

void Node::setVisited(bool visited) {
    Node::visited = visited;
}

void Node::setRootNode(bool rootNode) {
    Node::rootNode = rootNode;
}

void Node::setEndNode(bool endNode) {
    Node::endNode = endNode;
}

size_t Node::getX() const {
    return x;
}

size_t Node::getY() const {
    return y;
}

bool Node::isVisited() const {
    return visited;
}

bool Node::isRootNode() const {
    return rootNode;
}

bool Node::isEndNode() const {
    return endNode;
}

void Node::setLeftNeighbor(std::shared_ptr<Node> node) {
    leftNeighbor = node;
    neighborCount++;
}

std::shared_ptr<Node> Node::getLeftNeighbor() {
    return leftNeighbor.lock();
}

void Node::setRightNeighbor(std::shared_ptr<Node> node) {
    rightNeighbor = node;
    neighborCount++;
}

std::shared_ptr<Node> Node::getRightNeighbor() {
    return rightNeighbor.lock();
}

void Node::setTopNeighbor(std::shared_ptr<Node> node) {
    topNeighbor = node;
    neighborCount++;
}

std::shared_ptr<Node> Node::getTopNeighbor() {
    return topNeighbor.lock();
}

void Node::setBottomNeighbor(std::shared_ptr<Node> node) {
    bottomNeighbor = node;
    neighborCount++;
}

std::shared_ptr<Node> Node::getBottomNeighbor() {
    return bottomNeighbor.lock();
}

void Node::setPrevNode(std::weak_ptr<Node> prevNode) {
    this->prevNode = std::move(prevNode);
}

std::shared_ptr<Node> Node::getPrevNode() {
    return prevNode.lock();
}

bool Node::operator==(const Node &rhs) {
    return this == &rhs;
}

bool Node::isDeadEnd() const {
    return deadEnd;
}

void Node::setDeadEnd(bool deadEnd) {
    Node::deadEnd = deadEnd;
}

int Node::getNeighborCount() {
    return neighborCount;
}

int Node::getLeftNeighborDist() const {
    return leftNeighborDist;
}

int Node::getRightNeighborDist() const {
    return rightNeighborDist;
}

int Node::getBottomNeighborDist() const {
    return bottomNeighborDist;
}

int Node::getTopNeighborDist() const {
    return topNeighborDist;
}

int Node::getTentativeDist() const {
    return tentativeDist;
}

void Node::setTentativeDist(int tentativeDist) {
    Node::tentativeDist = tentativeDist;
}