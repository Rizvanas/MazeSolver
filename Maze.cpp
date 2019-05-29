#include "Maze.h"
#include <algorithm>
#include "DistComparator.h"

void Maze::setPNG_Surface(const std::string &imgPath, MyWindow &window) {
	SDL_Surface *tempSurface = IMG_Load(imgPath.c_str());
	if (!tempSurface) {
		throw std::invalid_argument("file doesn't exist.\n");
	}
	pngMazeSurface = SDL_ConvertSurfaceFormat(tempSurface, SDL_GetWindowPixelFormat(window.getSDLWindow()), 0);
	SDL_FreeSurface(tempSurface);
}

void Maze::freePNG_Surface() {
	if (pngMazeSurface) {
		SDL_FreeSurface(pngMazeSurface);
		pngMazeSurface = nullptr;
	}
}

bool Maze::pixelIsBlack(Uint32 pixel) {
	return pixel == pngData.blackPixel;
}

bool Maze::currentPositionIsOuterBorder(size_t startY, size_t currentX) {
	return startY == outerBorderYPos|| startY == pngData.endY - 1 || currentX == outerBorderXPos || currentX == pngData.endX - 1;
}

void Maze::addNeighbourToSearchQueue(std::queue<std::shared_ptr<Node>> &searchQueue, std::shared_ptr<Node> neighbour, int &visitedCount) {
	if (neighbour && !neighbour->isVisited()) {
		neighbour->setVisited(true);
		neighbour->setPrevNode(searchQueue.front());
		drawBetweenTwoNodes(searchQueue.front(), neighbour, pngData.greyPixel);
		searchQueue.push(neighbour);
	}
}

void Maze::addNeighboursToSearchQueue(std::queue<std::shared_ptr<Node>>& searchQueue, int &visitedCount) {
	addNeighbourToSearchQueue(searchQueue, searchQueue.front()->getTopNeighbor(), visitedCount);
	addNeighbourToSearchQueue(searchQueue, searchQueue.front()->getBottomNeighbor(), visitedCount);
	addNeighbourToSearchQueue(searchQueue, searchQueue.front()->getLeftNeighbor(), visitedCount);
	addNeighbourToSearchQueue(searchQueue, searchQueue.front()->getRightNeighbor(), visitedCount);
}

bool Maze::checkIfEndNodeIsVisited(std::queue<std::shared_ptr<Node>> &searchQueue) {
	return searchQueue.front()->isEndNode();
}

bool Maze::addNeighbourToSearchStack(std::stack<std::shared_ptr<Node>>& searchStack, std::shared_ptr<Node> neighbour) {
	if (neighbour && !neighbour->isVisited()) {
		searchStack.top()->setVisited(true);
		searchStack.push(neighbour);
		return true;
	}
	return false;
}

bool Maze::addNeighboursToSearchStack(std::stack<std::shared_ptr<Node>>& searchStack) {
	bool neighbourAdded = addNeighbourToSearchStack(searchStack, searchStack.top()->getBottomNeighbor())
		|| addNeighbourToSearchStack(searchStack, searchStack.top()->getLeftNeighbor())
		|| addNeighbourToSearchStack(searchStack, searchStack.top()->getRightNeighbor())
		|| addNeighbourToSearchStack(searchStack, searchStack.top()->getTopNeighbor());

	return neighbourAdded;
}

void Maze::popDeadEndNodeFromSearchStack(std::stack<std::shared_ptr<Node>> &searchStack) {
	searchStack.top()->setVisited(true);
	searchStack.top()->setDeadEnd(true);
	searchStack.pop();
}

std::vector<std::shared_ptr<Node>> Maze::getSolutionPathFromDFS() {
	std::vector<std::shared_ptr<Node>> solutionPath;
	for (auto &i : finalSolution) {
		if (!i->isDeadEnd()) {
			solutionPath.push_back(i);
		}
	}
	return solutionPath;
}

size_t Maze::getDistBetweenTwoNodes(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
	if (!node1 || !node2) {
		return -1;
	}
	if (node1->getX() == node2->getX()) {
		if (node1->getY() >= node2->getY()) {
			return node1->getY() - node2->getY();
		} else {
			return node2->getY() - node1->getY();
		}
	} else {
		if (node1->getX() >= node2->getX()) {
			return node1->getX() - node2->getX();
		} else {
			return node2->getX() - node1->getX();
		}
	}
}

void Maze::addNeighbourToSearchOrder(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>,
	DistComparator>& searchOrder, std::shared_ptr<Node> neighbour) {
	size_t totalDist = searchOrder.top()->getTentativeDist() + getDistBetweenTwoNodes(searchOrder.top(), neighbour);

	if (neighbour && !neighbour->isVisited() && (neighbour->getTentativeDist() == -1 || neighbour->getTentativeDist() > totalDist)) {
		size_t newNeighborDist{ getDistBetweenTwoNodes(searchOrder.top(), neighbour) };
		neighbour->setTentativeDist(searchOrder.top()->getTentativeDist() + newNeighborDist);
		neighbour->setPrevNode(searchOrder.top());

		drawBetweenTwoNodes(searchOrder.top(), neighbour, pngData.greyPixel);
		searchOrder.push(neighbour);
		finalSolution.push_back(searchOrder.top());
	}
}


void Maze::addNeighboursToSearchOrder(std::priority_queue<std::shared_ptr<Node>,
	std::vector<std::shared_ptr<Node>>, DistComparator> &searchOrder) {
	addNeighbourToSearchOrder(searchOrder, searchOrder.top()->getTopNeighbor());
	addNeighbourToSearchOrder(searchOrder, searchOrder.top()->getBottomNeighbor());
	addNeighbourToSearchOrder(searchOrder, searchOrder.top()->getLeftNeighbor());
	addNeighbourToSearchOrder(searchOrder, searchOrder.top()->getRightNeighbor());
}

void Maze::setDijkstraSolution() {
	std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, DistComparator> tempSearchOrder;
	tempSearchOrder.push(graph.at(getRootNodePosition()));
	tempSearchOrder.top()->setTentativeDist(0);

	finalSolution.clear();
	bool endVisited{ false };

	while (!tempSearchOrder.empty()) {
		addNeighboursToSearchOrder(tempSearchOrder);
		tempSearchOrder.top()->setVisited(true);
		if (tempSearchOrder.top()->isEndNode()) {
			endVisited = true;
		}
		tempSearchOrder.pop();
	}
}

std::vector<std::shared_ptr<Node>> Maze::getSolutionPathFromDIJKSTRA() {
	std::vector<std::shared_ptr<Node>> dijkstrasFinalSolutionPath;
	dijkstrasFinalSolutionPath.push_back(graph.at(getEndNodePosition()));
	bool rootIsVisited{ false };

	while (!rootIsVisited) {
		dijkstrasFinalSolutionPath.push_back(dijkstrasFinalSolutionPath.at(dijkstrasFinalSolutionPath.size() - 1)->getPrevNode());
		if (dijkstrasFinalSolutionPath.at(dijkstrasFinalSolutionPath.size() - 1)->isRootNode()) {
			rootIsVisited = true;
		}
	}
	std::reverse(dijkstrasFinalSolutionPath.begin(), dijkstrasFinalSolutionPath.end());

	return dijkstrasFinalSolutionPath;
}

int Maze::getColorChangeInterval(size_t solutionPathSize, float &gradientStepSize, float &gradientStepsCount) {
	if (solutionPathSize < gradientStepsCount) {
		gradientStepSize = static_cast<float>(255) / solutionPathSize;
		gradientStepsCount = solutionPathSize;
	}
	return static_cast<size_t>(solutionPathSize / gradientStepsCount);
}

bool Maze::changeColorInThisStep(size_t step, int colorChangeInterval) {
	return step % colorChangeInterval == 0 && step != 0;
}

void Maze::addNewNode(size_t x, size_t y) {
	std::shared_ptr<Node> sp(new Node(x, y));
	graph.push_back(std::move(sp));
}

bool Maze::setRootAndEndNode(bool rootNodeAdded) {
	if (!rootNodeAdded) {
		graph.at(graph.size() - 1)->setRootNode(true);
		rootNodeAdded = true;
	} else {
		graph.at(graph.size() - 1)->setEndNode(true);
	}
	return rootNodeAdded;
}

void Maze::setLeftAndRightNeighbours(bool neighbourIsBlocked) {
	if (!neighbourIsBlocked && graph.size() > 1) {
		graph.at(graph.size() - 1)->setLeftNeighbor(graph.at(graph.size() - 2));
		graph.at(graph.size() - 2)->setRightNeighbor(graph.at(graph.size() - 1));
		pngData.tempDist = static_cast<size_t>(graph.at(graph.size() - 1)->getX() - graph.at(graph.size() - 2)->getX());
		graph.at(graph.size() - 1)->setLeftNeighborDist(pngData.tempDist);
		graph.at(graph.size() - 2)->setRightNeighborDist(pngData.tempDist);
	}
}

void Maze::setTopAndBottomNeighbours(size_t index, bool neighbourIsBlocked) {
	if (!neighbourIsBlocked) {
		graph.at(index)->setBottomNeighbor(graph.at(index + 1));
		graph.at(index + 1)->setTopNeighbor(graph.at(index));
		pngData.tempDist = static_cast<size_t>(graph.at(index + 1)->getY() - graph.at(index)->getY());
		graph.at(index)->setBottomNeighborDist(pngData.tempDist);
		graph.at(index + 1)->setTopNeighborDist(pngData.tempDist);
	}
}

int Maze::getAdjacentWallCount(size_t index, bool &verticalWallShowedUp, bool &horizontalWallShowedUp) const {
	int wallCount{ 0 };
	if (pngData.pixels[index + pngData.startY * pngData.cols - 1] == pngData.blackPixel) { verticalWallShowedUp = true, ++wallCount; }
	if (pngData.pixels[index + pngData.startY * pngData.cols + 1] == pngData.blackPixel) { verticalWallShowedUp = true, ++wallCount; }
	if (pngData.pixels[index + (pngData.startY - 1) * pngData.cols] == pngData.blackPixel) { horizontalWallShowedUp = true, ++wallCount; }
	if (pngData.pixels[index + (pngData.startY + 1) * pngData.cols] == pngData.blackPixel) { horizontalWallShowedUp = true, ++wallCount; }
	return wallCount;
}

bool Maze::intersectionOrDeadEnd(int wallCount, bool verticalWallShowedUp, bool horizontalWallShowedUp) {
	return wallCount <= 1 || (wallCount == 2 && horizontalWallShowedUp && verticalWallShowedUp) || wallCount == 3;
}

bool Maze::addRootAndEndNode(size_t x, size_t y, bool neighbourIsBlocked, bool rootNodeAdded) {
	addNewNode(x, y);
	setLeftAndRightNeighbours(neighbourIsBlocked);
	rootNodeAdded = setRootAndEndNode(rootNodeAdded);
	return rootNodeAdded;
}

bool Maze::addIntersectionOrDeadEndNode(size_t x, size_t y, bool neighbourIsBlocked) {
	bool verticalWallShowedUp{ false };
	bool horizontalWallShowedUp{ false };
	int wallCount = getAdjacentWallCount(x, verticalWallShowedUp, horizontalWallShowedUp);

	if (intersectionOrDeadEnd(wallCount, verticalWallShowedUp, horizontalWallShowedUp)) {
		addNewNode(x, y);
		setLeftAndRightNeighbours(neighbourIsBlocked);
		neighbourIsBlocked = false;
	}
	return neighbourIsBlocked;
}

bool Maze::neighbourIsblockedVertically(size_t index) {
	for (auto start = graph.at(index)->getY(); start < graph.at(index + 1)->getY(); ++start) {
		if (pixelIsBlack(pngData.pixels[graph.at(index)->getX() + start * pngData.cols])) {
			return true;
		}
	}
	return false;
}

void Maze::sortGraphXPrimaryYSecondary() {
	std::sort(graph.begin(), graph.end(), [](std::shared_ptr<Node> n1, std::shared_ptr<Node> n2) {
		if (n1->getX() < n2->getX()) { return true; }
		return n1->getX() == n2->getX() && n1->getY() < n2->getY();
	});
}

void Maze::addNodeAndSetHorizontalRelation(size_t index, bool &rootNodeAdded, bool &neighbourIsBlocked, int &whitePixelCount) {
	if (currentPositionIsOuterBorder(pngData.startY, index)) {
		rootNodeAdded = addRootAndEndNode(index, pngData.startY, neighbourIsBlocked, rootNodeAdded);
		neighbourIsBlocked = false;
		++whitePixelCount;

		if (whitePixelCount > 2) {
			narrowSearchZone(whitePixelCount, rootNodeAdded);
		}
	} else {
		neighbourIsBlocked = addIntersectionOrDeadEndNode(index, pngData.startY, neighbourIsBlocked);
	}
}

void Maze::addAllNodesAndSetHorizontalRelations() {
	int whitePixelCount{ 0 };
	bool rootNodeAdded{ false };
	bool neighbourIsBlocked{ false };
	size_t start = pngData.startY;
	while (pngData.startY != pngData.endY) {
		for (size_t i = pngData.startX; i < pngData.endX; ++i) {
			if (!pixelIsBlack(pngData.pixels[i + pngData.startY * pngData.cols])) {
				addNodeAndSetHorizontalRelation(i, rootNodeAdded, neighbourIsBlocked, whitePixelCount);
			} else {
				neighbourIsBlocked = true;
			}
		}
		++pngData.startY;
	}
}

void Maze::setVerticalRelations() {
	for (size_t i = 0; i < graph.size() - 1; ++i) {
		if (graph.at(i)->getX() == graph.at(i + 1)->getX()) {
			setTopAndBottomNeighbours(i, neighbourIsblockedVertically(i));
		}
	}
}

void Maze::addAllNodesAndSetRelations() {
	addAllNodesAndSetHorizontalRelations();
	sortGraphXPrimaryYSecondary();
	setVerticalRelations();
}

std::vector<std::shared_ptr<Node>> Maze::getSolutionPath() {
	if (dfsUsed) {
		return getSolutionPathFromDFS();
	} else if (dijkstraUsed) {
		return getSolutionPathFromDIJKSTRA();
	} else if (bfsUsed) {
		return getSolutionPathFromBFS();
	}
	return std::vector<std::shared_ptr<Node>>();
}

size_t Maze::getRootNodePosition() {
	auto rootNode = std::find_if(graph.begin(), graph.end(), [](std::shared_ptr<Node> n) { return n->isRootNode(); });
	return static_cast<size_t>(std::distance(graph.begin(), rootNode));
}

size_t Maze::getEndNodePosition() {
	auto it = std::find_if(graph.begin(), graph.end(), [](std::shared_ptr<Node> n) {return n->isEndNode(); });
	return static_cast<size_t>(std::distance(graph.begin(), it));
}

Uint32 Maze::get1DColorGradient(RGB &color1, RGB &color2, float value) {
	RGB newColor{};
	newColor.r = static_cast<Uint8>(color1.r + (color2.r - color1.r) * value);
	newColor.g = static_cast<Uint8>(color1.g + (color2.g - color1.g) * value);
	newColor.b = static_cast<Uint8>(color1.b + (color2.b - color1.b) * value);
	return SDL_MapRGB(pngData.mappingFormat, newColor.r, newColor.g, newColor.b);;
}

void Maze::drawBetweenTwoNodes(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2, Uint32 color) {
	if (node1 && node2) {
		bool nodesAreVertical{ node1->getX() == node2->getX() };
		size_t node1Pos = nodesAreVertical ? node1->getY() : node1->getX();
		size_t node2Pos = nodesAreVertical ? node2->getY() : node2->getX();

		size_t startPos = (node1Pos < node2Pos) ? node1Pos : node2Pos;
		size_t endPos = (node1Pos < node2Pos) ? node2Pos : node1Pos;

		for (startPos; startPos <= endPos; ++startPos) {
			auto pixelPos = nodesAreVertical ? (node1->getX() + startPos * pngData.cols) : (startPos + node1->getY() * pngData.cols);
			pngData.pixels[pixelPos] = color;
		}
	}
}

void Maze::narrowSearchZone(int &whitePixelCount, bool &rootNodeAdded) {
	graph.clear();
	rootNodeAdded = false;
	whitePixelCount = 0;
	outerBorderXPos = 1;
	outerBorderYPos = 1;
	pngData.startX = 1;
	pngData.startY = 1;
	pngData.endX--;
	pngData.endY--;
}

void Maze::popNodeFromSearchQueue(std::queue<std::shared_ptr<Node>> &searchQueue, std::vector<std::shared_ptr<Node>> &searchOrder, int visitedCount) {
	if (visitedCount == 0) {
		searchQueue.front()->setDeadEnd(true);
	}
	searchOrder.push_back(searchQueue.front());
	searchQueue.pop();
}

void Maze::fillFinalSolution(std::queue<std::shared_ptr<Node>> &searchQueue) {
	finalSolution.clear();
	bool endIsVisited{ false };
	while (!searchQueue.empty() && !checkIfEndNodeIsVisited(searchQueue)) {
		int visitedCount{ 0 };
		addNeighboursToSearchQueue(searchQueue, visitedCount);
		popNodeFromSearchQueue(searchQueue, finalSolution, visitedCount);
	}
}

std::vector<std::shared_ptr<Node>> Maze::getSolutionPathFromBFS() {

	std::vector<std::shared_ptr<Node>> bfsSolutionPath;
	bfsSolutionPath.push_back(graph.at(getEndNodePosition()));
	bool rootIsVisited{ false };

	while (!rootIsVisited) {
		bfsSolutionPath.push_back(bfsSolutionPath.at(bfsSolutionPath.size() - 1)->getPrevNode());
		if (bfsSolutionPath.at(bfsSolutionPath.size() - 1)->isRootNode()) {
			rootIsVisited = true;
		}
	}
	std::reverse(bfsSolutionPath.begin(), bfsSolutionPath.end());

	return bfsSolutionPath;
}

Maze::Maze() : pngMazeSurface{ nullptr },
dfsUsed{ false }, bfsUsed{ false }, dijkstraUsed{ false },
outerBorderXPos{ 0 }, outerBorderYPos{ 0 } {}

Maze::~Maze() {
	SDL_FreeFormat(pngData.mappingFormat);
	pngData.mappingFormat = nullptr;
	freePNG_Surface();
}
// Viesi metodai
void Maze::convertPNGMazeToGraph(const std::string &imgPath, MyWindow &window) {
	setPNG_Surface(imgPath, window);
	pngData.setPNG_ConversionData(pngMazeSurface);
	addAllNodesAndSetRelations();
}

void Maze::breadthFirstSearch() {
    std::queue<std::shared_ptr<Node>> searchQueue;
    searchQueue.push(graph.at(getRootNodePosition()));
    searchQueue.front()->setVisited(true);
	bfsUsed = true;
    fillFinalSolution(searchQueue);
}

void Maze::depthFirstSearch() {
    std::stack<std::shared_ptr<Node>> searchStack;
	finalSolution.clear();
    searchStack.push(graph.at(getRootNodePosition()));
    finalSolution.push_back(searchStack.top());

    while (!searchStack.empty()) {
		if (addNeighboursToSearchStack(searchStack)) {
			drawBetweenTwoNodes(searchStack.top(), searchStack.top()->getTopNeighbor(), pngData.greyPixel);
			drawBetweenTwoNodes(searchStack.top(), searchStack.top()->getBottomNeighbor(), pngData.greyPixel);
			drawBetweenTwoNodes(searchStack.top(), searchStack.top()->getLeftNeighbor(), pngData.greyPixel);
			drawBetweenTwoNodes(searchStack.top(), searchStack.top()->getRightNeighbor(), pngData.greyPixel);
		} else {
			popDeadEndNodeFromSearchStack(searchStack);
		}
        finalSolution.push_back(searchStack.top());

        if (searchStack.top()->isEndNode()) {
            searchStack.top()->setVisited(true);
            break;
        }
    }
	dfsUsed = true;
}

void Maze::dijkstra() {
	setDijkstraSolution();
	dijkstraUsed = true;
}

void Maze::buildSolutionImage(const std::string &inputPath) {
	
	std::vector<std::shared_ptr<Node>> solutionPath = getSolutionPath();
	size_t solutionPathSize{ solutionPath.size() };
	float gradientStepSize = 0.05;
	float gradientStepsCount = roundf(255 / (255 * gradientStepSize));
	int colorChangeInterval = getColorChangeInterval(solutionPathSize, gradientStepSize, gradientStepsCount);
	int gradientValueModifier{ 0 };

	RGB purple = { 255, 0, 255 };
	RGB cyan = { 0, 255, 255 };
	Uint32 shiftingColor = get1DColorGradient(cyan, purple, gradientStepSize * gradientValueModifier);
	for (size_t i = 0; i < solutionPathSize - 1; ++i) {
		bool changeColor = changeColorInThisStep(i, colorChangeInterval);
		if (changeColor) {
			shiftingColor = get1DColorGradient(cyan, purple, gradientStepSize * gradientValueModifier);
		}
		if (changeColor && gradientValueModifier <= gradientStepsCount) {
			gradientValueModifier++;
		}
		drawBetweenTwoNodes(solutionPath.at(i), solutionPath.at(i + 1), shiftingColor);
	}

	std::string outputPath = inputPath.substr(0, inputPath.length() - 4) + "_SOLUTION.png";
	IMG_SavePNG(pngMazeSurface, outputPath.c_str());
}

size_t Maze::getTotalVisitedNodeCount() {
	size_t visitedNodeCount{ 0 };
	for (const auto &node : graph) {
		if (node->isVisited()) {
			++visitedNodeCount;
		}
	}
	return visitedNodeCount;
}

std::vector<std::shared_ptr<Node>> &Maze::getGraph() {
	return graph;
}

void Maze::resetVisitation() {
    graph.clear();
}