#include<stdio.h>
void main()
{
    float process[500],arrivalTime[500],burstTime[500],CompletionTime[500],waitingTime[500],turnAroundTime[500];
    int n = 0,i = 0 ;
    float averageWatingTime = 0, averageTurnAroundTime = 0;

    printf("*** FCFS Scheduling Algorithm Using Arrival Time ***\n");
    printf("\nEnter the number of process : ");
    scanf("%d",&n);

    printf("Enter the Arrival time and Burst time.\n\n");
    printf("\tA_Time B_Time\n");
    for(i = 0 ; i < n ; i++){
        process[i]=i+1;
        printf("P%d :\t", i+1);
        scanf("%f\t%f",&arrivalTime[i],&burstTime[i]);
    }
/*
    printf("\n\nProcess\tA_Time\tB_Time\n");
    for(i = 0 ; i < n ; i++){
        printf("P[%d]\t%.2f\t%.2f\n",i,arrivalTime[i],burstTime[i]);
    }
*/

    waitingTime[0] = 0;;
    CompletionTime[0] = burstTime[0]+arrivalTime[0];
    turnAroundTime[0] = CompletionTime[0] - arrivalTime[0];

    for( i = 1 ; i < n ; i++){
        CompletionTime[i] = CompletionTime[i-1] + burstTime[i];
        turnAroundTime[i] = CompletionTime[i] - arrivalTime[i];
        waitingTime[i] = turnAroundTime[i] - burstTime[i];
    }
    
    for(i = 0 ; i < n ; i++){
        averageWatingTime = averageWatingTime + waitingTime[i];
        averageTurnAroundTime = averageTurnAroundTime + turnAroundTime[i];
    }
    
    printf("\tA_time\tB_time\tC_time\tT_A_Time\tW_time\n");
    
    for(i = 0 ; i < n ; i++){
        printf("P[%d]\t%.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\n",i,arrivalTime[i],burstTime[i],CompletionTime[i],turnAroundTime[i],waitingTime[i]);
    }
    printf("\nAverage waiting time : %0.2f",averageWatingTime/n);
    printf("\nAverage turn around time : %0.2f",averageTurnAroundTime/n);
}
