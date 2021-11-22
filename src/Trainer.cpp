#include "Customer.h"
#include "Workout.h"
#include "Trainer.h"
using namespace std;
Trainer::Trainer(int t_capacity):capacity (t_capacity){
};
Trainer::~Trainer(){
    for(Customer *customer: customersList){
        delete customer;
    }
    orderList.clear();

};
// Copy Constructor
Trainer::Trainer(const Trainer & other){
    capacity = other.capacity;
    open = other.open;
    salary = other.salary;
    for(Customer *customer: other.customersList){
        customersList.push_back(customer->clone());
    }
    for(OrderPair order: other.orderList){
        orderList.push_back({order.first, order.second});//check if pair like that
    }
}
Trainer::Trainer(Trainer&&other):capacity(other.getCapacity()),open(other.open),salary(other.salary){
    other.capacity=0;
    other.open=false;
    other.salary=0;
    for(int i=0;i<other.customersList.size();i++){
        customersList.push_back(other.customersList[i]);
    }
    other.customersList.clear();
    for(int i=0;i<other.orderList.size();i++){
        orderList.push_back(other.orderList[i]);
        other.orderList.pop_back();
    }


}

Trainer & Trainer::operator=(const Trainer &other) {
    if(this!=&other){
        capacity=other.capacity;
        open=other.open;
        salary=other.salary;
        //clear my object
        customersList.clear();
        orderList.clear();
        //take the information from the other object and erase it.
        for(int i=0;i<other.orderList.size();i++){
            orderList.push_back({other.orderList[i].first,Workout(other.orderList[i].second.getId(),other.orderList[i].second.getName(),other.orderList[i].second.getPrice(),other.orderList[i].second.getType())});
        }
        for(int i=0;i<other.customersList.size();i++){
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
        other.capacity=0;
        other.open=false;
        other.salary=0;
        for(int i=0;i<other.customersList.size();i++){
            customersList.push_back(other.customersList[i]);
        }
        other.customersList.clear();
        for(int i=0;i<other.orderList.size();i++){
            orderList.push_back({other.orderList[i].first,other.orderList[i].second});
            other.orderList.pop_back();
        }
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
    for(int i=0;i<customersList.size()&&!isIdFound;i++){
        if(customersList[i]->getId()==id){
            isIdFound=true;
            //delete customersList[i];//need to be checked
            customersList.erase(customersList.begin()+i);

        }
    }
    if(isIdFound){
        capacity = capacity + 1; //if we remove customer we add 1 to the capacity.
    }

}Customer* Trainer:: getCustomer(int id){
    int returnCustomerIndex=0;
    bool isFound=false;
    for(int i=0;i<customersList.size()&&!isFound;i++){
        if(customersList[i]->getId()==id) {
            returnCustomerIndex=i;
            isFound=true;
        }
    }
    return customersList[returnCustomerIndex];
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
    for(int i=0;i<workout_ids.size();i++){

        orderList.push_back({customer_id,workout_options[workout_ids[i]]});
        //cout<<workout_ids[i];
        cout<<getCustomer(customer_id)->getName()<<" Is Doing "<<workout_options[workout_ids[i]].getName()<<endl;//changed by ziv
        salary += workout_options[workout_ids[i]].getPrice();//add by ziv
    }
}
void Trainer::openTrainer(){
    open=true;
}
void Trainer::removeOrder(bool reduceSalary, int customerId) { //add by ziv
    std::vector<OrderPair> tempOrderList;
    for(int i=0; i<orderList.size(); i++)
    {
        if(orderList[i].first != customerId){
            tempOrderList.push_back(orderList[i]);
        }
        else{
            if(reduceSalary){
                salary = salary - orderList[i].second.getPrice();
            }
        }
        orderList.clear();//check if its clear good
        for (OrderPair order : tempOrderList) {
            orderList.push_back(order);
        }
    }
}
void Trainer::closeTrainer() {
    for(Customer *customer: customersList){
        removeOrder(false, customer->getId());
        removeCustomer(customer->getId());
    }
    open = false;
}