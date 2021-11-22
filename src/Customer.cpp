#include "Customer.h"
#include <iostream>
#include <algorithm>
#include "Workout.h"
#include <functional>
using namespace std;
Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id),isOrder(false){
}

std::string Customer:: getName() const{
    return name;
}
int Customer:: getId() const{
    return id;
}
bool Customer::getIsOrder() const{
    return isOrder;
}
void Customer::setIsOrder() {
    isOrder = true;
}

Customer:: ~Customer(){

}
SweatyCustomer::SweatyCustomer(string _name, int _id):Customer(_name,_id)
{

}
SweatyCustomer::~SweatyCustomer(){

}
vector<int> SweatyCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> orderActivities;
    for (int i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == CARDIO) {
            orderActivities.push_back(workout_options[i].getId());
        }
    }
    return orderActivities;
};
std::string SweatyCustomer:: toString()const
{
    return "";
}
SweatyCustomer *SweatyCustomer:: clone(){
    return new SweatyCustomer(*this);
}

CheapCustomer::CheapCustomer(string _name, int _id):Customer(_name,_id)
{

}
CheapCustomer::~CheapCustomer(){

}
CheapCustomer *CheapCustomer:: clone(){
    return new CheapCustomer(*this);
}

vector<int> CheapCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> orderActivities;
    int minTypeId=workout_options[0].getId();
    int minPrice=workout_options[0].getPrice();
    for(int i=1;i<workout_options.size();i++){
        if(workout_options[i].getPrice() == minPrice and workout_options[i].getId() < minTypeId ){
            minTypeId = workout_options[i].getId();
        }
        if(workout_options[i].getPrice()<minPrice){
            minPrice=workout_options[i].getPrice();
            minTypeId=workout_options[i].getId();
        }
    }
    orderActivities.push_back(minTypeId);
    return orderActivities;
};
std::string CheapCustomer:: toString()const
{
    return "";
}
HeavyMuscleCustomer::HeavyMuscleCustomer(string _name, int _id):Customer(_name,_id)
{
}
HeavyMuscleCustomer::~HeavyMuscleCustomer(){
}
vector<int> HeavyMuscleCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> orderActivities;
    std::vector<int> prices;
    for(int i=0; i<workout_options.size(); i++){
        if(workout_options[i].getType() == ANAEROBIC){
            prices.push_back(workout_options[i].getPrice());
        }
    }
    std::sort(prices.begin(), prices.end());//add by ziv
    for(int price:prices) {
        for (Workout workout: workout_options) {
            if (workout.getType() == ANAEROBIC and workout.getPrice() == price) {
                orderActivities.push_back(workout.getId());
                break;
            }
        }
    }
    std::reverse(orderActivities.begin(), orderActivities.end());
    return  orderActivities;
};

std::string HeavyMuscleCustomer:: toString()const
{
    return "";
}
HeavyMuscleCustomer *HeavyMuscleCustomer:: clone(){
    return new HeavyMuscleCustomer(*this);
}
FullBodyCustomer::FullBodyCustomer(string _name, int _id):Customer(_name,_id)
{
}
FullBodyCustomer::~FullBodyCustomer(){

}

vector<int> FullBodyCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> orderActivities;
    bool isFound = false;
    int lowCrdPrice, lowCrdPriceId, highPrice, highPriceId, lowAnePrice, lowAnePriceId;
    for (int i = 0; i < workout_options.size(); i++) {//find the most expansive anaerobic
        if (workout_options[i].getType() == CARDIO) {
            if(not isFound){
                lowCrdPrice = workout_options[i].getPrice();
                lowCrdPriceId = workout_options[i].getId();
                isFound = true;
            }
            if(workout_options[i].getPrice() == lowCrdPrice and workout_options[i].getId() < lowCrdPriceId){
                lowCrdPriceId = workout_options[i].getId();
            }
            if(workout_options[i].getPrice() < lowCrdPrice){
                lowCrdPrice = workout_options[i].getPrice();
                lowCrdPriceId = workout_options[i].getId();
            }
        }


    }
    orderActivities.push_back(lowCrdPriceId);
    isFound = false;
    for(int i =0; i < workout_options.size(); i++){// need to be finish
        if (workout_options[i].getType() == MIXED) {
            if(not isFound){
                highPrice = workout_options[i].getPrice();
                highPriceId = workout_options[i].getId();
                isFound = true;
            }
            if(workout_options[i].getPrice() == highPrice and workout_options[i].getId() < highPriceId) {
                lowCrdPriceId = workout_options[i].getId();
            }
            if(workout_options[i].getPrice() >  highPrice){
                highPrice = workout_options[i].getPrice();
                highPriceId = workout_options[i].getId();
            }
        }

    }
    isFound = false; //add by ziv
    orderActivities.push_back(highPriceId);
    for (int i = 0; i < workout_options.size(); i++) {//find the most expansive anaerobic
        if (workout_options[i].getType() == ANAEROBIC) {
            if(not isFound){
                lowAnePrice = workout_options[i].getPrice();
                lowAnePriceId = workout_options[i].getId();
                isFound = true;
            }
            if(workout_options[i].getPrice() == lowAnePrice and workout_options[i].getId() < lowAnePriceId) {
                lowAnePriceId = workout_options[i].getId();
            }
            if(workout_options[i].getPrice() < lowAnePrice){
                lowAnePrice = workout_options[i].getPrice();
                lowAnePriceId = workout_options[i].getId();
            }
        }
    }
    orderActivities.push_back(lowAnePriceId);
    return orderActivities;

}
std::string FullBodyCustomer:: toString()const
{
    return "";
};
FullBodyCustomer *FullBodyCustomer:: clone(){
    return new FullBodyCustomer(*this);
}