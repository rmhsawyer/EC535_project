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
static unsigned int play_mode = 2;
static unsigned int numofpeople = 1;
static unsigned int state = 1;
static unsigned int Brightness = 128; // PWM  = Brightness/128
static unsigned int IRQ_1 = 0;
static unsigned int IRQ_2 = 0;
static int direction = 0;

/* GPIO pins */
static unsigned int GPIO_LEDR = 28; //Red
static unsigned int GPIO_LEDB = 29; //Blue
static unsigned int GPIO_IR0 = 101; //IR sensor0
static unsigned int GPIO_IR1 = 30; //IR sensor1

/* Timer */
int p_timer_interval = 1;
int p_timer_interval1 = 10;
int p_timer_interval2 = 100;
int p_timer_interval3 = 1000;

/* Global data structures */
struct timer_list p_timer;
struct timer_list p_timer1;

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
		mod_timer( &p_timer, jiffies+msecs_to_jiffies(p_timer_interval3));
	
	}

	else if(play_mode == 1 && numofpeople != 0){
		gpio_set_value(GPIO_LEDR, 0);
		gpio_set_value(GPIO_LEDB, 0);
		if(Brightness ==128)
			Brightness = 1;
		Brightness++;
		PWM_PWDUTY0 = (0<<10) | Brightness;
		mod_timer( &p_timer, jiffies+msecs_to_jiffies(p_timer_interval1));
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
		mod_timer( &p_timer, jiffies+msecs_to_jiffies(p_timer_interval3));
	}

	else{
		PWM_PWDUTY0 = (0<<10) | 128;
		gpio_set_value(GPIO_LEDR, 1);
		gpio_set_value(GPIO_LEDB, 1);
		mod_timer( &p_timer, jiffies+msecs_to_jiffies(p_timer_interval3));
	}


}



irqreturn_t gpio_irq0(int irq, void *dev_id, struct pt_regs *regs)
{
	//no debounce, active for both edges
	IRQ_1 = 1;
	if(IRQ_2 == 0)
		direction = 1;
	else 
		direction = 2;
	if(!IRQ_2)
		mod_timer( &p_timer1, jiffies+msecs_to_jiffies(p_timer_interval3));
	return IRQ_HANDLED;
}

irqreturn_t gpio_irq1(int irq, void *dev_id, struct pt_regs *regs)
{
	IRQ_2 = 1;
	if(IRQ_1 == 0)
		direction = 2;
	else 
		direction = 1;
	if(!IRQ_1)
		mod_timer( &p_timer1, jiffies+msecs_to_jiffies(p_timer_interval3));
	return IRQ_HANDLED;
}

void _TimerHandler1(unsigned long data){
	if(IRQ_2 == 1 && IRQ_1 == 1)
	{	
		if(direction == 1)
			numofpeople++;
		if(direction == 2)
			numofpeople = numofpeople<1? 0 : numofpeople-1;
	}
	IRQ_1 = 0;
	IRQ_2 = 0;
	direction = 0;
	printk(numofpeople);
	return;
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
	pxa_gpio_mode( GPIO16_PWM0_MD);
	PWM_CTRL0 = (1<<6) | 255;  
	PWM_PWDUTY0 = (0<<10) | Brightness;
	PWM_PERVAL0 = 128;
	CKEN |= CKEN0_PWM0;


	/* Set up Interrupts*/
	pxa_gpio_mode(GPIO_IR0 | GPIO_IN);
	int irq = IRQ_GPIO(GPIO_IR0);
	if (request_irq(irq, &gpio_irq0, SA_INTERRUPT | SA_TRIGGER_RISING,
				"GPIO_IR0", NULL) != 0 ) {
                printk ( "irq not acquired \n" );
                return -1;
        }

	pxa_gpio_mode(GPIO_IR1| GPIO_IN);
	int irq1 = IRQ_GPIO(GPIO_IR1);
	if (request_irq(irq1, &gpio_irq1, SA_INTERRUPT | SA_TRIGGER_RISING,
				"GPIO_IR1", NULL) != 0 ) {
                printk ( "irq not acquired \n" );
                return -1;
        }




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
	del_timer(&p_timer1);
	free_irq(IRQ_GPIO(GPIO_IR0 ), NULL);
	free_irq(IRQ_GPIO(GPIO_IR1 ), NULL);
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



















