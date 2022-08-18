# EDF_Scheduler_Project

EDF scheduler implementation project report 

Hyperperiod

100ms

CPU load

((1*2)+(1*2)+1+(1*5)+(10*5)+12)/100= 72%

Tasks 

Schedulability check 

Button_1_Monitor

✅

Button_2_Monitor

✅

Periodic_Transmitter

✅

Uart_Receiver

✅

Load_1_Simulation

✅

Load_2_Simulation

✅

*** Seems that all the tasks execute before its deadline.

System working as expected the tasks that have earliest deadline execute first cause it's have high priority.

Tasks execute before its deadline, but not at its time cause of priority, but it's been called after its deadline finished.
