/******************************************************************************
** Centeral.c 
** Ananya Kirti
*****************************************************************************/



#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(int argc, char const *argv[])
{
	printf("Process Created\n");
	int msgget(key_t key, int msgflg);		// functoion definitions.
	int central_temp, total;			// for the centeral and external net temperatures
	int msqid;					// for the mailbox
	int base = 80;

	central_temp = atoi(argv[1]);
	printf("Centeral temperature : %d\n",central_temp );

	// start the que
	msqid = msgget(base, 0666 | IPC_CREAT);			// creating the mailbox, if not present.
	printf("Mailbox has been created by the centeral process.\n");
	if (msqid <= 0){					// check for errors
		printf("error!, Message Que!\n" );
		return -1;
	}


	// initialise and define the given struct.
	struct message{
		long priority;
		int temp;
		int pid;
		int stable;
	} msgp , msgp_rcvd;

	msgp.priority = 1;
	msgp.temp = central_temp;
	msgp.stable = 0;				// system is not stable.



	int stat, rcqid;				// rcquid for receiving que id
	int external_temperature[4] = {0,0,0,0};	// starting temperature for the external ones are stored in a array
	int i, check = 0, dummy, dummy_sum;
	while(msgp.stable == 0){
		// recieve from all the external processes
		for (i = 0; i < 4; i++){
			rcqid = msgget(base + 1 + i , 0666 | IPC_CREAT);
			stat = msgrcv(rcqid, &msgp_rcvd,sizeof(msgp_rcvd)-sizeof(long), 1, 0);
			external_temperature[i] = msgp_rcvd.temp;	// stored in the array.
			// printf("recieved temperature: %d \n",msgp_rcvd.temp );
		}
		

		// check if system is stable.
		dummy = external_temperature[0];
		check = 1;
		dummy_sum = 0;
		for(i = 0; i < 4; i++){
			// printf("%d , %d , temperature \n",i, external_temperature[i] );
			if (dummy != external_temperature[i]){
				check = 0; 		//not reached final stage.
			}
			dummy_sum = dummy_sum + external_temperature[i];
		}
		if (dummy != msgp.temp){
			check = 0;			// external temperature is not equal to the external temperature
		}
		if (check == 1){
			// we are done!
			msgp.stable = 1;
		}
		else{ 
			msgp.temp = ((msgp.temp * 2) + dummy_sum ) / 6;
		}

		for (i = 0; i < 4; i++){
			stat = msgsnd(msqid, &msgp,sizeof(msgp)-sizeof(long), 0);
		}
		if (check == 1){
			// printf("STABLE: %d\n", msgp.stable);
			break;
		}


	}
	
	// delete the message que
	struct msqid_ds msqid_ds, *buf;
	buf = & msqid_ds;
	stat = msgctl(msqid, IPC_RMID, buf);

	printf("Process Ended!\n");
	printf("The temprature is : %d\n", msgp.temp);
	exit(0);
	return 1;	
}