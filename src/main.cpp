#include "../include/Studio.h"
#include <iostream>
#include <fstream>
#include "../include/Action.h"
#include "../include/Trainer.h"




using namespace std;


Studio *backup=nullptr;
int main(int argc, char** argv){
   if(argc!=2){
       std::cout << "usage: studio <config_path>" << std::endl;
       return 0;
   }
    backup=nullptr;
    string configurationFile = argv[1];
    Studio studio(configurationFile);
    studio.start();
    if(backup!=nullptr){
   	    delete backup;
   	    backup = nullptr;
   }

    return 0;
}   