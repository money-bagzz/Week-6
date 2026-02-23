#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <sstream>

using namespace std;

class level {
private:
    string levelName;
    vector<vector<char>> layout;
    int keys = 0;
    
    int HEALTH = 1000;
    int ATTACK = 100;
    int DEFENSE = 100;
    
public:
    level(string name) {
        levelName = name;
        fstream file(name + ".txt");
        
        string line;
        while(getline(file, line)) {
            vector<char> row;
            
            for(char val : line) {
                row.push_back(val);
            }
            layout.push_back(row);
        }
    }
    
    string getName() {
        return levelName;
    }
    
    int getHealth() {
        return HEALTH;
    }
    
    int getAttack() {
        return ATTACK;
    }
    
    int getDefense() {
        return DEFENSE;
    }
    
    //Using pair here to grab the hero position easily.
    pair<int, int> getHeroPosition() {
        int row = 0, col = 0;
        for(int r = 0; r < layout.size(); r++) {
            for(int c = 0; c < layout.at(r).size(); c++)
                if (layout[r].at(c) == 'H') {
                    row = r;
                    col = c;
                }
        }
        return {row, col};
    }
    
    pair<int, int> getMovementDirection(char direction) {
        int horizMovement = 0, vertMovement = 0;
        
        switch (direction) {
            case 'L': horizMovement = -1; break;
            case 'R': horizMovement = 1; break;
            case 'D': vertMovement = 1; break;
            case 'U': vertMovement = -1; break;
        }
        
        return {horizMovement, vertMovement};
    }
    
    bool checkValid(char direction) {
        auto [horizMovement, vertMovement] = getMovementDirection(direction);
        auto [row, col] = getHeroPosition();
        
        int newRow = row + vertMovement;
        int newCol = col + horizMovement;
        
        if(newRow < 0 || newRow >= layout.size()) {
            return false;
        }
        
        if(newCol < 0 || newCol >= layout[newRow].size()) {
            return false;
        }
        
        if(layout[newRow][newCol] == '#' || (layout[newRow][newCol] == 'X' && keys == 0)) {
            return false;
        }
        
        return true;
    }
    
    bool movePlayer(char direction) {
        auto [horizMovement, vertMovement] = getMovementDirection(direction);
        auto [row, col] = getHeroPosition();
        
        int newRow = row + vertMovement;
        int newCol = col + horizMovement;
        
        switch (layout[newRow][newCol]) {
            case 'K':
                keys++;
                cout << "You got a key!" << endl;
                break;
            case 'X':
                cout << "Door unlocked!" << endl;
                keys--;
                break;
            case 'E':
                cout << "An Enemy Approaches!" << endl;
                break;
            case '@':
                HEALTH += 10;
                cout << "Extra Health!" << endl;
                break;
            case '%':
                DEFENSE += 5;
                cout << "Defense Boost!" << endl;
                break;
            case '$':
                ATTACK += 5;
                cout << "Attack Buff!" << endl;
                break;
            case 'G':
                cout << "Reached the goal!" << endl;
                return true;
        }
        layout[newRow][newCol] = 'H';
        layout[row][col] = ' ';
        return false;
    }
    
    void printLayout() {
        for(int r = 0; r < layout.size(); r++) {
            for(int c = 0; c < layout.at(r).size(); c++)
                cout << layout.at(r).at(c);
            cout << endl;
        }
    }
};

class createLevel {
private:
    string levelName;
    vector<vector<char>> layout;
public:
    createLevel(string name, int rows, int cols) {
        levelName = name;
        layout = vector<vector<char>>(rows, vector<char>(cols, ' '));
    }
    
    vector<vector<char>> getLayout() {
        return layout;
    }
    
    void printCurrentLayout() {
        int rows = layout.size();
        int cols = layout[0].size();
        
        cout << "  ";
        for(int c = 0; c < cols; c++) {
            cout << c + 1 << ' ';
        }
        cout << endl;
        
        for(int r = 0; r < rows; r++) {
            cout << r + 1 << ' ';
            for(int c = 0; c < cols; c++) {
                cout << layout[r][c] << ' ';
            }
            cout << endl;
        }
        
        cout << endl;
    }
    
    void changeLayout(int row, int col, char obj) {
        layout[row][col] = obj;
    }
};

int getInput(int min, int max) {
    int input;
    
    do {
        if (cin.fail()) {
            cout << "Invalid input! Please try again." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter your selection: ";
        cin >> input;
    } while(cin.fail() || !(input <= max && input >= min));
    
    return input;
}

void main_menu() {
    cout << "Welcome to the Dungeon of Doom!" << endl << endl;
    
    cout << "1) Enter a dungeon" << endl;
    cout << "2) Design a dungeon" << endl;
    cout << "3) Exit" << endl << endl;
}

void gameState(level& curLvl) {
    string validInput = "L R U D";
    while(true) {
        cout << endl;
        
        curLvl.printLayout();
        cout << endl;
        
        cout << "[ " << curLvl.getName() << " ]" << endl << endl;
        cout << "HEALTH: " << curLvl.getHealth() << endl;
        cout << "ATTACK: " << curLvl.getAttack() << endl;
        cout << "DEFENSE: " << curLvl.getDefense() << endl << endl;
        
        string input;
        char direction;
        
        cout << "Enter a direction:" << endl
             << "[Left/L, Right/R, Up/U, Down/D] ";
        
        do {
            cin >> input;
            direction = static_cast<char>(toupper(input[0]));
            cout << endl;
            
            if(!curLvl.checkValid(direction)) {
                cout << "You cannot move there!" << endl;
                continue;
            }
            if(validInput.find(direction) == string::npos) {
                cout << "Please enter a valid input! ";
                continue;
            }
            break;
        } while(true);

        if (curLvl.movePlayer(direction)) {
            curLvl.printLayout();
            cout << endl << "You Win! Congratulations!" << endl << endl;
            break;
        }
    }
}

void level_select(vector<level>& levelList) {
    cout << endl << "Enter a level to play using the corresponding number:" << endl << endl;
    
    for(int i = 0; i < levelList.size(); i++) {
        cout << i + 1 << ") " << levelList.at(i).getName() << endl;
    }
    cout << endl;
    
    int input = getInput(1, levelList.size());
    gameState(levelList.at(input - 1));
}

vector<char> getObjects() {
    return {'X', 'E', 'K', 'H', 'G', '%', '$', '@', '#', ' '};
}

bool checkValidObject(int idx) {
    vector<char> validObjs = getObjects();
    return idx >= 1 && idx <= validObjs.size();
}

void addLevel(string name) {
    
}

string level_editor() {
    string levelName;
    cout << "Please enter a name for your level: ";
    cin >> levelName;
    
    cout << "What are the dimensions of your level? (separated by spaces): ";
    int rows = 0, cols = 0;
    do {
        if (cin.fail()) {
            cout << "Invalid input! Please try again. ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin >> rows >> cols;
    } while(cin.fail() || (rows <= 0 || cols <= 0));
    
    cout << endl;
    
    createLevel level = createLevel(levelName, rows, cols);
    level.printCurrentLayout();
    
    do {
        string input;
        cout << "Would you like to [S] Save your level OR [A] Add objects to your current level?: ";
        cin >> input;
        
        cout << endl;
        
        if(static_cast<char>(toupper(input[0])) == 'A') {
            int idx = 1;
            int obj;
            for(char c: getObjects()) {
                cout << "[" << idx << ": " << c << "] ";
                idx++;
            }
            cout << endl;
            cout << "Enter the index of the object you want to use: ";
            do {
                cin >> obj;
            } while(!checkValidObject(obj));
            
            cout << "Great! You have selected " << getObjects().at(obj - 1) << "." << endl;
            cout << "Where on the grid would you like to place this object? ";
            
            int plcRow, plcCol;
            do {
                cin >> plcRow >> plcCol;
            } while((plcRow < 1 || plcRow > rows) || (plcCol < 1 || plcCol > cols));
            
            level.changeLayout(plcRow - 1, plcCol - 1, getObjects().at(obj - 1));
        } else if(static_cast<char>(toupper(input[0])) == 'S') {
            ofstream oFile(levelName + ".txt");
            if (oFile.is_open()) {
                
                for(const auto& r : level.getLayout()) {
                    for(char c : r) {
                        oFile << c;
                    }
                    oFile << '\n';
                }
                
                oFile.close();
                cout << "Successfully saved level!";
            }
            break;
        } else {
            cout << "Invalid input! Please try again.";
        }
        
        cout << endl;
        level.printCurrentLayout();
    } while(true);
    return levelName;
}

int main() {
    //Default saved levels:
    level example1 = level("Example1");
    level example2 = level("Example2");
    vector<level> levels = {example1, example2};
    
    while(true) {
        main_menu();
        
        int input = getInput(1, 3);
        switch (input) {
            case 1:
                level_select(levels);
                break;
            case 2: {
                string newLevel = level_editor();
                
                level addLvl = level(newLevel);
                levels.push_back(addLvl);
                break;
            }
            case 3:
                cout << endl << "Thank you for playing!" << endl;
                return 0;
        }
    }
}

