#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/sched/signal.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <asm/page.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

//Source:http://venkateshabbarapu.blogspot.com.tr/2012/09/process-segments-and-vma.html
static int processid = 1;



static void print_mem(struct task_struct *task)
{
	printk("\n ---- Print mem ---- \n");
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count = 0;
        mm = task->mm;

	unsigned long rss = get_mm_rss(mm);

	unsigned long argusize = (unsigned long)mm->arg_end - (unsigned long)mm->arg_start;
	unsigned long envisize = (unsigned long)mm->env_end - (unsigned long)mm->env_start;
	
	struct vm_area_struct* cur = mm->mmap;
	
	unsigned long code_start = cur->vm_start;
	unsigned long code_end = cur->vm_end;
	unsigned long codesize = code_end - code_start;

	cur = cur->vm_next;
	unsigned long data_start = cur->vm_start;
	unsigned long data_end = cur->vm_end;
	unsigned long datasize = data_end - data_start;

	cur = cur->vm_next; //move over bss
	cur = cur->vm_next;
	unsigned long heap_start = cur->vm_start;
	unsigned long heap_end = cur->vm_end;
	unsigned long heapsize = heap_end - heap_start;

	cur = cur->vm_next;
	
	unsigned long stack_end;

	while(cur->vm_next != NULL)
		cur=cur->vm_next;

	unsigned long stack_start = ((cur->vm_prev)->vm_prev)->vm_start; //find stack start
	stack_end = ((cur->vm_prev)->vm_prev)->vm_end;

	unsigned long stacksize = stack_end - stack_start;


	// to be fixed and checked from proc/<pid>/status !
	
        printk( "\n%sCode:\t\t%sstart = 0x%lx, %send = 0x%lx, %ssize = %lu bytes"
                "\n%sData:\t\t%sstart = 0x%lx, %send = 0x%lx, %ssize = %lu bytes"
		"\n%sMain Args:\t%sstart = 0x%lx, %send = 0x%lx, %ssize = %lu bytes"
		"\n%sEnviroment var:\t%sstart = 0x%lx, %send = 0x%lx, %ssize = %lu bytes"
		"\n%sNumber of frames used = %lu %s"
		"\n%sTotal virtual memory used = %lu %s"
		"\n%sHeap:\t\t%sstart = 0x%lx, %send = 0x%lx, %ssize = %lu bytes"
		"\n%sStack:\t\t%sstart = 0x%lx, %send = 0x%lx, %ssize = %lu bytes",
                KBLU, KGRN, code_start,    KRED, code_end,    KWHT, codesize,
                KBLU, KGRN, data_start,    KRED, data_end,    KWHT, datasize,
		KBLU, KGRN, mm->arg_start, KRED, mm->arg_end, KWHT, argusize,
		KBLU, KGRN, mm->env_start, KRED, mm->env_end, KWHT, envisize,
	 	KBLU, rss, KWHT,
		KBLU, mm->total_vm, KWHT,
		KBLU, KGRN, heap_start,    KRED, heap_end,    KWHT, heapsize,
		KBLU, KGRN, stack_start,   KRED, stack_end,   KWHT, stacksize);

	printk("Mem printed.");
}

static void print_toptable(struct task_struct *task)
{
	printk("\n ---- Print top table ---- \n");
	struct mm_struct *mm;
        struct vm_area_struct *vma;
        mm = task->mm;

	pgd_t *pgd_ptr;
	unsigned long addr;
	int i;

	for (i = 0; i < 512; i++)
	{
		addr = i * (1UL << 39);
		pgd_ptr = pgd_offset(mm, addr);
		unsigned long content = pgd_ptr->pgd;
		printk("___________________________");
		printk("%sAddr: %lu  -  with entry number: %d \n%sPGD: %lu %s", KYEL, addr, i, KGRN, content, KYEL);
	}
}

static int mm_exp_load(void)
{
        struct task_struct *task;

        //printk("\nGot the process id to look up as %d.\n", processid);

        for_each_process(task){
                if ( task->pid == processid) {
                        printk("%s[pid:%d]\n", task->comm, task->pid);
			print_toptable(task);
                        print_mem(task);
			
                }
        }

        return 0;
}

static void mm_exp_unload(void)
{
        //printk("\nPrint segment information module exiting.\n");
}

module_init(mm_exp_load);
module_exit(mm_exp_unload);
module_param(processid, int, 0);

MODULE_DESCRIPTION ("Print segment information");
MODULE_LICENSE("GPL");
