#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h> 
#include <linux/netlink.h>
#include <linux/spinlock.h>
#include <net/sock.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include </usr/include/x86_64-linux-gnu/asm/unistd_64.h>
#include <linux/icmp.h>

MODULE_LICENSE("GPL");
char sys[50]="/boot/System.map-";
const char* src;
struct myfile 
{
	struct file *f;
	mm_segment_t fs;
	loff_t pos;
};

struct myfile* open_file_for_read(char* filename)
{	
	struct myfile * mf=kmalloc(sizeof(struct myfile),GFP_KERNEL);	

	mf->f = filp_open(filename,0,0);
	mf->pos=0;


	if(IS_ERR(mf->f))
	{	
		printk(KERN_ALERT "error");
		mf->f=NULL;
	}
	return mf;
}

volatile int read_from_file_until (struct myfile *mf, char *buf, unsigned long vlen)
{
    
	int read;

	mf->fs=get_fs();
	set_fs(KERNEL_DS);

	read= vfs_read(mf->f, buf, vlen, &mf->pos);

	set_fs(mf->fs);
	 
	return read;
}

void close_file(struct myfile *mf)
{
	if(mf)
		filp_close(mf->f,NULL);

	kfree(mf);
}


static int __init hello_init(void)
{
	printk(KERN_ALERT "Hello World CSCE-3402 :) \n");
	
	struct myfile * mf=kmalloc(sizeof(struct myfile), GFP_KERNEL);
        mf = open_file_for_read("/proc/version");
	

	int r;
	char *buf=kmalloc(1000,GFP_KERNEL);
	r=read_from_file_until(mf,buf,1000);
	
	int counter=0;
	int i=0;
	while(counter!=3)
	{
		if(buf[i]==' ')
			counter++;
		i++;
	}
	char buf2[i];
	strncpy(buf2,buf,i);
	buf2[i]=NULL;

	char*p_buf2=buf2;
	strsep(&p_buf2, " ");
	strsep(&p_buf2," ");
	src=p_buf2;
	char* list= strcat(sys,src);
	

	close_file(mf);	
	kfree(buf);


	strim(sys);
	struct myfile * mf2=kmalloc(sizeof(struct myfile), GFP_KERNEL);	
	mf2 = open_file_for_read(sys);

	int r2;
	char* buf3=kmalloc(2420000, GFP_KERNEL);
	r2=read_from_file_until(mf2,buf3,2420000);
	char search[14]=" sys_call_table";

	int j=0;
	int k=0;
	int ind=0;

	for(j=0;j<strlen(buf3)-14;j++)
	{
		if(buf3[j]==search[k])
		{
			k++;
			if(k==14)
			{
				ind=j;
			}
		}
		else {
			j-=k;
			k=0;
		}
	}

	char ans[16];
	int c=0;
	for(j=ind-32;j<ind-16;j++)
	{	
		ans[c]=buf3[j];
		c++;
	}
	printk(KERN_ALERT "%s\n", ans);
	
	close_file(mf2);
	kfree(buf3);
	
	long unsigned fork_addr;
	
	long unsigned *p;
	p=(long unsigned *) ans;
	fork_addr=p[__NR_fork];
	printk(KERN_ALERT "%lu\n", fork_addr);
	
	/*close_file(mf2);
	kfree(buf3);*/
	return 0;
}

static void __exit hello_cleanup(void)
{
	printk(KERN_ALERT "Bye Bye CSCE-3402 :) \n");
}

module_init(hello_init);
module_exit(hello_cleanup);

