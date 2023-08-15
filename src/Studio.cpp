#include "../include/Studio.h"
#include "../include/Workout.h"
#include "../include/Trainer.h"
#include "../include/Action.h"

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

extern Studio *backup;
//constructor
Studio::Studio():open(true), trainers(), workout_options(), actionsLog(){}
//rule of 5
 Studio::~Studio(){clear();}
 void Studio::clear(){
    int tsize=trainers.size(); 
    for (int i=0; i<tsize; i++)
        delete (trainers[i]);  
    trainers.clear();    
    actionsLog.clear();    
    while (!workout_options.empty())
        workout_options.pop_back();
//    int osize=workout_options.size(); 
//   for (int i=0;i<osize;i++)
//        workout_options[i].getType();
    workout_options.clear();    
    open=false;    
 }
Studio::Studio(const Studio &other):open(other.open),trainers(),workout_options(),actionsLog(){//copy
     int wsize=other.workout_options.size();
     int tsize=other.trainers.size();
     this->trainers.clear();
     for (int i=0; i<tsize;i++){
         trainers.push_back(new Trainer(*other.trainers[i]));
     }
     for (int i=0; i<wsize;i++)
        this->workout_options.push_back(*(new Workout(i ,other.workout_options[i].getName(),other.workout_options[i].getPrice(),other.workout_options[i].getType())));
     for (int i=0; i<int(other.actionsLog.size());i++){
         this->actionsLog.push_back(other.actionsLog[i]);
     }
     

 }
 Studio::Studio(Studio &&other):open(true), trainers(), workout_options(), actionsLog(){//move
    open=other.open;
    trainers=other.trainers;
    actionsLog=other.actionsLog;
    int osize=other.workout_options.size();
    for (int i=0; i<osize; i++)
            workout_options.push_back(other.workout_options[i]);
    for (int i=0; i<osize; i++)
            other.workout_options.pop_back();        
    other.clear();
   
 }  
 Studio& Studio::operator=(const Studio &&other){//copy assige
     if (this != &other){
         open=other.open;
         int wsize=other.workout_options.size();
         delete &workout_options;
         for (int i=0; i<wsize; i++)
            workout_options.push_back(other.workout_options[i]);
         int tsize=other.trainers.size(); 
         delete &trainers;  
         for (int i=0; i<tsize; i++)
            trainers.push_back(other.trainers[i]);
         delete &actionsLog;   
         actionsLog=other.actionsLog;
         //workout_options.clear();
          
     }
     return *this;
 }
 Studio& Studio::operator=(Studio &&other){//move assi
     if (this != &other){
         open=other.open;
         this->trainers.clear();
         int tsize=other.trainers.size();
         for(int i =0 ; i< tsize ; i++){            
            this->trainers.push_back(other.trainers[i]);
        }        
        // for(int i =0 ; i <tsize ; i++)
        //     other.trainers.pop_back();
         this->actionsLog=other.actionsLog;
         this->workout_options.clear();
         int wsize=other.workout_options.size();
         for (int i=0; i<wsize; i++)
            this->workout_options.push_back(other.workout_options[i]);
        //  for (int i=0; i<wsize; i++)
        //     other.workout_options.pop_back();   
        //  other.open=false;
         other.trainers.clear();
         other.actionsLog.clear();   
        //for (int i=0; i<osize; i++)
        //   other.workout_options.pop_back();
        //for (auto d: other.trainers)
        //    delete d;  
        // other.trainers.clear();
        //other.workout_options.clear();   
        // other.actionsLog.clear();
        // other.open=false;         
        
     }
     return *this;
 }

Studio::Studio(const std::string &configFilePath):open(true), trainers(), workout_options(), actionsLog(){
    std::string str = "";
    std::vector<std::string> vectorOfWords;
    ifstream inputFile(configFilePath);      
    while (getline(inputFile, str)) {
        if (str == "# Traines")
        {
            getline(inputFile, str);
            vectorOfWords = splitWords(str, ',');
            for (int i = 0; i < int(vectorOfWords.size()); i++)
            {
                this->trainers.push_back(new Trainer(std::stoi(vectorOfWords[i])));
            }
        }

        if (str == "# Work Options")
        {
            while (getline(inputFile, str)) {
                if (str == "")
                    break;
                vectorOfWords = splitWords(str, ',');
                WorkoutType type;
                if (vectorOfWords[1] == " Anaerobic")
                    type = ANAEROBIC;
                else if (vectorOfWords[1] == " Mixed")
                    type = MIXED;
                else
                    type = CARDIO;
                this->workout_options.push_back((Workout(this->workout_options.size(), vectorOfWords[0], std::stoi(vectorOfWords[2].substr(1, vectorOfWords[2].size()-1)), type)));
            }
            break;            
        }
    }
}

int Studio::getNumOfTrainers() const {
    return this->trainers.size();
}

Trainer* Studio::getTrainer(int tid){
    if ((tid >= int(this->trainers.size())) | (tid < 0))
        return nullptr;
    return this->trainers[tid];
}

const std::vector<BaseAction*>& Studio::getActionsLog() const {
    return this->actionsLog;
}

std::vector<Workout>& Studio::getWorkoutOptions(){
    return this->workout_options;
}

void Studio::start(){
    this->open=true;
    int id = 0;
    std::cout << "Studio is now open!" << std::endl;
    std::string str;
    std::cout <<"enter an action"<< std::endl;
    getline(cin,str);
    std::vector<std::string> vectorOfWords;
    vectorOfWords = splitWords(str, ' ');
    Trainer *t;
    while (vectorOfWords[0] != "closeall")
    {
        
        if (vectorOfWords[0] == "open")
        {
            std::vector<Customer *> costumers;
            Customer *newCostumer;
            t = this->getTrainer(std::stoi(vectorOfWords[1]));
            if ((t != nullptr) && (t->isOpen() == false))
            {
                int numOpenPlaces;
                
                if (int(vectorOfWords.size()) - 2 > this->getTrainer(std::stoi(vectorOfWords[1]))->getCapacity())
                    numOpenPlaces = this->getTrainer(std::stoi(vectorOfWords[1]))->getCapacity();
                else   
                    numOpenPlaces = vectorOfWords.size() - 2;
                for (int i = 2; i < numOpenPlaces + 2; i++)
                {
                    std::vector<std::string> costumer = splitWords(vectorOfWords[i],',');       
                    if(costumer[1] == "swt"){
                        newCostumer = new SweatyCustomer(costumer[0],id);
                    }

                    if(costumer[1] == "chp"){
                        newCostumer = new CheapCustomer(costumer[0],id);
                    }

                    if(costumer[1] == "mcl"){
                        newCostumer = new HeavyMuscleCustomer(costumer[0],id);
                    }

                    if(costumer[1] == "fbd"){
                        newCostumer = new FullBodyCustomer(costumer[0],id);
                    }         
                    costumers.push_back(newCostumer);
                    id++;
                }
            }
            OpenTrainer *openTrainer = new OpenTrainer(std::stoi(vectorOfWords[1]), costumers);
            openTrainer->act(*this);
            // delete(openTrainer);
        }

        if (vectorOfWords[0] == "order")
        {
            Order *order = new Order(std::stoi(vectorOfWords[1]));
            order->act(*this);
            // delete(order);
        }

        if (vectorOfWords[0] == "move")
        {
            MoveCustomer *move = new MoveCustomer(std::stoi(vectorOfWords[1]), std::stoi(vectorOfWords[2]), std::stoi(vectorOfWords[3]));
            move->act(*this);
            // delete(move);
        }

        if (vectorOfWords[0] == "workout_options")
        {
            PrintWorkoutOptions *print =new PrintWorkoutOptions();    
            print->act(*this);
            // delete(print);
        }

        if (vectorOfWords[0] == "status")
        {
            PrintTrainerStatus *printStatus = new PrintTrainerStatus(std::stoi(vectorOfWords[1]));    
            printStatus->act(*this);
            // delete(printStatus);
        }

        if (vectorOfWords[0] == "log")
        {
            PrintActionsLog *printLog = new PrintActionsLog();    
            printLog->act(*this);
            // delete(printLog);
        }

        if (vectorOfWords[0] == "close")
        {
            Close *close = new Close(std::stoi(vectorOfWords[1]));    
            close->act(*this);
            // delete(close);

        }
        if (vectorOfWords[0] == "backup")
        {
            BackupStudio *ba = new BackupStudio();    
            ba->act(*this);
        }

        if (vectorOfWords[0] == "restore")
        {
            RestoreStudio *re= new RestoreStudio();    
            re->act(*this);
        }
        
        std::cout <<"enter an action"<< std::endl;
        getline(cin,str);
        vectorOfWords = splitWords(str, ' ');
    }
    CloseAll *closeAll =new CloseAll();
    closeAll->act(*this);
//    for (int i=0; i<int(costumers.size()); i++)
//        {
//            delete (costumers[i]);
//        }
//     costumers.clear();
    // for (int i = 0; i<int(this->trainers.size()); i++){
    //    for (int j = 0; j<int(this->trainers.size()); j++){
    //        delete this->trainers[i]->getCustomers()[j];
    //    }
    //    delete this->trainers[i];
    // }
    for (int i = 0; i<int(this->actionsLog.size()); i++){
        delete this->actionsLog[i];
    }
    if (backup!=nullptr)
        backup->clear();
   
    this->clear();

}

void Studio::addActionLog(BaseAction* action){
    this->actionsLog.push_back(action);
}

std::vector<std::string> Studio::splitWords(std::string str, char splitSign)
{
    std::vector<std::string> vectorOfWords;
    std::string word = "";
    int wordStartIndex = 0;
    int wordLength = 0;
    for (int i = 0; i<int(str.size()); i++) 
    {
        if (str[i] != splitSign)                    
            wordLength++;
        else
        {
            word = word + str.substr(wordStartIndex,wordLength);
            vectorOfWords.push_back(word);
            word = "";
            wordStartIndex = i+1;
            wordLength = 0;
        }
    }
    word = word + str.substr(wordStartIndex,wordLength);
    vectorOfWords.push_back(word);
    return vectorOfWords;
}

void Studio::close(){
    this->open = false;
}

bool Studio::isopen(){
    return this->open;
}

