#include <bits/stdc++.h>
#include <string>
using namespace std;
class process
{
	public:
		int arrival_Time;
		int CPU_Burst_Time;
		int CPU_Burst_Time_original;
		int IO_burst_time;
		int CPU_burst_time_interval;
		int processNumber;

		int CPU_burst_time_temp;
		int IO_burst_time_temp;
		int IO_completed_at;
		bool isInIO ;
		bool isInCPU;

		int completionTime ;
		int waitingTime ;
		int turnAroundTime ;

		process(int pn=0,int at=0,int bt=0,int iobt=0,int bti=0)
		{
			processNumber = pn;
			arrival_Time = at;
			CPU_Burst_Time = bt;
			IO_burst_time = iobt;
			CPU_burst_time_interval = bti;

			CPU_Burst_Time_original = CPU_Burst_Time;
			completionTime = 0;
			waitingTime = -1;
			turnAroundTime = 0;
			isInIO = false;
			isInCPU = false;
			CPU_burst_time_temp = CPU_burst_time_interval;
			IO_burst_time_temp = IO_burst_time;
			IO_completed_at = 0;
		}

};

bool comparator(const process& lhs, const process& rhs) {
   return lhs.arrival_Time < rhs.arrival_Time;
}

bool done( vector <process> &processes )
{
	for (int i = 0; i < processes.size(); ++i)
	{
		if( processes[i].CPU_Burst_Time > 0 )
		{
			return false;
		}
	}
	return true;
}
string to_string(int n)
{
    stringstream ss;
    ss << n;
    string s=ss.str();
    return s;
}
int stoi(string s)
{
    int value;
    istringstream text_stream(s);
    text_stream >> value;
    return value;
}
int getInput(vector <process> &processes)
{
    string line;
    ifstream fin;
    fin.open("input.txt");
    int n;
    int i = 0;
    while (fin)
    {
        getline(fin, line);
        if( fin )
        {
        	if( i == 0 )
        	{
        		cout << "Number of processes : " << line << endl;
        		n = stoi(line);
        	}
        	else
        	{
    			cout << "arrival_Time , CPU Burst time , IO_burst_time , CPU_burst_time_interval for processes " << i-1 << endl;
        		cout << line << endl;
				vector <int> tokens;
	            stringstream check1(line);
	            string intermediate;
	            while(getline(check1, intermediate, ' '))
	            {
                    tokens.push_back(stoi(intermediate));
	            }
				int arrival_Time , CPU_burst_time_interval , CPU_Burst_Time , IO_burst_time;
				arrival_Time = tokens[0];
				CPU_Burst_Time = tokens[1];
				IO_burst_time = tokens[2];
				CPU_burst_time_interval = tokens[3];
				processes.push_back(process(i-1 , arrival_Time , CPU_Burst_Time , IO_burst_time , CPU_burst_time_interval));
        	}
        	i++;
        }
    }
    fin.close();
    return n;
}
void putOutput(vector <string> &ganttChartIO , vector<string> &ganttChartBurst)
{
    ofstream fout;
    fout.open("output.txt");
    fout << "CPU Burst : " ;
    for (int i = 0; i < ganttChartBurst.size(); ++i)
    {
    	fout << ganttChartBurst[i] ;
    }
    fout << endl ;
    fout << "IO BURST  : " ;
    for (int i = 0; i < ganttChartIO.size(); ++i)
    {
    	fout << ganttChartIO[i] ;
    }
    fout << endl ;
    fout.close();
}

int main(int argc, char const *argv[])
{
	vector <process> processes;
	vector <string> ganttChartIO;
	vector <string> ganttChartBurst;

	int n = getInput(processes);

	int timeSpend = 0;

    sort(processes.begin(), processes.end(), &comparator);

    queue <int> waiting_queue;
    queue <int> waiting_queue_io;

    while( !done(processes) )
    {
		int k = 0;
		while( k < n && processes[k].arrival_Time <= timeSpend )
		{
		    if(processes[k].CPU_Burst_Time > 0 && processes[k].isInIO == false && processes[k].isInCPU == false)
		    {
		    	waiting_queue.push(k);
		    	processes[k].isInCPU = true;
		    }
			k++;
		}
		if( !waiting_queue_io.empty() )
		{
			int prc = waiting_queue_io.front();
	    	processes[prc].IO_burst_time_temp -= 1;
			string s;
	    	s = "<"+ to_string(timeSpend) +  "-P" + to_string(processes[prc].processNumber) + ">";
	    	ganttChartIO.push_back(s);

	    	if( processes[prc].IO_burst_time_temp == 0 )
	    	{
	    		waiting_queue_io.pop();
	    		processes[prc].isInIO = false;
	    		processes[prc].IO_completed_at = timeSpend;
		    	processes[prc].IO_burst_time_temp = processes[prc].IO_burst_time;
	    	}
		}
		else
		{
			string s;
			s = "<"+ to_string(timeSpend) +  "--->";
	    	ganttChartIO.push_back(s);
		}

    	if( !waiting_queue.empty() )
    	{
	    	int prc = waiting_queue.front();
	    	if( processes[prc].waitingTime == -1 )
	    	{
	    		processes[prc].waitingTime = timeSpend - processes[prc].arrival_Time ;
	    	}
	    	else if( processes[prc].isInIO == false && processes[prc].CPU_burst_time_interval == processes[prc].CPU_burst_time_temp )
	    	{
	    		processes[prc].waitingTime += timeSpend - processes[prc].IO_completed_at - 1 ;
	    		cout << "process : " << processes[prc].processNumber << " | timeSpend: " << timeSpend  <<  " | IO_completed_at " << processes[prc].IO_completed_at << endl;		
	    	}

	    	processes[prc].CPU_burst_time_temp -= 1;
			processes[prc].CPU_Burst_Time = processes[prc].CPU_Burst_Time - 1;
			
			string s;
	    	s = "<"+ to_string(timeSpend) +  "-P" + to_string( processes[prc].processNumber ) + ">";
	    	ganttChartBurst.push_back(s);
			if( processes[prc].CPU_Burst_Time == 0 )	// process completed
			{
				processes[prc].completionTime = timeSpend;
				processes[prc].turnAroundTime = processes[prc].completionTime - processes[prc].arrival_Time;
	    		waiting_queue.pop();
		    	processes[prc].isInCPU = false;
	    	}
	    	else if( processes[prc].CPU_burst_time_temp == 0 )
	    	{
	    		waiting_queue.pop();
		    	processes[prc].isInCPU = false;

	    		processes[prc].isInIO = true;
		    	waiting_queue_io.push(prc);
				processes[prc].CPU_burst_time_temp = processes[prc].CPU_burst_time_interval;
	    	}
		}
		else
		{
			string s;
			s = "<"+ to_string(timeSpend) +  "--->";
			ganttChartBurst.push_back(s);
		}

    	timeSpend++;
    }
    cout << endl << endl ;
    cout << "process no , arrival_Time , CPU Burst time , IO_burst_time , CPU_burst_time_interval , completionTime , waitingTime , turnAroundTime " << endl ;
    for (int i = 0; i < n; ++i)
    {
    	cout <<processes[i].processNumber << "\t" << processes[i].arrival_Time << "\t" << processes[i].CPU_Burst_Time_original << "\t" << processes[i].IO_burst_time << "\t" << processes[i].CPU_burst_time_interval << "\t" << processes[i].completionTime << "\t" << processes[i].waitingTime << "\t" << processes[i].turnAroundTime << endl;
    }
    cout << endl << endl ;
    cout << "CPU BURST : " ;
    for (int i = 0; i < ganttChartBurst.size(); ++i)
    {
    	cout << ganttChartBurst[i] ;
    }
    cout << endl ;
    cout << "IO BURST  : " ;
	for (int i = 0; i < ganttChartIO.size(); ++i)
    {
    	cout << ganttChartIO[i] ;
    }
    putOutput(ganttChartIO,ganttChartBurst);
    cout << endl << endl;
	return 0;
}
