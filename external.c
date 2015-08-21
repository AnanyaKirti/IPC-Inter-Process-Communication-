#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main(int argc, char const *argv[])
{
	printf("Process %s Created\n", argv[2]);
	int msgget(key_t key, int msgflg);              // functoion definitions.
	int external_temp, total;                       // for the centeral and external net temperatures
	int msqid;                                      // for the mailbox
	int base = 80;

	external_temp = atoi(argv[1]);
	printf("temperature of process %s: %d\n",argv[2],external_temp );

	// start the que
	int pid = atoi(argv[2]) + base;
	printf("%d\n", pid);
	msqid = msgget(pid, 0600 | IPC_CREAT);
	if (msqid < 0){
		printf("error!, Message Que!\n" );
		return -1;
	}
	printf("Mailbox has been created by the %d process.\n" ,pid);

	// initialise the struct.
	struct message{
		long priority;
		int temp;
		int pid;
		int stable;
	} msgp , msgp_rcvd;

	msgp.priority = 1;
	msgp.temp = external_temp;
	msgp.pid = pid;
	msgp.stable = 0;                                // system is not stable.



	int stat, dummy , rcqid;
	



	while(msgp.stable == 0){
		// send the message.
		stat = msgsnd(msqid, &msgp,sizeof(msgp)-sizeof(long), 0);
		// printf("Process %s sent to the centeral! \n", argv[2]);

		rcqid = msgget(base , 0666 | IPC_CREAT);
		stat = msgrcv(rcqid, &msgp_rcvd,sizeof(msgp_rcvd)-sizeof(long), 1, 0);
		// printf("Process %s recieved from the centeral! \n", argv[2]);

		if (msgp_rcvd.stable == 1){
			printf("The stable temperature has been recieved!\n");
			msgp.stable = 1;
			break;
		}
		else{
			msgp.temp = ((msgp.temp * 3)+ (2 * msgp_rcvd.temp))/5;
			// printf("the current temperature is: %d\n", msgp.temp);
			// printf("STABLE %d\n", msgp_rcvd.stable);
		}

	}

	//  delete the message que.
	struct msqid_ds msqid_ds, *buf;
	buf = & msqid_ds;
	stat = msgctl(msqid, IPC_RMID, buf);

	printf("%s Process Ended!\n", argv[2]);
	printf("The stable temperature is %d\n", msgp.temp);							
	exit(0);
	return 1;
}