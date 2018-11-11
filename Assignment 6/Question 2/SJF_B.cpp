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
		bool isInList;

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

			completionTime = 0;
			waitingTime = -1;
			turnAroundTime = 0;
			isInIO = false;
			isInList = false;
			CPU_Burst_Time_original = CPU_Burst_Time;
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

    vector <int> waiting_list;
    queue <int> waiting_queue_io;

	int k = 0;
	waiting_list.push_back(k);
	processes[k].isInList = true;
	timeSpend = processes[k].arrival_Time;
	k++;

	for (int i = 0; i < timeSpend; ++i)
	{
		string s = "<"+ to_string(timeSpend) +  "--->";
    	ganttChartIO.push_back(s);
		ganttChartBurst.push_back(s);
	}

	while( k < n && processes[k].arrival_Time <= timeSpend  )
	{
	    if(processes[k].CPU_Burst_Time > 0 && processes[k].isInIO == false && processes[k].isInList == false)
	    {
	    	waiting_list.push_back(k);
	    	processes[k].isInList = true;
	    }
		k++;
	}
	int minBurstTimeIndex = waiting_list[0] ;
	bool minBurstTimeAvailable = false;
	for (int i = 0; i < waiting_list.size() ; ++i)
	{
		k = waiting_list[i];
    	if( processes[minBurstTimeIndex].CPU_Burst_Time >= processes[k].CPU_Burst_Time )
    	{
    		minBurstTimeIndex = k;
    		minBurstTimeAvailable = true;
    	}
	}

    while( !done(processes) )
    {
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

    	if( minBurstTimeAvailable )
    	{
	    	if( processes[minBurstTimeIndex].waitingTime == -1 )
	    	{
	    		processes[minBurstTimeIndex].waitingTime = timeSpend - processes[minBurstTimeIndex].arrival_Time ;
	    	}
	    	else if( processes[minBurstTimeIndex].isInIO == false && processes[minBurstTimeIndex].CPU_burst_time_interval == processes[minBurstTimeIndex].CPU_burst_time_temp )
	    	{
	    		processes[minBurstTimeIndex].waitingTime += timeSpend - processes[minBurstTimeIndex].IO_completed_at - 1 ;
	    		cout << "process : " << processes[minBurstTimeIndex].processNumber << " | timeSpend: " << timeSpend  <<  " | IO_completed_at " << processes[minBurstTimeIndex].IO_completed_at << endl;		
	    	}

	    	processes[minBurstTimeIndex].CPU_burst_time_temp -= 1;
			string s;
	    	s = "<"+ to_string(timeSpend) +  "-P" + to_string(processes[minBurstTimeIndex].processNumber) + ">";
	    	
	    	ganttChartBurst.push_back(s);

			processes[minBurstTimeIndex].CPU_Burst_Time = processes[minBurstTimeIndex].CPU_Burst_Time - 1;

			if( processes[minBurstTimeIndex].CPU_Burst_Time == 0 )	// process completed
			{
				processes[minBurstTimeIndex].completionTime = timeSpend;
				processes[minBurstTimeIndex].turnAroundTime = processes[minBurstTimeIndex].completionTime - processes[minBurstTimeIndex].arrival_Time;
				minBurstTimeAvailable = false;
			}
	    	else if( processes[minBurstTimeIndex].CPU_burst_time_temp == 0 )
	    	{
	    		processes[minBurstTimeIndex].isInIO = true;
		    	waiting_queue_io.push(minBurstTimeIndex);
				processes[minBurstTimeIndex].CPU_burst_time_temp = processes[minBurstTimeIndex].CPU_burst_time_interval;

				minBurstTimeIndex = waiting_list[0] ;
				int minBurstTime = INT_MAX;
				minBurstTimeAvailable = false;
				for (int i = 0; i < waiting_list.size() ; ++i)
				{
					k = waiting_list[i];
			    	if( minBurstTime >= processes[k].CPU_Burst_Time && processes[k].isInIO == false && processes[k].CPU_Burst_Time > 0 )
			    	{
			    		minBurstTimeIndex = k;
			    		minBurstTime = processes[k].CPU_Burst_Time;
			    		minBurstTimeAvailable = true;
			    	}
				}
	    	}
		}
		else
		{
			string s;
			s = "<"+ to_string(timeSpend) +  "--->";
	    	ganttChartBurst.push_back(s);	

			minBurstTimeIndex = waiting_list[0] ;
			int minBurstTime = INT_MAX;
			minBurstTimeAvailable = false;
			for (int i = 0; i < waiting_list.size() ; ++i)
			{
				k = waiting_list[i];
		    	if( minBurstTime >= processes[k].CPU_Burst_Time && processes[k].isInIO == false && processes[k].CPU_Burst_Time > 0 )
		    	{
		    		minBurstTimeIndex = k;
		    		minBurstTime = processes[k].CPU_Burst_Time;
		    		minBurstTimeAvailable = true;
		    	}
			}
		
		}

    	timeSpend++;    	
		k = 0;
		while( k < n && processes[k].arrival_Time <= timeSpend  )
		{
		    if(processes[k].CPU_Burst_Time > 0 && processes[k].isInIO == false && processes[k].isInList == false)
		    {
		    	waiting_list.push_back(k);
		    	processes[k].isInList = true;
		    }
			k++;
		}
    }
    
    cout << endl << endl ;
    cout << "arrival_Time , CPU Burst time , IO_burst_time , CPU_burst_time_interval , completionTime , waitingTime , turnAroundTime " << endl ;
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
    cout << "IO BURST  : ";
	for (int i = 0; i < ganttChartIO.size(); ++i)
    {
    	cout << ganttChartIO[i] ;
    }

    putOutput(ganttChartIO,ganttChartBurst);
    cout << endl << endl;
	return 0;
}
