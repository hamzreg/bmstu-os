#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Khamzina IU7-63B");

#define MAX_COOKIE_LENGTH PAGE_SIZE

#define DIRNAME "seqfile_dir"
#define FILENAME "seqfile_file"
#define SYMLINK "seqfile_link"

static char *cookie_pot = NULL; 
char tmp_buffer[MAX_COOKIE_LENGTH];

static struct proc_dir_entry *seqfile_dir;
static struct proc_dir_entry *seqfile_file;
static struct proc_dir_entry *seqfile_link;

static int read_index = 0;
static int write_index = 0;


static void free_memory(void)
{
    if (seqfile_link != NULL)
        remove_proc_entry(SYMLINK, NULL);
    
    if (seqfile_file != NULL)
        remove_proc_entry(FILENAME, NULL);
    
    if (seqfile_dir != NULL)
        remove_proc_entry(DIRNAME, NULL);

    vfree(cookie_pot);
}

static void *my_seq_start(struct seq_file *s, loff_t *pos)
{
    printk(KERN_INFO "+ START\n");

    if (*pos == 0)
        return cookie_pot;

    *pos = 0;

    return NULL;
}

static void *my_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
    printk(KERN_INFO "+ NEXT\n");

    (*pos)++;

    return NULL;
}

static void my_seq_stop(struct seq_file *s, void *v)
{
    printk(KERN_INFO "+ STOP\n");
}

static int my_seq_show(struct seq_file *s, void *v)
{
    printk(KERN_INFO "+ SHOW\n");

    int read_len;

    if (write_index == 0)
        return 0;

    if (read_index >= write_index)
        read_index = 0;

    read_len = snprintf(tmp_buffer, MAX_COOKIE_LENGTH, "%s\n", cookie_pot + read_index);

    seq_printf(s, "%s", tmp_buffer);

    read_index += read_len;

    return 0;
}

static struct seq_operations my_seq_ops = 
{
    .start = my_seq_start,
    .next = my_seq_next,
    .stop = my_seq_stop,
    .show = my_seq_show
};

static ssize_t my_write(struct file *file, const char __user *buf,
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

static int my_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "+ OPEN\n");

    return seq_open(file, &my_seq_ops);
}

static int my_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "+ RELEASE\n");

    return 0;
}

static const struct proc_ops fops =
{
    .proc_open = my_open,
    .proc_read = seq_read,
    .proc_write = my_write,
    .proc_lseek = seq_lseek,
    .proc_release = my_release
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

    if ((seqfile_dir = proc_mkdir(DIRNAME, NULL)) == NULL)
    {
        free_memory();
        printk(KERN_INFO "+ Ошибка при создании директории\n");

        return -ENOMEM;
    }
    
    if ((seqfile_file = proc_create(FILENAME, 0666, NULL, &fops)) == NULL)
    {
        free_memory();
        printk(KERN_INFO "+ Ошибка при создании файла\n");

        return -ENOMEM;
    }
    
    if ((seqfile_link = proc_symlink(SYMLINK, NULL, DIRNAME)) == NULL)
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
