#ifndef MAZESOLVER_MAZE_H
#define MAZESOLVER_MAZE_H

#include "Node.h"
#include "MyWindow.h"
#include "SDL_image.h"
#include <vector>
#include <queue>
#include <stack>
#include "PNG_ConversionData.h"
#include "DistComparator.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>

struct RGB {
	Uint8 r, g, b;
};

class Maze {
private:
	size_t outerBorderXPos, outerBorderYPos;
	SDL_Surface *pngMazeSurface;
	PNG_ConversionData pngData;
    std::vector<std::shared_ptr<Node>> graph;
	std::vector<std::shared_ptr<Node>> finalSolution;
	bool dfsUsed, dijkstraUsed, bfsUsed;
	void setPNG_Surface(const std::string &imgPath, MyWindow &window);
	void freePNG_Surface();
	bool pixelIsBlack(Uint32 pixel);
	bool currentPositionIsOuterBorder(size_t startY, size_t currentX);

	// BFS paieskos algoritmo pagalbines funkcijos
	void addNeighbourToSearchQueue(std::queue<std::shared_ptr<Node>> &searchQueue, std::shared_ptr<Node> neighbour, int &visitedCount);
	void addNeighboursToSearchQueue(std::queue<std::shared_ptr<Node>> &searchQueue, int &visitedCount);
	bool checkIfEndNodeIsVisited(std::queue<std::shared_ptr<Node>> &searchQueue);
	void popNodeFromSearchQueue(std::queue<std::shared_ptr<Node>> &searchQueue, std::vector<std::shared_ptr<Node>> &searchOrder, int visitedCount);
	void fillFinalSolution(std::queue<std::shared_ptr<Node>> &searchQueue);
	std::vector<std::shared_ptr<Node>> getSolutionPathFromBFS();
	
	// DFS paieskos algoritmo pagalbines funkcijos
	bool addNeighbourToSearchStack(std::stack<std::shared_ptr<Node>> &searchStack, std::shared_ptr<Node> neighbour);
	bool addNeighboursToSearchStack(std::stack<std::shared_ptr<Node>> &searchStack);
	void popDeadEndNodeFromSearchStack(std::stack<std::shared_ptr<Node>> &searchStack);
	std::vector<std::shared_ptr<Node>> getSolutionPathFromDFS();

	// Dijkstros paieskos algoritmo pagalbines funkcijos
	size_t getDistBetweenTwoNodes(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);
	void addNeighbourToSearchOrder(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, DistComparator> &searchOrder, std::shared_ptr<Node> neighbour);
	void addNeighboursToSearchOrder(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, DistComparator> &searchOrder);
	void setDijkstraSolution();
	std::vector<std::shared_ptr<Node>> getSolutionPathFromDIJKSTRA();
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	
	int getColorChangeInterval(size_t solutionPathSize, float &gradientStepSize, float &gradientStepsCount); // grazina spalvu pokycio intervala.
	bool changeColorInThisStep(size_t step, int colorChangeInterval);										 // tikrina ar einamajame zinsnyje reikes keisti spalva.
	void addNewNode(size_t x, size_t y);																	 // prideda nauja virsune i grafa
	bool setRootAndEndNode(bool rootNodeAdded);																 // nustato nurodyta virsune kaip saknine arba pabaigos
	void setLeftAndRightNeighbours(bool neighbourIsBlocked);												 // grafo virsune yra sujungiama su horizontaliais kaimynais.
	void setTopAndBottomNeighbours(size_t index, bool neighbourIsBlocked);									 // grafo virsune yra sujungima su vertikaliais kaimynais.

	int getAdjacentWallCount(size_t index, bool &verticalWallShowedUp, bool &horizontalWallShowedUp) const;  // nustato kiek sienu supa nurodyta virsune (naudojama nustatant ar virsune yra sankryzoje, ar akligatvyje).
	bool intersectionOrDeadEnd(int wallCount, bool verticalWallShowedUp, bool horizontalWallShowedUp);       // nustato ar virsune yra sankryzoje, ar akligatvyje.
	bool addRootAndEndNode(size_t x, size_t y, bool neighbourIsBlocked, bool rootNodeAdded);				 // prideda nauja virsune ir nustato ja kaip saknine, jeigu saknine jau prideta, tada nustato kaip pabaigos virsune.
	bool addIntersectionOrDeadEndNode(size_t x, size_t y, bool neighbourIsBlocked);							 // prideda virsune ir nustato horizontalius kaimynus.
	bool neighbourIsblockedVertically(size_t index);														 // tikrina ar tame paciame stulpelyje esanti virsune yra neblokuojama sienos ir gali buti laikoma kaimynu.
	void sortGraphXPrimaryYSecondary();																		 // isrusiuoja grafa vertikaliu kaimynu paieskai, kad butu galima sutrumpinti kaimynu paieskos laika O(n).
	void addNodeAndSetHorizontalRelation(size_t index, bool &rootNodeAdded, bool &neighbourIsBlocked, int &whitePixelCount); // prideda virsune ir nustato horizontalius kaimynus.
	void addAllNodesAndSetHorizontalRelations();                                                             // prideda visas virsunes ir sudeda horizontalius rysius.
	void setVerticalRelations();																		     // nustato vertikalius rysius tarp visu kaimynu.
	void addAllNodesAndSetRelations();																		 // sudeda visas virsune, nustato horizontalius ir vertikalius rysius.

	std::vector<std::shared_ptr<Node>> getSolutionPath();													 // gaunamas sprendimo kelias, atsizvelgiant i grafo sprendimo buda.

	size_t getRootNodePosition();																			 // gaunama saknines virsunes pozicija.
	size_t getEndNodePosition();																			 // gaunama pabaigos virsunes pozicija.
	Uint32 get1DColorGradient(RGB &color1, RGB &color2, float value);										 // gaunamas vienamtis gradientas tarp dvieju spalvu.

	void drawBetweenTwoNodes(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2, Uint32 color);        // nupaisomas kelias tarp dvieju virsuniu.
	void narrowSearchZone(int &whitePixelCount, bool &rootNodeAdded);										 // jeigu paveiksliukas apsuptas bet kokio kito paveiksliuko, susiaurina paieskos zona.
public:
    Maze();
    ~Maze();
    void convertPNGMazeToGraph(const std::string &imgPath, MyWindow &window);
    void breadthFirstSearch();
    void depthFirstSearch();
    std::vector<std::shared_ptr<Node>> &getGraph();
    void dijkstra();
	void buildSolutionImage(const std::string &inputPath);
	size_t getTotalVisitedNodeCount();
    void resetVisitation();
};
#endif //MAZESOLVER_MAZE_H