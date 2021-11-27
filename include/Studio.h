#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "../include/Workout.h"
#include "../include/Trainer.h"
#include "../include/Action.h"

class Studio{
public:
    Studio();
    ~Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer *> getTrainers();
    Studio(const Studio &other);//Copy Cunstractor
    Studio(Studio &&other);//Move Constructor
    Studio& operator=(const Studio &other);//Copy Assignment
    Studio& operator=( Studio &&other);//Move Assignment

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    int customerId;
};

#endif