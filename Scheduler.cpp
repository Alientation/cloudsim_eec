//
//  Scheduler.cpp
//  CloudSim
//
//  Created by ELMOOTAZBELLAH ELNOZAHY on 10/20/24.
//

#include "Scheduler.hpp"

static bool migrating = false;

void Scheduler::Init() {
    // Find the parameters of the clusters
    // Get the total number of machines
    // For each machine:
    //      Get the type of the machine
    //      Get the memory of the machine
    //      Get the number of CPUs
    //      Get if there is a GPU or not
    //
    SimOutput("Scheduler::Init(): Total number of machines is " + to_string(Machine_GetTotal()), 3);
    SimOutput("Scheduler::Init(): Initializing scheduler", 1);

    for (unsigned int i = 0; i < Machine_GetTotal(); i++) {
        CPUType_t cpu = Machine_GetCPUType(i);
        machines.push_back(i);

        vms.push_back(VM_Create(LINUX, cpu));
        VM_Attach(vms.back(), machines.back());
        vms.push_back(VM_Create(LINUX_RT, cpu));
        VM_Attach(vms.back(), machines.back());

        if (cpu == ARM || cpu == X86) {
            vms.push_back(VM_Create(WIN, cpu));
            VM_Attach(vms.back(), machines.back());
        } else if (cpu == POWER) {
            vms.push_back(VM_Create(AIX, cpu));
            VM_Attach(vms.back(), machines.back());
        }

        MachineInfo_t info = Machine_GetInfo(i);
        for (unsigned int j = 0; j < info.num_cpus; j++) {
            Machine_SetCorePerformance(i, j, P0);
        }
        Machine_SetState(i, S0);
    }
}

void Scheduler::MigrationComplete(Time_t time, VMId_t vm_id) {
    // Update your data structure. The VM now can receive new tasks
}

void Scheduler::NewTask(Time_t now, TaskId_t task_id) {
    // Get the task parameters
    //  IsGPUCapable(task_id);
    //  GetMemory(task_id);
    //  RequiredVMType(task_id);
    //  RequiredSLA(task_id);
    //  RequiredCPUType(task_id);
    // Decide to attach the task to an existing VM,
    //      vm.AddTask(taskid, Priority_T priority); or
    // Create a new VM, attach the VM to a machine
    //      VM vm(type of the VM)
    //      vm.Attach(machine_id);
    //      vm.AddTask(taskid, Priority_t priority) or
    // Turn on a machine, create a new VM, attach it to the VM, then add the task
    //
    // Turn on a machine, migrate an existing VM from a loaded machine....
    //
    // Other possibilities as desired
    // Priority_t priority = (task_id == 0 || task_id == 64)? HIGH_PRIORITY : MID_PRIORITY;
    // if(migrating) {
    //     VM_AddTask(vms[0], task_id, priority);
    // }
    // else {
    //     VM_AddTask(vms[task_id % active_machines], task_id, priority);
    // }// Skeleton code, you need to change it according to your algorithm

    VMId_t best_vm;
    VMId_t possible_vm;
    bool found_possible = false;
    bool found_best = false;
    MachineInfo_t best_machine;
    MachineInfo_t possible_machine;

    TaskInfo_t task_info = GetTaskInfo(task_id);
    for (size_t i = 0; i < vms.size(); i++) {
        VMInfo_t vm_info = VM_GetInfo(vms[i]);
        if (vm_info.cpu == task_info.required_cpu && vm_info.vm_type == task_info.required_vm) {
            MachineInfo_t machine_info = Machine_GetInfo(vm_info.machine_id);
            if (!found_possible || possible_machine.memory_used * machine_info.memory_size > machine_info.memory_used * possible_machine.memory_size) {
                possible_vm = vms[i];
                possible_machine = machine_info;
                found_possible = true;
            }
            if (!found_best || (best_machine.memory_used * machine_info.memory_size > machine_info.memory_used * best_machine.memory_size && machine_info.active_tasks < machine_info.num_cpus)) {
                best_machine = machine_info;
                best_vm = vms[i];
                found_best = true;
            }
        }
    }

    if (!found_possible) {
        cout << "NOT FOUND" << endl;
        return;
    }

    VM_AddTask(found_best ? best_vm : possible_vm, task_id, HIGH_PRIORITY);
}

void Scheduler::PeriodicCheck(Time_t now) {
    // This method should be called from SchedulerCheck()
    // SchedulerCheck is called periodically by the simulator to allow you to monitor, make decisions, adjustments, etc.
    // Unlike the other invocations of the scheduler, this one doesn't report any specific event
    // Recommendation: Take advantage of this function to do some monitoring and adjustments as necessary
}

void Scheduler::Shutdown(Time_t time) {
    // Do your final reporting and bookkeeping here.
    // Report about the total energy consumed
    // Report about the SLA compliance
    // Shutdown everything to be tidy :-)
    for(auto & vm: vms) {
        VM_Shutdown(vm);
    }
    SimOutput("SimulationComplete(): Finished!", 4);
    SimOutput("SimulationComplete(): Time is " + to_string(time), 4);
}

void Scheduler::TaskComplete(Time_t now, TaskId_t task_id) {
    // Do any bookkeeping necessary for the data structures
    // Decide if a machine is to be turned off, slowed down, or VMs to be migrated according to your policy
    // This is an opportunity to make any adjustments to optimize performance/energy
    SimOutput("Scheduler::TaskComplete(): Task " + to_string(task_id) + " is complete at " + to_string(now), 4);
}

// Public interface below

static class Scheduler Scheduler;

void InitScheduler() {
    SimOutput("InitScheduler(): Initializing scheduler", 4);
    Scheduler.Init();
}

void HandleNewTask(Time_t time, TaskId_t task_id) {
    SimOutput("HandleNewTask(): Received new task " + to_string(task_id) + " at time " + to_string(time), 4);
    Scheduler.NewTask(time, task_id);
    Scheduler.n_tasks_requested++;
}

void HandleTaskCompletion(Time_t time, TaskId_t task_id) {
    SimOutput("HandleTaskCompletion(): Task " + to_string(task_id) + " completed at time " + to_string(time), 4);
    Scheduler.TaskComplete(time, task_id);
}

void MemoryWarning(Time_t time, MachineId_t machine_id) {
    // The simulator is alerting you that machine identified by machine_id is overcommitted
    // SimOutput("MemoryWarning(): Overflow at " + to_string(machine_id) + " was detected at time " + to_string(time), 0);
    Scheduler.n_memory_warnings++;
}

void MigrationDone(Time_t time, VMId_t vm_id) {
    // The function is called on to alert you that migration is complete
    SimOutput("MigrationDone(): Migration of VM " + to_string(vm_id) + " was completed at time " + to_string(time), 4);
    Scheduler.MigrationComplete(time, vm_id);
    migrating = false;
}

void SchedulerCheck(Time_t time) {
    // This function is called periodically by the simulator, no specific event
    SimOutput("SchedulerCheck(): SchedulerCheck() called at " + to_string(time), 4);
    Scheduler.PeriodicCheck(time);
    static unsigned counts = 0;
    counts++;
    // if(counts == 10) {
        // migrating = true;
        // VM_Migrate(1, 9);
    // }
}

void SimulationComplete(Time_t time) {
    // This function is called before the simulation terminates Add whatever you feel like.
    cout << "SLA violation report" << endl;
    cout << "SLA0: " << GetSLAReport(SLA0) << "%" << endl;
    cout << "SLA1: " << GetSLAReport(SLA1) << "%" << endl;
    cout << "SLA2: " << GetSLAReport(SLA2) << "%" << endl;     // SLA3 do not have SLA violation issues
    cout << "Total Energy " << Machine_GetClusterEnergy() << "KW-Hour" << endl;
    cout << "Simulation run finished in " << double(time)/1000000 << " seconds" << endl;
    cout << "Tasks requested: " << Scheduler.n_tasks_requested << endl;
    cout << "SLA violations: " << Scheduler.n_sla_violations << endl;
    cout << "Memory warnings: " << Scheduler.n_memory_warnings << endl;
    SimOutput("SimulationComplete(): Simulation finished at time " + to_string(time), 4);

    Scheduler.Shutdown(time);
}

void SLAWarning(Time_t time, TaskId_t task_id) {
    Scheduler.n_sla_violations++;
}

void StateChangeComplete(Time_t time, MachineId_t machine_id) {
    // Called in response to an earlier request to change the state of a machine
}
