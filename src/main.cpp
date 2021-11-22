#include "Studio.h"
#include <iostream>
#include "Trainer.h"
using namespace std;

Studio* backup = nullptr;

int main(int argc, char** argv){//argv = array of strings that save information
//    if(argc!=2){
//        std::cout << "usage: studio <config_path>" << std::endl;
//        return 0;
//    }
    string configurationFile = "/Users/zivseker/CLionProjects/trial/example.txt";
   // string configurationFile = argv[1];
    Studio studio(configurationFile);
    studio.start();

    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}