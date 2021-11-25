#include "Action.h"
#include <iostream>
#include "Trainer.h"
#include "Studio.h"
#include <vector>

using namespace std;
extern Studio* backup;
BaseAction::BaseAction():errorMsg(),status(){

}
BaseAction::~BaseAction(){

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
OpenTrainer::~OpenTrainer(){
//    for(Customer* customer: customers){
//        delete customer;
//    }
    customers.clear();

}

void OpenTrainer::act(Studio &studio){
    int numOfTrainers=studio.getNumOfTrainers()-1;
    bool isOpen=studio.getTrainer(trainerId)->isOpen();
    if(trainerId>numOfTrainers or trainerId<0)
    {
        error("Workout session does not exist or is already open");
    }
    else if (isOpen){
        error("Workout session does not exist or is already open");
    }
    else{
        studio.getTrainer(trainerId)->openTrainer();
        for(int i=0;i<(int)customers.size();i++){
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
Order::Order(int id):trainerId(id){

}
Order::~Order(){

}
void Order::act(Studio &studio){
    std::vector<Customer *> &tempCustomersList=studio.getTrainer(trainerId)->getCustomers();
    for(int i=0;i<(int)tempCustomersList.size();i++) {
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
MoveCustomer::~MoveCustomer(){
}

void MoveCustomer::act(Studio &studio) {
    int numOfTrainers=studio.getNumOfTrainers();
    if (srcTrainer >= numOfTrainers or srcTrainer < 0 or dstTrainer >= numOfTrainers or
        dstTrainer < 0) {
        error("Cannot move customer");
    } else {
        Trainer *srcTrainerObj = studio.getTrainer(srcTrainer);
        Trainer *dstTrainerObj = studio.getTrainer(dstTrainer);
        int capacityCurrent = dstTrainerObj->getCapacity();
        if (capacityCurrent== 0 or srcTrainerObj->getCustomer(id) == nullptr or
            not(srcTrainerObj->isOpen()) or not(dstTrainerObj->isOpen())) {
            error("Cannot move customer");
        } else {
            Customer* customerToMove = srcTrainerObj->getCustomer(id)->clone();
            dstTrainerObj->addCustomer(customerToMove);
            if (customerToMove->getIsOrder()) {
                srcTrainerObj->removeOrder(true, id);
                dstTrainerObj->order(id, customerToMove->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
            }
            srcTrainerObj->removeCustomer(id);
            complete();
            if (srcTrainerObj->getCustomers().empty()) {
                srcTrainerObj->closeTrainer();
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
Close::~Close(){

}
void Close::act(Studio &studio) {//check if it works
    Trainer* trainerToClose = studio.getTrainer(trainerId);
    cout << "Trainer " << trainerId << " closed." << " Salary " << trainerToClose->getSalary() << "NIS" << endl;
    trainerToClose->closeTrainer();
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
CloseAll::~CloseAll(){

}
void CloseAll::act(Studio &studio) {//need to be fixed
    for (int i = 0; i < (int)studio.getTrainers().size(); i++) {
        Close close = Close(i);
        close.act(studio);
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
PrintActionsLog::~PrintActionsLog()
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
PrintWorkoutOptions::~PrintWorkoutOptions(){

}
void PrintWorkoutOptions::act(Studio &studio){ //added by nir
    std::vector<Workout> &tempWorkOutList=studio.getWorkoutOptions();
    string type;
    for(int i=0;i<(int)tempWorkOutList.size();i++){
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
    return "";
}
PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id){//added by nir
}
PrintTrainerStatus::~PrintTrainerStatus(){

}

void PrintTrainerStatus::act(Studio &studio){//added by nir
    Trainer *tempTrainer = studio.getTrainer(trainerId);
    std::vector<Customer *> tempCustomersList=studio.getTrainer(trainerId)->getCustomers();
    std::vector<OrderPair> tempOrdersList=studio.getTrainer(trainerId)->getOrders();
    string status;
    if(tempTrainer->isOpen()){
        cout<<"Trainer "<<trainerId<<" status: "<<"open"<<endl;
        cout<<"customers:"<<endl;
        for(int i=0;i<(int)tempCustomersList.size();i++){
            cout<<tempCustomersList[i]->getId()<<" "<<tempCustomersList[i]->getName()<<endl;
        }
        cout<<"Orders:"<<endl;
        for(int j=0;j<(int)tempOrdersList.size();j++){
            cout<<tempOrdersList[j].second.getName()<<" "<<tempOrdersList[j].second.getPrice()<<"NIS "<<tempOrdersList[j].first<<endl;
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
BackupStudio::~BackupStudio(){
}
void BackupStudio::act(Studio &studio){
    delete(backup);
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
RestoreStudio::~RestoreStudio(){
}
void RestoreStudio::act(Studio &studio){
    if(backup == nullptr){
        error("No backup available");
    }
//    if(backup != nullptr){
//        delete(backup);
//    }
    else {
        studio = *backup;
        complete();
    }
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
    return new RestoreStudio(*this);
}