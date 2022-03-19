#include <linux/init.h>
#include <linux/module.h>

#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khamzina IU7-63B");
MODULE_DESCRIPTION("lab03: part2 md3");

static int __init md_init(void)
{
    printk(KERN_INFO "- Module md3 loaded!\n");
    printk(KERN_INFO "- md3 data exported from md1: %s\n", data);
    printk(KERN_INFO "- md3 returned md1_proc: %s\n", md1_proc());

    return -1;
}

module_init(md_init);