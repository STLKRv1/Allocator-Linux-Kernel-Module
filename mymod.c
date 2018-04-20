#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>

static int pid_mem = 1;



static void print_mem(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count = 0;
        mm = task->mm;
        printk("\nThis mm_struct has %d vmas.\n", mm->map_count);

	unsigned long rss = get_mm_rss(mm);

        /*for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
                printk ("\nVma number %d: \n", ++count);
                printk("  Starts at 0x%lx, Ends at 0x%lx\n",
                          vma->vm_start, vma->vm_end);
        }*/ // commented out since we probably dont need it.


	unsigned long codesize = mm->end_code - mm->start_code;	
	unsigned long datasize = mm->end_data - mm->start_data;
	unsigned long heapsize = mm->brk - mm->start_brk;
	unsigned long argusize = mm->arg_end - mm->arg_start;
	unsigned long envisize = mm->env_end - mm->env_start;

	// to be fixed and checked from proc/<pid>/status !
	
        printk("\nCode  Segment start = 0x%lx, end = 0x%lx, size = %lu \n"
                 "Data  Segment start = 0x%lx, end = 0x%lx, size = %lu \n"
		 "Heap  Segment start = 0x%lx, end = 0x%lx, size = %lu \n"
		 "Arg.  Segment start = 0x%lx, end = 0x%lx, size = %lu \n"
		 "Envi  Segment start = 0x%lx, end = 0x%lx, size = %lu \n"
                 "Stack Segment start = 0x%lx \n"
		 "RSS = %lu \n"
		 "Total vir. mem. = %lu \n",
                 mm->start_code, mm->end_code, codesize,
                 mm->start_data, mm->end_data, datasize,
		 mm->start_brk, mm->brk, heapsize,
		 mm->arg_start, mm->arg_end, argusize,
		 mm->env_start, mm->env_end, envisize,
                 mm->start_stack,
	 	 rss,
		 mm->total_vm);
}

static int mm_exp_load(void)
{
        struct task_struct *task;

        printk("\nGot the process id to look up as %d.\n", pid_mem);

        for_each_process(task){
                if ( task->pid == pid_mem) {
                        printk("%s[%d]\n", task->comm, task->pid);
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
module_param(pid_mem, int, 0);

MODULE_AUTHOR ("Dank Memes, 420@lol.com");
MODULE_DESCRIPTION ("Print segment information");
MODULE_LICENSE("GPL");
