#include "../include/Workout.h"
using namespace std;
Workout::Workout( int _id, string _name,  int _price, WorkoutType _type):
        id(_id), name(_name),price(_price),type(_type)
{
};

int Workout:: getId() const{
    return id;
};
std::string Workout:: getName() const{
    return name;
};
int Workout::getPrice() const{
    return price;
};
WorkoutType Workout:: getType() const{
    return type;
};