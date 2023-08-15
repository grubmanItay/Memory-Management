#ifndef STUDIO_H_
#define STUDIO_H_
#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <iostream>
#include <fstream>

class Studio{		
public:
	Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void addActionLog(BaseAction* action);//new
    std::vector<std::string> splitWords(std::string str, char splitSign); //new
    void close();//change the bool open
    bool isopen();//check for bool NEED TO IMPLENT BOTH^^
    //rule of 5
     virtual ~Studio();//destrucor
     Studio(const Studio &other);//copy
     Studio(Studio &&other);//move
     Studio& operator=(const Studio &&other);//copy assigment
     Studio& operator=(Studio &&other);//move assigment

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    void clear();
};

#endif
