#include"unp.h"

void timer_ten()
{
	float time;
	time=0.00;
	char info[40];
	for(;;)
	{
		usleep(500000);
/*		FILE *fp;
		if((fp=fopen("1.txt","r+"))==NULL)
		{
			printf("file error\n");
			return;
		}
		flock(fileno(fp),LOCK_EX);*/
		snprintf(info,sizeof(info),"time elapsed %.2f\n",time);
		printf("%s",info);
		time=time+0.01;
	/*	fputs(info,fp);
		fclose(fp);
		flock(fileno(fp),LOCK_UN);*/
	}
}

void timer_unsure()
{
	long int i;
	for(;;)
	{
		i=rand();
		i=i%990;
		i=i+10;
		usleep(i*1000);
		printf("i sleep %ldms\n",i);
	}
}

void getkey()
{
	char key;
	for(;;)
	{
		key=getchar();
		if(key=='q')
			break;
		else
			printf("%c is press\n",key);
		getchar();
	}
}
void main()
{
	int a;
	pid_t pid;
	if(fork()==0)
	{
		timer_ten();		
	}
	if(fork()==0)
	{
		timer_unsure();
	}
	if((pid=fork())==0)
	{
		getkey();
		exit(0);
	}
	waitpid(pid,&a,0);
	wait(&a);
	wait(&a);
}
