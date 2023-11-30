#pragma once
#include<string>
#include<stdexcept>
using namespace std;
class Task{
    string title; // Title of the Task
    string description; // description of the task

    int subTasksSize=0;
    Task *parentTask=nullptr;
    Task *firstSubTask=nullptr;
    Task *lastSubTask=nullptr;
    Task *nextTask=nullptr;
    Task *previousTask=nullptr;

    /**
     * adds the pointed task to the sub tasks of the invoked task
     * This will not copy the task, It will directly use the pointed task.
    */
    static void addSubTaskNoClone(Task *root,Task *task,bool addAtStart=false);
    public:
    Task(string title="", string descrition="");
    ~Task();

    inline void set_title(string newTitle){
        this->title=newTitle;
    }
    inline string get_title(){
        return title;
    }

    inline void set_description(string newdescription){
        this->description=description;
    }
    inline string get_description(){
        return description;
    }

    /**
     * Returns the number of sub tasks of the invoked task
    */
    inline int getSubTasksSize(){
        return subTasksSize;
    }
    /**
     * Returns the first sub task of the invoked task
     * returns nullptr if no child task exist
     * 
     * To get next sub task see the function getNextTask()
    */
    inline Task *getFirstSubTask(){
        return firstSubTask;
    }

    /**
     * Returns the next task that has the same parent task as the invoked task
    */
    inline Task *getNextTask(){
        return nextTask;
    }
    
    /**
     * Returns a pointer to last sub task
    */
    inline Task *getLastSubTask(){
        return lastSubTask;
    }

    /**
     * creates a deep copy of the task and returns it
    */
    static Task *clone(const Task &task);
    
    /**
     * Adds the given task as a sub task to the the invoked task.
     * By default it adds task at the end, to add at the start pass `true` as the second parameter.
    */
    void addSubTask(const Task &task,bool addAtStart=false);

    /**
     * Removes the indexed sub task, From the begining.
     * 
    */
    void removeSubTask(int index);
    /**
     * deletes the the provided task,
     * (removes all subtasks, remove parent connections ..etc)
     * here before Task is the before sibling of the taskToBeRemoved.
     * if the task to be removed is the first task, then it would be a nullptr
    */
    static void removeSubTask(Task *taskToBeRemoved, Task *beforeTask);
};