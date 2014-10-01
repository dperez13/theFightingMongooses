#include <linux/module.h>   
#include <linux/kernel.h>
#include <linux/proc_fs.h>    
#include <linux/init.h>
#include <linux/time.h>
#include <linux/seq_file.h>	 
     

#define PROC_NAME "currentdate"
//Used code from http://linux.die.net/lkmpg/x861.html as base for seq_file portions
//Also used some code from the slides provided in the project powerpoint specifications
MODULE_LICENSE("Dual BSD/GPL");



static int mydate_seq_show(struct seq_file *s, void *v)
{
	int hour,min,sec,year,mon,temp;
	struct timeval time; 
	struct tm alltime;
	do_gettimeofday(&time);
	temp = time.tv_sec;
	time_to_tm(temp,0, &alltime);
	sec = temp%60;
	min = (temp/60)%60;
	hour = (temp/3600)%24 -4;
    mon = alltime.tm_mon + 1;
	year = 1900+alltime.tm_year;	//See tm specification, the year is stored as "years since 1900"

	seq_printf(s, "%d-%02d-%02d %02d:%02d:%02d\n", year, mon, alltime.tm_mday, hour, min, sec);
	return 0;
}
static int mydate_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, mydate_seq_show, NULL);
}
static const struct file_operations mydate_proc_fops = {
  .owner = THIS_MODULE,
  .open = mydate_proc_open,
  .read = seq_read,
  .release = single_release,
};

static int my_date_init(void)
{
    //printk(KERN_INFO "Hello world!\n");
    proc_create(PROC_NAME, 0, NULL, &mydate_proc_fops);
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void my_date_exit(void)
{
    //printk(KERN_INFO "Cleaning up module.\n");
    remove_proc_entry(PROC_NAME, NULL);
}

module_init(my_date_init);
module_exit(my_date_exit);
