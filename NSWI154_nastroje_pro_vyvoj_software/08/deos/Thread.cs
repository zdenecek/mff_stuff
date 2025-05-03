using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using log4net;

[assembly: log4net.Config.XmlConfigurator(Watch = true)]

namespace DEOS
{
    
    class Thread
    {
        const int   threadStatusNotCreated = 0;
        const int   threadStatusDormant = 1;
        const int   threadStatusActive = 2;
        const int   threadStatusKernelExceptionPending = 3;
        ThreadListNode     timeoutNode;
        public ThreadListNode     startOfPeriodWaitNode;
        public ThreadListNode preemptionNode;
        Budget             itsBudget;
        public Budget itsCurrentBudget;
        public int itsLastExecution;
        public int itsLastCompletion;
        public StartOfPeriodEvent itsPeriodicEvent;
        public int itsPeriodIndex;
        int                itsCurrentPriority;
        int                itsCreationStatus;
        string             itsName;
        DEOSThread         body;

    
        static ILog logger = log4net.LogManager.GetLogger(typeof(Thread));

        public Thread (string name) 
        {
            itsName = name;


            timeoutNode = new ThreadListNode(this);
            startOfPeriodWaitNode = new ThreadListNode(this);
            preemptionNode = new ThreadListNode(this);
            itsBudget = new Budget();
            itsCreationStatus = threadStatusNotCreated;
            Assertion.AddThread(this);

            if (name == "main") 
            {
                body = new DEOSMainThread(this);
            } 
            else if (name == "idle") 
            {
                body = new DEOSIdleThread(this);
            } 
            else 
            {
                body = new DEOSThread(this);
            }

            logger.Debug($"Successfully created thread {name}");
        }

        // Finish Thread Class

        public DEOSThread GetBody()
        {
            return body;
        }

        public void SetCPUBudget(int b)
        {
            itsBudget.SetTotalBudgetInUsec(b);
        }

        public void SetCurrentPriority(int p)
        {
            itsCurrentPriority = p;
        }

        public bool IsIdle()
        {
            return (itsName == "idle");
        }

        public bool IsMain()
        {
            return (itsName == "main");
        }

        public bool ConceptualObjectConstructor(int period)
        {
            itsPeriodIndex = period;
            itsCurrentPriority = Scheduler.PriorityForPeriodIndex(itsPeriodIndex);
            itsPeriodicEvent = StartOfPeriodEvent.EventForPeriodIndex(itsPeriodIndex);
            itsCurrentBudget = itsBudget;
            itsCreationStatus = threadStatusDormant;

            return true;
        }

        public void ConceptualObjectDestructor()
        {
            itsCreationStatus = threadStatusNotCreated;
        }

        public Budget Budget()
        {
            return itsBudget;
        }

        public void CompleteForPeriod()
        {
            WaitForNextTriggeringEvent();
            itsLastCompletion = itsPeriodicEvent.CurrentPeriod();
        }

        public void CpuAllowanceExceeded()
        {
            if (this == Scheduler.IdleThread())
            {
                StartChargingCPUTime();
            }
            else
            {
                WaitForNextPeriod();
            }
        }

        public int CpuBudget()
        {
            return itsBudget.TotalBudgetInUsec();
        }

        public int CurrentPriority()
        {
            return itsCurrentPriority;
        }

        public void InitiateStopAndDelete() 
        {
            Thread current = Scheduler.CurrentThread();

            if (current != this) 
            {
                DEOS.println("Current running thread (" + current + 
                                 ") != thread trying to delete itself!" + this);
                return;
            }

            current.StopThread();
            current.itsCreationStatus = threadStatusNotCreated;
            DEOSProcess.DeallocateCPUBudgetForThread(current);

            Scheduler.ScheduleOtherThread();
        }

        public int PeriodIndex()
        {
            return itsPeriodIndex;
        }

        public void StartChargingCPUTime () 
        {
            DEOS.println("Thread.startChargingCPUTime");
            int cp = itsPeriodicEvent.CurrentPeriod();
            int budget;

            if (IsIdle()) 
            {
                budget = itsCurrentBudget.TotalBudgetInUsec();
            } 
            else 
            {
                if (cp == itsLastExecution) 
                {
                    budget = itsCurrentBudget.RemainingBudgetInUsec();
                } 
                else 
                {
                    budget = itsCurrentBudget.TotalBudgetInUsec();
                    itsLastExecution = cp;

                    int remainingTime = itsCurrentBudget.RemainingBudgetInUsec();
                }
            }
            itsCurrentBudget.SetRemainingBudgetInUsec(budget);

            Assertion.Check();

            itsCurrentBudget.StartTimer();
        }

        public void StartThread(int theCPUBudget)
        {
            itsCurrentPriority = Scheduler.PriorityForPeriodIndex(itsPeriodIndex);
            itsBudget.SetTotalBudgetInUsec(theCPUBudget);
            StartThreadInternal();
            itsLastCompletion = itsPeriodicEvent.CurrentPeriod() - 1;
            WaitForNextTriggeringEvent(); // assumes critical!
            itsLastExecution = itsPeriodicEvent.CurrentPeriod();
            itsLastCompletion = itsPeriodicEvent.CurrentPeriod();

            logger.Debug($"Successfully started thread {this.itsName}");

        }

        public void StartThreadInternal()
        {
            itsCreationStatus = threadStatusActive;
            itsBudget.SetRemainingBudgetInUsec(itsBudget.TotalBudgetInUsec());
            itsCurrentBudget = itsBudget;
        }

        public void StopChargingCPUTime(int bonus)
        {
            int remainingTime = bonus +
                                DEOS.theTimer.GetRemainingTime(
                                      DEOS.systemClock.GetCurrentTime());
            itsCurrentBudget.SetRemainingBudgetInUsec(remainingTime);
        }

        public void StopThread()
        {
            itsLastCompletion = itsPeriodicEvent.CurrentPeriod();
            itsCreationStatus = threadStatusDormant;
            Assertion.RemoveThread(this);

            logger.Debug($"Successfully stopped thread {this.itsName}");
        }

        public String ToString()
        {
            return itsName;
        }

        public void WaitForNextPeriod()
        {  
            int interruptState = CPU.EnterCritical();
            CompleteForPeriod();

            Scheduler.ScheduleOtherThread();
            CPU.ExitCritical(interruptState);
        }

        public void WaitForNextTriggeringEvent()
        {
            itsPeriodicEvent.MakeThreadWait(this);
        }
    }
}
