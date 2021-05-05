#include "textdisplay.h"
#include "subject.h"
TextDisplay::TextDisplay(int n) : gridSize{n} {
    const char init = '-';
    std::vector<char> row;
    for(int i = 0; i < n; i += 1) {
        for (int j = 0; j < n; j += 1) {
            row.emplace_back(init);
        }
        theDisplay.push_back(row);
        row.clear();
    }
}

std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    for(int i = 0; i < td.gridSize; i += 1) {
        for(int j = 0; j < td.gridSize; j += 1) {
            out << td.theDisplay.at(i).at(j);
        }
        out << std::endl;
    }
    return out;
}

void TextDisplay::notify(Subject<Info, State> &whoNotified) {
    if(whoNotified.getInfo().colour == Colour::Black) {
        this->theDisplay.at(whoNotified.getInfo().row).at(whoNotified.getInfo().col) = 'B';
    } 
    if(whoNotified.getInfo().colour == Colour::White) {
        this->theDisplay.at(whoNotified.getInfo().row).at(whoNotified.getInfo().col) = 'W';
    }
}
