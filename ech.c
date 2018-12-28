#include	"unp.h"
void fileoperation(int sockfd)
{
	char filename[20];
	char buf[20];
	FILE *fp;
	int i=0;
	int n;
	int b_size;
	union{
		int size;
		char cs[sizeof(int)];
	}size;
	read(sockfd, filename,sizeof(filename));
	printf("%s size is\n ",filename);
	if((n=read(sockfd, buf, 5))>=4)
	{	
		for(i=0;i<sizeof(int);i++)
		{
			size.cs[i]=buf[i]-1;
		}
		printf("%d\n",size.size);
		i=strlen(filename);
		if(filename[i-1]=='\n')
			filename[i-1]='\0';
		fp=fopen(filename,"wb");
		b_size=10;
		for(;size.size>0;size.size=size.size-b_size)
		{
			if(size.size>=b_size)
			{	
				read(sockfd, buf, b_size);
				fwrite(buf,b_size,1,fp);
			}
			else
			{
				read(sockfd, buf, size.size);
				fwrite(buf,size.size,1,fp);
			}
		}
		fclose(fp);
	}
}
void
str_echo(int sockfd)
{
	ssize_t		n;
	char		buf[MAXLINE];
/*	int l;
	long a,b;*/
again:
	while ( (n = read(sockfd, buf, MAXLINE)) > 0)
	{	
	/*	if((l=sscanf(buf,"%ld%ld",&a,&b))==2)
		{
			snprintf(buf,sizeof(buf),"%ld+%ld=%ld %ld*%ld=%ld\n",a,b,a+b,a,b,a*b);
			n=strlen(buf);
			Writen(sockfd,buf,n);
			snprintf(buf,sizeof(buf),"%ld-%ld=%ld %ld/%ld=%f\n",a,b,a-b,a,b,(float)a/(float)b);
			n=strlen(buf);
			Writen(sockfd,buf,n);
			}
	*/
		printf("%d\n",n);
		if(buf[0]=='5')
		{
		//	Writen(sockfd,buf,n);
		//	printf("try to get file\n");
		//	snprintf(buf,sizeof(buf),"ready to get file\n\0");
		//    Writen(sockfd,buf,strlen(buf));
			fileoperation(sockfd);
		}
		else
		{
			Writen(sockfd,buf,n);
		}
	}
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		err_sys("str_echo: read error");
}
