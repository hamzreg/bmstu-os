#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <asm/io.h>

#include "symbols.h"

#define IRQ     1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gadgi Kishov");

typedef struct
{
    struct work_struct my_work;
    int code;
} my_work_t;

my_work_t *work1;
my_work_t *work2;
struct workqueue_struct *wq;

void my_queue_function1(struct work_struct *work)
{   
    my_work_t *my_work = (my_work_t *) work;

    if (my_work->code < 84) 
    {
        printk(KERN_INFO "+++ MY_QUEUE: keyboard %s was pressed.\n", ascii[my_work->code]);
    }

    printk(KERN_INFO "!!! MY_QUEUE: 1 worker clicked key.\n");
}

void my_queue_function2(struct work_struct *work)
{
    printk(KERN_INFO "!!! MY_QUEUE: 2 worker clicked key, sleep start.\n");
    msleep(10);
    printk(KERN_INFO "!!! MY_QUEUE: 2 worker clicked key, sleep end.\n");
}


irqreturn_t irq_handler(int irq, void *dev)
{
    printk(KERN_INFO "MY_QUEUE: irq_handler().\n");
    int code = inb(0x60);

    work1->code = code;

    if (irq == IRQ)
    {
        queue_work(wq, (struct work_struct *) work1);
        queue_work(wq, (struct work_struct *) work2);

        return IRQ_HANDLED;
    }

    return IRQ_NONE;
}

static int __init md_init(void)
{
    if (request_irq(IRQ,
                    irq_handler,
                    IRQF_SHARED,
                    "my_irq_handler",
                    (void *) irq_handler))
    {
        printk(KERN_ERR "MY_QUEUE: request_irq() failed.\n");
        destroy_workqueue(wq);
        return 1;
    }
    else
    {
        printk(KERN_INFO "MY_QUEUE: md loaded.\n");
    }

    wq = create_workqueue("my_queue");

    if (wq)
    {
        work1 = (my_work_t*) kmalloc(sizeof(my_work_t), GFP_KERNEL);

        if (work1)
        {
            INIT_WORK((struct work_struct *) work1, my_queue_function1);
        }

        work2 = (my_work_t *) kmalloc(sizeof(my_work_t), GFP_KERNEL);

        if (work2)
        {
            INIT_WORK((struct work_struct *) work2, my_queue_function2);
        }
    }

    // if (!(wq = create_workqueue("my_queue")))
    // {
    //     free_irq(IRQ, irq_handler);
    //     printk(KERN_ERR "MY_QUEUE: create_workqueue() failed.");
    //     return -ENOMEM;
    // }

    // INIT_WORK(&work1, my_queue_function1);
    // INIT_WORK(&work2, my_queue_function2);

    return 0;
}

static void __exit md_exit(void)
{
    flush_workqueue(wq);
    destroy_workqueue(wq);
    free_irq(IRQ, irq_handler);

    printk(KERN_INFO "MY_QUEUE: md unloaded.\n");
}

module_init(md_init);
module_exit(md_exit);