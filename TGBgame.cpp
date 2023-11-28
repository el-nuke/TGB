#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

class guardian{
    private:
        string name;
        int powerlvl;
        guardian * master; //if null, is master.

    public:
        string getName(){return name;}
        void setName(string newName){name = newName;}

        int getPwrLv(){return powerlvl;}
        void setPwrLv(int newLvl){powerlvl = newLvl;}

        guardian* getMaster(){return master;}
        void setMaster(guardian newMaster){*master = newMaster;}

        void printData(){
            cout << "Name: " << name << endl;
            cout << "Power lvl: " << powerlvl << endl;
            cout << "Master: ";
            if (master != nullptr){cout << master->getName() << endl;}
            else {cout << "No master" << endl;}
        }
        
        guardian(string inname, int inpwr){
            name = inname;
            powerlvl = inpwr;
            master = nullptr;
        };
        
        guardian(string inname, int inpwr, guardian* inmaster){
            name = inname;
            powerlvl = inpwr;
            master = inmaster;
        };      
};

class city{
    list<guardian> CitysGuardians;
};

int main(){
    
    guardian defaultmasterguardian = guardian("SHADOW WIZARD", 100);
    cout << "1st guardian created" << endl;
    guardian apprenticeguardian = guardian("", 96, &defaultmasterguardian);
    cout << "2nd guardian created" << endl;

    bool loop=true;
    do{
        string strinput;

        cout << "enter 1 to leave" << endl;
        cout << "enter 2 to print guardians' data" << endl;
        cin >> strinput;
        int input = stoi(strinput);
        

        switch (input)
        {
        case 1:
            cout << "loop is now false k bye" << endl;
            loop = false;
            break;

        case 2:
            defaultmasterguardian.printData();
            cout<<"---"<<endl;
            apprenticeguardian.printData();

            break;
        
        default:
            cout << "invalid option, get looped" << endl;
            break;
        }
        
    }while(loop);

    return 0;
}