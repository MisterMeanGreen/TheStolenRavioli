#pragma once
#include <queue> //For telling what to search first
#include <math.h>
#include <algorithm>
#include <vector>
#include <cstdlib>
#define MAP_LENGTH 60 
#define MAP_HEIGHT 15
void PaintCollsionMap(int Length, int Height, int *Destination, int TargetX, int TargetY, int X, int Y);
using namespace std;
int distancetogoal;
int collsionm[15][60] = { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } }; //CollsionMap
struct Cell {
	int cellid, cellidpointer, MoveCost, Weight;
};deque<Cell> ClosedMap; //Which paths we have already explored
struct Comp {
	bool operator()(const Cell& a, const Cell& b) {
		return a.Weight>b.Weight;
	}
};
void GetNextMoveToTarget(int TargetX, int TargetY, short ReturnX, short ReturnY, int* dist, int* Return) {
	*dist = 0;
	*Return = ((ReturnY * MAP_LENGTH) + ReturnX);
	distancetogoal = 0;
	//Reset stuff to clear the menu
	priority_queue<Cell, vector<Cell>, Comp> Searching; //To sort the things
	ClosedMap.clear();
Cell CellTemp;
	PaintCollsionMap(MAP_LENGTH, MAP_HEIGHT, &collsionm[0][0], TargetX, TargetY, ReturnX, ReturnY);
	if ((ReturnX == TargetX) && (ReturnY == TargetY)) {
		//Our postion is the same as the target we are done
		return;
	}
	int CurrentNodeX = ReturnX;
	int CurrentNodeY = ReturnY;
	//This just pushes the 4 blocks around the starting point into the Searching queue
	if (collsionm[CurrentNodeY + 1][CurrentNodeX] != 1) {
		CellTemp = { (((CurrentNodeY + 1) * MAP_LENGTH) + CurrentNodeX), ((CurrentNodeY * MAP_LENGTH) + CurrentNodeX), 1, abs(TargetX - CurrentNodeX) + abs(TargetY - CurrentNodeY) + 1 }; //Pushing the Cell we are looking at and where its parent
		Searching.push(CellTemp);
		if (collsionm[CurrentNodeY + 1][CurrentNodeX] != 2) {
			collsionm[CurrentNodeY + 1][CurrentNodeX] = 1;
		}
	}
	if (collsionm[CurrentNodeY - 1][CurrentNodeX] != 1) {
		CellTemp = { (((CurrentNodeY - 1) * MAP_LENGTH) + CurrentNodeX), ((CurrentNodeY * MAP_LENGTH) + CurrentNodeX), 1, abs(TargetX - CurrentNodeX) + abs(TargetY - CurrentNodeY) + 1 }; //Pushing the Cell we are looking at and where its parent
		Searching.push(CellTemp);
		if (collsionm[CurrentNodeY - 1][CurrentNodeX] != 2) {
			collsionm[CurrentNodeY - 1][CurrentNodeX] = 1;
		}
	}
	if (collsionm[CurrentNodeY][CurrentNodeX + 1] != 1) {
		CellTemp = { (((CurrentNodeY)* MAP_LENGTH) + CurrentNodeX + 1), ((CurrentNodeY * MAP_LENGTH) + CurrentNodeX), 1, abs(TargetX - CurrentNodeX) + abs(TargetY - CurrentNodeY) + 1 }; //Pushing the Cell we are looking at and where its parent
		Searching.push(CellTemp);
		if (collsionm[CurrentNodeY][CurrentNodeX + 1] != 2) {
			collsionm[CurrentNodeY][CurrentNodeX + 1] = 1;
		}
	}
	if (collsionm[CurrentNodeY][CurrentNodeX - 1] != 1) {
		CellTemp = { (((CurrentNodeY)* MAP_LENGTH) + CurrentNodeX - 1), ((CurrentNodeY * MAP_LENGTH) + CurrentNodeX), 1, abs(TargetX - CurrentNodeX) + abs(TargetY - CurrentNodeY) + 1 }; //Pushing the Cell we are looking at and where its parent
		Searching.push(CellTemp);
		if (collsionm[CurrentNodeY][CurrentNodeX - 1] != 2) {
			collsionm[CurrentNodeY][CurrentNodeX - 1] = 1;
		}
	}
	while (!Searching.empty()) {
			//Checking to see if the first entry in Searching is the goal, if not put in closed.
		CellTemp = Searching.top();
		short Y = CellTemp.cellid / MAP_LENGTH;
		short X = abs(CellTemp.cellid - (MAP_LENGTH * Y));
			if (collsionm[Y][X] == 2) {
				int prevID = CellTemp.cellid; //Used to return X and tell the next move
				int currID = CellTemp.cellidpointer; //For the current value
				int i = 0; //To search Closed
				while (!(currID == ((ReturnY* MAP_LENGTH) + ReturnX))) { //Finding the start position (previd value would be the next move)
					i = 0;
					while ((ClosedMap[i].cellid != currID) || ( currID ==((ReturnY* MAP_LENGTH) + ReturnX))) {
						i++;
					}
					distancetogoal++;
					prevID = currID;
					currID = ClosedMap[i].cellidpointer;
				}
				//Outputting
				*dist = distancetogoal; //Setting the distance to the make number of steps to the goal
				*Return = prevID;
				return;
			}
			else {
				//Adds it too closed and sets the map to make sure not to check it again. Then rechecks the surrounding blocks
				ClosedMap.push_back(Searching.top());
				Searching.pop();
				collsionm[Y][X] = 1;
				CurrentNodeX = X;
				CurrentNodeY = Y;
				if (collsionm[CurrentNodeY + 1][CurrentNodeX] != 1) {
					CellTemp = { (((CurrentNodeY + 1) * MAP_LENGTH) + CurrentNodeX), ((CurrentNodeY * MAP_LENGTH) + CurrentNodeX), ClosedMap[0].MoveCost + 1, abs(TargetX - CurrentNodeX) + abs(TargetY - CurrentNodeY) + ClosedMap[0].MoveCost + 1 }; //Pushing the Cell we are looking at and where its parent
					Searching.push(CellTemp);
					if (collsionm[CurrentNodeY + 1][CurrentNodeX] != 2) {
					collsionm[CurrentNodeY + 1][CurrentNodeX] = 1;
				}
				}
				if (collsionm[CurrentNodeY - 1][CurrentNodeX] != 1) {
					CellTemp = { (((CurrentNodeY - 1) * MAP_LENGTH) + CurrentNodeX), ((CurrentNodeY * MAP_LENGTH) + CurrentNodeX), ClosedMap[0].MoveCost + 1, abs(TargetX - CurrentNodeX) + abs(TargetY - CurrentNodeY) + ClosedMap[0].MoveCost + 1 }; //Pushing the Cell we are looking at and where its parent
					Searching.push(CellTemp);
					if (collsionm[CurrentNodeY - 1][CurrentNodeX] != 2) {
						collsionm[CurrentNodeY - 1][CurrentNodeX] = 1;
					}
				}
				if (collsionm[CurrentNodeY][CurrentNodeX + 1] != 1) {
					CellTemp = { (((CurrentNodeY)* MAP_LENGTH) + CurrentNodeX + 1), ((CurrentNodeY * MAP_LENGTH) + CurrentNodeX), ClosedMap[0].MoveCost + 1, abs(TargetX - CurrentNodeX) + abs(TargetY - CurrentNodeY) + ClosedMap[0].MoveCost + 1 }; //Pushing the Cell we are looking at and where its parent
					Searching.push(CellTemp);
					if (collsionm[CurrentNodeY][CurrentNodeX + 1] != 2) {
						collsionm[CurrentNodeY][CurrentNodeX + 1] = 1;
					}
				}
				if (collsionm[CurrentNodeY][CurrentNodeX - 1] != 1) {
					CellTemp = { (((CurrentNodeY)* MAP_LENGTH) + CurrentNodeX - 1), ((CurrentNodeY * MAP_LENGTH) + CurrentNodeX), ClosedMap[0].MoveCost + 1, abs(TargetX - CurrentNodeX) + abs(TargetY - CurrentNodeY) + ClosedMap[0].MoveCost + 1}; //Pushing the Cell we are looking at and where its parent
					Searching.push(CellTemp);
					if (collsionm[CurrentNodeY][CurrentNodeX - 1] != 2) {
						collsionm[CurrentNodeY][CurrentNodeX - 1] = 1;
					}
				}
			}
		}
	return;
	}
void PaintCollsionMap(int Length, int Height, int *Destination, int TargetX, int TargetY, int X, int Y) { //For the pathfinding
		//Paint the Map
		//Sets the map blank
		for (int i = 0; i < Height; i++)
		{
			for (int l = 0; l < Length; l++) {
				*(Destination + ((i * Length) + l)) = 0;
			}
		}

		//Paints walls
		for (int i = 0; i < Height; i++)
		{
			for (int l = 0; l < Length; l++) {
				if (finalmap[loc][i][l].blocktype > 0) {
					*(Destination + ((i * Length) + l)) = 1; //i is the height and l is the length
				}
			}
		}
		//Paints Objects
		for (int objfinder = 0; objfinder < (sizeof(Objmap) / sizeof(Objmap[0])); objfinder++) {
			if (Objmap[objfinder].ObjLoc == loc) {
				if (Objmap[objfinder].ObjCollsion == 1) {
					*(Destination + ((Objmap[objfinder].ObjCurrent.Y * Length) + Objmap[objfinder].ObjCurrent.X)) = 1;
				}
			}
		}
		//Paints goal
		*(Destination + ((TargetY * Length) + TargetX)) = 2; //This finds the Height and Length and sets that value to 1
	}
struct DistancePoint {
	int distance;
	int a_pointer;
	int ReturnVal;
};
struct CompDist {
	bool operator()(const DistancePoint& a, const DistancePoint& b) {
		return a.distance>b.distance;
	}
};