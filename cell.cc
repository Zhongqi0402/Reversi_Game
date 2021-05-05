#include "cell.h"

//constructor for cell class
Cell::Cell(size_t r, size_t c) : r{r}, c{c} {}

//Place a piece of a given color 
void Cell::setPiece(Colour colour) {
    this->colour = colour;
    State s{StateType::NewPiece, colour, Direction::N};
    this->setState(s);
}

// Toggles my colour.
void Cell::toggle() {
    if(this->colour == Colour::Black) {
        this->colour = Colour::White;
    }
    if(this->colour == Colour::White) {
        this->colour = Colour::Black;
    }
} 

Direction findDir(const Subject<Info,State>& s, const Cell& o) {
    Direction d;
    if(o.getInfo().row == s.getInfo().row - 1 && o.getInfo().col == s.getInfo().col - 1) {
        d = Direction::NW;
    }
    if(o.getInfo().row == s.getInfo().row - 1 && o.getInfo().col == s.getInfo().col + 1) {
        d = Direction::NE;
    }
    if(o.getInfo().row == s.getInfo().row + 1 && o.getInfo().col == s.getInfo().col - 1) {
        d = Direction::SW;
    }
    if(o.getInfo().row == s.getInfo().row + 1 && o.getInfo().col == s.getInfo().col + 1) {
        d = Direction::SE;
    }
    if(o.getInfo().row == s.getInfo().row - 1 && o.getInfo().col == s.getInfo().col) {
        d = Direction::N;
    }
    if(o.getInfo().row == s.getInfo().row + 1 && o.getInfo().col == s.getInfo().col) {
        d = Direction::S;
    }
    if(o.getInfo().row == s.getInfo().row && o.getInfo().col == s.getInfo().col - 1) {
        d = Direction::W;
    }
    if(o.getInfo().row == s.getInfo().row && o.getInfo().col == s.getInfo().col + 1) {
        d = Direction::E;
    }
    return d;
}

Direction reverseDir(const Subject<Info,State>& s, const Cell& o) {
    Direction normal = findDir(s, o);
    if(normal == Direction::N) { return Direction::S; }
    if(normal == Direction::S) { return Direction::N; }
    if(normal == Direction::W) { return Direction::E; }
    if(normal == Direction::E) { return Direction::W; }
    if(normal == Direction::NE) { return Direction::SW; }
    if(normal == Direction::NW) { return Direction::SE; }
    if(normal == Direction::SW) { return Direction::NE; }
    if(normal == Direction::SE) { return Direction::NW; }
}


void Cell::notify(Subject<Info, State> &whoFrom) {
    //if whoFrom is a new piece
    if(whoFrom.getState().type == StateType::NewPiece) {
        Direction d1 = findDir(whoFrom, *this);
        if(this->colour == Colour::NoColour) {
            return;
        }
        //if this cell's(current observer's) colour is not current Subject's colour
        if( this->colour != whoFrom.getState().colour ) {
            State s {StateType::Relay, whoFrom.getState().colour, d1}; //define a state as relay, subject's colour, derection n
            this->setState(s);                                        //set current observer state
            this->notifyObservers();
        }
        //if this cell's (current observer's) colour is the same as Subject's colour
        if(this->colour == whoFrom.getState().colour) {
            //find reverse direction
            Direction correct = reverseDir(whoFrom, *this);
            State p {StateType::Reply, whoFrom.getState().colour, correct}; //change this observer's state
            this->setState(p);
            this->notifyObservers();
        } 
    }
    //if whoFrom is a Relay
    if(whoFrom.getState().type == StateType::Relay) {
        Direction d2 = findDir(whoFrom, *this);
        if(d2 == whoFrom.getState().direction) {
            if(this->colour == Colour::NoColour) {
                return;
            }
            if(this->colour != whoFrom.getState().colour) {
                State s2 {StateType::Relay, whoFrom.getState().colour, d2};
                this->setState(s2);
                this->notifyObservers();
            } else {
                Direction d22 = reverseDir(whoFrom, *this);
                State s22 {StateType::Reply, whoFrom.getState().colour, d22};
                this->setState(s22);
                this->notifyObservers();
            }
            
        }
    }
    if(whoFrom.getState().type == StateType::Reply) {
        Direction d3 = findDir(whoFrom, *this);
        if(d3 == whoFrom.getState().direction) {
            if(this->colour == Colour::NoColour) {
                return;
            }
            if(this->colour != whoFrom.getInfo().colour) {
                this->colour = whoFrom.getInfo().colour;
                State s3 {StateType::Reply, whoFrom.getState().colour,d3};
                this->setState(s3);
                this->notifyObservers();
            } else {
                return;
            }
        }  
    } 
}// My neighbours will call this when they've changed state

//return the infotype of this cell
Info Cell::getInfo() const {
    Info info{this->r, this->c, colour};
    return info;
}