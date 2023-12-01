#include <iostream>
#include <list>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

class city{
    private:
        string name;
        list<city*> neighborCities;

    public:
        city(string inName){
            name = inName;
        }

        string getName(){return name;}
        void setName(string inName){name = inName;}

        void addNeighbour(city* nb) {
            neighborCities.push_back(nb);
        }

        const list<city*>& getNeighbours() const {
        return neighborCities;
    }
};

class graph { //Not really sure if this counts as a graph, but it works, so whatever.
    public:
        list<city*> cities;

        //Adds a city
        void addCity(string cityName) {
            cities.push_back(new city(cityName));
        }

        //Connects two existing cities
        void connectCities(city* city1, city* city2) {
            city1->addNeighbour(city2);
            city2->addNeighbour(city1);
        }

        //Prints every city and its neighbors
        void printAllCitiesAndNeighbours() {
            for (const auto& city : cities) {
                cout << city->getName() << " is connected to: ";
                for (const auto& neighbour : city->getNeighbours()) {
                    cout << neighbour->getName() << " - ";
                }
                cout << endl;
            }
        }

        void PrintASingleCityAndItsNeighbours(string search) {
            bool found=false;
            for (const auto& city : cities) {
                if (city->getName() == search){
                    found=true;
                    cout << city->getName() << " is connected to: ";
                    for (const auto& neighbour : city->getNeighbours()) {
                        cout << neighbour->getName() << " - ";
                    }
                }
                cout << endl;
                if (!found){
                    cout << "Couldn't find '" << search <<"' (check for upper cases and try again)" << endl;
                }
                return;
            }
        }
};


class guardian{
    private:
        string name;
        int powerlvl;
        guardian *master; //points to its master
        city *home; //points to their current city
        list <guardian*> apprentices; //stores pointers to its apprentices
        
    public:
        //Placeholder guardian
        guardian(string inName, int inPwr){
            name = inName;
            powerlvl = inPwr;
            master = nullptr;
            home = nullptr;
        };


        //Full guardian
        guardian(string inName, int inPwr, guardian* inMaster, city* inHome){
            name = inName;
            powerlvl = inPwr;
            master = inMaster;
            home = inHome;
        };

        string getName(){return name;}
        void setName(string newName){name = newName;}

        int getPwrLv(){return powerlvl;}
        void setPwrLv(int newLvl){powerlvl = newLvl;}

        guardian* getMaster(){return master;}
        void setMaster(guardian newMaster){*master = newMaster;}

        city* getCity(){return home;}
        void setCity(city newCity){*home = newCity;}

        const list<guardian*>& getApprentices() const {return apprentices;}

        void printData(){
            cout << "Name: " << name << endl;
            cout << "Power lvl: " << powerlvl << endl;
            cout << "Master: ";
            if (master != nullptr){cout << master->getName() << endl;}
            else {cout << "No master" << endl;}
            cout << "City: " << home->getName() << endl;
        }
};

struct biTreeNode
{
    guardian* data = nullptr;
    biTreeNode* left = nullptr;
    biTreeNode* right = nullptr;

    biTreeNode(){
        data = nullptr;
        left = nullptr;
        right = nullptr;
    }

    biTreeNode(guardian* g){
        data = g;
        left = nullptr;
        right = nullptr;
    }
};

biTreeNode* createBiTreeNode(guardian* g){
    biTreeNode* newNode = new biTreeNode(g);
    newNode->data = g;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

biTreeNode* insertBiTreeNode(biTreeNode* root, guardian* g){
    if (root == NULL){
        return new biTreeNode(g);
    }

    if (g->getPwrLv() < root->data->getPwrLv()){
        root->left = insertBiTreeNode(root->left, g);
    }else{
        root->right = insertBiTreeNode(root->right, g);
    }

    return root;
}

biTreeNode* createRoot(string gname, int gpower) {
    biTreeNode* root = new biTreeNode;
    root->data = new guardian(gname, gpower);
    root->left = nullptr;
    root->right = nullptr;
    return root;
}

void printInOrder(biTreeNode* root) {
    if (root == nullptr) {
        return;
    }
    printInOrder(root->left);
    cout << "> [" << root->data->getName() << ": " << root->data->getPwrLv() << "] >" << endl;
    printInOrder(root->right);
}


int main(){
    
    graph theGuardianWorld;
    list<guardian*> allGuardians;

    int cityCount=0; int guardianCount=0; int linesRead=0;
    ifstream citiesfile("cities.conf");
    if (!citiesfile.is_open()) {
        cerr << "Couldn't find 'cities.conf'" << endl;
        return 1;
    }
    cout << "Found 'cities.conf' to load cities from!" << endl;
    string line;
    while (getline(citiesfile, line)) {
        istringstream iss(line);
        string city1, city2;
        bool found1 = false;
        bool found2 = false;

        if (getline(iss, city1, ',') && getline(iss >> ws, city2) && city1 != "City") {
            city2.erase(find_if(city2.rbegin(), city2.rend(), [](unsigned char ch) {
                return !isspace(ch);
            }).base(), city2.end());    


            //Check through the list if either city already exists
            for (const auto& city : theGuardianWorld.cities) {
                if(city->getName() == city1){found1 = true;}
                if(city->getName() == city2){found2 = true;}
            }

            //If they dont exist, create and add that city
            if (!found1){theGuardianWorld.addCity(city1);cityCount++;}
            if (!found2){theGuardianWorld.addCity(city2);cityCount++;}

            //check through the list once again to get pointers for easier access later
            const city* cityPointer = nullptr;
            const city* cityPointer2 = nullptr;
            for (const auto& city : theGuardianWorld.cities) {
                if(city->getName() == city1){cityPointer = city;}
                if(city->getName() == city2){cityPointer2 = city;}
            }

            //check neighbouring cities of city for a connection to city2
            bool foundconnection = false;
            for (const auto& city : cityPointer->getNeighbours()) {
                if (city->getName() == city2){foundconnection = true;}
            }
            //If they weren't connected, now they are
            if (!foundconnection){
                theGuardianWorld.connectCities(const_cast<city*>(cityPointer), const_cast<city*>(cityPointer2));
            }
            linesRead++;
        }
    }
    cout << "Loaded and connected " << cityCount << " cities after reading " << linesRead << " lines" << endl;
    citiesfile.close();


    ifstream guardiansfile("guardians.conf");
    if (!guardiansfile.is_open()) {
        cerr << "Couldn't find 'guardians.conf'" << endl;
        return 1;
    }
    cout << "Found 'guardians.conf' to load guardians from!" << endl;
    bool lvl100 = false;
    int lvl90s = 0;
    while (getline(guardiansfile, line)) {
        istringstream iss(line);
        string rname, rlvl, rmaster, rcity;
        city* gCityPointer = nullptr;
        guardian* gMasterPointer = nullptr;
        int powerlvl=0;

        if (getline(iss, rname, ',') && getline(iss, rlvl, ',') && getline(iss, rmaster, ',') && getline(iss, rcity) && rname != "Name") {
            
            /*
            cout << "DEBUGGING.----" << endl;
            cout << "name: " << rname << endl;
            cout << "power level: " << rlvl << endl;
            cout << "master: " << rmaster << endl;
            cout << "city: " << rcity << endl;
            */

            for (const auto& city : theGuardianWorld.cities) {
                if(city->getName() == rcity){gCityPointer = city;}
            }
            if (gCityPointer == nullptr){
                cout << "Couldnt find " << rname << "'s city (" << rcity << ")" << endl;
            }

            powerlvl = stoi(rlvl);
            if (powerlvl >= 100){
                powerlvl = 100;
                if (lvl100){
                    cout << "Tried to load " << rname << ", but there's another power 100 guardian already! \n This guardian's power has been reduced." << endl;
                    powerlvl = powerlvl - 11 - (rand()%10);
                }else{
                    lvl100  = true;
                }
            }
            if (powerlvl >= 90 && powerlvl <=99){
                if (lvl90s >= 3){
                    cout << "Tried to load " << rname << ", but there are another 3 power 90 guardians already! \n This guardian's power has been reduced." << endl;
                    powerlvl = powerlvl - 11 - (rand()%10);
                }
                lvl90s++;
            }

            for (const auto& guardian : allGuardians) {
                if(guardian->getName() == rmaster){gMasterPointer = guardian;}
            }
            if (gCityPointer == nullptr && lvl100 && powerlvl != 100){
                cout << "Couldnt find " << rname << "'s city (" << rcity << ")" << endl;
            }

            allGuardians.push_back(new guardian(rname, powerlvl, gMasterPointer, gCityPointer));
            guardianCount++;
        }
    }
    cout << "Loaded " << guardianCount << " guardians" << endl;
    guardiansfile.close();

    biTreeNode* ranksRoot = nullptr;
    ranksRoot = createRoot("Ranker", 10);
    for (const auto& guardian : allGuardians) {
        ranksRoot = insertBiTreeNode(ranksRoot, guardian);
    }

    bool loop=true;
    bool found=false;
    do{
        string strinput;

        cout << "\nenter 9 to leave" << endl;
        cout << "enter 1 to print all cities and their connections" << endl;
        cout << "enter 2 to check a city and its connections by name" << endl;
        cout << "enter 3 to print ranks from lowest to highest" << endl;
        cout << "enter 4 to print a guardian by name" << endl;
        cout << "-- sadly still a wip but everything works so far -- \ninput: ";
        cin >> strinput;
        int input = stoi(strinput);
        

        switch (input)
        {
        case 9:
            cout << "Closing program..." << endl;
            loop = false;
            break;

        case 1:
            theGuardianWorld.printAllCitiesAndNeighbours();
            break;

        case 2:
            cout << "Enter a city's name (CAse SenSiTivE) : ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, strinput);
            theGuardianWorld.PrintASingleCityAndItsNeighbours(strinput);
            break;

        case 3:
            printInOrder(ranksRoot);
            break;

        case 4:
            cout << "Enter a guardian's name (CAse SenSiTivE) : ";
            cin >> strinput;
            found=false;
            for (const auto& guardian : allGuardians) {
                if (guardian->getName() == strinput){
                    found=true;
                    guardian->printData();
                }
            }
            if (!found){
                cout << "Couldn't find '" << strinput <<"' (check for upper cases and try again)" << endl;
            }
            break;
        
        default:
            cout << "Invalid option" << endl;
            break;
        }
        
    }while(loop);

    return 0;
}