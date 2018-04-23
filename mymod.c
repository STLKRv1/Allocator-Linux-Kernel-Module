#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>
#include <linux/mm_types.h>

//Source:http://venkateshabbarapu.blogspot.com.tr/2012/09/process-segments-and-vma.html
static int processid = 1;



static void print_mem(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count = 0;
        mm = task->mm;

	unsigned long rss = get_mm_rss(mm);
	
	struct vm_area_struct* cur = mm->mmap;
	
	unsigned long code_start = cur->vm_start;
	unsigned long code_end = cur->vm_end;
	unsigned long codesize = code_end - code_start;

	cur = cur->vm_next;
	unsigned long data_start = cur->vm_start;
	unsigned long data_end = cur->vm_end;
	unsigned long datasize = data_end - data_start;

	cur = cur->vm_next;
	cur = cur->vm_next;
	unsigned long heap_start = cur->vm_start;
	unsigned long heap_end = cur->vm_end;
	unsigned long heapsize = heap_end - heap_start;

	cur = cur->vm_next;
	cur = cur->vm_next;
	cur = cur->vm_next;
	while(cur->vm_flags != VM_STACK)
	{
		printk("ehehehehe\n");
		cur=cur->vm_next;
	}
	unsigned long stack_start = cur->vm_start;
	unsigned long argusize = (unsigned long)mm->arg_end - (unsigned long)mm->arg_start;
	unsigned long envisize = (unsigned long)mm->env_end - (unsigned long)mm->env_start;

	// to be fixed and checked from proc/<pid>/status !
	
        printk("\nCode: 		start = 0x%lx, end = 0x%lx, size = %lu bytes"
               "\nData: 		start = 0x%lx, end = 0x%lx, size = %lu bytes"
		"\nHeap:		start = 0x%lx, end = 0x%lx, size = %lu bytes"
		 "\nMain Args:	start = 0x%lx, end = 0x%lx, size = %lu bytes"
		 "\nEnviroment var:  start = 0x%lx, end = 0x%lx, size = %lu bytes"
                 "\nStack: 		start = 0x%lx"
		 "\nNumber of frames used = %lu \n"
		 "Total virtual memory used = %lu \n",
                 code_start, code_end, codesize,
                 data_start, data_end, datasize,
		 heap_start, heap_end, heapsize,
		 mm->arg_start, mm->arg_end, argusize,
		 mm->env_start, mm->env_end, envisize,
                 stack_start,
	 	 rss,
		 mm->total_vm);
}

static int mm_exp_load(void)
{
        struct task_struct *task;

        //printk("\nGot the process id to look up as %d.\n", processid);

        for_each_process(task){
                if ( task->pid == processid) {
                        printk("%s[pid:%d]\n", task->comm, task->pid);
                        print_mem(task);
                }
        }

        return 0;
}

static void mm_exp_unload(void)
{
        printk("\nPrint segment information module exiting.\n");
}

module_init(mm_exp_load);
module_exit(mm_exp_unload);
module_param(processid, int, 0);

MODULE_AUTHOR ("Dank Memes, 420@lol.com");
MODULE_DESCRIPTION ("Print segment information");
MODULE_LICENSE("GPL");
