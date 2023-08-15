#include "../include/Action.h"
#include "../include/Studio.h"
#include "../include/Trainer.h"
#include <iostream>

extern Studio *backup;

BaseAction::BaseAction():errorMsg(), status(){}
BaseAction::~BaseAction(){}
ActionStatus BaseAction::getStatus() const{
    return this->status;
}

void BaseAction::complete(){
    this->status = COMPLETED;
}

void BaseAction::error(std::string errorMsg){
    this->status = ERROR;
    this->errorMsg = errorMsg;
}

std::string BaseAction::getErrorMsg() const{
    return this->errorMsg;
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id),customers(customersList){}
OpenTrainer::~OpenTrainer(){}
void OpenTrainer::act(Studio &studio){
    Trainer *t=studio.getTrainer(this->trainerId);//studio shuold run constractur and act and to string
    if ((t==nullptr) || (t->isOpen()==true)){
        this->error("this trainer does not exist or already has an open session");
    }
    else {
        this->complete();
        t->openTrainer();        
        for (int i = 0; i < int(this->customers.size()); i++) 
            t->addCustomer(this->customers[i]);
    }
    studio.addActionLog(this);
}

std::string OpenTrainer::toString() const{
    std::string str = "open " + std::to_string(this->trainerId);
    for (int i = 0; i < int(this->customers.size()); i++)
    {
        str = str + " " + this->customers[i]->getName() + ",";
        if(SweatyCustomer* v = dynamic_cast<SweatyCustomer*>(this->customers[i])){
            str = str + "swt";
        }

        if(CheapCustomer* v = dynamic_cast<CheapCustomer*>(this->customers[i])){
            str = str + "chp";
        }

        if(HeavyMuscleCustomer* v = dynamic_cast<HeavyMuscleCustomer*>(this->customers[i])){
            str = str + "mcl";
        }

        if(FullBodyCustomer* v = dynamic_cast<FullBodyCustomer*>(this->customers[i])){
            str = str + "fbd";
        }
    }
        if (this->getStatus() == COMPLETED)                
            str = str + " Completed"; 

        else
            str = str  + " Error: " + this->getErrorMsg();

        return str;   
}

// std::vector<Customer *> OpenTrainer::getCustomers(){
//     return this->customers;
// }

// std::string toString() const{
//     if (this.Actionstatus(ERROR){
        
//     })
//     std::string str("OpenTrainer");
//     return (str);
// }

Order::Order(int id):trainerId(id){}

//Order::~Order(){}
void Order::act(Studio &studio){
    Trainer *t = studio.getTrainer(this->trainerId);
    std::vector<int> workoutList;
    if (t == nullptr || t->isOpen() == false)
        this->error("this trainer does not exist or or is not open");
    else
    {
        this->complete();
        std::vector<Customer*> costumerList = t->getCustomers();        
        for (int i = 0; i<int(costumerList.size()); i++)
        {
            workoutList = costumerList[i]->order(studio.getWorkoutOptions());
            t->order(costumerList[i]->getId(), workoutList, studio.getWorkoutOptions());
            for(int j = 0; j<int(workoutList.size()); j++)
            {
                std::string str = costumerList[i]->getName() + " is doing " + studio.getWorkoutOptions()[workoutList[j]].getName();
                std::cout <<str<< std::endl;
            }
        }
    }
    studio.addActionLog(this);
}

std::string Order::toString() const{
    std::string str = "order " + std::to_string(this->trainerId);
    if (this->getStatus() == COMPLETED)                
        str = str + " Completed"; 

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;   
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId): srcTrainer(src), dstTrainer(dst), id(customerId) {}

void MoveCustomer::act(Studio &studio){
    Trainer *t1 = studio.getTrainer(this->srcTrainer);
    Trainer *t2 = studio.getTrainer(this->dstTrainer);

    if (((t1== nullptr) | (t2 == nullptr ))|| ((t1->isOpen() == false) |(t2->isOpen() == false ))|| ((t1->getCustomer(this->id) == nullptr) | (t2->getCapacity() == 0)))
    {
        this->error("cannot move costumer");
    }

    else
    {
        this->complete();
        std::vector<OrderPair> srcOrdersList = t1->getOrders();
        std::vector<OrderPair> dstOrdersList = t2->getOrders();
        std::vector<int> workoutIds;
        
        for (int i =0; i<int(srcOrdersList.size()); i++)
        {
            if (srcOrdersList[i].first == this->id)
            {
                workoutIds.push_back(srcOrdersList[i].second.getId());
            }
        }
        t2->addCustomer(t1->getCustomer(this->id));
        t2->order(this->id,workoutIds, studio.getWorkoutOptions());
        t1->removeCustomer(this->id);
        studio.addActionLog(this);
        if (t1->getCustomers().size() == 0)
        {
            Close close = Close(this->srcTrainer);
            close.act(studio);
        }
    }
}
    
std::string MoveCustomer::toString() const{    
    std::string str = "move " + std::to_string(this->srcTrainer) + " " + std::to_string(this->dstTrainer) + " " + std::to_string(this->id);
    if (this->getStatus() == COMPLETED)                
        str = str + " Completed"; 

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;  
}

Close::Close(int id):trainerId(id){}

void Close::act(Studio &studio){
    Trainer *t = studio.getTrainer(this->trainerId);        

    if ((t==nullptr) || (t->isOpen()==false))
        this->error("trainer does not exist or is not open");

    else{
        t->closeTrainer();
        this->complete();
        std::string str = "close " + std::to_string(this->trainerId) + ". Salary " + std::to_string(t->getSalary()) + "NIS";
        std::cout<<str<<std::endl;
    }

    studio.addActionLog(this);
}
std::string Close::toString() const{
    std::string str = "Trainer " + std::to_string(this->trainerId) + " closed.";

    if (this->getStatus() == COMPLETED)                
        str = str + " Completed"; 

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;  
}

CloseAll::CloseAll(){}
void CloseAll::act(Studio &studio){
    this->complete();
    for (int i=0; i < studio.getNumOfTrainers(); i++){
        Trainer *t = studio.getTrainer(i);
        if (t->isOpen()){
            t->closeTrainer();
            std::string str = "Trainer " + std::to_string(i) + " closed." + " Salary " + std::to_string(t->getSalary()) + "NIS";
            std::cout<<str<<std::endl;
        }
    }

    studio.addActionLog(this);
    studio.close();
    //delete(&backup);
}

std::string CloseAll::toString() const{
    std::string str = "closeall";
    if (this->getStatus() == COMPLETED)                
        str = str + " Completed"; 

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;  
}

std::string typeToText(WorkoutType type){
    std::string str;
    if (type == 0)
        str = "Anaerobic";
    else if (type == 1)
        str = "Mixed";
    else
        str = "Cardio";
    return str;
}

PrintWorkoutOptions::PrintWorkoutOptions(){}

void PrintWorkoutOptions::act(Studio &studio){
    this->complete();
    for (int i = 0; i < int(studio.getWorkoutOptions().size()); i++)
    {
        Workout w = studio.getWorkoutOptions()[i];
        std::string str = w.getName() + ", " + typeToText(w.getType()) + ", " + std::to_string(w.getPrice());
        std::cout <<str<< std::endl;
    }                
    studio.addActionLog(this);
}

std::string PrintWorkoutOptions::toString() const{
    std::string str = "workout_options";
    if (this->getStatus() == COMPLETED)                
        str = str + " Completed"; 

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;  
}

PrintTrainerStatus::PrintTrainerStatus(int id): trainerId(id){}

void PrintTrainerStatus::act(Studio &studio){
    this->complete();
    std::string str;
    std::vector<int> idByOrder; 
    Trainer *t = studio.getTrainer(this->trainerId);
    if (t != nullptr)
    {   
        std::vector<Customer *> copyCustomersList = t->getCustomers();
    
        if (t->isOpen() == false)
        {
            str = "Trainer " + std::to_string(this->trainerId) + " status: closed";
            std::cout <<str<< std::endl;
        }
        else
        {
            for (int i = 0; i<int(t->getCustomers().size()); i++)
            {
                int minI = 0;
                for (int j = 0; j<int(copyCustomersList.size()); j++)
                    if (copyCustomersList[j]->getId() < copyCustomersList[minI]->getId())
                    {
                        minI = j;
                    }
                idByOrder.push_back(copyCustomersList[minI]->getId());
                copyCustomersList.erase(copyCustomersList.begin() + minI);
            }
            str = "Trainer " + std::to_string(this->trainerId) + " status: open";
            std::cout <<str<< std::endl;
            std::cout <<"Costumers:"<< std::endl;
            for (int i = 0; i < int(idByOrder.size()); i++)
            {
                std::cout <<t->getCustomer(idByOrder[i])->getId();
                std::cout <<" ";
                std::cout <<t->getCustomer(idByOrder[i])->getName()<< std::endl;
            }

            std::cout <<"Orders:"<< std::endl;
            for (int i = 0; i < int(idByOrder.size()); i++)
            {
                for (int j = 0; j < int(t->getOrders().size()); j++)
                {
                    if (t->getOrders()[j].first == idByOrder[i])
                    {
                        std::cout <<t->getOrders()[j].second.getName();
                        std::cout <<" ";
                        std::cout <<t->getOrders()[j].second.getPrice();
                        std::cout <<"NIS ";
                        std::cout <<t->getOrders()[j].first<< std::endl;
                    }
                }
            }
            std::cout <<"Current Trainer's Salary: ";
            std::cout <<t->getSalary();
            std::cout <<"NIS"<< std::endl;//d
        }
    }
    studio.addActionLog(this);
}

std::string PrintTrainerStatus::toString() const{
    std::string str = "status " + std::to_string(this->trainerId);
    if (this->getStatus() == COMPLETED)                
        str = str + " Completed"; 

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;  
}

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Studio &studio){
    this->complete();
    for (int i = 0; i < int(studio.getActionsLog().size()); i++)
    {
        BaseAction *action = studio.getActionsLog()[i];
        std::string str = action->toString();
        std::cout <<str<< std::endl;
    }
    studio.addActionLog(this);
}

std::string PrintActionsLog::toString() const{
    std::string str = "log";
    if (this->getStatus() == COMPLETED)               
        str = str + " Completed";

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;  
}

BackupStudio::BackupStudio(){}
void BackupStudio::act(Studio &studio){
    studio.addActionLog(this);
    if (backup!=nullptr){
        delete backup;
        backup=nullptr;
        studio.addActionLog(this);
    }
    backup = new Studio(studio);
    this->complete();
}
std::string BackupStudio::toString() const{
    std::string str = "backup";
    if (this->getStatus() == COMPLETED)               
        str = str + " Completed";

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;  
}

RestoreStudio::RestoreStudio(){}
void RestoreStudio::act(Studio &studio){
    if (backup == nullptr){
        this->error("No backup available");
    }
    else    
    {
        studio = Studio(*backup);
        this->complete();
    }
    studio.addActionLog(this);
}
std::string RestoreStudio::toString() const{
    std::string str = "restore";
    if (this->getStatus() == COMPLETED)               
        str = str + " Completed";

    else
        str = str  + " Error: " + this->getErrorMsg();

    return str;  
}
