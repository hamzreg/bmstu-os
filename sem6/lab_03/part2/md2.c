#include <linux/init.h>
#include <linux/module.h>

#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khamzina IU7-63B");
MODULE_DESCRIPTION("lab03: part2 md2");

static int __init md_init(void)
{
    printk(KERN_INFO "- Module md2 loaded!\n");
    printk(KERN_INFO "- md2 data exported from md1: %s\n", data);
    printk(KERN_INFO "- md2 returned md1_proc: %s\n", md1_proc());

    // printk( "- md2 use local from md1: %s\n", md1_local());
    // printk( "- md2 use noexport from md1: %s\n", md1_noexport());

    return 0;
}

static void __exit md_exit(void)
{
    printk(KERN_INFO "- Good by!\n");
}

module_init(md_init);
module_exit(md_exit);