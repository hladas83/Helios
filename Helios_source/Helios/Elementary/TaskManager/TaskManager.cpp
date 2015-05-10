#include "TaskManager.h"

namespace Helios
{

  //===================================================================================================
  //Single task parametrs

  DEFINE_CASTING(ITaskParametrs);

  //===================================================================================================
  //Single task requested to be done

  DEFINE_CASTING(Task);

  int Task::_idCounter = 0;

  //------------------------------------------------------------------------------  

  Task::Task(ITaskParametrs *param, TaskList* parentList) 
    : _parametrs(param), _state(ETaskListState::TSReady) 
  { 
    _id = _idCounter++;
    if(parentList) _parentList = parentList;
  };

  //-----------------------------------------------------------------------------

  Task::~Task() 
  {
    Assert(_state == ETaskListState::TSFinished); 
  };

  //-----------------------------------------------------------------------------

  void Task::Run() 
  { 
    Assert(_state == ETaskListState::TSInProgress);
    Pefrom(); 
    if(_parentList) _parentList->ReportFinishedTask();
    SetTaskFinished();
  };

  //===================================================================================================
  //List of task requested to be done

  DEFINE_CASTING(TaskList)

  TaskList::TaskList()
  {
  };

  //------------------------------------------------------------------------------  

  void TaskList::AddTask(Task *task)
  {
    _listQueue.push(task); 
    _remainingToFinish.exchange(_listQueue.size());
  };

  //------------------------------------------------------------------------------  

  void TaskList::GetNextTask(Ref<Task> &retTask)
  {
    if(!_listQueue.empty())
    {
      retTask = _listQueue.front();
      retTask->SetTaskInProgress();
      _listQueue.pop();
    }
  };

  //------------------------------------------------------------------------------  

  void TaskList::ReportFinishedTask() 
  { 
    _processingList_lock.lock();
    _remainingToFinish.fetch_sub(1);
    if(_remainingToFinish.load() == 0) _allIsDone_cv.notify_all();
    _processingList_lock.unlock();
  };

  //===================================================================================================
  //All threads manager

  DEFINE_CASTING(TaskManager)


  //------------------------------------------------------------------------------  

  //!Start main threads (render and simulation)
  void TaskManager::RunMainThreads()
  {
    //initialize render and simulation threads
    _simulationTask->SetTaskInProgress();
    _simulateThread = std::thread( &Task::Run, std::ref(_simulationTask));

    _renderTask->SetTaskInProgress();
    _renderThread = std::thread( &Task::Run, std::ref(_renderTask) );

    //initialize workers
    for(int i = 0 ; i < NUMBER_OF_WORKERS; i++) _workersThreads[i] = std::thread( &TaskManager::ProcessTasks, this);

    _simulateThread.join();
    _renderThread.join();

	  //render and simulation is finished, terminate workers workers and wait for them to finish
    Terminate();
    for(int i = 0 ; i < NUMBER_OF_WORKERS; i++) _workersThreads[i].join();

  };

  //------------------------------------------------------------------------------  

  //!Run list of tasks
  void TaskManager::RunTaskListAsynchronous(TaskList *taskList)
  {
    if(!IsRunnung()) return;
    if(!taskList) return;
    //lock queue
    _processingTaskQueue_lock.lock();
    // add new list
    _taskListQueue.push(taskList);
    _waitingForTask_cv.notify_all();
    //unlock queue
    _processingTaskQueue_lock.unlock();
    //exit
  };

  //------------------------------------------------------------------------------  

  //!Run list of tasks
  void TaskManager::RunTaskListSynchronous(TaskList *taskList)
  {
    if(!IsRunnung()) return;
    if(!taskList) return;
    {
      // lock queue
      std::lock_guard<std::mutex> guard(_processingTaskQueue_lock);
      // add new list
      _taskListQueue.push(taskList);
      _waitingForTask_cv.notify_all();
    }
    //current thread is working as well (process task only from given list)
    ProcessTaskList(taskList);
  };

  //------------------------------------------------------------------------------  

  void TaskManager::Terminate()
  {
    _terminate = true;

    // lock queue
    std::lock_guard<std::mutex> guard(_processingTaskQueue_lock);
    //wake up all workers
    _waitingForTask_cv.notify_all();
  }

  //------------------------------------------------------------------------------  

  bool TaskManager::IsRunnung()
  {
    return !_terminate;
  }

  //------------------------------------------------------------------------------  

  void TaskManager::GetNextTask(Ref<Task> &retTask)
  {
	//_processingTaskQueue_lock must be locked
    while (!_taskListQueue.empty())
    {
      _taskListQueue.front()->GetNextTask(retTask);
      if(retTask) return;
      else _taskListQueue.pop();
    }
  };

  //------------------------------------------------------------------------------  

  //!Perform next task
  void TaskManager::ProcessTasks()
  {
    while(IsRunnung())
    {
      std::unique_lock<std::mutex> lk(_processingTaskQueue_lock);

      Ref<Task> task = nullptr;
      GetNextTask(task);
      if(task)
      {
        lk.unlock();
        task->Run();
      }
      else
      {
        _waitingForTask_cv.wait(lk, [this]{return !IsQueuListEmpty() || !IsRunnung();});
      }
    }
  };

  //------------------------------------------------------------------------------  

  void TaskManager::GetNextTask(Ref<Task> &retTask, TaskList *taskList)
  {
	  //_processingTaskQueue_lock must be locked
	  if (taskList) taskList->GetNextTask(retTask);
  };

  //------------------------------------------------------------------------------  

  //!Perform next task (working onyl with given task list)
  void TaskManager::ProcessTaskList(TaskList *taskList)
  {
    if(!taskList) return;
    while(!_terminate)
    {
      Ref<Task> task = nullptr;
      {
        std::lock_guard<std::mutex> guard(_processingTaskQueue_lock);
        GetNextTask(task);
      }
      if(task) task->Run();
      else break;
    }

	  //wait for other workers to finish work on this list
    std::unique_lock<std::mutex> lk(taskList->_processingList_lock);
    if(taskList->IsFinished()) 
    {
      //not needed - raii
      lk.unlock();
    }
    else
    {
      taskList->_allIsDone_cv.wait(lk, [taskList]{return taskList->IsFinished();});
    }
  };

} // Helios namespace