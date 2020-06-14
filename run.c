#include <linux/fs.h>
#include<linux/init_task.h>
#include<linux/slab.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

static int dev_open(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static int dev_release(struct inode *, struct file *);
struct task_struct *current_pos;

static struct file_operations fops=
{
        .open = dev_open,
        .read = dev_read,
        .release = dev_release,
};


static int process_init(void)  // this will be called during insmod
{
        int t = register_chrdev(240,"process_list",&fops);

        if(t<0)
 	printk(KERN_ALERT "Registration Failed");
        else
        printk(KERN_ALERT "Registration Successful");

        current_pos = next_task(&init_task);
return 0;
}


static void process_exit(void) // called during rmmod
{
        unregister_chrdev(240 , "process_list");
        printk(KERN_ALERT "Exit Successful");
}



static int dev_open(struct inode *inod, struct file *fil)
{
        printk(KERN_ALERT "Device Opened");
        current_pos = next_task(&init_task);//intial position when opened
        return 0;
}


static ssize_t dev_read(struct file *filep, char *buf, size_t size, loff_t *off)
{
        int status;
	struct task_struct *task;
        char msg[300];
        memset(msg,0,sizeof(char)*300);//memory allocation for storing the message
        printk(KERN_INFO "Reading in progress\n");

        for_each_process(task)
        {
                if(current_pos == task)
                {
                memset(msg,0,sizeof(char)*300);
                sprintf(msg,"PID = %d PPID = %d CPU = %d STATE = %ld\n",task->pid,task->parent->pid,
                        task_cpu(task),task->state);
                printk(KERN_INFO "Sending....\n");
                status = copy_to_user(buf,msg,strlen(msg));
                if(status!=0)
                {
                printk(KERN_ALERT "Error parsing\n");
                return 0;
                }
                current_pos = next_task(task);//looping to next task

                break;
                }

        }
        return strlen(msg); //anything else is causing infinte loop
}

static int dev_release(struct inode *inod, struct file *fil)
{
        printk(KERN_ALERT "Device Closed\n");
        current_pos = &init_task;
return 0;
}


module_init(process_init);

module_exit(process_exit);


