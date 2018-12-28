#include	"unp.h"

int file_size(char *filename)
{
	FILE *fp=fopen(filename,"rb");
	int size;
	if(fp==NULL)
		return -1;
	fseek(fp,0,	SEEK_END);
	size=ftell(fp);
	fclose(fp);
	return size;
}
void fileoperation(int sockfd)
{
	char filename[20];
	char buf[20];
	FILE *fp;
	int n;
	int i=0;
	int b_size;
	union{
		int size;
		char cs[sizeof(int)];
	}size;
	printf("please input your file name\n");
	scanf("%s",filename);
	size.size=file_size(filename);
	i=strlen(filename);
	fp=fopen(filename,"rb");
	filename[i]='\n';
	filename[i+1]='\0';
	Writen(sockfd, filename, strlen(filename));	
	printf("size is %d\n",size.size);
	for(i=0;i<sizeof(int);i++)
	{
		buf[i]=size.cs[i]+1;
	}
	buf[i]='\n';
	buf[i+1]='\0';
//	printf("%d",strlen(buf));
	Writen(sockfd, buf,strlen(buf));
	b_size=10;
	for(;size.size>0;size.size=size.size-b_size)
	{
		if(size.size>=b_size)
		{
			fread(buf,b_size,1,fp);
			buf[b_size]='\0';
			if((n=write(sockfd, buf, strlen(buf)))<b_size)
			{
				printf("write error");
			}
		}
		else
		{
			fread(buf,size.size,1,fp);
			buf[size.size]='\0';
			if((n=write(sockfd, buf, strlen(buf)))<size.size)
			{
				printf("write end error");
			}
		}
	}
	fclose(fp);
/*	char buf[20];
	snprintf(buf,sizeof(buf),"ready to file\n\0");
	Writen(sockfd, buf, strlen(buf));*/
}

void
str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];
	char byor[MAXLINE];
	char CF[MAXLINE];
	int third_flag=0;
	snprintf(CF,sizeof(CF),"exit\n");
	union{
		short s;
		char c[sizeof(short)];
	}un;
	un.s = 0x0102;
	if (sizeof(short) == 2) {
		if (un.c[0] == 1 && un.c[1] == 2)
		{	
			//printf("big-endian\n");
		}
		else if (un.c[0] == 2 && un.c[1] == 1)
		{
		//	printf("little-endian\n");
		}
		else
			printf("unknown\n");
	} 
	byor[0]=un.c[0]+'0';
	byor[1]='\n';
	byor[2]='\0';
//	Writen(sockfd,byteorder,strlen(byteorder));
	
	FD_ZERO(&rset);
	for ( ; ; ) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			Writen(sockfd, sendline, strlen(sendline));
			if(third_flag==0)
			{
				switch (sendline[0])
				{
					case '1':break;
					case '2':Writen(sockfd, byor, strlen(byor));break;
					case '3':third_flag=1;
		/*strcmp(sendline,CF)!=0*/
				
						break;
					case '4':break;
					case '5':fileoperation(sockfd);break;
				}
			}
			if(strcmp(sendline,CF)==0)
				third_flag=0;
		}
	}
}
