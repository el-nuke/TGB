#include <iostream>
#include <list>
#include <string>
#include <string.h>
#include <vector>
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

class graph {
    public:
        list<city> cities;

        //Adds a city
        void addCity(const string& cityName) {
            cities.emplace_back(cityName);
        }

        //Connects two existing cities
        void connectCities(city* city1, city* city2) {
            city1->addNeighbour(city2);
            city2->addNeighbour(city1);
        }

        //Prints every city and its neighbors
        void printCityAndNeighbours() {
        for (const auto& city : cities) {
            cout << city.name << " is connected to: ";
            for (const auto& neighbour : city.neighborCities) {
                cout << neighbour->name << " ";
            }
            cout << endl;
        }
    }
};

class guardian{
    private:
        string name;
        int powerlvl;
        city *home; //points to their home city
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
        
        guardian(string inname, int inpwr){
            name = inname;
            powerlvl = inpwr;
            master = nullptr;
        };
        
        guardian(string inName, int inPwr, guardian *inMaster, city *inHome){
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

    city testCity = city("TIS WORKING YO");
    
    guardian masterguardian = guardian("MASTER WIZARD", 100, nullptr, &testCity);
    cout << "test guardian created" << endl;
    //guardian apprenticeguardian = guardian("random dude", 96, &masterguardian, &testCity);
    //cout << "2nd guardian created" << endl;

    
    FILE *file = fopen("cities.conf", "r");
    if (file == NULL) {
        perror("\nCouldnt find 'cities.conf' \n");
        return 1;
    }else{        
        cout << "\nFound 'cities.conf' to load cities from!" << endl;
        
        char line[100];        
        while (fgets(line, 200, file) != NULL)
        {
            char *token;
            token = strtok(line, ", ");
            
            string city1 = token;
            bool found1 = false;
            

            string city2 = strtok(NULL, ",");
            bool found2 = false;
            

            //Check through the list if either city already exists
            for (const auto& city : theGuardianWorld.cities) {
                if(city.name == city1){found1 = true;}
                if(city.name == city2){found2 = true;}
            }

            //If they dont exist, create and add that city
            if (!found1){theGuardianWorld.addCity(city1);}
            if (!found2){theGuardianWorld.addCity(city2);}

            //check through the list once again to get pointers for easier access later
            const city* cityPointer = nullptr;
            const city* cityPointer2 = nullptr;
            for (const auto& city : theGuardianWorld.cities) {
                if(city.name == city1){cityPointer = &city;}
                if(city.name == city2){cityPointer2 = &city;}
            }

            //check neighbouring cities of city for a connection to city2
            bool foundconnection = false;
            for (const auto& city : cityPointer->neighborCities) {
                if (city->name == city2){foundconnection = true;}
            }
            if (!foundconnection){
                theGuardianWorld.connectCities(const_cast<city*>(cityPointer), const_cast<city*>(cityPointer2));;
            }
        }
        fclose(file);
    }


    file = fopen("guardians.conf", "r");
    if (file == NULL) {
        perror("\nCouldnt find 'guardians.conf' \n");
        return 1;
    }else{        
        cout << "\nFound 'guardians.conf' to load guardians from!" << endl;
        
        char line[100];        
        while (fgets(line, 200, file) != NULL)
        {
            char *token;
            token = strtok(line, ",");
            
            string name;
            name = token;

            int power = atoi(strtok(NULL, ","));
        
            string master = strtok(NULL, ",");

            string city = strtok(NULL, ",");

            allGuardians.push_back(guardian(name, power, &masterguardian, &testCity));
        }
        fclose(file);
    }

    bool loop=true;
    do{
        string strinput;

        cout << "\nenter 1 to leave" << endl;
        cout << "enter 2 to print test guardians' data" << endl;
        cin >> strinput;
        int input = stoi(strinput);
        

        switch (input)
        {
        case 1:
            cout << "loop is now false k bye" << endl;
            loop = false;
            break;

        case 2:
            masterguardian.printData();
            break;
        
        default:
            cout << "invalid option, get looped" << endl;
            break;
        }
        
    }while(loop);

    return 0;
}