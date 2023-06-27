#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khamzina IU7-63B");

#define MAX_COOKIE_LENGTH PAGE_SIZE

#define DIRNAME "fortunes_dir"
#define FILENAME "fortunes_file"
#define SYMLINK "fortunes_link"

static char *cookie_pot = NULL;
char tmp_buffer[MAX_COOKIE_LENGTH];

static struct proc_dir_entry *fortunes_dir;
static struct proc_dir_entry *fortunes_file;
static struct proc_dir_entry *fortunes_link;

static int read_index = 0;
static int write_index = 0;

static void free_memory(void)
{
    if (fortunes_link != NULL)
        remove_proc_entry(SYMLINK, NULL);
    
    if (fortunes_file != NULL)
        remove_proc_entry(FILENAME, NULL);
    
    if (fortunes_dir != NULL)
        remove_proc_entry(DIRNAME, NULL);

    vfree(cookie_pot);
}

static int fortunes_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "+ OPEN\n");

    return 0;
}

static int fortunes_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "+ RELEASE\n");

    return 0;
}

static ssize_t fortunes_write(struct file *file, const char __user *buf,
                              size_t len, loff_t *pos)
{
    printk(KERN_INFO "+ WRITE\n");

    if (len > MAX_COOKIE_LENGTH - write_index + 1)
    {
        printk(KERN_INFO "+ Ошибка: буфер переполнен\n");

        return -ENOSPC;
    }

    if (copy_from_user(&cookie_pot[write_index], buf, len) != 0)
    {
        printk(KERN_INFO "+ Ошибка при вызове copy_from_user\n");

        return -EFAULT;
    }

    write_index += len;
    cookie_pot[write_index - 1] = '\0';

    return len;
}

static ssize_t fortunes_read(struct file *file, char __user *buf,
                              size_t len, loff_t *pos)
{
    printk(KERN_INFO "+ READ\n");

    int read_len;

    if ((*pos > 0) || (write_index == 0))
        return 0;

    if (read_index >= write_index)
        read_index = 0;

    read_len = snprintf(tmp_buffer, MAX_COOKIE_LENGTH, "%s\n", &cookie_pot[read_index]);

    if (copy_to_user(buf, tmp_buffer, read_len) != 0)
    {
        printk(KERN_INFO "+ Ошибка при вызове copy_to_user\n");

        return -EFAULT;
    }

    read_index += read_len;
    *pos += read_len;

    return read_len;
}

static const struct proc_ops fops =
{
    .proc_open = fortunes_open,
    .proc_read = fortunes_read,
    .proc_write = fortunes_write,
    .proc_release = fortunes_release
};

static int __init md_init(void)
{
    printk(KERN_INFO "+ INIT\n");

    if ((cookie_pot = vmalloc(MAX_COOKIE_LENGTH)) == NULL)
    {
        free_memory();
        printk(KERN_INFO "+ Ошибка при вызове vmalloc\n");

        return -ENOMEM;
    }

    memset(cookie_pot, 0, MAX_COOKIE_LENGTH);

    if ((fortunes_dir = proc_mkdir(DIRNAME, NULL)) == NULL)
    {
        free_memory();
        printk(KERN_INFO "+ Ошибка при создании директории\n");

        return -ENOMEM;
    }
    
    if ((fortunes_file = proc_create(FILENAME, 0666, NULL, &fops)) == NULL)
    {
        free_memory();
        printk(KERN_INFO "+ Ошибка при создании файла\n");

        return -ENOMEM;
    }
    
    if ((fortunes_link = proc_symlink(SYMLINK, NULL, DIRNAME)) == NULL)
    {
        free_memory();
        printk(KERN_INFO "+ Ошибка при создании ссылки\n");

        return -ENOMEM;
    }

    read_index = 0;
    write_index = 0;

    return 0;
}

static void __exit md_exit(void)
{
    free_memory();
    printk("+ EXIT\n");
}

module_init(md_init);
module_exit(md_exit);
