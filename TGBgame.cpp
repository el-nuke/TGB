#include <iostream>
#include <list>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

class city{
    public:
        string name;
        list<city*> neighborCities;

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
        void printCityAndNeighbours() {
        for (const auto& city : cities) {
            cout << city->name << " is connected to: ";
            for (const auto& neighbour : city->neighborCities) {
                cout << neighbour->name << " - ";
            }
            cout << endl;
        }
    }
};

class guardian{
    private:
        string name;
        int powerlvl;
        city *home; //points to their current city
        list <guardian> apprentices; //stores pointers to its apprentices
        guardian *master; //points to its master

    public:
        string getName(){return name;}
        void setName(string newName){name = newName;}

        int getPwrLv(){return powerlvl;}
        void setPwrLv(int newLvl){powerlvl = newLvl;}

        guardian* getMaster(){return master;}
        void setMaster(guardian newMaster){*master = newMaster;}

        city* getCity(){return home;}
        void setCity(city newCity){*home = newCity;}

        void printData(){
            cout << "Name: " << name << endl;
            cout << "Power lvl: " << powerlvl << endl;
            cout << "Master: ";
            if (master != nullptr){cout << master->getName() << endl;}
            else {cout << "No master" << endl;}
            cout << "City: " << home->getName() << endl;
        }
        
        guardian(string inName, int inPwr, guardian* inMaster, city* inHome){
            name = inName;
            powerlvl = inPwr;
            master = inMaster;
            home = inHome;
        };

        ~guardian(){
            delete master;
        }
};

int main(){
    graph theGuardianWorld;
    list<guardian> allGuardians;
    
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

        if (getline(iss, city1, ',') && getline(iss >> ws, city2)) {
            city2.erase(find_if(city2.rbegin(), city2.rend(), [](unsigned char ch) {
                return !isspace(ch);
            }).base(), city2.end());    


            //Check through the list if either city already exists
            for (const auto& city : theGuardianWorld.cities) {
                if(city->name == city1){found1 = true;}
                if(city->name == city2){found2 = true;}
            }

            //If they dont exist, create and add that city
            if (!found1){theGuardianWorld.addCity(city1);}
            if (!found2){theGuardianWorld.addCity(city2);}

            //check through the list once again to get pointers for easier access later
            const city* cityPointer = nullptr;
            const city* cityPointer2 = nullptr;
            for (const auto& city : theGuardianWorld.cities) {
                if(city->name == city1){cityPointer = city;}
                if(city->name == city2){cityPointer2 = city;}
            }

            //check neighbouring cities of city for a connection to city2
            bool foundconnection = false;
            for (const auto& city : cityPointer->neighborCities) {
                if (city->name == city2){foundconnection = true;}
            }
            //If they weren't connected, now they are
            if (!foundconnection){
                theGuardianWorld.connectCities(const_cast<city*>(cityPointer), const_cast<city*>(cityPointer2));
            }
        }
    }
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
        int powerlvl=0;

        if (getline(iss, rname, ',') && getline(iss, rlvl, ',') && getline(iss, rmaster, ',') && getline(iss, rcity)) {
            
            cout << "READING.----" << endl;
            cout << "name: " << rname << endl;
            cout << "power level: " << rlvl << endl;
            cout << "master: " << rmaster << endl;
            cout << "city: " << rcity << endl;


            for (const auto& city : theGuardianWorld.cities) {
                if(city->name == rcity){gCityPointer = city;}
            }
            if (gCityPointer == nullptr){
                cout << "Couldnt find " << rname << "'s city (" << rcity << ")" << endl;
            }

            powerlvl = stoi(rlvl);
            if (powerlvl >= 100){
                powerlvl = 100;
                if (lvl100){
                    cout << "Tried to load " << rname << ", but there's another power 100 guardian already! \n This guardian's power has been halved." << endl;
                    powerlvl = powerlvl/2;
                }else{
                    lvl100  = true;
                }
            }
            if (powerlvl >= 90 && powerlvl <=99){
                if (lvl90s >= 3){
                    cout << "Tried to load " << rname << ", but there are another 3 power 90 guardians already! \n This guardian's power has been halved." << endl;
                    powerlvl = powerlvl/2;
                }
                lvl90s++;
            }

            guardian g = guardian(rname, powerlvl, nullptr, gCityPointer);
            allGuardians.push_back(g);
        }
    }
    guardiansfile.close();

    

    bool loop=true;
    do{
        string strinput;

        cout << "\nenter 9 to leave" << endl;
        cout << "enter 1 to print all cities" << endl;
        cout << "enter 2 to print a city by name" << endl;
        cout << "enter 3 to print all guardians" << endl;
        cout << "enter 4 to print a guardian by name" << endl;
        cout << "------ STILL A WIP ------" << endl;
        cin >> strinput;
        int input = stoi(strinput);
        

        switch (input)
        {
        case 9:
            cout << "loop is now false k bye" << endl;
            loop = false;
            break;

        case 1:
            theGuardianWorld.printCityAndNeighbours();
            break;
        
        default:
            cout << "invalid or unfinished option, get looped" << endl;
            break;
        }
        
    }while(loop);

    return 0;
}