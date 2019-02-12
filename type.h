/*************************************************************************
  > File Name: type.h
  > Author: gaopeng QQ:22389860 all right reserved
  > Mail: gaopp_200217@163.com
  > Created Time: Wed 08 Feb 2017 07:50:30 AM CST
 ************************************************************************/
#ifndef __TYPE
#define __TYPE
#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>




#include"mysql.h"
#include"log.h"

#define GET_LOG(c,d,e,f) ITCAST_LOG(__FILE__,__LINE__,c,d,e,f)
#define MAX_PAR_LEN 500

//#define OPENFLAG O_CREAT|O_WRONLY|O_NOFOLLOW|O_EXCL|O_APPEND
#define OPENFLAG "a+"

#define ERROR 4
#define WARN  3
#define INFO  2
#define DEBUG 1

#define MAXSTATNAME 200
#define ERRORDATA   -65535


/*
 *PAR[0]:mysql username
 *PAR[1]:mysql password
 *PAR[2]:mysql socket
 *PAR[3]:mon logfile
 *PAR[4]:mysql host
 *PAR[5]:mysql port
 */
typedef char (PAR)[6][500];
#endif
/* ret=0 is sucss  ret<0 is false*/
int conn(char (*par)[500]/*in*/,int t,int n,FILE* fd,unsigned int myport);
/*GOLBAL VAR*/
int debug;
int write_file;
