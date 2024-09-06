#include <bits/stdc++.h>
using namespace std;

void print_queue(priority_queue<tuple<int, int, string>, vector<tuple<int, int, string>>, greater<tuple<int, int, string>>> ready_queue) {
    cout << "{ ";
    while(!ready_queue.empty()) {
        cout << get<2>(ready_queue.top()) << ' ';
        ready_queue.pop();
    }
    cout << "}\n";
}

void SRTF(queue<tuple<int, int, string>> process, int n, int max_time) {
    priority_queue<tuple<int, int, string>, vector<tuple<int, int, string>>, greater<tuple<int, int, string>>> ready_queue;
    vector<int> arrival_time(n), burst_time(n), remaining_time(n), waiting_time(n, 0), response_time(n, -1), turnaround_time(n);
    int current_process = -1, end = 0, time = 0;
    string cpu_state = "Idle";

    // Initialize process times
    int idx = 0;
    while (idx < n) {
        auto p = process.front();
        arrival_time[idx] = get<0>(p);
        burst_time[idx] = get<1>(p);
        remaining_time[idx] = burst_time[idx];
        process.pop();
        process.push(p);
        idx++;
    }

    while (time < max_time) {
        // Add new arrivals to the ready queue
        while (!process.empty() && time == get<0>(process.front())) {
            cout << "EVENT: New process arrived at Time = " << time << "\n";
            ready_queue.push({remaining_time[stoi(get<2>(process.front()).substr(1)) - 1], get<1>(process.front()), get<2>(process.front())});
            process.pop();
            cout << "Ready Queue: "; print_queue(ready_queue);
            cout << "CPU State = " << cpu_state << '\n';
            cout << "------------------------\n";
        }

        // Preempt the current process if a process with shorter remaining time arrives
        if (current_process != -1 && !ready_queue.empty()) {
            int new_process = stoi(get<2>(ready_queue.top()).substr(1)) - 1;
            if (remaining_time[new_process] < remaining_time[current_process]) {
                cout << "EVENT: Process preempted at Time = " << time << "\n";
                ready_queue.push({remaining_time[current_process], burst_time[current_process], "P" + to_string(current_process + 1)});
                current_process = -1; // Preempt current process
                cpu_state = "Idle";
                cout << "Ready Queue: "; print_queue(ready_queue);
                cout << "CPU State = " << cpu_state << '\n';
                cout << "------------------------\n";
            }
        }

        if (current_process != -1) {
            remaining_time[current_process]--;
            if (remaining_time[current_process] == 0) {
                cout << "EVENT: Process Terminated at Time = " << time << "\n";
                turnaround_time[current_process] = time - arrival_time[current_process];
                waiting_time[current_process] = turnaround_time[current_process] - burst_time[current_process];
                current_process = -1; // Process finished
                cpu_state = "Idle"; // CPU is idle
                cout << "Ready Queue: "; print_queue(ready_queue);
                cout << "CPU State = " << cpu_state << '\n';
                cout << "------------------------\n";
            }
        }

        // Select new process if current one has finished or if no process is running
        if (current_process == -1 && !ready_queue.empty()) {
            current_process = stoi(get<2>(ready_queue.top()).substr(1)) - 1;
            end = time + burst_time[current_process];
            // Response time calculation
            if (response_time[current_process] == -1)
                response_time[current_process] = time - arrival_time[current_process];
            cpu_state = get<2>(ready_queue.top()); // Update CPU state
            ready_queue.pop();
        }

        time++;
    }

    cout << "EVENT: Process Terminated at Time = " << time << "\n";
    turnaround_time[current_process] = time - arrival_time[current_process];
    waiting_time[current_process] = turnaround_time[current_process] - burst_time[current_process];
    cpu_state = "Idle"; // CPU is idle
    cout << "Ready Queue: "; print_queue(ready_queue);
    cout << "CPU State = " << cpu_state << '\n';
    cout << "------------------------\n";

    // Print table header
    cout << "\nProcess Metrics:\n";
    cout << setw(10) << "Process"
         << setw(15) << "Arrival Time"
         << setw(15) << "Burst Time"
         << setw(15) << "Waiting Time"
         << setw(15) << "Response Time"
         << setw(20) << "Turnaround Time" << '\n';

    // Calculate and print the process metrics in table format
    double total_waiting_time = 0, total_response_time = 0, total_turnaround_time = 0;
    for(int i = 0; i < n; ++i) {
        total_waiting_time += waiting_time[i];
        total_response_time += response_time[i];
        total_turnaround_time += turnaround_time[i];
        cout << setw(10) << "P" + to_string(i+1)
             << setw(15) << arrival_time[i]
             << setw(15) << burst_time[i]
             << setw(15) << waiting_time[i]
             << setw(15) << response_time[i]
             << setw(20) << turnaround_time[i] << '\n';
    }

    // Print average metrics
    cout << "\nAverage Metrics:\n";
    cout << setw(15) << "Average Waiting Time: " << total_waiting_time / n << "\n";
    cout << setw(15) << "Average Response Time: " << total_response_time / n << "\n";
    cout << setw(20) << "Average Turnaround Time: " << total_turnaround_time / n << "\n";
}

int main() {
    ifstream processes("process_file");
    if (!processes.is_open()) {
        cerr << "No processes!" << endl;
        return 1;
    }
    int n;
    processes >> n;
    cout << "Number of processes = " << n << "\n------------------------\n";
    queue<tuple<int, int, string>> process;

    int max_time = 0;

    for (int i = 0; i < n; ++i) {
        int arrival, burst;
        string name = "P" + to_string(i+1);
        processes >> arrival >> burst;
        process.push({arrival, burst, name});
        max_time = max(max_time, arrival);
        max_time += burst;
    }

    SRTF(process, n, max_time);
    return 0;
}
