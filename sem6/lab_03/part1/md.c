#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khamzina IU7-63B");
MODULE_DESCRIPTION("lab03: part1");

static int __init md_init(void)
{
    struct task_struct *task =  &init_task;

    printk(KERN_INFO "Module md loaded!\n");

    do
    {
        printk(KERN_INFO "name - %s; pid - %d; state - %ld; static_prio - %d; p_name - %s; p_pid - %d\n",
               task->comm, task->pid, task->state, task->static_prio, task->parent->comm, task->parent->pid);
    } while ((task = next_task(task)) != &init_task);

    return 0;
}

static void __exit md_exit(void)
{
    printk("Good by!\n");
    printk(KERN_INFO "current name - %s; pid - %d; state - %ld; static_prio - %d; p_name - %s; p_pid - %d\n",
           current->comm, current->pid, current->state, current->static_prio, current->parent->comm, current->parent->pid);
}

module_init(md_init);
module_exit(md_exit);