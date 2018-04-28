#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define REP_SIZE 10000

int fibo(int x, char str[1024], int flag, int filler[])
{
	sleep(0.5);

	if (x == 0)
	{
		if (flag == 0)
		{
			system(str);
			system("sudo rmmod mymod");
			system("tail /var/log/syslog -n 8");
	
			flag = 1;
		}
	
		return 0;
	}

	else if (x == 1)
		return 1;

	else{
		int t[100];
		t[7] = fibo(x - 1, str, flag, t) + fibo(x - 2, str, flag, NULL);
	}
}

int main() 
{
	pid_t this_pid = getpid(); 
	printf("this pid = %d\n", this_pid);

	char insmodstr[1024];

	sprintf(insmodstr, "sudo insmod mymod.ko processid=%d", this_pid);

	//system("sudo rmmod mymod"); // remove any previous module	
	

	// initial log
	system(insmodstr);
	system("sudo rmmod mymod");
	system("tail /var/log/syslog -n 8");
	//system("dmesg");

	// call to recursive function
	fibo(1000, insmodstr, 0, NULL);
	
		
	return 0;
}
