// TheStolenRavioli.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <conio.h>
#include <bitset>
#include <stdlib.h>
#include "Maps.h"
#include "Pathing.h"
#include <queue>
#define KB_UP 72 //Makes moving and esc commands more readable
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27
#define KB_SPACE 32
using namespace std;
bool PreformedAct = false;
short nPlayerX = 1;
short nPlayerY = 1;
short previousx = nPlayerX; //previous X for deleleting previous person
short previousy = nPlayerY; //previous y for deleting previous person
HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);
HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
HWND console = GetConsoleWindow();
RECT r; // Remember how things were when we started
CONSOLE_SCREEN_BUFFER_INFO csbi;
size_t a = 0; //For command cpy in Case KB_ESC
int collsiontest = 0; //To test for collosion
int RemoveBackground(int HexVal); //For removing the background and placing a white character
void RenderBlock(int x, int y, int loc); //To render the speicifc block
void PlayerMove(int OffsetY, int OffsetX, int Test4Collsion); //For moving the player
void RenderObjects(int DebugError); //Rendering the Objects
void RenderMap(); //Render the map
void PaintCollsionMap(int Length, int Height, int *Destination, int TargetX, int TargetY, int X, int Y); //Paint a Collsion map and the Target
void DrawPlayer(int color, int nPlayerY, int nPlayerX, int loc, char Output); //Draw the player
char CommandList[20][60] = { "help", "w", "west", "e", "east", "n", "north", "s", "south", "inv", "inventory", "ins", "inspect", "grab", "grb", "pk", "peek", "./excavate", "pke", "poke" }; //All the commands
int main()
{
	LoadTestGame();
	GetWindowRect(console, &r); //stores the console's current dimensions
	GetConsoleScreenBufferInfo(hstdout, &csbi);
	MoveWindow(console, r.left, r.top, 800, 500, TRUE);
SwitchLoc: //To redraw the scene
	RenderMap();
	//Drawing Commandline and other things
	RenderObjects(1); //Drawing Objects
	DrawPlayer(15, nPlayerY, nPlayerX, loc, '\x1'); //Drawing player
	while (true) {
		//For drawing the x coords on screen
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 17 });
		cout << "                                                                                ";
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 17 });
		cout << "X : " << nPlayerX << " Y: " << nPlayerY;
		SetConsoleTextAttribute(hstdout, 0x0F); //Sets the color of the person.
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 18 });
		SetConsoleTextAttribute(hstdout, 0x0A);
		cout << "==================================== CONSOLE ===================================";
	INVALIDKEY: //If the user presses an invalid key
		SetConsoleTextAttribute(hstdout, 0x0F); //Sets the color of the person.
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { nPlayerX , nPlayerY });
		int keypress = _getch();
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 19 }); //Sets the cursor below console text
		cout << "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               "; /*Clears Console */
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 }); //Resets the cursor
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 19 }); //Sets the cursor below console text
		SetConsoleTextAttribute(hstdout, 0x0A);
		switch (keypress) {
		case KB_LEFT:
			cout << "west" << endl;
		MoveWest:
			PlayerMove(-1, 0, 1); //Move West
			break;
		case KB_RIGHT:
			cout << "east" << endl;
		MoveEast:
			PlayerMove(1, 0, 1); //Move East
			break;
		case KB_UP:
			cout << "north" << endl;
		MoveNorth:
			PlayerMove(0, -1, 1); //Move North
			break;
		case KB_DOWN:
			cout << "south" << endl; 
		MoveSouth:
			PlayerMove(0, 1, 1); //Move South
			break;
		case KB_ESCAPE:
			/*
			This works by first clearing the Console Screen
			Takes an input for a command
			Copies that to an array Command
			And creates another array CommandCopy and sets it to Null
			Then in a loop, set Null to a row in CommandList (Any command ever)
			And compares that against the Command until it is true. 
			Then uses the row number in a switch case 
			And executes the command
			*/
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 19 }); //Sets the cursor below console text
			cout << "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                "; /*Clears Console */
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 }); //Resets the cursor
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 19 }); //Sets the cursor below console text
			int OffsetX;
			int OffsetY;
			OffsetX = 0;
			OffsetY = 0;
			char Command[60];
			SetConsoleTextAttribute(hstdout, 0x0A); 
			cin >> Command; //To input a Command
			a = 0;
			while (Command[a] != '\0') {
				Command[a] = tolower(Command[a]);
				a++;
}
			char CommandCopy[60]; //For copying Commands, from CommandList
			strcpy_s(CommandCopy, "Null");
			a = (sizeof(CommandList) / sizeof(CommandList[0]) + 2);
			for (int i = 0; i < (sizeof(CommandList) / sizeof(CommandList[0]) + 1); i++) {
				strcpy_s(CommandCopy, CommandList[i]);
				if (strcmp(Command, CommandCopy) == 0) {
					a = i + 1;
				}
			}
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 20 }); //Sets the cursor below console text
				if (a < (sizeof(CommandList) / sizeof(CommandList[0]) + 1)) {
					switch (a) { //Making cases easier
					case 1: //help
						cin >> Command;
						a = 0;
						while (Command[a] != '\0') {
							Command[a] = tolower(Command[a]);
							a++;
						}
						strcpy_s(CommandCopy, "list");
						a = (sizeof(CommandList) / sizeof(CommandList[0]) + 2);
						for (int i = 0; i < (sizeof(CommandList) / sizeof(CommandList[0]) + 1); i++) {
							strcpy_s(CommandCopy, CommandList[i]);
							if (strcmp(Command, CommandCopy) == 0) {
								a = i + 1;
							}
						}
						if (strcmp(Command, "list") == 0) {
							a = 0;
						}
						//Below if is for telling what each command does!
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 21 }); //Sets the cursor below console text
						if (a < (sizeof(CommandList) / sizeof(CommandList[0]) + 1)) {
							switch (a) { //Making cases easier
							case 1: //Help
								cout << "To give the defintion of a command.";
								break;
							case 2: //Moving the player West
							case 3:
								cout << "Moves the player west.";
								break;
							case 4: //Moving the player east
							case 5:
								cout << "Moves the player east.";
								break;
							case 6: //Moving the player north
							case 7:
								cout << "Moves the player north.";
								break;
							case 8: //Moving the player south
							case 9:
								cout << "Moves the player south.";
								break;
							case 10: //Open the inventory
							case 11:							
								cout << "Opens the inventory.";
								break;
							case 12: //Inspect objects
							case 13:								
								cout << "Inform what the object near the player is...";
								break;
							case 14: //Grab an object
							case 15:						
								cout << "Pick up a nearby object.";
								break;
							case 16:
							case 17:
								cout << "Look at a specific object in the Objmap Vector.";
								break;
							case 18:
								cout << "Hacks the system.";
								break;
							case 0:
								cout << "All Commands" << "\n\n" << "help" << "\n\n";
								for (a = 1; a < (sizeof(CommandList) / sizeof(CommandList[0])); a+= 2) {
									if (a != 17) {
										cout << CommandList[a] << " \\ " << CommandList[a + 1] << "\n";
									}
								}
								break;
							}
						}
						else {
							cout << "No Command with name of " << Command;
						}
						break;
					case 2: // Moving the player West
					case 3:
						goto MoveWest;
						break;
					case 4: // Moving the player East
					case 5:
						goto MoveEast;
						break;
					case 6: // Moving the player North
					case 7:
						goto MoveNorth;
						break;
					case 8: // Moving the player South
					case 9:
						goto MoveSouth;
						break;
					case 10: //Opening the Inventory
					case 11:
						//Clears the screen
						cout << "                                                                                ";
						if (!(Inventory.empty())) {
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 }); //Resets the cursor
							cout << "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       "; /*Clears Console */
							cout << "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         "; /*Clears Console */
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 }); //Resets the cursor
							cout << "Inventory items" << endl << endl;
							a = 0;
							while (a < Inventory.size()) {
								cout << "  " <<Inventory[a].Name << endl;
								a++;
							}
							size_t pos = 0;
							int posget = 0;
							do {
								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, short(pos + 2) }); //Resets the cursor
								cout << '\x1A';
								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, short(pos + 2) }); //Resets the cursor
								posget = _getch();
								if (posget == KB_DOWN) {
									if (pos < (Inventory.size() - 1)) {
										SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, short(pos + 2) }); //Resets the cursor
										cout << "  ";
										pos++;
									}
								}
								else if (posget == KB_UP) {
									if (pos > 0) {
										SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, short(pos + 2) }); //Resets the cursor
										cout << "  ";
										pos--;
									}
								}
							} while (!(posget == KB_ESCAPE));
							cout << "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               "; /*Clears Console */
							cout << "                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                "; /*Clears Console */
							RenderMap();
							//Drawing Commandline and other things
							RenderObjects(1); //Drawing Objects
							DrawPlayer(15, nPlayerY, nPlayerX, loc, '\x1'); //Drawing player
								//For drawing the x coords on screen
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 17 });
							cout << "                                                                                ";
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 17 });
							cout << "X : " << nPlayerX << " Y: " << nPlayerY;
								SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 18 });
								SetConsoleTextAttribute(hstdout, 0x0A);
								cout << "==================================== CONSOLE ===================================";
								SetConsoleTextAttribute(hstdout, 0x0F);
						}
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 20 }); //Sets the cursor below console text
						cout << "You have nothing!";
						break;
					case 12: //Inspecting objects
					case 13:
						//Doing offsets and using them to find the location of an object relative to the player
						cin >> CommandCopy;
						a = 0;
						while (Command[a] != '\0') {
							Command[a] = tolower(Command[a]);
							a++;
						}
						if ((strcmp(CommandCopy, "n") == 0) || (strcmp(CommandCopy, "north") == 0)) {
							OffsetY = -1;
						}
						else if ((strcmp(CommandCopy, "s") == 0) || (strcmp(CommandCopy, "south") == 0)) {
							OffsetY = 1;
						}
						else if ((strcmp(CommandCopy, "e") == 0) || (strcmp(CommandCopy, "east") == 0)) {
							OffsetX = 1;
						}
						else if ((strcmp(CommandCopy, "w") == 0) || (strcmp(CommandCopy, "west") == 0)) {
							OffsetX = -1;
						}
						a = 0;
						while (!((Objmap[a].ObjLoc == loc) && (Objmap[a].ObjCurrent.X == nPlayerX + OffsetX) && (Objmap[a].ObjCurrent.Y == nPlayerY + OffsetY)) && (a < Objmap.size() - 1)) {
							a++;
						}
						//Looking for an item
						if (a >= Objmap.size() - 1) {
							cout << "Theres nothing ";
							if (OffsetX == 0 && OffsetY == 0) {
								cout << "here!";
							}
							else {
								cout << "there!";
							}
						}
						else {
							//Default way to display object titles, if it is a sign, the "Sign :" is used.
							if (Objmap[a].ObjType == 1) {
								cout << "Sign : ";
							}
							cout << Objmap[a].Objstring << endl;

						}
						break;
					case 14:
					case 15://Grabbing objects
						//Doing offsets and using them to find the location of an object relative to the player
						cin >> CommandCopy;
						a = 0;
						while (Command[a] != '\0') {
							Command[a] = tolower(Command[a]);
							a++;
						}
						if ((strcmp(CommandCopy, "n") == 0) || (strcmp(CommandCopy, "north") == 0)) {
							OffsetY = -1;
						}
						else if ((strcmp(CommandCopy, "s") == 0) || (strcmp(CommandCopy, "south") == 0)) {
							OffsetY = 1;
						}
						else if ((strcmp(CommandCopy, "e") == 0) || (strcmp(CommandCopy, "east") == 0)) {
							OffsetX = 1;
						}
						else if ((strcmp(CommandCopy, "w") == 0) || (strcmp(CommandCopy, "west") == 0)) {
							OffsetX = -1;
						}
						a = 0;
						while (!((Objmap[a].ObjLoc == loc) && (Objmap[a].ObjCurrent.X == nPlayerX + OffsetX) && (Objmap[a].ObjCurrent.Y == nPlayerY + OffsetY) && (Objmap[a].ObjType == 4)) && (a < Objmap.size() - 1)) {
							a++;
						}
						//If it cant find the object
						if (a >= Objmap.size() - 1) {
							cout << "Theres nothing to grab ";
							if (OffsetX == 0 && OffsetY == 0) {
								cout << "here!";
							}
							else {
								cout << "there!";
							}
						}
						else {
							cout << "Got " << Objmap[a].Objstring << "!"<< endl;
							Inventory.push_back({ Objmap[a].Objstring, (int)Objmap[a].ObjNum });
							Objmap.erase(Objmap.begin() + a );
							//To tell whether to render the block on the postion where the item stood
							bool somethingthere = false;
							if (OffsetX == 0 && OffsetY == 0) {
								somethingthere = true;
							}
							else {
								for (a = 0; a < Objmap.size(); a++) {
									if (Objmap[a].ObjCurrent.X == (nPlayerX + OffsetX) && Objmap[a].ObjCurrent.Y == (nPlayerY + OffsetY)) {
										somethingthere = true;
									}
								}
							}
							if (somethingthere == false) {
								RenderBlock((nPlayerX + OffsetX), (nPlayerY + OffsetY), loc);
								a = Objmap.size();
							}
						}
						break;
					case 16: //Show an object stats
					case 17:
						cin >> Command; //Peek at objects or walls?
						a = 0;
						//Set to lower
						while (Command[a] != '\0') {
							Command[a] = tolower(Command[a]);
							a++;
						}
						if (strcmp(Command, "obj") == 0 || strcmp(Command, "object") == 0) {
						cin >> Command; //Output current object in console
						a = 0;
						//Set to lower
						while (Command[a] != '\0') {
							Command[a] = tolower(Command[a]);
							a++;
						}
						if (isdigit(*Command) && size_t(atoi(Command)) <= Objmap.size()) { //If 0 print all else print the requested one
							if (atoi(Command) < 0) {
								//Give object info
								cout << "\nType : " << Objmap[size_t(atoi(Command - 1))].ObjType << endl;
								cout << "X: " << Objmap[size_t(atoi(Command - 1))].ObjCurrent.X << " Y: " << Objmap[size_t(atoi(Command - 1))].ObjCurrent.Y << " Loc: " << Objmap[size_t(atoi(Command - 1))].ObjLoc << endl;
								cout << "Collsion :  " << bool(Objmap[size_t(atoi(Command - 1))].ObjCollsion != 0) << endl; //Convert from the 1/0 to tell whether we have collsion or not
								cout << "Opt. Double : " << Objmap[size_t(atoi(Command - 1))].ObjNum << endl;
								cout << "Opt. Text : \"" << Objmap[size_t(atoi(Command - 1))].Objstring << "\"" << endl;
								cout << "ID : " << Objmap[size_t(atoi(Command - 1))].ObjID << endl << endl;
							}
							else {
								for (size_t i = 1; i <= Objmap.size(); i++) {
									//Give the objects info
									cout << '\n' << "Object : " << i << endl;
									cout << "Type : " << Objmap[i - 1].ObjType << endl;
									cout << "X : " << Objmap[i - 1].ObjCurrent.X << " Y : " << Objmap[i - 1].ObjCurrent.Y << " Loc : " << Objmap[i - 1].ObjLoc << endl;
									cout << "Collsion :  " << bool(Objmap[i - 1].ObjCollsion != 0) << endl; //Convert from the 1/0 to tell whether we have collsion or not
									cout << "Opt. Double : " << Objmap[i - 1].ObjNum << endl;
									cout << "Opt. Text : \"" << Objmap[i - 1].Objstring << "\"" << endl;
									cout << "ID : " << Objmap[i - 1].ObjID << endl << endl;
								}
							}			
						} else { //Else for isdigit
							cout << "Input is non-numeric or is larger than the total size of the object map. Total size of map is " << Objmap.size() << "." << endl; 
						}
						}
						else if (strcmp(Command, "wll") == 0 || strcmp(Command, "wall") == 0) { //To peek at a wall
							cin >> Command; //Get X
							/*
							First Input is X then Y
							*/
							int TempX, TempY, TempLoc; //Temporary locations to peek
							if (isdigit(*Command) && atoi(Command) <= 60 && atoi(Command) >= 0) { //If 0 print all else print the requested one
								TempX = atoi(Command);
								cin >> Command; //Get y
								if (isdigit(*Command) && atoi(Command) <= 15 && atoi(Command) >= 0) { //If 0 print all else print the requested one
									TempY = atoi(Command);
									cin >> Command; //Get loc
									if (isdigit(*Command) && atoi(Command) >= 0) { //If 0 print all else print the requested one
										TempLoc = atoi(Command);
										cout << "\nX: " << TempX << " Y: " << TempY << " Loc: " << TempLoc << endl;
										cout << "Type : " << finalmap[TempLoc][TempY][TempX].blocktype << endl;
										cout << "Block Color : " << finalmap[TempLoc][TempY][TempX].blockcolor << endl;
									} else {
										cout << "Input is non-numeric or is smaller than 0.";
									}
								} else {
									cout << "Input is non-numeric or is larger than the total size of the map (15).";
								}
							} else {
								cout << "Input is non-numeric or is larger than the total size of the map (60).";
							}
							
						}
						break;
					case 18: //./excavate
						while (true) {
							for (a = 0; a < 255; a++) {
								SetConsoleTextAttribute(hstdout, WORD(a)); //Sets the color of the person.
								cout << "#############################RETREIVING INFO ON KAI#############################";
							}
						}
						break;
					case 19: //poke command
					case 20:

					case 0:
					default:
						//If a person enters Null
						break;
					}
				}
				else {
					//If a person enters a command that doesnt exist
					cout << "I can't " << Command << '!' << endl;
					cin.clear();
					cin.ignore(INT_MAX, '\n');
				}
			break;
		case KB_SPACE:
			//For the player to do nothing
			PreformedAct = true;
			break;
		default: //If the user presses a key that isnt Escape, or an arrow key.
			//Blah blah, goto is bad, blah blah. This is just an easy way of not doing anything for an invalid keypress
			goto INVALIDKEY;
			break;
		}
		int blockdata = finalmap[loc][previousy][previousx].blocktype; //Sets the block type for the current block
		RenderBlock(previousx, previousy, loc);
		if (PreformedAct == true) { //If the player did an action do AI stuff
			//Point of this priorty queue is too use the closest AI first
			a = 0;
			priority_queue<DistancePoint, vector<DistancePoint>, CompDist> SortAI; //To sort the things
			while (a < Objmap.size() - 1) {
				while (!((Objmap[a].ObjLoc == loc) && (Objmap[a].ObjType == 2)) && (a < Objmap.size() - 1)) {
					a++;
				}
				if (Objmap[a].ObjType == 2) {
					DistancePoint TempDistancePoint = { 0, int(a) , 0 };
					GetNextMoveToTarget(nPlayerX, nPlayerY, Objmap[a].ObjCurrent.X, Objmap[a].ObjCurrent.Y, &TempDistancePoint.distance, &TempDistancePoint.ReturnVal);
					SortAI.push(TempDistancePoint);
					a++;
				}
			}
			//To basically run the AI code and use the a_pointer value inside the actual Objmap to point to the Object data inside it IE: X cord and Y cord etc
				while (!SortAI.empty()) {
					a = SortAI.top().a_pointer;
					Objmap[a].ObjCurrent.Y = SortAI.top().ReturnVal / MAP_LENGTH;
					Objmap[a].ObjCurrent.X = abs(SortAI.top().ReturnVal - (MAP_LENGTH * Objmap[a].ObjCurrent.Y));
					SortAI.pop();
					//These are too make sure another AI isnt there
					int OldA = a;
					bool arraycontains = false;
					a = 0;
					//Tests to make sure it isnt the same object, in the same location, same x and y, same Type too and if it went over the total elements in the objectmap
   					while (!(OldA != a && Objmap[a].ObjLoc == loc && Objmap[OldA].ObjCurrent.X == Objmap[a].ObjCurrent.X && Objmap[OldA].ObjCurrent.Y == Objmap[a].ObjCurrent.Y && Objmap[a].ObjType == 2) && (a < (Objmap.size() - 1))) {
						a++;
					}
					if (a < (Objmap.size() - 1)) { //If a didnt go over the total elements in ObjMap then something must be the same
						arraycontains = true;
					}
					a = OldA; //Reset the value of a to the orginal state to actually use it
							  //To make sure the object doesnt appear ontop of the player or another AI
					if ((Objmap[a].ObjCurrent.X == nPlayerX && Objmap[a].ObjCurrent.Y == nPlayerY) || arraycontains == true) {
						Objmap[a].ObjCurrent.X = Objmap[a].ObjOld.X;
						Objmap[a].ObjCurrent.Y = Objmap[a].ObjOld.Y;
					}
		}
				PreformedAct = false;
		}
		//Gets the location of a tp spot if the player is on one
		a = 0;
		while (!(Objmap[a].ObjLoc == loc && Objmap[a].ObjCurrent.X == nPlayerX && Objmap[a].ObjCurrent.Y == nPlayerY && Objmap[a].ObjType == 3) && (a < (Objmap.size() - 1))) {
			a++;
		}
		if (a >= Objmap.size() - 1) {} //If it could find one
		else {
			//Uses floats and stuff
			double E = Objmap[a].ObjNum;
			//Maths for calculating the stuff... similiar to the path finding one
			double Y = floor(E / (61 * 15));
			double X = floor((E / 15) - (Y * 61));
			double Loc = round((E - (((Y * 61) + X) * 15)) * 100);
			loc = static_cast<int>(Loc);
			nPlayerY = static_cast<int>(Y);
			nPlayerX = static_cast<int>(X);
			goto SwitchLoc;
		}
		RenderObjects(0); //Drawing Objects
		SetConsoleTextAttribute(hstdout, 0x0F); //Sets the color of the person.
		DrawPlayer(15, nPlayerY, nPlayerX, loc, '\x1'); //Drawing player
	}
    return 0;
}
int RemoveBackground(int HexVal) {
	stringstream ss;
	ss << "0x" << hex << HexVal << "F"; //Turns the integer into a hex number. It has 3 hex letters 
										  /*
										  Number could be (0xA2F)
										  0x = To define the string as a hex number
										  A = The background color of the current block the player is on
										  2 = Is the foreground color
										  F = White, this is constant as the color of the player should always be white
										  */
	string StringHex = ss.str(); //To set the stringstream to a string
	StringHex.erase(3, 1); //Erase the the third letter, which would've been the foreground color but is now White.
	int IntHex = (stoi(StringHex, nullptr, 0)); //Turn the string to an integer
												//This is if the the lumonsity is on. For visibility reasons it will subtract 15 from the hex # to make the player black
	if (IntHex == 255 || IntHex == 239 || IntHex == 0xAF || IntHex == 0xBF || IntHex == 0xCF || IntHex == 0xDF || IntHex == 127) {
		return (IntHex - 15);
	}
	else {
		return IntHex;
	}
}
void PlayerMove(int OffsetX, int OffsetY, int Test4Collsion) {
	int arrayval = 0; //To increment the arrayvalue in Objmap
	if (Test4Collsion == 0) { goto CollsionBypass; }
	//For setting a negative block val to zero.  if a < 0 then (a + |a|) / 2 = 0, then dividing by two because if a > 0 then (a + |a|) / 2 = a
	collsiontest = (abs(finalmap[loc][nPlayerY + OffsetY][nPlayerX + OffsetX].blocktype) + finalmap[loc][nPlayerY + OffsetY][nPlayerX + OffsetX].blocktype) / 2; 
	a = 0;
	while (!(Objmap[a].ObjLoc == loc && Objmap[a].ObjCurrent.X == nPlayerX + OffsetX && Objmap[a].ObjCurrent.Y == nPlayerY + OffsetY) && (a < (Objmap.size() - 1))) {
		a++;
	}
	if ((Objmap[a].ObjCollsion >= 1) && !(a > (Objmap.size() - 1))) {
		collsiontest = collsiontest + Objmap[arrayval].ObjCollsion;
	}
	if  (collsiontest <= 0) {
		CollsionBypass: //Incase we want to ignore collsion for some reason
		previousx = nPlayerX; //Sets the previous X and Y
		previousy = nPlayerY;
		nPlayerY = nPlayerY + OffsetY;
		nPlayerX = nPlayerX + OffsetX;
	}
	else {
		cout << "There is something in the way!";
	}
	PreformedAct = true;
}
void RenderObjects(int DebugError) {
	short errors = 19;
	vector<int> TempPointer; //Where to point in the Objmap
	for (size_t i = 0; i < Objmap.size(); i++) { //Draws the objects
		if (Objmap[i].ObjOld.X != Objmap[i].ObjCurrent.X || Objmap[i].ObjOld.Y != Objmap[i].ObjCurrent.Y) { //To redraw over blocks in the old position
			bool ObjInPos = false;
			for (size_t a = 0; a < Objmap.size(); a++) {
				if (Objmap[i].ObjOld.X == Objmap[a].ObjCurrent.X && Objmap[i].ObjOld.Y == Objmap[a].ObjCurrent.Y && Objmap[a].ObjLoc == loc) { //To check to make sure their are no objects in the position
					ObjInPos = true;
				}				
			}
			if (ObjInPos == false) {
				RenderBlock(Objmap[i].ObjOld.X, Objmap[i].ObjOld.Y, loc);
			}
			Objmap[i].ObjOld.X = Objmap[i].ObjCurrent.X;
			Objmap[i].ObjOld.Y = Objmap[i].ObjCurrent.Y;
		}
		if (!(Objmap[i].ObjType == 2)) {
			SetConsoleTextAttribute(hstdout, 0x0F); //Sets the color of the object.
			if (!(finalmap[loc][Objmap[i].ObjCurrent.Y][Objmap[i].ObjCurrent.X].blockcolor < 15)) { //Checks to see if there is a background color
				SetConsoleTextAttribute(hstdout, RemoveBackground(finalmap[loc][Objmap[i].ObjCurrent.Y][Objmap[i].ObjCurrent.X].blockcolor)); //Sets the color of the object and removes the background clash with the player
			}
			if (Objmap[i].ObjLoc == loc) {
				switch (Objmap[i].ObjType) {
				case 1: //Sign
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { Objmap[i].ObjCurrent.X, Objmap[i].ObjCurrent.Y });
					cout << "*";
					break;
				case 3: //TP
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { Objmap[i].ObjCurrent.X, Objmap[i].ObjCurrent.Y });
					cout << " ";
					RenderBlock(Objmap[i].ObjCurrent.X, Objmap[i].ObjCurrent.Y, loc);	
					break;
				case 4: //Item
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { Objmap[i].ObjCurrent.X, Objmap[i].ObjCurrent.Y });
					cout << '\xF8';
					break;
				case 404:
					break;
				default:
					if (DebugError == 1) {
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, errors }); //Goes to Console
						SetConsoleTextAttribute(hstdout, 0x4C); //Sets the color of the Error
						cout << "Unrecognized Objmap type: " << Objmap[i].ObjType << " at X: " << Objmap[i].ObjCurrent.X << " and Y: " << Objmap[i].ObjCurrent.Y << endl;
						SetConsoleTextAttribute(hstdout, 0x0F); //Sets the color of the person.
						errors++;
					}
					break;
				}
			}
		}
		else {
			TempPointer.push_back(i);
		}
	}
	for (size_t i = 0; i < TempPointer.size(); i++) { //Draws the AI Objects so they are infront of the rest
		SetConsoleTextAttribute(hstdout, 0x0F); //Sets the color of the object.
		if (!(finalmap[loc][Objmap[TempPointer[i]].ObjCurrent.Y][Objmap[TempPointer[i]].ObjCurrent.X].blockcolor < 15)) { //Checks to see if there is a background color
			SetConsoleTextAttribute(hstdout, RemoveBackground(finalmap[loc][Objmap[TempPointer[i]].ObjCurrent.Y][Objmap[TempPointer[i]].ObjCurrent.X].blockcolor)); //Sets the color of the object and removes the background clash with the player
		}
		if (Objmap[TempPointer[i]].ObjLoc == loc) { //To only render on the current location
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { Objmap[TempPointer[i]].ObjCurrent.X, Objmap[TempPointer[i]].ObjCurrent.Y });
			cout << "@";
			}
		Objmap[TempPointer[i]].ObjOld.X = Objmap[TempPointer[i]].ObjCurrent.X;
		Objmap[TempPointer[i]].ObjOld.Y = Objmap[TempPointer[i]].ObjCurrent.Y;
	}
}
void RenderMap() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 }); //Resets the cursor
	for (int y = 0; y < 15; y++) { //Rows
		for (int x = 0; x < 60; x++) { //Columns
			RenderBlock(x, y, loc);
		}
		cout << endl;
	}
}
void RenderBlock(int x, int y, int loc) {
	int blockdata = finalmap[loc][y][x].blocktype; //Sets the block type for the current block
	SetConsoleTextAttribute(hstdout, finalmap[loc][y][x].blockcolor); //Sets the block color for the current block
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { (short)x, (short)y });
	switch (blockdata) { //For drawing the block
	case 1:
		cout << "#";
		break;
	case -2:
		cout << "+";
		break;
	case 0:
	case 3:
		cout << " ";
		break;
	}
}
void DrawPlayer(int color, int nPlayerY, int nPlayerX,int loc,char Output) {
	SetConsoleTextAttribute(hstdout, color); //Sets the color of the person.
if (!(finalmap[loc][nPlayerY][nPlayerX].blockcolor < color)) { //Checks to see if there is a background color
	SetConsoleTextAttribute(hstdout, RemoveBackground(finalmap[loc][nPlayerY][nPlayerX].blockcolor)); //Sets the color of the person and removes the background clash with the player
}
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),{ (short)nPlayerX, (short)nPlayerY });
cout << Output;
}