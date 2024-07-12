#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "ipc_device"
#define BUFFER_SIZE 1024

static int major_number;
static char *message_buffer;
static int message_size;
static int open_count = 0;

static int dev_open(struct inode *inodep, struct file *filep) {
    open_count++;
    printk(KERN_INFO "IPC Device: Opened %d times\n", open_count);
    return 0;
}

static int dev_release(struct inode *inodep, struct file *filep) {
    printk(KERN_INFO "IPC Device: Closed\n");
    return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;

    if (*offset >= message_size) {
        return 0;
    }

    if (len > message_size - *offset) {
        len = message_size - *offset;
    }

    error_count = copy_to_user(buffer, message_buffer + *offset, len);

    if (error_count == 0) {
        *offset += len;
        printk(KERN_INFO "IPC Device: Sent %zu characters to the user\n", len);
        return len;
    } else {
        printk(KERN_INFO "IPC Device: Failed to send %d characters to the user\n", error_count);
        return -EFAULT;
    }
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
    if (len > BUFFER_SIZE) {
        len = BUFFER_SIZE;
    }
    if (copy_from_user(message_buffer, buffer, len)) {
        return -EFAULT;
    }

    message_size = len;
    printk(KERN_INFO "IPC Device: Received %zu characters from the user\n", len);
    return len;
}

static struct file_operations fops = {
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
    .release = dev_release,
};

static int __init ipc_driver_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "IPC Device: Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "IPC Device: Registered with major number %d\n", major_number);

    message_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!message_buffer) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "IPC Device: Failed to allocate memory\n");
        return -ENOMEM;
    }
    memset(message_buffer, 0, BUFFER_SIZE);

    return 0;
}

static void __exit ipc_driver_exit(void) {
    kfree(message_buffer);
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "IPC Device: Unregistered\n");
}

module_init(ipc_driver_init);
module_exit(ipc_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple IPC device driver");
MODULE_VERSION("1.0");
