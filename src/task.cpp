#include "task.h"

Task::Task(string title,string description){
    this->title=title;
    this->description=description;
}
Task::~Task(){
    while(firstSubTask){
        Task *nextTask=firstSubTask->nextTask;
        delete firstSubTask;
        firstSubTask=nextTask;
    }
}

Task* Task::clone(const Task &task){
    Task *root=new Task(task.title,task.description);
    Task *childTask=task.firstSubTask;
    while(childTask){
        root->addSubTaskNoClone(root,clone(*childTask));
        childTask=childTask->nextTask;
    }
    return root;
}

void Task::addSubTask(const Task &task,bool addAtStart){
    Task *taskClone=Task::clone(task);
    addSubTaskNoClone(this,taskClone,addAtStart);
}
void Task::addSubTaskNoClone(Task *root,Task *task,bool addAtStart){
    if(root->subTasksSize){
        if(addAtStart){
            task->nextTask=root->firstSubTask;
            root->firstSubTask=task;
        }
        else{
            root->lastSubTask->nextTask=task;
            root->lastSubTask=task;
        }
    }
    else{
        root->firstSubTask=task;
        root->lastSubTask=task;
    }
    root->subTasksSize++;
    task->parentTask=root;
}

void Task::removeSubTask(int index){
    if(index>=subTasksSize || index<0){
        throw std::runtime_error("Index out of bound, trying to remove task index "
                                    +to_string(index)+" of size "+to_string(subTasksSize));
    }
    Task *beforeTask=nullptr;
    Task *subTask=getFirstSubTask();
    for(int i=0;i<index;i++){
        beforeTask=subTask;
        subTask=subTask->getNextTask();
    }
    removeSubTask(subTask,beforeTask);
}

void Task::removeSubTask(Task *taskToBeRemoved,Task *beforeTask){
    if(!taskToBeRemoved->parentTask){
        throw std::runtime_error("Trying to remove a task with no parent task is not allowed");
    }

    if(beforeTask){
        beforeTask->nextTask=taskToBeRemoved->nextTask;
    }
    else{
        //this is the first task in the list
        taskToBeRemoved->parentTask->firstSubTask=taskToBeRemoved->nextTask;
    }
    if(!taskToBeRemoved->nextTask){
        // There is no other next task, this is the last sub task
        // so we need to make parent's last sub task point to the before task
        taskToBeRemoved->parentTask->lastSubTask=beforeTask;
    }
    taskToBeRemoved->parentTask->subTasksSize--;
    delete taskToBeRemoved;
}