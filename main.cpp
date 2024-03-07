#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "mazeStack.h"

using namespace std;

//Ada Canoglu

struct cell{ //Cell structure containing data about the walls of the cell and if the cell has been visited. Used inside the 2D vector data structure.

    int x;
    int y;

    bool l;
    bool r;
    bool u;
    bool d;

    bool unvisited;

    cell(){ // Default constructor

        x = 0;
        y = 0;

        l = true;
        r = true;
        u = true;
        d = true;

        unvisited = true;

    }

};

struct simpleCell{ //Cell structure only containing the x-y axis of the cell. used in stacks for maze and path generation.

    int x;
    int y;

    simpleCell(){ // Default constructor
        x = 0;
        y = 0;
    }

    simpleCell(int var1, int var2) {
        x = var1;
        y = var2;
    }

};

void inputRead(string mazeSize, int & M, int & N){ //Function for reading the input from the user. Reads input in a form seperated by a single space.

    string stringM;
    string stringN;
    bool fillN = false;

    for (int i = 0; i < mazeSize.size(); i++) {

        if(fillN)
            stringN += mazeSize[i];
        else
            stringM += mazeSize[i];
        if(mazeSize[i] == ' ')
            fillN = true;
    }

    M = stoi(stringM);
    N = stoi(stringN);

}

vector<vector<cell>> mazeInfoConstruct(int M, int N){ //Function for creating the 2D matrix data structure for the maze info to be stored in.
                                                      //All values of the cells are set to default (all cells have walls and are unvisited).
    vector<vector<cell>> mazeInfo;
    cell curr;

    for (int y = 0; y < M; y++) {
        vector<cell> row;
        for (int x = 0; x < N; x++) {

            curr.x = x;
            curr.y = y;

            row.push_back(curr);

        }
        mazeInfo.push_back(row);
    }

    return mazeInfo;
}

string emptyNeighbors(vector<vector<cell>> & mazeInfo, simpleCell cell, char cameFrom, int M, int N){ //Function for checking the unvisited neighbors of a cell.
                                                                                                      //Accesses 2D vector info to check for unvisited status.
    string output;                                                                                    //Takes to consideration where the cell came from and if next to a wall.
                                                                                                      //Used only by the maze generation function.
    int x = cell.x;
    int y = cell.y;

    if(cameFrom != 'u' && y != (M-1) && mazeInfo[y+1][x].unvisited) // up
        output += 'u';
    if(cameFrom != 'd' && y != 0 && mazeInfo[y-1][x].unvisited) // down
        output += 'd';
    if(cameFrom != 'r' && x != (N-1) && mazeInfo[y][x+1].unvisited) // right
        output += 'r';
    if(cameFrom != 'l' && x != 0 && mazeInfo[y][x-1].unvisited) // left
        output += 'l';

    return output; //Returns a simple string containing characters representing the direction of the unvisited neighbors.
}

void pathChose(vector<vector<cell>> & mazeInfo, mazeStack<simpleCell> & stack , char chosenPath, char & cameFrom, simpleCell & curr, bool generation){
    //Function for pushing cells into stack and updating the data about visit and wall information after a path has been chosen.
    //Used both by the maze and path generation functions.

    if (chosenPath == 'u') { // up
        cameFrom = 'd';
        if(generation) {
            mazeInfo[curr.y][curr.x].u = false;
            mazeInfo[curr.y + 1][curr.x].d = false;
        }
        curr.y += 1;
        stack.push(curr);
        mazeInfo[curr.y][curr.x].unvisited = false;
    }
    else if (chosenPath == 'd') { // down
        cameFrom = 'u';
        if(generation) {
            mazeInfo[curr.y][curr.x].d = false;
            mazeInfo[curr.y - 1][curr.x].u = false;
        }
        curr.y -= 1;
        stack.push(curr);
        mazeInfo[curr.y][curr.x].unvisited = false;
    }
    else if (chosenPath == 'r') { // right
        cameFrom = 'l';
        if(generation) {
            mazeInfo[curr.y][curr.x].r = false;
            mazeInfo[curr.y][curr.x + 1].l = false;
        }
        curr.x += 1;
        stack.push(curr);
        mazeInfo[curr.y][curr.x].unvisited = false;
    }
    else if (chosenPath == 'l') { // left
        cameFrom = 'r';
        if(generation) {
            mazeInfo[curr.y][curr.x].l = false;
            mazeInfo[curr.y][curr.x - 1].r = false;
        }
        curr.x -= 1;
        stack.push(curr);
        mazeInfo[curr.y][curr.x].unvisited = false;
    }
}

void mazeGeneration(vector<vector<cell>> & mazeInfo, int M, int N, int seed){
    //Function for generating a random maze using a stack.

    simpleCell curr;
    mazeStack<simpleCell> stack;
    char cameFrom = 'd';
    string emptyCells;
    int choice;
    char chosenPath;

    stack.push(curr);   //Pushes 0,0 (starting cell) into the stack.
    mazeInfo[curr.y][curr.x].unvisited = false;

    //Seed for the random path choosing.
    srand(time(0)+ seed);

    while(!stack.isEmpty()){ //Loops until stack is empty which happens when all cells are visited and stack backtracks to (0,0)

        emptyCells = emptyNeighbors(mazeInfo, curr, cameFrom, M, N); //Check for the unvisited neighbors of the cell.
        if(!emptyCells.empty()) {

            choice = rand() % size(emptyCells); //Chooses a random number between 0 and given amount empty cells.

            chosenPath = emptyCells[choice]; //Uses the random number to choose the next cell.

            pathChose(mazeInfo, stack, chosenPath, cameFrom, curr, true); //Moving to the chosen path.

        }
        else {  //If no possible path exists, backtracks through the stack.
            cameFrom = ' ';
            stack.pop();
            if(!stack.isEmpty()){
                curr.x = stack.top().x;
                curr.y = stack.top().y;
            }
        }
    }
}

void mazeVisitReset(vector<vector<cell>> & mazeInfo, int M, int N){ //Resets the unvisited status of the cells inside the 2D vector.
                                                                    //Used after generating the maze so path generation can be done.
    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            mazeInfo[y][x].unvisited = true;
        }
    }
}

string possiblePath(vector<vector<cell>> & mazeInfo, simpleCell cell , char cameFrom, int M, int N){ //Different version of the emptyNeighbors function.
                                                                                                     //Used for path generation only.
    string output;                                                                                   //Needs to check if a wall exists between cells for a cell to accessible.
                                                                                                     //Is a different function because of the different if statements.
    int x = cell.x;
    int y = cell.y;

    if(cameFrom != 'u' && y != (M-1) && !mazeInfo[y+1][x].d && mazeInfo[y+1][x].unvisited) // up
        output += 'u';
    if(cameFrom != 'd' && y != 0 && !mazeInfo[y-1][x].u && mazeInfo[y-1][x].unvisited) // down
        output += 'd';
    if(cameFrom != 'r' && x != (N-1) && !mazeInfo[y][x+1].l && mazeInfo[y][x+1].unvisited) // right
        output += 'r';
    if(cameFrom != 'l' && x != 0 && !mazeInfo[y][x-1].r && mazeInfo[y][x-1].unvisited) // left
        output += 'l';

    return output;  //Same output style of the emptyNeighbors function.
}

mazeStack<simpleCell> mazePathFinder(vector<vector<cell>> & mazeInfo, simpleCell entryCell, simpleCell exitCell, int M, int N, int seed, int & stackSize) {
    //Function for the path generation using a stack.

    simpleCell curr = entryCell;
    mazeStack<simpleCell> stack;
    char cameFrom = ' ';
    string emptyCells;
    int choice;
    char chosenPath;

    mazeVisitReset(mazeInfo, M, N); //Resets the visit status of the cell back to unvisited

    stack.push(curr);   //Pushes the starting cell into the stack.
    mazeInfo[curr.y][curr.x].unvisited = false;
    stackSize++;

    //Seed for the random path choosing.
    srand(time(0) + seed);

    while (!(curr.x == exitCell.x && curr.y == exitCell.y)) {   //Loops until the exit cell inputted by the user is reached.

        emptyCells = possiblePath(mazeInfo, curr, cameFrom, M, N); //Check for the unvisited neighbors of the cell.
        if (!emptyCells.empty()) {

            choice = rand() % size(emptyCells); //Chooses a random number between 0 and given amount empty cells.

            chosenPath = emptyCells[choice]; //Uses the random number to choose the next cell.

            pathChose(mazeInfo, stack, chosenPath, cameFrom, curr, false); //Moving to the chosen path.
            stackSize++;

        }
        else { //If no possible path exists, backtracks through the stack.
            cameFrom = ' ';
            stack.pop();
            stackSize--;
            if (!stack.isEmpty()) {
                curr.x = stack.top().x;
                curr.y = stack.top().y;
            }
        }
    }

    return stack;
}

void printMazeInfo(vector<vector<cell>> & mazeInfo, string & fileName, int M, int N){ //Function for printing the maze information to a file.

    cell curr{};

    ofstream file;
    file.open (fileName);

    file << M << " " << N << endl; //Prints maze size into the first line.

    for (int y = 0; y < M - 1; y++) { //Loops through the 2D vector maze information.
        for (int x = 0; x < N; x++) {

            curr = mazeInfo[y][x];
            file << "x=" << x << " y=" << y << " l=" << curr.l << " r=" << curr.r << " u=" << curr.u << " d=" << curr.d << endl;

        }
    }

    for (int x = 0; x < N-1; x++) { //The following lines are printed differently so that there would not be an empty line at the end of the file.

        curr = mazeInfo[M-1][x];
        file << "x=" << x << " y=" << M-1 << " l=" << curr.l << " r=" << curr.r << " u=" << curr.u << " d=" << curr.d << endl;

    }

    curr = mazeInfo[M-1][N-1];
    file << "x=" << N-1 << " y=" << M-1 << " l=" << curr.l << " r=" << curr.r << " u=" << curr.u << " d=" << curr.d;

    file.close();
}

void stackToArray(mazeStack<simpleCell> & stack, int stackSize, ofstream &file){
    //For output format to be correct stack needs to be printed in a reverse order
    //I have tried this with a recursive print function but this results with a stack overflow in larger mazes.
    //Stack info is transferred to an array so that it can be printed in a reverse order.
    //Array memory is allocated in the heap, because otherwise stack overflow occurs in larger mazes.

    simpleCell curr;
    simpleCell * array = new simpleCell[stackSize];
    int count = stackSize-1;

    while(!stack.isEmpty()){ //Stack information transferred to an array in reverse.
        curr = stack.topAndPop();
        array[count] = curr;
        count--;
    }

    for (int i = 0; i < stackSize-1; i++) { //Array is printed to the file.

        file << array[i].x << " " << array[i].y << endl;

    }

    file << array[stackSize-1].x << " " << array[stackSize-1].y; //This is so that file would not have an empty line at the end.

    delete[] array;
}

void printPath(mazeStack<simpleCell> & stack, simpleCell entryCell, simpleCell exitCell, string & ID, int stackSize){
    //Function for printing the path information to a file.

    string fileName = "maze_" + ID + "_path_" + to_string(entryCell.x) + "_" + to_string(entryCell.y) + "_" + to_string(exitCell.x) + "_" + to_string(exitCell.y) + ".txt";

    ofstream file;
    file.open (fileName);

    stackToArray(stack, stackSize, file); //Converts stack into an array in reverse order and then writes to the file.

    file.close();
}

int main() {

    vector<vector<cell>> mazeInfo;
    vector<vector<vector<cell>>> mazes;
    string mazeSize;
    string smazeAmount;
    int mazeAmount;
    int M;
    int N;

    cout << "Enter the number of mazes: ";
    getline(cin, smazeAmount);
    cout << "Enter the number of rows and columns (M and N): ";
    getline(cin, mazeSize);

    inputRead(mazeSize, M, N);

    mazeAmount = stoi(smazeAmount);
    string fileName;

    for (int i = 0; i < mazeAmount; i++) {

        mazeInfo = mazeInfoConstruct(M, N); //Maze vector created. All cells have full walls.
        mazeGeneration(mazeInfo, M, N, i); //Creates random mazes. "i" is transferred as an increment to the seed so that each maze would be different.
        mazes.push_back(mazeInfo); //Vector that keeps all the mazes.

        fileName = "maze_" + to_string(i+1) + ".txt";

        printMazeInfo(mazeInfo,fileName, M, N); //Important note: files for the maze information are made here before asking for the path.
    }

    cout << "All mazes are generated." << endl;

    string sID;
    int ID;

    cout << "Enter a maze ID between 1 to " << mazeAmount << " inclusive to find a path: ";
    getline(cin, sID);

    ID = stoi(sID);

    int entryX;
    int entryY;
    string entryPoint;

    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    getline(cin, entryPoint);

    inputRead(entryPoint, entryX, entryY);

    int exitX;
    int exitY;
    string exitPoint;

    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    getline(cin, exitPoint);

    inputRead(exitPoint, exitX, exitY);

    simpleCell entryCell;
    entryCell.x = entryX;
    entryCell.y = entryY;

    simpleCell exitCell;
    exitCell.x = exitX;
    exitCell.y = exitY;

    int stackSize = 0;

    mazeStack<simpleCell> path;
    path = mazePathFinder(mazes[ID-1], entryCell, exitCell, M, N, ID + 5, stackSize); //Path generation.
    printPath(path, entryCell, exitCell, sID, stackSize); //Prints path information to a file.

    return 0;
}