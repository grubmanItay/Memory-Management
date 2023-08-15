
#include "../include/Studio.h"

//constructor
Trainer::Trainer(int t_capacity):capacity(t_capacity), open(false), salary(0), customersList(), orderList(){}
//rule of 5
 Trainer::~Trainer(){clear();}//destru

 void Trainer::clear(){
    int csize= int(customersList.size());
        for (int i=0; i<csize;i++)
            delete customersList[i];
    customersList.clear();     
    orderList.clear();
    capacity=0;
    open=false;
}

  //copy
Trainer::Trainer(const Trainer &other):capacity(other.capacity),open(other.open),salary(other.salary),customersList(),orderList(){
    int osize=other.orderList.size();    
    for (int i=0; i<osize;i++)
        orderList.push_back(other.orderList[i]);
    int csize=other.customersList.size();
    for (int i=0; i<csize; i++) {
        if(SweatyCustomer* v = dynamic_cast<SweatyCustomer*>(other.customersList[i])){
            SweatyCustomer* sc=new SweatyCustomer(other.customersList[i]->getName(),other.customersList[i]->getId());
            customersList.push_back(sc);
        }
        if(HeavyMuscleCustomer* v = dynamic_cast<HeavyMuscleCustomer*>(other.customersList[i])){
             HeavyMuscleCustomer* mc=new HeavyMuscleCustomer(other.customersList[i]->getName(),other.customersList[i]->getId());
             customersList.push_back(mc);
        }
        if(CheapCustomer* v = dynamic_cast<CheapCustomer*>(other.customersList[i])){
             CheapCustomer* cc=new CheapCustomer(other.customersList[i]->getName(),other.customersList[i]->getId());
             customersList.push_back(cc);
        }
        if(FullBodyCustomer* v = dynamic_cast<FullBodyCustomer*>(other.customersList[i])){
             FullBodyCustomer* fc=new FullBodyCustomer(other.customersList[i]->getName(),other.customersList[i]->getId());
             customersList.push_back(fc);
        }
         
    }   
    
}

 Trainer::Trainer(Trainer &&other):capacity(other.capacity),open(other.open),salary(other.salary),customersList(),orderList()  {//move
    int osize=int(orderList.size());    
    for (int i=0; i<osize;i++)
        orderList.push_back(other.orderList[i]); 
     int csize=int(customersList.size());    
    for (int i=0; i<int(csize);i++)
        customersList.push_back(other.customersList[i]);     
    other.customersList.clear();
    other.orderList.clear();
    other.open=false;
    other.capacity=0;
 }  
 Trainer& Trainer::operator=(const Trainer &&other){//copy assige
     if (this != &other){
         //if (open) delete customersList;
         //if (open) delete orderList;           //or clear
         capacity=other.capacity;
         open=other.open;
         salary=other.salary;
         delete &customersList;
         customersList = other.customersList;
         delete &orderList;
        int osize=int(orderList.size());    
        for (int i=0; i<osize;i++)
            orderList.push_back(std::pair<int,Workout> (other.orderList[i].first,other.orderList[i].second));
     }
     return *this;
 }
 Trainer& Trainer::operator=(Trainer &&other){//move assi
      if (this != &other){
         this->capacity =other.capacity;
         salary=other.salary;
        //orderList.clear();
        int osize=int(orderList.size());    
        for (int i=0; i<osize;i++)
            orderList.push_back(other.orderList[i]);
        customersList.clear(); 
        int csize=customersList.size();    
        for (int i=0; i<csize;i++)
            customersList.push_back(other.customersList[i]);   
        other.customersList.clear();
        //other.orderList.clear();
      }
      return *this;
 }

int Trainer::getCapacity() const {
    return this->capacity;
}

// void Trainer::addCostumerOrders(int costumerId,int originTrainerId){
//     std::vector<OrderPair> originTrainerOrderList =  this.gethis->getOrders();
//     for (int i =0; i<ordersList.size(); i++)
//     if (ordersList[i].first == id)
//     {
//         this->salary = this->salary - ordersList[i].second.getPrice();
//         ordersList.erase(ordersList.begin()+ i-1);
//     }
// }

void Trainer::addCustomer(Customer* customer){
    this->customersList.push_back(customer);
    this->capacity--;
}

void Trainer::removeCustomer(int id){
    int csize=this->customersList.size();
    for (int i = 0; i<csize; i++)
    {
        if (this->customersList[i]->getId() == id)
            this->customersList.erase(this->customersList.begin()+i);
    }
    this->capacity++;
    std::vector<OrderPair> newOrdersList;
    // std::vector<OrderPair> ordersList = this->orderList;
    int osize=this->orderList.size();
    for (int i =0; i<osize; i++)
    {
        if (this->orderList[i].first == id)
            {
                this->salary = this->salary - this->orderList[i].second.getPrice();            
            }
        else
            {
                newOrdersList.push_back(this->orderList[i]);
            }
    }

    this->orderList.clear();
    for (int i = 0; i < int(newOrdersList.size()); i++)
    {
        this->orderList.push_back(newOrdersList[i]);
    }
}

Customer* Trainer::getCustomer(int id){
    int index = -1;
    if (id < 0)
        return nullptr;
    int csize=this->customersList.size();
    for (int i = 0; i < csize; i++)
    {
        if (this->customersList[i]->getId() == id)
        {
            index = i;
            break;
        }
    }
    
    if (index == -1)
        return nullptr;
    return this->customersList[index];
}

std::vector<Customer*>& Trainer::getCustomers(){
    return this->customersList;
}

std::vector<OrderPair>& Trainer::getOrders(){
    return this->orderList;
}

void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    int osize=workout_ids.size();
    for (int i = 0; i < osize; i++){
        this->orderList.push_back(std::make_pair(customer_id,workout_options[workout_ids[i]]));
        this->salary = this->salary + workout_options[workout_ids[i]].getPrice();
    }
}

void Trainer::openTrainer(){
    this->open = true;
}

void Trainer::closeTrainer(){
    this->open = false;
}

int Trainer::getSalary(){
    return this->salary;
}

bool Trainer::isOpen(){
    return this->open;
}

