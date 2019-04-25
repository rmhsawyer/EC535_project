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
static int play_mode = 1;
static int numofpeople = 1;
static int state = 1;
static unsigned int Brightness = 128; // PWM  = Brightness/128

/* GPIO pins */
static unsigned int GPIO_LEDR = 28; //Red
static unsigned int GPIO_LEDB = 29; //Blue
static unsigned int GPIO_IR0 = 113; //IR sensor0
static unsigned int GPIO_IR1 = 30; //IR sensor1

/* Timer */
int p_timer_interval = 1;
int p_timer_interval1 = 100;
int p_timer_interval2 = 1000;

/* Global data structures */
struct timer_list p_timer;

struct gpio_user_info {
	int play_mode;
	int numofpeople;
	unsigned int count_value;
	char count_period[4];
	char direction[10];
	char state[10];
	char brightness[10];
};
static struct gpio_user_info* gpio_data;

struct BrightnessList{
    unsigned int val;
    char brightness[10];
    struct BrightnessList *next;
 };
struct BrightnessList* head; 
struct BrightnessList* second; 
struct BrightnessList* third; 
struct BrightnessList* cur;





void _TimerHandler(unsigned long data){
	

	if(play_mode == 0 || numofpeople == 0){
		PWM_PWDUTY0 = (0<<10) | 0;
		gpio_set_value(GPIO_LEDR, 0);
		gpio_set_value(GPIO_LEDB, 0);
		mod_timer( &p_timer, jiffies+msecs_to_jiffies(p_timer_interval2));
	
	}

	else if(play_mode == 1 && numofpeople != 0){
		gpio_set_value(GPIO_LEDR, 0);
		gpio_set_value(GPIO_LEDB, 0);
		if(Brightness ==128)
			Brightness = 1;
		Brightness++;
		PWM_PWDUTY0 = (0<<10) | Brightness;
		mod_timer( &p_timer, jiffies+msecs_to_jiffies(p_timer_interval));
	}


	else if(play_mode == 2 && numofpeople != 0){
		if(state >= 7)
				state = 1;
			else
				state = state + 1;
		int val = state;
		if(val%2 == 0)
			PWM_PWDUTY0 = (0<<10) | 0;//CKEN &= ~(1 << 0);
		else
			PWM_PWDUTY0 = (0<<10) | 128;
		val /= 2;
		gpio_set_value(GPIO_LEDR, val%2);
		val /= 2;
		gpio_set_value(GPIO_LEDB, val%2);
		strcpy(gpio_data->brightness, cur->brightness);
		mod_timer( &p_timer, jiffies+msecs_to_jiffies(p_timer_interval2));
	}





}




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
	head= kmalloc(sizeof (struct BrightnessList), GFP_KERNEL);
	second= kmalloc(sizeof (struct BrightnessList), GFP_KERNEL);
	third= kmalloc(sizeof (struct BrightnessList), GFP_KERNEL);
	cur= kmalloc(sizeof (struct BrightnessList), GFP_KERNEL);
	if (!gpio_data)
	{ 
		result = -ENOMEM;
		goto fail; 
	}

	/* Initialize data */
	gpio_data->count_value = 0;
	gpio_data->play_mode = 0;
	gpio_data->numofpeople = 0;
	strcpy(gpio_data->count_period, "H");
	strcpy(gpio_data->direction, "Down");
	strcpy(gpio_data->state, "Hold");
	strcpy(gpio_data->brightness, "L");


	/* Set up GPIO*/
    	gpio_direction_input(GPIO_IR0);
    	gpio_direction_input(GPIO_IR1);
	gpio_direction_output(28, 0);
	gpio_direction_output(29, 0);

	/* Set up PWM */
	head->val = 1;
	strcpy(head->brightness, "L");
	head->next = second;
	second->val = 16;
	strcpy(second->brightness, "M");
	second->next = third;
	third->val = 128;
	strcpy(third->brightness, "H");
	third->next = head;





	cur = head;
	pxa_gpio_mode( GPIO16_PWM0_MD);
	PWM_CTRL0 = (1<<6) | 255;  
	PWM_PWDUTY0 = (0<<10) | Brightness;
	PWM_PERVAL0 = 128;
	CKEN |= CKEN0_PWM0;






	/* Set up timer  */
	setup_timer(&p_timer, _TimerHandler, 0);
	mod_timer( &p_timer, jiffies+msecs_to_jiffies(p_timer_interval));


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
	kfree(head);
	kfree(second);
	kfree(third);
	kfree(cur);
	del_timer(&p_timer);
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



















