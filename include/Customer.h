#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"
#include <algorithm>
#include <functional>

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual ~Customer(); //destructor
    virtual Customer* clone()=0;
    bool getIsOrder() const;//by ziv
    void setIsOrder();//by ziv
    bool getIsRemoved() const;
    void setIsRemoved(bool isRemoved);
private:
    const std::string name;
    const int id;
    bool isOrder; //by ziv
};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    ~SweatyCustomer();
    SweatyCustomer* clone();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;

private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    ~CheapCustomer();
    CheapCustomer* clone();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;

private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    ~HeavyMuscleCustomer();
    HeavyMuscleCustomer* clone();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;

private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    ~FullBodyCustomer();
    FullBodyCustomer* clone();
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;

private:
};


#endif