#include<bits/stdc++.h>
using namespace std;

void print_queue(queue<tuple<int , int , string>> ready_queue){
    cout << "{ ";
    while(!ready_queue.empty()){
        cout << get<2>(ready_queue.front()) << ' ';
        ready_queue.pop();
    }
    cout << "}\n";
}

void FCFS(queue<tuple<int , int , string>> process, int n, int max_time){
    queue<tuple<int , int , string>> ready_queue;
    string status = "Idle";
    int end = 0;
    
    // Data structures for time calculations
    vector<int> arrival_time(n), burst_time(n), waiting_time(n, 0), response_time(n, -1), turnaround_time(n);
    int current_process = -1;

    // Initialize process times
    int idx = 0;
    while (idx < n) {
        auto p = process.front();
        arrival_time[idx] = get<0>(p);
        burst_time[idx] = get<1>(p);
        process.pop();
        process.push(p);
        idx++;
    }

    for(int time = 0; time <= max_time; ++time){
        while(!process.empty() && time == get<0>(process.front())){ // add arrivals
            cout << "EVENT: New process arrived at Time = " << time << "\n";
            ready_queue.push(process.front());
            if(status == "Idle"){
                status = get<2>(ready_queue.front());
                current_process = stoi(status.substr(1)) - 1; // Extract process index from name
                end = time + burst_time[current_process];

                // Response time calculation
                if (response_time[current_process] == -1)
                    response_time[current_process] = time - arrival_time[current_process];

                ready_queue.pop();
            }
            cout << "Ready Queue: "; print_queue(ready_queue);
            cout << "Status = " << status << '\n';
            process.pop();
            cout << "------------------------\n";
        }
        if(time == end && current_process != -1){
            cout << "EVENT: Process Terminated at Time = " << time << "\n";
            
            // Turnaround and Waiting time calculations
            turnaround_time[current_process] = time - arrival_time[current_process];
            waiting_time[current_process] = turnaround_time[current_process] - burst_time[current_process];

            if(!ready_queue.empty()){
                status = get<2>(ready_queue.front());
                current_process = stoi(status.substr(1)) - 1; // Extract process index from name
                end = time + burst_time[current_process];

                // Response time calculation
                if (response_time[current_process] == -1)
                    response_time[current_process] = time - arrival_time[current_process];

                ready_queue.pop();
            }else{
                status = "Idle";
                current_process = -1;  // No process is currently being executed
            }
            cout << "Ready Queue: "; print_queue(ready_queue);
            cout << "Status = " << status << '\n';
            cout << "------------------------\n";
        }
    }

    // Print table header
    cout << "\nProcess Metrics:\n";
    cout << std::setw(10) << "Process" 
         << std::setw(15) << "Arrival Time" 
         << std::setw(15) << "Burst Time" 
         << std::setw(15) << "Waiting Time" 
         << std::setw(15) << "Response Time" 
         << std::setw(20) << "Turnaround Time" << '\n';
    
    // Calculate and print the process metrics in table format
    double total_waiting_time = 0, total_response_time = 0, total_turnaround_time = 0;
    for(int i = 0; i < n; ++i){
        total_waiting_time += waiting_time[i];
        total_response_time += response_time[i];
        total_turnaround_time += turnaround_time[i];
        cout << std::setw(10) << "P" + to_string(i+1) 
             << std::setw(15) << arrival_time[i] 
             << std::setw(15) << burst_time[i] 
             << std::setw(15) << waiting_time[i] 
             << std::setw(15) << response_time[i] 
             << std::setw(20) << turnaround_time[i] << '\n';
    }

    // Print average metrics
    cout << "\nAverage Metrics:\n";
    cout << std::setw(15) << "Average Waiting Time: " << total_waiting_time / n << "\n";
    cout << std::setw(15) << "Average Response Time: " << total_response_time / n << "\n";
    cout << std::setw(20) << "Average Turnaround Time: " << total_turnaround_time / n << "\n";
}

int main(){
    ifstream processes("process_file");
    if (!processes.is_open()) {
        cerr << "No processes!" << endl;
        return 1;
    }
    int n;
    processes >> n;
    cout << "Number of processes = " << n << "\n------------------------\n";
    queue<tuple<int , int , string>> process;

    int max_time = 0;

    for(int i = 0; i < n; ++i){
        int arrival , burst;
        string name = "P" + to_string(i+1);
        processes >> arrival >> burst;
        process.push({arrival , burst , name});
        max_time = max(max_time , arrival);
        max_time += burst;
    }

    FCFS(process , n , max_time);
    return 0;
}
