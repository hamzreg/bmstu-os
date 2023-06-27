#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/interrupt.h>

#include "symbols.h"

#define IRQ     1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gadgi Kishov");

struct tasklet_struct *my_tasklet;
char *my_tasklet_data = "My tasklet function was called";

void my_tasklet_function(unsigned long data)
{
    if (data < 84) 
    {
        printk(KERN_INFO "+++ MY_TASKLET: keyboard %s was pressed with code %d.\n", ascii[data], data);
    }

    printk(KERN_INFO "!!! MY_TASKLET: state: %ld, count: %ld, use_callback: %d, data: %s\n", 
                     my_tasklet->state, my_tasklet->count,
                     my_tasklet->use_callback, (char *)my_tasklet->data);
}

irqreturn_t irq_handler(int irq, void *dev)
{
    printk(KERN_INFO "MY_TASKLET: my_irq_handler.\n");

    int code = inb(0x60);
    my_tasklet->data = code;

    if (irq == IRQ)
    {
        printk("MY_TASKLET: irq_handler().\n");
		printk("MY_TASKLET: state before tasklet_schedule(): %lu\n", my_tasklet->state);
        tasklet_schedule(my_tasklet);
        printk("MY_TASKLET: state after tasklet_schedule(): %lu\n", my_tasklet->state);
        return IRQ_HANDLED;
    }

    return IRQ_NONE;
}

static int __init md_init(void)
{
    my_tasklet = kmalloc(sizeof(struct tasklet_struct), GFP_KERNEL);

    if (!my_tasklet)
    {
        printk(KERN_ERR "MY_TASKLET: kmalloc() failed.\n");
        return 1;
    }

    tasklet_init(my_tasklet, my_tasklet_function, (unsigned long) my_tasklet_data);

    if (request_irq(IRQ,
                    irq_handler,
                    IRQF_SHARED,
                    "my_irq_handler",
                    (void *) irq_handler))
    {
        printk(KERN_ERR "MY_TASKLET: request_irq() failed.\n");
        kfree(my_tasklet);
        return 1;
    }
    else
    {
        printk(KERN_INFO "MY_TASKLET: md loaded.\n");
    }

    return 0;
}

static void __exit md_exit(void)
{
    synchronize_irq(IRQ);
    tasklet_kill(my_tasklet);
    kfree(my_tasklet);
    free_irq(IRQ, irq_handler);

    printk(KERN_INFO "MY_TASKLET: md unloaded.\n");
}

module_init(md_init);
module_exit(md_exit);