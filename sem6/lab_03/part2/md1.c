#include <linux/init.h>
#include <linux/module.h>

#include "md.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khamzina IU7-63B");
MODULE_DESCRIPTION("lab03: part2 md1");

char *data = "I have magic hair that glows when I write code";

extern char *md1_proc(void)
{
    return data;
}

static char *md1_local(void)
{
    return data;
}

extern char *md1_noexport(void)
{
    return data;
}

EXPORT_SYMBOL(data);
EXPORT_SYMBOL(md1_proc);


static int __init md_init(void)
{
    printk(KERN_INFO "- Module md1 loaded!\n");
    printk(KERN_INFO "- md1 use local from md1: %s\n", md1_local());
    printk(KERN_INFO "- md1 use noexport from md1: %s\n", md1_noexport());
    
    return 0;
}

static void __exit md_exit(void)
{
    printk(KERN_INFO "- Good by!\n");
}

module_init(md_init);
module_exit(md_exit);