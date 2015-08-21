# IPC
The **IPC(inter process comunication)** has been implemented by using a message passing.
The Message passing is implemened by using system calls**(for LINUX)**.

The system calls used are **msgget(), msgsnd(), msgrcv()**. msgget() generate a unique que id for a given que name.(creates the que if it's not present.), This que id is required to send and recieve the messages.

## Synopsis
The code implemened is a simple real life example. There is a centeral process ,and 4 external processes , each one has it's own temperature.
The system is trying to attain a stable temperature, such that the temperature of all the processes is same. This requires two-way communication between the processes.


## Code Example
To run the application, run the 4 external processes and then run the centeral process. Ensure the que is empty before. You need to give a unique ID to each process (1-4)
```C
./external TEMP ID &
./centeral CENT_TEMP &
````


## Brief Explanation of code.
The external process genreates a **msquid** for it's sending que.
The external process add a struct to it's sending que.
The centeral process looks up the msquid for the external process and gets the message from the external sending que.

Centeral then decides if the system is stable, if not it changes it's temperature.
Centeral then sends its struct to the external processes.

If the system is stable all the processes show the final temperature and terminate.



## Test

```C
an example of the code would be like:
$ ./external 100 1 &
$ ./external 22 2 &
$ ./external 50 3 &
$ ./external 40	4 &
$ ./centeral 60 &
```
