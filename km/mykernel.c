/* Necessary includes for device drivers */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/slab.h> /* kmalloc() */
#include <linux/fs.h> /* everything... */
#include <linux/errno.h> /* error codes */
#include <linux/types.h> /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h> /* O_ACCMODE */
#include <asm/system.h> /* cli(), *_flags */
#include <asm/uaccess.h> /* copy_from/to_user */
#include <linux/timer.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <asm/gpio.h>
#include <asm/arch/pxa-regs.h>
#include <asm/hardware.h>
#include <linux/delay.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Zhiyu Wang, Zhiwei Tang, Minghe Ren");
MODULE_DESCRIPTION("EC535 Friday Team project kernel module");

/* Declaration of functions */
static int mygpio_open(struct inode *inode, struct file *filp);
static int mygpio_release(struct inode *inode, struct file *filp);
static ssize_t mygpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos);
static ssize_t mygpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos);
static void mygpio_exit(void);
static int mygpio_init(void);


/* Declaration of the init and exit functions */
module_init(mygpio_init);
module_exit(mygpio_exit);

/* Structure that declares the usual file */
/* access functions */
struct file_operations gpio_fops = {
	read: mygpio_read,
	write: mygpio_write,
	open: mygpio_open,
	release: mygpio_release
};




/* Global varibles */
static int mygpio_major = 61;
/* GPIO pins */
static unsigned int GPIO_LEDs[3] = {16, 28, 29};
static unsigned int GPIO_IR0 = 113;
static unsigned int GPIO_IR1 = 30;


/* Global data structures */
struct gpio_user_info {
	unsigned int count_value;
	char count_period[4];
	char direction[10];
	char state[10];
	char brightness[10];
};
static struct gpio_user_info* gpio_data;




static int mygpio_init(void)
{
	int result;
	/* Registering device */
	result = register_chrdev(mygpio_major, "mygpio", &gpio_fops);
	if (result < 0)
	{
		printk(KERN_ALERT
			"mygpio: cannot obtain major number %d\n", mygpio_major);
		return result;
	}
	
	/* Allocate memory */
	gpio_data = kmalloc(sizeof (struct gpio_user_info), GFP_KERNEL);
	if (!gpio_data)
	{ 
		result = -ENOMEM;
		goto fail; 
	}

	/* Initialize data */
	gpio_data->count_value = 0;
	strcpy(gpio_data->count_period, "H");
	strcpy(gpio_data->direction, "Down");
	strcpy(gpio_data->state, "Hold");
	strcpy(gpio_data->brightness, "L");


	/* Set up GPIO*/
	int i;	
    	gpio_direction_input(GPIO_IR0);
    	gpio_direction_input(GPIO_IR1);
	for(i = 0; i < 3; i++){
		gpio_direction_output(GPIO_LEDs[i], 0);
	}
	for(i = 0; i < 3; i++){
		gpio_set_value(GPIO_LEDs[i], 1);
	}


	printk(KERN_INFO "P-Timer inserted.\n");
	return 0;

fail:
	mygpio_exit();
	return result;


}




static void mygpio_exit(void)
{
	/* Freeing the major number */
	unregister_chrdev(mygpio_major, "mygpio");
	/* Freeing buffer memory */
	kfree(gpio_data);
	printk(KERN_ALERT "Removing gpio module\n");
	
}




static int mygpio_open(struct inode *inode, struct file *filp)
{	

	return 0;
}



static int mygpio_release(struct inode *inode, struct file *filp)
{
	
	return 0;
}


static ssize_t mygpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{

	return count;
}


static ssize_t mygpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	return count;
}



















