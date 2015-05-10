#pragma once

#ifndef _HELIOS_TASK_MANAGER
#define _HELIOS_TASK_MANAGER

// Elementary types
#include "../Types.h"
#include <thread>
#include "../LogF.h"
#include "../Assert.h"
#include <queue>

// std thread
#include <atomic>
#include <mutex>
#include <condition_variable>


namespace Helios
{

  //===================================================================================================
  // single task parametrd

  class ITaskParametrs: public RefCount, public OLinkBase<ITaskParametrs>
  {
  public:
    ITaskParametrs(){};
    ~ITaskParametrs(){};

    USE_CASTING_ROOT;
  };

  //===================================================================================================
  // single task
  class TaskList;
  class Task : public RefCount, public OLinkBase<Task>
  {
  private:
    int _id;
    static int _idCounter;

    enum class ETaskListState
    {
      TSReady,
      TSInProgress,
      TSFinished
    } _state;

  protected:
    Ref<ITaskParametrs> _parametrs;
    virtual void Pefrom() = 0;

    OLink<TaskList> _parentList;

  public:
    Task(ITaskParametrs *param, TaskList* parentList = nullptr);
    ~Task();
    virtual void Run() final;

    //-----------------------------------------------------------------------------
    __forceinline int GetID() {return _id;};
    __forceinline void SetTaskInProgress(){ Assert(_state == ETaskListState::TSReady); _state = ETaskListState::TSInProgress;};
    __forceinline void SetTaskFinished(){ Assert(_state == ETaskListState::TSInProgress); _state = ETaskListState::TSFinished;};

    USE_CASTING_ROOT;
  };

  //===================================================================================================
  //List of task requested to be done

  class TaskList : public RefCount, public OLinkBase<TaskList>
  {
  public:
    //-----------------------------------------------------------------------------
    //!list of tasks to proceed
    std::queue<Ref<Task>> _listQueue;
    //!manipulating with queue lock
    std::mutex _processingList_lock;  
    //! parent thread waiting condition - wait until all is done
    std::condition_variable _allIsDone_cv;
    //! finisted tasks count down 
    std::atomic<int> _remainingToFinish;

  public: 
    TaskList();

    //-----------------------------------------------------------------------------
    //* add new task to list
    void AddTask(Task *task);
    //! return next task to perform
    void GetNextTask(Ref<Task> &retTask);
    //! report that single task was finished, wake up parent thread if it was last one
    void ReportFinishedTask();
    //! true in all tasks are finished
    __forceinline bool IsFinished() const {return _remainingToFinish.load() == 0;};

    USE_CASTING_ROOT;
  };



  //===================================================================================================
  //All threads manager

  class TaskManager : public Singleton<TaskManager>,  public RefCount, public OLinkBase<TaskManager>
  {
  private:
    bool _terminate;

#pragma region DRAW
    //!main draw thread
    std::thread _renderThread;
    //!render task
    Ref<Task> _renderTask;  
#pragma endregion


#pragma region SIMULATE
    //!main simulation thread
    std::thread _simulateThread;
    //!simulation task
    Ref<Task> _simulationTask;  
#pragma endregion


#pragma region WORKERS
#define NUMBER_OF_WORKERS  4
    //! thread workers
    std::thread _workersThreads[NUMBER_OF_WORKERS];
    //!task list queue
    std::queue<Ref<TaskList>> _taskListQueue;
    //!manipulating with queue lock
    std::mutex _processingTaskQueue_lock;  
    //! woker waiting condition - wait until there is some new task
    std::condition_variable _waitingForTask_cv;
#pragma endregion

  public:
    TaskManager(): _terminate(false) {};

    //-----------------------------------------------------------------------------
    // simulate and render threads
  public:
    void SetSimualtionTask(Task *simulationTask) {_simulationTask = simulationTask;};
    void SetRenderTask(Task *renderTask) {_renderTask = renderTask;};
    //!Run render and simulation
    void RunMainThreads();

    //-----------------------------------------------------------------------------
    // workers methods 
  private:
    //!Get next task (any task)
    void GetNextTask(Ref<Task> &retTask);
    //! return next task to perform (from gien list)
    void GetNextTask(Ref<Task> &retTask, TaskList *taskList);

    __forceinline bool IsQueuListEmpty() const {return _taskListQueue.empty();}; 

    //-----------------------------------------------------------------------------
    //!Perform next task while there is some
    void ProcessTasks();
    //!Perform tasks from given list while there is some, wait until everthing is finished
    void ProcessTaskList(TaskList *taskList);

  public:
    //-----------------------------------------------------------------------------
    //! Run list of tasks, thread will not wait or participate
    void RunTaskListAsynchronous(TaskList *taskList);
    //! Run list of tasks, thread will wait and participate
    void RunTaskListSynchronous (TaskList *taskList);

    void Terminate();
    bool IsRunnung();

    USE_CASTING_ROOT;
  };

} // Helios namespace

#endif // _HELIOS_TASK_MANAGER
