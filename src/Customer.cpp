#include "../include/Customer.h"
#include <vector>
#include <string>
#include <algorithm>
#include "../include/Workout.h"


Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){}
Customer::~Customer(){}
std::string Customer::getName() const{
    return this->name;
}

int Customer::getId() const{
    return this->id;
}

SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name, id){}

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> workoutList;
    WorkoutType type = CARDIO;
    for (int i = 0; i < int(workout_options.size()); i++)
    {
        if (workout_options[i].getType() == type)
            workoutList.push_back(workout_options[i].getId());
    }
    return workoutList;
}

std::string SweatyCustomer::toString() const{
    std::string str;
    str = this->getName() + "," + std::to_string(this->getId());
    return str;
}

CheapCustomer::CheapCustomer(std::string name, int id): Customer(name, id){}

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<int> workoutList;
    int cheapestWorkoutIndex = 0;
    for (int i = 1; i < int(workout_options.size()); i++)
    {
        if ((workout_options[i].getPrice() < workout_options[cheapestWorkoutIndex].getPrice()) | ((workout_options[i].getPrice() == workout_options[cheapestWorkoutIndex].getPrice()) & (workout_options[i].getId() < workout_options[cheapestWorkoutIndex].getId())))
            cheapestWorkoutIndex = i;
    }
    workoutList.push_back(workout_options[cheapestWorkoutIndex].getId());
    return workoutList;
}

std::string CheapCustomer::toString() const{
    std::string str;
    str = this->getName() + "," + std::to_string(this->getId());
    return str;
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name, id){}

// bool priceComparator(const Workout& w1, const Workout& w2) {
//    return ((w1.getPrice() >= w2.getPrice()) & (w1.getId() < w2.getId()));
// }

// bool typeComparator(const Workout& w1, const Workout& w2) {
//    return w1.getType() > w2.getType();
// }

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){
    std::vector<Workout*> newWorkout_options;
    std::vector<Workout> copyWorkout_options;
    std::vector<int> workoutList;
    int maxIndex = 0;
    // std::sort(newWorkout_options.begin(), newWorkout_options.end(), &priceComparator);
    WorkoutType type = ANAEROBIC;

    for (int i = 0; i < int(workout_options.size()); i++)
    {
        copyWorkout_options.push_back(workout_options[i]);//????
    }

    for (int i = 0; i < int(copyWorkout_options.size()); i++)
    {
        if (copyWorkout_options[i].getType() == type)
            newWorkout_options.push_back(&copyWorkout_options[i]);
    }

    int size = int(newWorkout_options.size());
    
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < int(newWorkout_options.size()); j++)
        {
            if ((*(newWorkout_options[j])).getPrice() > (*(newWorkout_options[maxIndex])).getPrice())
            {                
                maxIndex = j;
            }
        }
        workoutList.push_back(copyWorkout_options[maxIndex].getId());
        newWorkout_options.erase(newWorkout_options.begin()+ maxIndex);
        maxIndex = 0;
    }
    return workoutList;
}

std::string HeavyMuscleCustomer::toString() const{
    std::string str;
    str = this->getName() + "," + std::to_string(this->getId());
    return str;
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name, id){}

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
        std::vector<Workout> copyWorkout_options;
    std::vector<Workout*> cardioWorkout_options;
    std::vector<Workout*> mixedWorkout_options;
    std::vector<Workout*> anaerobicWorkout_options;
    WorkoutType anaerobic = ANAEROBIC;
    WorkoutType cardio = CARDIO;
    WorkoutType mixed = MIXED;

    for (int i = 0; i < int(workout_options.size()); i++)
    {
        copyWorkout_options.push_back(workout_options[i]);//????
    }

    for (int i = 0; i < int(workout_options.size()); i++)
    {
        if (workout_options[i].getType() == anaerobic)
            anaerobicWorkout_options.push_back(&(copyWorkout_options[i]));
        if (workout_options[i].getType() == cardio)
            cardioWorkout_options.push_back(&(copyWorkout_options[i]));
        if (workout_options[i].getType() == mixed)
            mixedWorkout_options.push_back(&(copyWorkout_options[i]));
    }

    int cheapestCardioIndex = 0;
    int expensiveMixIndex = 0;
    int cheapestAnaerobicIndex = 0;
    std::vector<int> workoutList;
    // std::sort(newWorkout_options.begin(), newWorkout_options.end(), &typeComparator);
    // for (int i = 1; i < newWorkout_options.size()-1; i++)
    // {
    //     if (newWorkout_options[i].getType() == CARDIO)
    //         if (newWorkout_options[i].getPrice() <= newWorkout_options[cheapestCardioIndex].getPrice() & workout_options[i].getId() < newWorkout_options[cheapestCardioIndex].getId())
    //             cheapestCardioIndex = i;
    //     else if (newWorkout_options[i].getType() == MIXED)
    //     {
    //         if (mixCounter = 0)
    //         {
    //             mixCounter++;
    //             expensiveMixIndex = i;
    //         }
    //         if (newWorkout_options[i].getPrice() >= newWorkout_options[expensiveMixIndex].getPrice() & newWorkout_options[i].getId() < newWorkout_options[expensiveMixIndex].getId())
    //             expensiveMixIndex = i;
                
    //     }
    //     else
    //     {
    //         if (newWorkout_options[i].getPrice() <= newWorkout_options[cheapestAnaerobicIndex].getPrice() & newWorkout_options[i].getId() < newWorkout_options[cheapestAnaerobicIndex].getId())
    //         {
    //             cheapestAnaerobicIndex = i;
    //         }
    //     }         
    // }

    for (int i = 1; i < int(cardioWorkout_options.size()); i++)
    {
        if ((*(cardioWorkout_options[i])).getPrice() < (*(cardioWorkout_options[cheapestCardioIndex])).getPrice())
        {
            cheapestCardioIndex = i;
        }
    }

    for (int i = 1; i < int(mixedWorkout_options.size()); i++)
    {
        if ((*(mixedWorkout_options[i])).getPrice() > (*(mixedWorkout_options[expensiveMixIndex])).getPrice())
        {
            expensiveMixIndex = i;
        }
    }

    for (int i = 1; i < int(anaerobicWorkout_options.size()); i++)
    {
        if ((*(anaerobicWorkout_options[i])).getPrice() < (*(anaerobicWorkout_options[cheapestAnaerobicIndex])).getPrice())
        {            
            cheapestAnaerobicIndex = i;
        }
    }

    workoutList.push_back((*(cardioWorkout_options[cheapestCardioIndex])).getId());
    workoutList.push_back((*(mixedWorkout_options[expensiveMixIndex])).getId());
    workoutList.push_back((*(anaerobicWorkout_options[cheapestAnaerobicIndex])).getId());
    return workoutList;
}

std::string FullBodyCustomer::toString() const{
    std::string str;
    str = this->getName() + "," + std::to_string(this->getId());
    return str;
}
