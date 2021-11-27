#include "../include/Customer.h"
#include <algorithm>
#include "../include/Workout.h"
#include <functional>
using namespace std;
Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){
}
Customer::~Customer(){

}
std::string Customer:: getName() const{
    return name;
}
int Customer:: getId() const{
    return id;
}

SweatyCustomer::SweatyCustomer(string _name, int _id):Customer(_name,_id)
{

}
SweatyCustomer::~SweatyCustomer(){
}
vector<int> SweatyCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> orderActivities={};
    for (int i = 0; i < (int)workout_options.size(); i++) {
        WorkoutType type=workout_options[i].getType();
        if (type == CARDIO) {
            int workoutId=workout_options[i].getId();
            orderActivities.push_back(workoutId);
        }
    }
    return orderActivities;
};
std::string SweatyCustomer:: toString()const
{
    return "";
}
SweatyCustomer *SweatyCustomer:: clone(){
    return new SweatyCustomer(getName(),getId());
}

CheapCustomer::CheapCustomer(string _name, int _id):Customer(_name,_id)
{

}
CheapCustomer::~CheapCustomer(){

}
CheapCustomer *CheapCustomer:: clone(){
    return new CheapCustomer(getName(),getId());
}

vector<int> CheapCustomer:: order(const std::vector<Workout> &workout_options) {
    vector<int> orderActivities={};
    int minTypeId=workout_options[0].getId();
    int minPrice=workout_options[0].getPrice();
    for(int i=1;i<(int)workout_options.size();i++){
        int priceCurrent=workout_options[i].getPrice();
        int currentId=workout_options[i].getId();
        if(priceCurrent == minPrice and currentId < minTypeId ){
            minTypeId = currentId;
        }
        if(priceCurrent<minPrice){
            minPrice=priceCurrent;
            minTypeId=currentId;
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
    std::vector<int> prices={};
    for(Workout workout:workout_options){
        WorkoutType type=workout.getType();
        if( type== ANAEROBIC){
            int price=workout.getPrice();
            prices.push_back(price);
        }
    }
    std::sort(prices.begin(), prices.end());//add by ziv
    std::vector<int> anerobicIds = {};
    for(int price:prices) {
        for (Workout workout: workout_options) {
            WorkoutType type=workout.getType();
            int currentVecPrice=workout.getPrice();
            if (type == ANAEROBIC and currentVecPrice == price) {
                int workoutId=workout.getId();
                bool isExist = false;
                for(int id:anerobicIds){
                    if(workoutId == id){
                        isExist = true;
                        break;
                    }
                }
                if(not isExist){
                    anerobicIds.push_back(workoutId);
                    break;
                }
            }

        }

    }
    std::reverse(anerobicIds.begin(), anerobicIds.end());
    return anerobicIds;
};

std::string HeavyMuscleCustomer:: toString()const
{
    return "";
}
HeavyMuscleCustomer *HeavyMuscleCustomer:: clone(){
    return new HeavyMuscleCustomer(getName(),getId());
}
FullBodyCustomer::FullBodyCustomer(string _name, int _id):Customer(_name,_id)
{
}
FullBodyCustomer::~FullBodyCustomer(){

}

vector<int> FullBodyCustomer:: order(const std::vector<Workout> &workout_options) {//find the 3 workout the customer wants
    vector<int> orderActivities={};
    bool isFound = false;
    int lowCrdPrice=0, lowCrdPriceId=0, highPrice=0, highPriceId=0, lowAnePrice=0, lowAnePriceId=0;
    for (int i = 0; i < (int)workout_options.size(); i++) {//find the most expansive anaerobic
        int currentPrice=workout_options[i].getPrice();
        int workoutId=workout_options[i].getId();
        WorkoutType type=workout_options[i].getType();
        if (type == CARDIO) {
            if(not isFound){
                lowCrdPrice = workout_options[i].getPrice();
                lowCrdPriceId = workout_options[i].getId();
                isFound = true;
            }
            if( currentPrice== lowCrdPrice and workoutId < lowCrdPriceId){
                lowCrdPriceId = workout_options[i].getId();
            }
            if(currentPrice < lowCrdPrice){
                lowCrdPrice = currentPrice;
                lowCrdPriceId = workoutId;
            }
        }


    }
    orderActivities.push_back(lowCrdPriceId);
    isFound = false;
    for(int i =0; i < (int)workout_options.size(); i++){
        WorkoutType type=workout_options[i].getType();
        int price=workout_options[i].getPrice();
        int WorkoutId=workout_options[i].getId();
        if (type == MIXED) {
            if(not isFound){
                highPrice =price;
                highPriceId = WorkoutId;
                isFound = true;
            }

            if(price == highPrice and WorkoutId < highPriceId) {
                lowCrdPriceId = WorkoutId;
            }
            if(price >  highPrice){
                highPrice =price;
                highPriceId =WorkoutId;
            }
        }

    }
    isFound = false;
    orderActivities.push_back(highPriceId);
    for (int i = 0; i < workout_options.size(); i++) {//find the most expansive anaerobic
        WorkoutType type=workout_options[i].getType();
        int price=workout_options[i].getPrice();
        int workOutId=workout_options[i].getId();
        if (type == ANAEROBIC) {
            if(not isFound){
                lowAnePrice = price;
                lowAnePriceId = workOutId;
                isFound = true;
            }
            if(price == lowAnePrice and workOutId < lowAnePriceId) {
                lowAnePriceId = workOutId;
            }
            if(price < lowAnePrice){
                lowAnePrice = price;
                lowAnePriceId = workOutId;
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
    return new FullBodyCustomer(getName(),getId());
}