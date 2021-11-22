#include "Action.h"
#include <iostream>
#include "Trainer.h"
#include "Studio.h"
#include <vector>

using namespace std;
extern Studio* backup;
BaseAction::BaseAction(){

}
ActionStatus BaseAction::getStatus() const{
    return status;
}
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}
void BaseAction::error(std::string errorMsg){
    status=ERROR;
    this->errorMsg=errorMsg;
    cout<<"ERROR: "<<errorMsg<<endl;
}
void BaseAction::complete(){
    status=COMPLETED;
}
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id),customers(customersList){
}

void OpenTrainer::act(Studio &studio){
    if(trainerId>studio.getNumOfTrainers()-1 or trainerId<0)
    {
        error("Workout session does not exist or is already open");
    }
    else if (studio.getTrainer(trainerId)->isOpen()){
        error("Workout session does not exist or is already open");
    }
    else{
        studio.getTrainer(trainerId)->openTrainer();
        for(int i=0;i<customers.size();i++){
            studio.getTrainer(trainerId)->addCustomer(customers[i]);
        }
    }
    complete();
};
std::string OpenTrainer::toString() const{//changes by ziv
    std::string logString = inputAction;
    if(getStatus() == ERROR){
        logString = logString +" Error: " + getErrorMsg();
    }
    else{
        logString = logString + " Completed";
    }
    return logString;
}

OpenTrainer *OpenTrainer::clone() {
    return new OpenTrainer(*this);
}
Order::Order(int id):trainerId(id){//add the salary

}
void Order::act(Studio &studio){
    std::vector<Customer *> tempCustomersList=studio.getTrainer(trainerId)->getCustomers();
    for(int i=0;i<tempCustomersList.size();i++) {
        studio.getTrainer(trainerId)->order(tempCustomersList[i]->getId(),
                                                tempCustomersList[i]->order(studio.getWorkoutOptions()),
                                                studio.getWorkoutOptions());
    }
    complete();

}

std::string Order::toString() const{
    std::string logString = inputAction;
    if(getStatus() == ERROR){
        logString = logString +" Error: " ;//+ getErrorMsg();
    }
    else{
        logString = logString + " Completed";
    }
    return logString;
}
Order *Order::clone(){
    return new Order(*this);
}


MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTrainer(src),dstTrainer(dst),id(customerId)
{
};

void MoveCustomer::act(Studio &studio) {
    if (srcTrainer >= studio.getNumOfTrainers() or srcTrainer < 0 or dstTrainer >= studio.getNumOfTrainers() or
        dstTrainer < 0) {
        error("Cannot move customer");
    } else {
        Trainer &srcTrainerObj = *studio.getTrainer(srcTrainer);
        Trainer &dstTrainerObj = *studio.getTrainer(dstTrainer);
        if (dstTrainerObj.getCapacity() == 0 or srcTrainerObj.getCustomer(id) == nullptr or
            not(srcTrainerObj.isOpen()) or not(dstTrainerObj.isOpen())) {
            error("Cannot move customer");
        } else {
            Customer &customerToMove = *srcTrainerObj.getCustomer(id);
            dstTrainerObj.addCustomer(&customerToMove);
           // if (customerToMove.getIsOrder()) {
                //srcTrainerObj.removeOrder(true, id);
                //dstTrainerObj.order(id, customerToMove.order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
            //}
            cout << "remove customer from src";
            srcTrainerObj.removeCustomer(id);
            complete();
            if (srcTrainerObj.getCustomers().empty()) {
                srcTrainerObj.closeTrainer();
            }
            complete();
        }

    }
}
    std::string MoveCustomer::toString() const { // fix when we know how we want to do actionslog print
        std::string logString = inputAction;
        if (getStatus() == ERROR) {
            logString = logString + " Error: ";// + getErrorMsg();
        } else {
            logString = logString + " Completed";
        }
        return logString;
    }
    MoveCustomer *MoveCustomer::clone(){
        return new MoveCustomer(*this);
}


    Close::Close(int id):trainerId(id)
    {
    }

    void Close::act(Studio &studio) {//check if it works
        Trainer &trainerToClose = *studio.getTrainer(trainerId);
        trainerToClose.closeTrainer();
        //trainerToClose.removeOrder(false, trainerId);
        cout << "Trainer " << trainerId << " closed." << " Salary " << trainerToClose.getSalary() << "NIS" << endl;
        for (int i = 0; i < trainerToClose.getCustomers().size(); i++) {
            trainerToClose.removeCustomer(trainerToClose.getCustomers()[i]->getId());
        }
        complete();
    }
    std::string Close::toString() const {
        std::string logString = inputAction;
        if (getStatus() == ERROR) {
            logString = logString + " Error: ";//+ getErrorMsg();
        } else {
            logString = logString + " Completed";
        }
        return logString;
    }
    Close *Close::clone(){
        return new Close(*this);
}
    CloseAll::CloseAll()
    {
    }
    void CloseAll::act(Studio &studio) {//need to be fixed
        for (int i = 0; i < studio.getTrainers().size(); i++) {
            Close close = Close(i);
        }
    }
std::string CloseAll::toString() const {
    std::string logString = inputAction;
    if (getStatus() == ERROR) {
        logString = logString + " Error: "+ getErrorMsg();
    } else {
        logString = logString + " Completed";
    }
    return logString;
}
CloseAll *CloseAll::clone(){
    return new CloseAll(*this);
}


    PrintActionsLog::PrintActionsLog()
    {
    }
    void PrintActionsLog::act(Studio &studio) {
        for (BaseAction *action: studio.getActionsLog()) {
            cout << action->toString() << endl;
            complete();
        }
    }
    std::string PrintActionsLog::toString() const {
        std::string logString = inputAction;
        if (getStatus() == ERROR) {
            logString = logString + " Error: "+ getErrorMsg();
        } else {
            logString = logString + " Completed";
        }
        return logString;
    }
    PrintActionsLog *PrintActionsLog::clone(){
        return new PrintActionsLog(*this);
}

PrintWorkoutOptions::PrintWorkoutOptions(){//added by nir
}
void PrintWorkoutOptions::act(Studio &studio){ //added by nir
    std::vector<Workout> &tempWorkOutList=studio.getWorkoutOptions();
    string type;
    for(int i=0;i<tempWorkOutList.size();i++){
        if(tempWorkOutList[i].getType()==0){
            type="ANAEROBIC";
        }
        else if(tempWorkOutList[i].getType()==1){
            type="MIXED";}
        else{type="CARDIO";
        }
        cout<<tempWorkOutList[i].getName()<<", "<<type<<", "<<tempWorkOutList[i].getPrice()<<endl;
    }
    complete();
}
PrintWorkoutOptions *PrintWorkoutOptions::clone(){
    return new PrintWorkoutOptions(*this);
}
std::string PrintWorkoutOptions::toString() const{//added by nir
}
PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id){//added by nir
}
void PrintTrainerStatus::act(Studio &studio){//added by nir
    Trainer &tempTrainer = *studio.getTrainer(trainerId);
    std::vector<Customer *> &tempCustomersList=studio.getTrainer(trainerId)->getCustomers();
    std::vector<OrderPair> &tempOrdersList=studio.getTrainer(trainerId)->getOrders();
    string status;
    if(tempTrainer.isOpen()){
        cout<<"Trainer "<<trainerId<<" status: "<<"open"<<endl;
        cout<<"customers:"<<endl;
        for(int i=0;i<tempCustomersList.size();i++){
            cout<<tempCustomersList[i]->getId()<<" "<<tempCustomersList[i]->getName()<<endl;
        }
        cout<<"Orders:"<<endl;
        for(int j=0;j<tempOrdersList.size();j++){
            int customerId=tempCustomersList[tempOrdersList[j].first]->getId();
            cout<<tempOrdersList[j].second.getName()<<" "<<tempOrdersList[j].second.getPrice()<<"NIS "<<customerId<<endl;
        }
    }
    else{
        cout<<"Trainer "<<trainerId<<" status: "<<"closed"<<endl;
    }
}

std::string PrintTrainerStatus::toString() const{
    std::string logString = inputAction;
    if (getStatus() == ERROR) {
        logString = logString + " Error: "+ getErrorMsg();
    } else {
        logString = logString + " Completed";
    }
    return logString;
}

PrintTrainerStatus *PrintTrainerStatus:: clone(){
    return new PrintTrainerStatus(*this);
}
BackupStudio::BackupStudio(){
}
void BackupStudio::act(Studio &studio){
    if(backup!= nullptr){//check how to delete
        delete(backup);
    }
    backup = new Studio(studio);
    complete();
}
std::string BackupStudio::toString() const {
    std::string logString = inputAction;
    if (getStatus() == ERROR) {
        logString = logString + " Error: "+ getErrorMsg();
    } else {
        logString = logString + " Completed";
    }
    return logString;
}
BackupStudio *BackupStudio:: clone() {
    return new BackupStudio(*this);
}
RestoreStudio::RestoreStudio(){
}
void RestoreStudio::act(Studio &studio){
    if(backup == nullptr){
        error("No backup available");
    }
    if(backup != nullptr){
        delete(backup);
    }
    studio =*backup;
    complete();
};
std::string RestoreStudio::toString() const {
    std::string logString = inputAction;
    if (getStatus() == ERROR) {
        logString = logString + " Error: "+ getErrorMsg();
    } else {
        logString = logString + " Completed";
    }
    return logString;
}

RestoreStudio *RestoreStudio::clone() {

}