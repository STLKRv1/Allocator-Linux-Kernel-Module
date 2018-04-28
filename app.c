#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define REP_SIZE 10000

int fibo(int x, char str[1024], int flag)
{
	sleep(1);

	if (x == 0)
	{
		if (flag == 0)
		{
			system(str);
			system("sudo rmmod mymod");
			system("dmesg");
	
			flag = 1;
		}
	
		return 0;
	}

	else if (x == 1)
		return 1;

	else{
		char *ok = malloc(15);
		fibo(x - 1, str, flag) + fibo(x - 2, str, flag);
		free(ok);
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
	//system("tail /var/log/syslog -n 8");
	system("dmesg");

	// call to recursive function
	fibo(10, insmodstr, 0);
	
		
	return 0;
}
