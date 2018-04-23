#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() 
{
	pid_t lol = getpid();

	char insmodstr[1024];

	sprintf(insmodstr, "sudo insmod mymod.ko processid=%d", lol);	

	// for (i => [10, 1000, 10000, 1000000, ...])

		// MAKE SOME ALLOC HERE ?!?
	
	// log this process to /var/log/syslog
	system("sudo rmmod mymod");	
	system(insmodstr);

	system("tail /var/log/syslog -n 8");

		
	return 0;
}
