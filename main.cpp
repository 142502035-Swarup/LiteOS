#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int priority;
    int completion;
    int waiting;
    int turnaround;
};

void calculateTimes(vector<Process>& p){
    for(auto &pr : p){
        pr.turnaround = pr.completion - pr.arrival;
        pr.waiting = pr.turnaround - pr.burst;
    }
}

void printTable(vector<Process> p){

    float totalWT=0,totalTAT=0;

    cout<<"\nPID\tAT\tBT\tPR\tCT\tWT\tTAT\n";

    for(auto pr:p){
        cout<<"P"<<pr.pid<<"\t"<<pr.arrival<<"\t"<<pr.burst<<"\t"
            <<pr.priority<<"\t"<<pr.completion<<"\t"
            <<pr.waiting<<"\t"<<pr.turnaround<<"\n";

        totalWT+=pr.waiting;
        totalTAT+=pr.turnaround;
    }

    cout<<"\nAverage Waiting Time: "<<totalWT/p.size()<<endl;
    cout<<"Average Turnaround Time: "<<totalTAT/p.size()<<endl;
}

void printGantt(vector<int> order){

    cout<<"\nGantt Chart\n| ";
    for(int p:order)
        cout<<"P"<<p<<" | ";
    cout<<"\n";
}

void FCFS(vector<Process> p){

    sort(p.begin(),p.end(),[](Process a,Process b){
        return a.arrival<b.arrival;
    });

    int time=0;
    vector<int> order;

    for(auto &pr:p){

        if(time<pr.arrival)
            time=pr.arrival;

        order.push_back(pr.pid);

        time+=pr.burst;
        pr.completion=time;
    }

    calculateTimes(p);
    printGantt(order);
    printTable(p);
}

void SJF(vector<Process> p){

    int n=p.size();
    vector<bool> done(n,false);

    int time=0,completed=0;
    vector<int> order;

    while(completed<n){

        int idx=-1;
        int minBurst=1e9;

        for(int i=0;i<n;i++){
            if(!done[i] && p[i].arrival<=time && p[i].burst<minBurst){
                minBurst=p[i].burst;
                idx=i;
            }
        }

        if(idx==-1){
            time++;
            continue;
        }

        order.push_back(p[idx].pid);

        time+=p[idx].burst;
        p[idx].completion=time;

        done[idx]=true;
        completed++;
    }

    calculateTimes(p);
    printGantt(order);
    printTable(p);
}

void SRTF(vector<Process> p){

    int n=p.size();

    for(int i=0;i<n;i++)
        p[i].remaining=p[i].burst;

    int time=0,completed=0;
    vector<int> order;

    while(completed<n){

        int idx=-1;
        int minRem=1e9;

        for(int i=0;i<n;i++){
            if(p[i].arrival<=time && p[i].remaining>0 && p[i].remaining<minRem){
                minRem=p[i].remaining;
                idx=i;
            }
        }

        if(idx==-1){
            time++;
            continue;
        }

        order.push_back(p[idx].pid);

        p[idx].remaining--;
        time++;

        if(p[idx].remaining==0){
            p[idx].completion=time;
            completed++;
        }
    }

    calculateTimes(p);
    printGantt(order);
    printTable(p);
}

void PriorityScheduling(vector<Process> p){

    int n=p.size();
    vector<bool> done(n,false);

    int time=0,completed=0;
    vector<int> order;

    while(completed<n){

        int idx=-1;
        int bestPriority=1e9;

        for(int i=0;i<n;i++){
            if(!done[i] && p[i].arrival<=time && p[i].priority<bestPriority){
                bestPriority=p[i].priority;
                idx=i;
            }
        }

        if(idx==-1){
            time++;
            continue;
        }

        order.push_back(p[idx].pid);

        time+=p[idx].burst;
        p[idx].completion=time;

        done[idx]=true;
        completed++;
    }

    calculateTimes(p);
    printGantt(order);
    printTable(p);
}

void RoundRobin(vector<Process> p,int quantum){

    int n=p.size();

    for(int i=0;i<n;i++)
        p[i].remaining=p[i].burst;

    queue<int> q;
    vector<bool> inQueue(n,false);
    vector<int> order;

    int time=0;

    q.push(0);
    inQueue[0]=true;

    while(!q.empty()){

        int i=q.front();
        q.pop();

        order.push_back(p[i].pid);

        int exec=min(quantum,p[i].remaining);

        p[i].remaining-=exec;
        time+=exec;

        for(int j=0;j<n;j++){
            if(!inQueue[j] && p[j].arrival<=time){
                q.push(j);
                inQueue[j]=true;
            }
        }

        if(p[i].remaining>0)
            q.push(i);
        else
            p[i].completion=time;
    }

    calculateTimes(p);
    printGantt(order);
    printTable(p);
}

int main(){

    int n;

    cout<<"Advanced CPU Scheduling Simulator\n";
    cout<<"Enter number of processes: ";
    cin>>n;

    vector<Process> p(n);

    for(int i=0;i<n;i++){

        p[i].pid=i+1;

        cout<<"\nProcess P"<<p[i].pid<<endl;

        cout<<"Arrival Time: ";
        cin>>p[i].arrival;

        cout<<"Burst Time: ";
        cin>>p[i].burst;

        cout<<"Priority (lower value = higher priority): ";
        cin>>p[i].priority;
    }

    int choice;

    while(true){

        cout<<"\n===== Scheduling Menu =====\n";
        cout<<"1. FCFS\n";
        cout<<"2. SJF\n";
        cout<<"3. SRTF\n";
        cout<<"4. Priority Scheduling\n";
        cout<<"5. Round Robin\n";
        cout<<"6. Exit\n";

        cout<<"Enter choice: ";
        cin>>choice;

        if(choice==1) FCFS(p);
        else if(choice==2) SJF(p);
        else if(choice==3) SRTF(p);
        else if(choice==4) PriorityScheduling(p);
        else if(choice==5){

            int quantum;
            cout<<"Enter Time Quantum: ";
            cin>>quantum;

            RoundRobin(p,quantum);
        }
        else break;
    }

    return 0;
}
