#ifndef TRAINER_H_
#define TRAINER_H_
#include <vector>
#include "Customer.h"
#include "Workout.h"


typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    // void updateSalary(int differential);//new
    bool isOpen();
    void addCostumerOrders(int costumerId,int originTrainerId);
    //rule of 5
     virtual ~Trainer();//destrucor
     Trainer(const Trainer &other);//copy
     Trainer(Trainer &&other);//move
     Trainer& operator=(const Trainer &&other);//copy assigment
     Trainer& operator=(Trainer &&other);//move assigment
    
private:
    int capacity;
    bool open;
    int salary;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    void clear();
};


#endif

