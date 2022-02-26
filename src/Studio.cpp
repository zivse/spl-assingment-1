#include <iostream>
#include "../include/Studio.h"
#include "string"
#include <fstream>
#include <vector>
#include <sstream>
#include "../include/Workout.h"
#include "../include/Action.h"
#include "../include/Customer.h"
using namespace std;
extern Studio* backup;
Studio::Studio():open(false),trainers(),workout_options(),actionsLog(),customerId(0){

}
//Copy Constructor
Studio::Studio(const Studio& other):open(other.open), trainers(), workout_options(), actionsLog(), customerId(other.customerId){// add by ziv
    for(Trainer *trainer: other.trainers){
        trainers.push_back(new Trainer(*trainer));
    }
    for(BaseAction *action : other.actionsLog){
        actionsLog.push_back(action ->clone());
    }
    for(Workout workout: other.workout_options){
        workout_options.push_back(Workout(workout.getId(),workout.getName(), workout.getPrice(), workout.getType()));
    }
}
//Move Constructor
Studio::Studio(Studio&& other):open(other.open), trainers(), workout_options(), actionsLog(), customerId(other.customerId){
    for(Trainer *trainer: other.trainers){
        trainers.push_back(trainer);
    }
    other.trainers.clear();
    for(BaseAction *action : other.actionsLog){
        actionsLog.push_back(action);
    }
    other.actionsLog.clear();
    for(Workout workout: other.workout_options){
        workout_options.push_back(workout);
        other.workout_options.pop_back();
    }
}
// Copy Assignment
Studio& Studio::operator=(const Studio &other){
    if(this == &other)
    {
        return *this;
    }
    else
    {
        for(Trainer* trainer:trainers)
        {
            delete trainer;
        }
        trainers.clear();
        for(Trainer *trainer: other.trainers)
        {
            trainers.push_back(new Trainer(*trainer));
        }
        for(BaseAction* action:actionsLog)
        {
            delete action;
        }
        actionsLog.clear();
        for(BaseAction *action : other.actionsLog)
        {
            actionsLog.push_back(action ->clone());
        }
        workout_options.clear();
        for(Workout workout: other.workout_options)
        {
            workout_options.push_back(Workout(workout.getId(),workout.getName(), workout.getPrice(), workout.getType()));//check
        }
    }
    return *this;
}
//Move Assignment
Studio& Studio::operator=(Studio &&other){
    open = other.open;
    for(int i=0; i< (int)trainers.size();i++){
        delete trainers[i];
    }
    trainers.clear();
    for(Trainer *trainer: other.trainers){
        trainers.push_back(trainer);
    }
    for(int i=0; i<(int)actionsLog.size(); i++){
        delete actionsLog[i];
    }
    actionsLog.clear();
    for(BaseAction *action : other.actionsLog){
        actionsLog.push_back(action);
    }
    for(int i=0; i<(int)other.workout_options.size(); i++){
        workout_options.push_back(other.workout_options[i]);
    }
    return *this;
}

Studio::~Studio(){
    for(Trainer *trainer: trainers){
        delete trainer;
    }
    trainers.clear();
    for(BaseAction *action: actionsLog){
        delete action;
    }
    actionsLog.clear();
}

const vector<BaseAction*>&Studio::getActionsLog() const{
    return actionsLog;
}
Studio::Studio(const std::string &configFilePath):open(true),trainers(),workout_options(),actionsLog(),customerId(0) {
    std::ifstream myfile(configFilePath);
    int index = 0;
    std::string line;
    int j = 0;
    if(myfile.is_open()){
        int counter = 0;
        while(getline(myfile, line)){
            if(line.empty()){
                continue;
            }
            if (line[0] == '#'){
                counter = counter+1;
            }
            else if (counter == 1 && line[0]!='#') {
                continue;
            }
            else if(counter == 2 && line[0]!='#') {
                int i = 0;
                std::string input = line;
                std::istringstream ss(input);
                std::string token;
                while(std::getline(ss,token, ',')) {
                    trainers.push_back(new Trainer(stoi(token)));
                    i++;
                }
            }
            else if(counter==3 && line[0]!='#'){
                std::string input = line;
                std::istringstream ss(input);
                std::string workoutName;
                std::string workoutTypeName;
                std::string workoutPrice;
                if(workoutTypeName[0] == ' '){
                    workoutTypeName.substr(1);
                }
                getline(ss, workoutName, ',');
                getline(ss, workoutTypeName, ',');
                getline(ss, workoutPrice, ',');
                //std::cout << workoutName << std:: endl;
                WorkoutType WorkoutTypeEnum;
                if(workoutTypeName.compare(" Anaerobic")==0){
                    WorkoutTypeEnum=WorkoutType(0);
                }
                else if(workoutTypeName.compare(" Mixed")==0){
                    WorkoutTypeEnum=WorkoutType(1);
                }
                else {
                    WorkoutTypeEnum=WorkoutType(2);
                }
                workout_options.push_back(Workout(index, workoutName,stoi(workoutPrice), WorkoutTypeEnum));
                j++;
                index++;
            }
        }
        myfile.close();
    }
};
int Studio::getNumOfTrainers() const {
    return trainers.size();
};
Trainer* Studio::getTrainer(int tid) {
    return trainers[tid];
};
std::vector<Workout>& Studio::getWorkoutOptions() {
    return workout_options;
};
vector<Trainer *> Studio::getTrainers() {
    return trainers;
}
void Studio::start(){
    cout << "Studio is now open!" << endl;
    open = true;
    string orders; //inpus from user
    string command; //value that contain the command from the input. for example: open/order..
    string customerName;
    string customerType;
    string trainerIdString;
    int trainerId;
    string input;//the input from the user
    BaseAction *action;

    do {
        cin >> orders;
        getline(cin, input);
        orders = orders + input;
        stringstream ss(orders);
        getline(ss, command, ' ');
        if (command.compare("closeall") == 0) {
            open = false;
            break;
        }
        if (command.compare("open") == 0) {
            std::vector<Customer *> customersList;
            getline(ss, trainerIdString, ' ');
            trainerId = stoi(trainerIdString);
            int capacity = trainers[trainerId]->getCapacity();
            //int tempCustomerId = customerId;
            while (getline(ss, customerName, ',') and capacity > 0) {
                getline(ss, customerType, ' ');
                if (customerType == "swt") {
                    SweatyCustomer *newSwtCustomer = new SweatyCustomer(customerName, customerId);
                    customersList.push_back(newSwtCustomer);
                } else if (customerType == "chp") {
                    CheapCustomer *newChpCustomer = new CheapCustomer(customerName, customerId);
                    customersList.push_back(newChpCustomer);
                } else if (customerType == "mcl") {
                    HeavyMuscleCustomer *newMclCustomer = new HeavyMuscleCustomer(customerName, customerId);
                    customersList.push_back(newMclCustomer);
                } else if (customerType == "fbd"){
                    FullBodyCustomer *newFbdCustomer = new FullBodyCustomer(customerName, customerId);
                    customersList.push_back(newFbdCustomer);
                }
                else {
                    cout<<"testing"<<endl;
                }
                capacity--;
                customerId++;
            }
            action = new OpenTrainer(trainerId, customersList);
            action->inputAction = orders;
            action->act(*this);
        } else if (command.compare("order") == 0) {
            getline(ss, trainerIdString, ' ');
            action = new Order(stoi(trainerIdString));
            action->inputAction = orders;
            action->act(*this);
        } else if (command.compare("move") == 0) {
            string srcTrainer;
            string dstTrainer;
            string customerId;
            getline(ss, srcTrainer, ' ');
            getline(ss, dstTrainer, ' ');
            getline(ss, customerId, ' ');
            action = new MoveCustomer(stoi(srcTrainer), stoi(dstTrainer), stoi(customerId));
            action->inputAction = orders;
            action->act(*this);
        } else if (command.compare("log") == 0) {
            action = new PrintActionsLog();
            action->inputAction = orders;
            action->act(*this);

        } else if (command.compare("workout_options") == 0) {
            action = new PrintWorkoutOptions();
            action->inputAction = orders;
            action->act(*this);
        } else if (command.compare("status") == 0) {
            getline(ss, trainerIdString, ' ');
            action = new PrintTrainerStatus(stoi(trainerIdString));
            action->inputAction = orders;
            action->act(*this);
        } else if (command.compare("close") == 0) {
            cout << "close" << endl;
            string trainerId;
            getline(ss, trainerId, ' ');
            action = new Close(stoi(trainerId));
            action->inputAction = orders;
            action->act(*this);
        }
        else if (command.compare("backup") == 0){
            action = new BackupStudio();
            action->inputAction = orders;
            action->act(*this);
        }
        else if (command.compare("restore") == 0){
            action = new RestoreStudio();
            action->inputAction = orders;
            action->act(*this);
        }
        actionsLog.push_back(action);
    }while (open);

    action=new CloseAll();
    action->inputAction = orders;
    action->act(*this);
    delete action;

};