#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/time.h>

#include <linux/slab.h>

#define MAGIC_NUM 0xABBABAAB
#define SLAB_NAME "my_cache"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Regina Khamzina");

struct my_vfs_inode
{
    int i_mode;
    unsigned long i_ino;
} my_vfs_inode;

struct kmem_cache *cache = NULL;
static void **line = NULL;
static int size = sizeof(struct my_vfs_inode);

void co(void *p)
{
    *(int *)p = (int)p;
}

static struct inode *my_vfs_make_inode(struct super_block *sb, int mode)
{
    printk(KERN_INFO "my_vfs: Объект структуры inode создан.\n");

    struct inode *ret = new_inode(sb);

    if (ret)
    {
        inode_init_owner(&init_user_ns, ret, NULL, mode);
        ret->i_size = PAGE_SIZE;
        ret->i_atime = ret->i_mtime = ret->i_ctime = current_time(ret);
        ret->i_private = &my_vfs_inode;
    }

    return ret;
}

static void my_vfs_put_super(struct super_block *sb)
{
    printk(KERN_INFO "my_vfs: Уничтожение объекта структуры super_block.\n");
}

static struct super_operations const my_vfs_super_ops =
{
    .put_super = my_vfs_put_super,
    .statfs = simple_statfs,
    .drop_inode = generic_delete_inode
};

static int my_vfs_fill_sb(struct super_block *sb, void *data, int silent)
{
    printk(KERN_ERR "my_vfs: Корень my_vfs создан.\n");

    struct inode *root = NULL;

    sb->s_blocksize = PAGE_SIZE;
    sb->s_blocksize_bits = PAGE_SHIFT;
    sb->s_magic = MAGIC_NUM;
    sb->s_op = &my_vfs_super_ops;

    root = my_vfs_make_inode(sb, S_IFDIR | 0755);

    if (!root)
    {
        printk(KERN_ERR "my_vfs: Ошибка создания inode для корнего каталога my_vfs.\n");
        return -ENOMEM;
    }

    root->i_op = &simple_dir_inode_operations;
    root->i_fop = &simple_dir_operations;

    sb->s_root = d_make_root(root);

    if (!sb->s_root)
    {
        printk(KERN_ERR "my_vfs: Ошибка создания корня my_vfs.\n");
        iput(root);
        return -ENOMEM;
    }

    return 0;
}

static struct dentry* my_vfs_mount(struct file_system_type *type, int flags,
                                   char const *dev, void *data)
{
    printk(KERN_INFO "my_vfs: my_vfs смонтирована.\n");

    struct dentry* const entry = mount_nodev(type, flags, data, my_vfs_fill_sb);
    
    if (IS_ERR(entry))
        printk(KERN_ERR "my_vfs: Ошибка монтирования my_vfs.\n");
    
    return entry;
}

void my_kill_litter_super(struct super_block *sb)
{
    printk(KERN_INFO "my_vfs: Вызов my_kill_litter_super.\n");

    return kill_litter_super(sb);
}

struct file_system_type my_vfs_type =
{
    .owner = THIS_MODULE,
    .name = "my_vfs",
    .mount = my_vfs_mount,
    .kill_sb = my_kill_litter_super
};

static int __init my_vfs_init(void)
{
    printk(KERN_INFO "my_vfs: Модуль загружен.\n");

    int ret = register_filesystem(&my_vfs_type);

    if (ret != 0)
    {
        printk(KERN_ERR "my_vfs: Ошибка регистрации my_vfs.\n");
        return ret;
    }

    line = kmalloc(sizeof(void *), GFP_KERNEL);

    if (!line)
    {
        printk(KERN_ERR "my_vfs: Ошибка при вызове kmalloc.\n");
        return -ENOMEM;
    }

    cache = kmem_cache_create(SLAB_NAME, size, 0, 0, co);

    if (!cache)
    {
        printk(KERN_ERR "my_vfs: Ошибка при вызове kmem_cache_create.\n");
        kfree(line);
        return -ENOMEM;
    }

    if (((*line) = kmem_cache_alloc(cache, GFP_KERNEL)) == NULL)
    {
        printk(KERN_ERR "my_vfs: Ошибка при вызове kmem_cache_alloc.\n");
        kmem_cache_free(cache, *line);
        kmem_cache_destroy(cache);
        kfree(line);
        return -ENOMEM;
    }

    return 0;
}

static void __exit my_vfs_exit(void)
{
    printk(KERN_INFO "my_vfs: Модуль выгружен.\n");

    kmem_cache_free(cache, *line);
    kmem_cache_destroy(cache);
    kfree(line);

    int ret = unregister_filesystem(&my_vfs_type);

    if (ret != 0)
        printk(KERN_ERR "my_vfs: Ошибка дерегистрации my_vfs.\n");
}

module_init(my_vfs_init);
module_exit(my_vfs_exit);
