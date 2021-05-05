#include "grid.h"
#include "textdisplay.h"


void Grid::init(size_t n) {
    if(td) {
        delete td;
        for(int i = 0; i < theGrid.size(); i += 1) {
            theGrid.at(i).clear();
        }
        theGrid.clear();
    }
    td = new TextDisplay{int(n)};
    std::vector<Cell> row;
    for(size_t i = 0; i < n; i += 1) {       //go through each row
        for(size_t j = 0; j < n; j += 1) {   //go through each column
            row.emplace_back(Cell{i,j});
        }
        theGrid.push_back(row);
        row.clear();
    }
    
    int length = theGrid.size();
    for(int i = 0; i < length; i += 1) {
        for(int j = 0; j < length; j += 1) {
            Cell& cell = this->theGrid.at(i).at(j);
            if(i-1 >= 0 && i-1 < length && j >= 0 && j < length) { //cell above it(North of curretn cell)
                cell.attach(&(theGrid[i-1][j]));
            }

            if(i+1 >= 0 && i+1 < length && j >= 0 && j < length) { //cell below(South of current cell)
                cell.attach(&(theGrid[i+1][j]));
            }

            if(i >= 0 && i < length && j-1 >= 0 && j-1 < length) { //cell on the left(West of current cell)
                cell.attach(&(theGrid[i][j-1]));
            }

            if(i >= 0 && i < length && j+1 >= 0 && j+1 < length) { //cell on the right(East of current cell)
                cell.attach(&(theGrid[i][j+1]));
            }

            if(i-1 >= 0 && i-1 < length && j-1 >= 0 && j-1 < length) { //North West of current cell
                cell.attach(&(theGrid[i-1][j-1]));
            }

            if(i-1 >= 0 && i-1 < length && j+1 >= 0 && j+1 < length) { //North East of current cell
                cell.attach(&(theGrid[i-1][j+1]));
            }

            if(i+1 >= 0 && i+1 < length && j-1 >= 0 && j-1 < length) { //South West of current cell
                cell.attach(&(theGrid[i+1][j-1]));
            }

            if(i+1 >= 0 && i+1 < length && j+1 >= 0 && j+1 < length) { //South East of current cell
                cell.attach(&(theGrid[i+1][j+1]));
            }
            cell.attach(td);
        }
    }
} // Sets up an n x n grid.  Clears old grid, if necessary.


//destructor for grid object
Grid::~Grid() {
    delete td;
    delete ob;
}

bool Grid::isFull() const {
    unsigned int length = this->theGrid.size();  //check the size of this grid
    unsigned int pieceCount = 0;                 //set a counter to count cell that is either black or white
    for(int i = 0; i < length; i += 1) {         //go through each row
        for(int j = 0; j < length; j += 1) {     //go through each column
            Colour currColour = (this->theGrid[i][j]).getInfo().colour;      //get the colour of current cell
            if(currColour == Colour::Black || currColour == Colour::White) { //if black or white, pieceCount plus 1
                pieceCount += 1;
            }
        }
    }
    return pieceCount == (length * length);    //check if pieceCount is equal to total # of cells in grid
} // Is the game over, i.e., is the grid full?



Colour Grid::whoWon() const {
    unsigned int length = this->theGrid.size();
    unsigned int wCount = 0;
    unsigned int bCount = 0;
    for(int i = 0; i < length; i += 1) {
        for(int j = 0; j < length; j += 1) {
            Colour c = (this->theGrid[i][j]).getInfo().colour;
            if(c == Colour::Black) {
                bCount += 1;
            }
            if(c == Colour::White) {
                wCount += 1;
            }
        }
    }
    if(bCount > wCount) { return Colour::Black; }
    if(wCount > bCount) { return Colour::White; }
    if(wCount == bCount) { return Colour::NoColour; }
}// Who has more pieces when the board is full?


void Grid::setPiece(size_t r, size_t c, Colour colour) {
    if( r < this->theGrid.size() && c < this->theGrid.size() ) {
        if (this->theGrid[r][c].getInfo().colour == Colour::NoColour) {
            (this->theGrid[r][c]).setPiece(colour);
            this->theGrid[r][c].notifyObservers();
        } else {
            throw InvalidMove{};
        }
    } else {
        throw InvalidMove{};
    }
    
}// Plays piece at row r, col c.


void Grid::toggle(size_t r, size_t c) {
    (this->theGrid[r][c]).toggle();
}// Flips piece at row r, col c.


std::ostream &operator<<(std::ostream &out, const Grid &g) {
    return operator<<(out, *(g.td));
}


void Grid::setObserver(Observer<Info, State> *ob) {
    this->ob = ob;
}