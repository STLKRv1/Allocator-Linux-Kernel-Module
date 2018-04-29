#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define REP_SIZE 10000

int flag = 0;

int fibo(int x, char str[1024], int filler[])
{
	// sleep(0.5);

	if (x == 0)
	{
		if (flag == 0)
		{
			system(str);
			system("sudo rmmod mymod");
			system("tail /var/log/syslog -n -2");
			printf("──────────────────────────────────────────────\n");
	
			flag = 1;
		}
	
		return 0;
	}

	else if (x == 1)
	{
		return 1;
	}

	else{
		int *ptr_1;
		int *ptr_2;
		int *ptr_3;
		ptr_1 = (int *)malloc(50000);
		ptr_2 = (int *)malloc(50000);
		ptr_3 = (int *)malloc(50000);

		int t[100];

		int random_index = rand() % 100 + 1; // generate random index to work around the optimization

		t[random_index] = fibo(x - 1, str, t) + fibo(x - 2, str, NULL);

		free(ptr_1);
		free(ptr_2);
		free(ptr_3);

		return (t[random_index]);
	}
}

int main() 
{
	pid_t this_pid = getpid(); 

	char insmodstr[1024];

	sprintf(insmodstr, "sudo insmod mymod.ko processid=%d", this_pid);

	// initial log
	printf("\nInitial readings for PID: %d\n", this_pid);
	system(insmodstr);
	system("sudo rmmod mymod");
	system("tail /var/log/syslog -n -2");

	printf("──────────────────────────────────────────────\nReadings after some recursive calls: \n");

	// call to recursive function
	fibo(100, insmodstr, NULL);	
		
	return 0;
}
