#include <iostream>
#include "MyWindow.h"
#include <algorithm>
#include "Maze.h"
#include "MyTimer.h"
#include <cmath>
#include "SDL_image.h"
#include <string>
#include <sstream>

const int WINDOW_WIDTH{ 1536 };
const int WINDOW_HEIGHT{ 801 };
const std::string BFS{ "BFS" };
const std::string DFS{ "DFS" };
const std::string DIJKSTRA{ "DIJKSTRA" };

bool init(MyWindow &window);
std::string solveMaze(MyWindow &gWindow, Maze& maze, const std::string &filePath, const std::string &traversalMethod);
void close();

int main(int argc, char *argv[]) {
	MyWindow gWindow;
	MyTimer timer;
	Maze maze;

	std::string mazePath{};
	std::string traversalMethod{};

	if (argc < 3) {
		std::cout << "Not enough arguments passed" << std::endl;
		return 1;
	} else {
		mazePath = argv[1];
		traversalMethod = argv[2];
	}

	if (traversalMethod != BFS && traversalMethod != DFS && traversalMethod != DIJKSTRA) {
		std::cout << "Method parameter is incorrect";
		return 1;
	}

	if (init(gWindow)) {
		bool run{ true };
		timer.start();
		std::string outputStatement = solveMaze(gWindow, maze, mazePath, traversalMethod);
		timer.pause();
		std::stringstream timeToString("");
		timeToString << static_cast<float>(timer.getTicks()) / 1000;
		outputStatement += ("\nTime taken: ");
		outputStatement += timeToString.str();
		outputStatement += "s.";

		std::cout << outputStatement << std::endl;
		close();
	}
	return 0;
}

bool init(MyWindow &window) {
	return window.init("Maze Solver", WINDOW_WIDTH, WINDOW_HEIGHT);
}

std::string solveMaze(MyWindow &gWindow, Maze& maze, const std::string &filePath, const std::string &traversalMethod) {
	std::string outputStatement{ "The program has finished solving a maze with " };

	try {
		maze.convertPNGMazeToGraph(filePath, gWindow);
	} catch (const std::invalid_argument &e) {
		std::cout << e.what();
	}

	if (traversalMethod == BFS) {
		maze.breadthFirstSearch();
		outputStatement += BFS + " algorithm.\nA total of ";
		outputStatement += std::to_string(maze.getTotalVisitedNodeCount()) + " nodes have been visited.";
	}
	if (traversalMethod == DFS) {
		maze.depthFirstSearch();
		outputStatement += DFS + " algorithm.\nA total of ";
		outputStatement += std::to_string(maze.getTotalVisitedNodeCount()) + " nodes have been visited.";
	}
	if (traversalMethod == DIJKSTRA) {
		maze.dijkstra();
		outputStatement += DIJKSTRA + " algorithm.\nA total of ";
		outputStatement += std::to_string(maze.getTotalVisitedNodeCount()) + " nodes have been visited.";
	}
	maze.buildSolutionImage(filePath);
	return outputStatement;
}

void close() {}
