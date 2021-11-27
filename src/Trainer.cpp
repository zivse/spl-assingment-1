#include "../include/Customer.h"
#include "../include/Trainer.h"
using namespace std;
Trainer::Trainer(int t_capacity):capacity (t_capacity),open(false),customersList(),orderList(),salary(){
};
Trainer::~Trainer(){
    for(Customer *customer: customersList){
        delete customer;
    }
    customersList.clear();
    orderList.clear();
};
// Copy Constructor
Trainer::Trainer(const Trainer & other): capacity(other.capacity), open(other.open), salary(other.salary), customersList(), orderList(){
    for(Customer *customer: other.customersList){
        customersList.push_back(customer->clone());
    }
    orderList.clear();
    orderList = std::vector<OrderPair>(other.orderList);
}

//move constructor
Trainer::Trainer(Trainer&&other):capacity(other.getCapacity()),open(other.open),salary(other.salary), customersList(), orderList(){
    for(int i=0;i<(int)other.customersList.size();i++){
        customersList.push_back(other.customersList[i]);
    }
    orderList.clear();
    orderList = std::vector<OrderPair>(other.orderList);
}

Trainer & Trainer::operator=(const Trainer &other) {
    if(this!=&other){
        capacity=other.capacity;
        open=other.open;
        salary=other.salary;
        //clear my object
        for (Customer *customer: customersList)
        {
            delete customer;
        }
        customersList.clear();
        orderList.clear();
        //take the information from the other object and erase it.
        orderList = std::vector<OrderPair>(other.orderList);
        for(int i=0;i<(int)other.customersList.size();i++){
            customersList.push_back(other.customersList[i]->clone());
        }
    }
    return *this;
}
Trainer & Trainer::operator=(Trainer &&other) {
    if(this!=&other){
        capacity=other.capacity;
        open=other.open;
        salary=other.salary;
        for (Customer *customer: customersList){
            delete customer;
        }
        customersList.clear();
        for(int i=0;i<(int)other.customersList.size();i++){
            customersList.push_back(other.customersList[i]);
        }

        orderList.clear();
        orderList = std::vector<OrderPair>(other.orderList);
    }
    return *this;
}

int Trainer::getCapacity() const{
    return capacity;
};
int Trainer::getSalary() {
    return salary;
}
void Trainer::addCustomer(Customer* customer) {
    customersList.push_back(customer);
    capacity--;
}

void Trainer::removeCustomer(int id){
    bool isIdFound=false;
    for(int i=0;i<(int)customersList.size()&&!isIdFound;i++){
        int currentId = customersList[i]->getId();
        if(currentId==id){
            isIdFound=true;
            Customer* a=customersList[i];
            customersList.erase(customersList.begin()+i);
            delete a;
        }
    }
    if(isIdFound){
        capacity = capacity + 1; //if we remove customer we add 1 to the capacity.
    }

}Customer* Trainer:: getCustomer(int id){
    int returnCustomerIndex=0;
    bool isFound=false;
    for(int i=0;i<(int)customersList.size()&&!isFound;i++){
        int customretId=customersList[i]->getId();
        if(customretId==id) {
            returnCustomerIndex=i;
            isFound=true;
        }
    }
    if(isFound){
        return customersList[returnCustomerIndex];
    }
    else{
        return nullptr;
    }
};
std::vector<Customer*>& Trainer:: getCustomers(){
    return customersList;
}
std::vector<OrderPair>& Trainer:: getOrders(){
    return orderList;
}
bool Trainer::isOpen(){
    return open;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    for(int i=0;i<(int)workout_ids.size();i++){
        orderList.push_back(OrderPair(customer_id,workout_options[workout_ids[i]]));
        cout<<getCustomer(customer_id)->getName()<<" Is Doing "<<workout_options[workout_ids[i]].getName()<<endl;
        salary += workout_options[workout_ids[i]].getPrice();
    }
}
void Trainer::openTrainer(){
    open=true;
}
void Trainer::removeOrder(bool reduceSalary, int customerId) {//reduce salary = true if we move customer and false if we close the trainer.

    std::vector<OrderPair> tempOrderList;
    for(int i=0; i<(int)orderList.size(); i++) {
        if (orderList[i].first != customerId) {
            tempOrderList.push_back(orderList[i]);
        } else {
            if (reduceSalary) {
                salary = salary - orderList[i].second.getPrice();
            }
        }
    }
    orderList.clear();
    for (OrderPair order : tempOrderList) {
        orderList.push_back(order);
    }
}
void Trainer::closeTrainer() {
    for(Customer *customer: customersList){
        int cid = customer->getId();
        cout<<cid<<endl;
        removeCustomer(customersList[0]->getId());
        removeOrder(false, cid);
    }
    open = false;
}