#include <iostream>
#include <cstdlib>

using namespace std;

bool check(string, char);
bool check1(string, string);
string update(string, string, char);
string chooseWord();

const string FIGURE[] = {
    " ------------- \n"
    " |             \n"
    " | \n"
    " | \n"
    " | \n"
    " | \n"
    " ----- \n",
    " ------------- \n"
    " |           | \n"
    " | \n"
    " | \n"
    " | \n"
    " | \n"
    " ----- \n",
    " ------------- \n"
    " |           | \n"
    " |           O \n"
    " | \n"
    " | \n"
    " | \n"
    " ----- \n",
    " ------------- \n"
    " |           | \n"
    " |           O \n"
    " |           | \n"
    " | \n"
    " | \n"
    " ----- \n",
    " ------------- \n"
    " |           | \n"
    " |           O \n"
    " |          /| \n"
    " | \n"
    " | \n"
    " ----- \n",
    " ------------- \n"
    " |           | \n"
    " |           O \n"
    " |          /|\\ \n"
    " | \n"
    " | \n"
    " ----- \n",
    " ------------- \n"
    " |           | \n"
    " |           O \n"
    " |          /|\\ \n"
    " |          / \n"
    " | \n"
    " ----- \n",
    " ------------- \n"
    " |           | \n"
    " |           O \n"
    " |          /|\\ \n"
    " |          / \\ \n"
    " | \n"
    " ----- \n",
};

string word_list []= {"angle", "ant", "apple", "arch", "arm", "army",
"baby", "bag", "ball", "band", "basin", "basket", "bath", "bed", "bee", "bell", "berry",
"bird", "blade", "board", "boat", "bone", "book", "boot", "bottle", "box", "boy",
"brain", "brake", "branch", "brick", "bridge", "brush", "bucket", "bulb", "button",
"cake", "camera", "card", "cart", "carriage", "cat", "chain", "cheese", "chest",
"chin", "church", "circle", "clock", "cloud", "coat", "collar", "comb", "cord",
"cow", "cup", "curtain", "cushion",
"dog", "door", "drain", "drawer", "dress", "drop", "ear", "egg", "engine", "eye",
"face", "farm", "feather", "finger", "fish", "flag", "floor", "fly",
"foot", "fork", "fowl", "frame", "garden", "girl", "glove", "goat", "gun",
"hair", "hammer", "hand", "hat", "head", "heart", "hook", "horn", "horse",
"hospital", "house", "island", "jewel", "kettle", "key", "knee", "knife", "knot",
"leaf", "leg", "library", "line", "lip", "lock",
"map", "match", "monkey", "moon", "mouth", "muscle",
"nail", "neck", "needle", "nerve", "net", "nose", "nut",
"office", "orange", "oven", "parcel", "pen", "pencil", "picture", "pig", "pin",
"pipe", "plane", "plate", "plow", "pocket", "pot", "potato", "prison", "pump",
"rail", "rat", "receipt", "ring", "rod", "roof", "root",
"sail", "school", "scissors", "screw", "seed", "sheep", "shelf", "ship", "shirt",
"shoe", "skin", "skirt", "snake", "sock", "spade", "sponge", "spoon", "spring",
"square", "stamp", "star", "station", "stem", "stick", "stocking", "stomach",
"store", "street", "sun", "table", "tail", "thread", "throat", "thumb", "ticket",
"toe", "tongue", "tooth", "town", "train", "tray", "tree", "trousers", "umbrella",
"wall", "watch", "wheel", "whip", "whistle", "window", "wire", "wing", "worm"};

int main(){
    char guess;
    string process = "", secret = chooseWord();
    int dem = 0; 
    const int total = secret.length(), figurelength = 7;
    for (int i = secret.length(); i >= 1; --i){
        process += "-";
    }
    while (dem != total && check1(secret,process) == false){
        cout << FIGURE[dem + (figurelength - secret.length())] << endl << "type your guess: " << endl ;
        cin >> guess;
        if (check(secret,guess)){
            process = update(secret,process,guess);
            cout << FIGURE[dem + (figurelength - secret.length())] << endl << "nice! continue your work we ain't done yet" << endl << process << endl ;
        }
        else {
            ++dem;
            cout << FIGURE[dem + (figurelength - secret.length())] << endl << "wrong guess!" << endl << process << endl ;          
        }
    }
    if (check1(secret,process) == true && dem < total){
        cout << FIGURE[dem + (figurelength - secret.length())] << endl << "Wow! You won! The word is: " << secret << endl ; 
    }
    else cout << FIGURE[dem + (figurelength - secret.length())] << endl << "Oops! looks like we got a loser! The word is: " << secret << endl ;
    return 0;
}

bool check1(string secret, string process){
    if(secret == process) return true;
    else return false;
}

bool check(string secret, char guess){
    if (secret.find(guess) != string :: npos) return true;
    else return false;
}

string update(string secret, string process, char guess){
    for (int i = secret.length() - 1; i >= 0; --i){
        if (secret[i] == guess){
            process[i] = guess;
        }
    }
    return process;
}

string chooseWord (){
    srand(time(0));
    int arraysize = sizeof(word_list) / sizeof(word_list[0]);
    int loca = rand() % arraysize;
    return word_list[loca];
}


