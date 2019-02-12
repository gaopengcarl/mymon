/*************************************************************************
  > File Name: main.c
  > Author: gaopeng QQ:22389860 all right reserved
  > Mail: gaopp_200217@163.com
  > Created Time: Wed 08 Feb 2017 08:01:58 AM CST
 ************************************************************************/

#include"type.h"

int useage(void)
{
	printf(
	    "Author: gaopeng QQ:22389860 Mail: gaopp_200217@163.com \n"
			"Info:This tool collect data from 'show global status' and show interval (-t) seconds data change\n"
			"help DBA to diagnose performance problem,when error or (-d) run log is at current dir name \n"
			"is mymon.log.\n"
			"WIN32 platform is not support\n"
		 	"Useage:./mymon [-u username] [-p password] -S socketfile/{-h hosname -P port} -t seconds -n times [-l logfile] [-d]\n"
		 	"[-?]           :help info;\n"
			"[-u username]  :connect to mysqld username,when no username requisite is optional;\n"
			"[-p password]  :connect to mysqld password,when no password requisite is optional;\n"
			"[-S socketfile]:connect to mysqld socket file;\n"
			"[-h hostname]  :connect to mysqld host;\n"
			"[-P port]      :connect to mysqld port;\n"
			"                socketfile or [hostname port]  must;\n"
			"[-t seconds]   :seconds of interval,this is necessary;\n"
			"[-n times]     :how many times you watch,this is necessary;\n"
			"[-l logfile]   :when you want to write information to a logfile provide a logfile name\n"
			"                when a logfile provide no information at foreground,is optional;\n"
			"[-d]           :more running log at logfile,otherwise error log in mymon.log;\n"
		);
	return 0;
}


int wel(void)
{
	printf(
	    "welcome used this mysql monitor tool\n"
		  "Author: gaopeng QQ:22389860 Mail: gaopp_200217@163.com \n"
			"Info:This tool collect data from 'show global status' and show interval (-t) seconds data change\n"
			"help DBA to diagnose performance problem,when error or (-d) run log is at current dir name \n"
			"is mymon.log.\n"
			"WIN32 platform is not support\n"
		 	"Useage:./mymon [-u username] [-p password] -S socketfile/{-h hosname -P port} -t seconds -n times [-l logfile] [-d]\n"
		 	"more help info in ./mymon -?\n"
		 	);
	return 0;
}

int getpar( char (*par)[500],int n)
{
	int i;
	for(i=0;i<4;i++)
	{
		printf("your parameter is %s\n",*(par+i));
	}
}

int checkpar(char (*par)[500],int t,int n)
{
	if(strlen(*(par+2)) == 0 && (strlen(*(par+4)) == 0 || strlen(*(par+5)) == 0 ))
	{

		useage();
		return -1;
	}

	if(t<1||n<1)
	{
		useage();
		return -1;
	}
	return 0;
}

int main(int argc,char* argv[])
{
	int ch;
	int t = 0; //t how long 
	int n = 0;//n count
	FILE* fd = 0;//log file fs
	unsigned int mport = 0;//mysql connect port

	debug = 0;
	write_file = 0;
	PAR par;
	setbuf(stdout,NULL);//stdio buffer disable
	memset(par,0,sizeof(PAR));
	if(argc<2)
	{
		useage();
		return -1;
	}
	while ((ch = getopt(argc, argv, "u:p:S:t:n:l:h:P:d?")) != -1)
	{
		switch (ch)  
		{
			case 'u':
				if(strlen(optarg)>MAX_PAR_LEN)
				{
					printf("MYMON:username max length more than 500 bytes\n");
					return -1;
				}
				strcpy(par[0],optarg);
				break;
			case 'h':
				if(strlen(optarg)>MAX_PAR_LEN)
				{
					printf("MYMON:hostname max length more than 500 bytes\n");
					return -1;
				}
				strcpy(par[4],optarg);
				break;
			case 'P':
				if(strlen(optarg)>MAX_PAR_LEN)
				{
					printf("MYMON:port max length more than 500 bytes\n");
					return -1;
				}
				strcpy(par[5],optarg);
				sscanf (par[5],"%u",&mport);
				break;
			case 'p':
				if(strlen(optarg)>MAX_PAR_LEN)
				{
					printf("MYMON:password max length more than 500 bytes\n");
					return -1;
				}
				strcpy(par[1],optarg);
				break;
			case 'S':
				if(strlen(optarg)>MAX_PAR_LEN)
				{
					printf("MYMON:socket max length more than 500 bytes\n");
					return -1;
				}
				strcpy(par[2],optarg);
				break;
			case 't':
				sscanf (optarg,"%d",&t);
				break;
			case 'n':
				sscanf (optarg,"%d",&n);
				break;
			case 'l':
				if(strlen(optarg)>MAX_PAR_LEN)
				{
					printf("MYMON:logfile name max length more than 500 bytes\n");
					return -1;
				}
				strcpy(par[3],optarg);
				if((fd = fopen(par[3],OPENFLAG)) == NULL)
				{
					perror("MYMON:");
					return -1;
				}
				write_file = 1;
				setbuf(fd,NULL);//stdio buffer disable
				break;
			case 'd':
				debug = 1;
				break;
			default:
			case '?':
				useage();
				return -1;
		}
	}

	if(checkpar(par,t,n) == -1)
	{
		printf("MYMON:must parameter error\n");
		return -1;
	}
	wel();
	if(conn(par,t,n+1,fd,mport) !=0)
		{
			printf("MYMON:error pelase check log\n");
			return -1;
		}
	if(write_file == 1)
		{
			fclose(fd);
		}
	return 0;
}
