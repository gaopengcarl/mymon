/*************************************************************************
  > File Name: fun.c
  > Author: gaopeng QQ:22389860 all right reserved
  > Mail: gaopp_200217@163.com
  > Created Time: Wed 08 Feb 2017 10:08:56 AM CST
 ************************************************************************/
 /*
	This file frist store new and old data to one data struct 
	**struct->*struct->struct
	struct have three pointer
	statname
	olddata
	newdata
	
	Then sub new and old data to one struct array
	Last use array to print data
*/

 

#include"type.h"
#include"fun.h"

/*
 * field    pointer to status name
 * olddata  pointer to prev data
 * newdata  pointer to now  data
 */


typedef struct MDATA
{
        char* statname;
        char* olddata;
        char* newdata;
} MDATA;

typedef struct D_STROE
{
	char statname[MAXSTATNAME];
	int64_t data;
} D_STROE;


/* ret=0 is sucss  ret<0 is false*/
int handle_data(MYSQL_RES *result/*in*/,uint64_t row_count,unsigned int row_field,FILE* fd,MDATA* *datap/*in*/,int i,int t);
/* ret=0 is sucss  ret<0 is false*/ 
int memaloc(MDATA** *datap/*out*/,uint64_t row_count);
/* ret=0 is sucss  ret<0 is false*/ 
int sub_store_data(MDATA* *datap,D_STROE* *last_data/*out*/,uint64_t row_count,int* lenth/*in*/);
/* ret=0 is sucss  ret<0 is false*/ 
int print_data(D_STROE* last_data,int lenth,FILE* fd,int t) ;
/* ret=-1 is no data return  >=0  and -65535 is data return */ 
int64_t findva(const char* ins,D_STROE* last_data,int lenth);
/* no retrun value*/
int freedatap(MDATA* *datap,uint64_t row_count);

static freecouti=0;

void test(void)
{
	;
}


int mprintf(FILE* outfile,const char *format,...)
{
    va_list args;
    va_start(args,format);
    if( vfprintf(outfile,format,args) <0 )
    	{
    		GET_LOG(LogLevel[4],ERROR,"ERROR:mprintf() error:%d\n",ERROR);
			printf("MYMON: mprintf() error:%d\n",ERROR);
			exit(-1);
    	}
    va_end(args);
	return 0;
}



int xfree(void* p)
{
	if(p==NULL)
	{
		freecouti++;
		if(debug == 3)
		{	
			GET_LOG(LogLevel[1],DEBUG,"DEBUG:xfree(NULL) debug:%d\n",freecouti);
		}
		return 0;
	}
	free(p);
	p=NULL;
	return 0;
}

int conn(char (*par)[500]/*in*/,int t,int n,FILE* fd,unsigned int myport)
{
	MYSQL conn_ptr;
	MYSQL_RES *query_res =NULL;
	MYSQL_ROW sqlrow_data;
	int ret = 0;
	uint64_t  row_count = 0;
	unsigned int row_field = 0;
	MDATA* *datap =NULL;
	int i=0;//used to check frist time no old data
	

	/*
	 *MYSQL *mysql_init(MYSQL *mysql)
	 *An initialized MYSQL*handle. NULLif there was insufficient memory to allocate a new object.
	 */
	if(mysql_init(&conn_ptr) == NULL) 
	{
		ret = -1;
		GET_LOG(LogLevel[4],ERROR,"ERROR:mysql_init() error:%d\n",ERROR);
		printf("MYMON: mysql_init() error\n");
		return ret;
	}
	/*
	 *MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user,
	 *const char *passwd, const char *db, unsigned int port, const char *unix_socket,
	 *unsigned long client_flag)
	 *A MYSQL*connection handle if the connection was successful, NULL if the connection was unsuccessful.
	 *For a successful connection, the return value is the same as the value of the first parameter
	 */
	if(mysql_real_connect(&conn_ptr,*(par+4),*par,*(par+1),NULL,myport,*(par+2),0)==NULL)
	{
		ret = -2;
		GET_LOG(LogLevel[4],ERROR,"ERROR:mysql_real_connect() error:%d\n",ERROR);
		printf("MYMON: mysql_real_connect() error\n");
		if(mysql_errno(&conn_ptr))
		{
			GET_LOG(LogLevel[4],ERROR,"ERROR:mysql_real_connect() error:%s\n",mysql_error(&conn_ptr));
			printf("MYMON: mysql_real_connect() error:%s\n",mysql_error(&conn_ptr));
		}
		return ret;
	}

	while(n--)
	{
		/*
		 *int mysql_query(MYSQL *mysql, const char *stmt_str)
		 *Zero for success. Nonzero if an error occurred.
		 */
		if((mysql_query(&conn_ptr,"show global status")) != 0)
		{
			ret = -3;
			GET_LOG(LogLevel[4],ERROR,"ERROR:mysql_query() error:%s\n",mysql_error(&conn_ptr));
			printf("MYMON: mysql_query() error:%s\n",mysql_error(&conn_ptr));
			return ret;
		}
		/*
		 * MYSQL give memory out 
		 * MYSQL_RES *mysql_store_result(MYSQL *mysql)
		 * A MYSQL_RESresult structure with the results. NULL(0) if an error occurred
		 */

		if((query_res=mysql_store_result(&conn_ptr)) == NULL)
		{
			ret = -4;
			GET_LOG(LogLevel[4],ERROR,"ERROR:mysql_store_result() error:%s\n",mysql_error(&conn_ptr));
			printf("MYMON: mysql_store_result() error:%s\n",mysql_error(&conn_ptr));		
			return ret;
		}
		/*
		 * my_ulonglong mysql_num_rows(MYSQL_RES *result)
		 * The number of rows in the result set.
		 * unsigned int mysql_num_fields(MYSQL_RES *result)
		 * An unsigned integer representing the number of columns in a result set
		 */
		row_count = mysql_num_rows(query_res);
		row_field = mysql_field_count(&conn_ptr);
		if(debug == 1)
		{
			GET_LOG(LogLevel[2],INFO,"INFO:mysql_num_rows() rows is %llu\n",row_count);
			GET_LOG(LogLevel[2],INFO,"INFO:mysql_field_count() fileds is %ld\n",row_field);
			printf("MYMON: mysql_num_rows() rows is %llu\n",row_count);
			printf("MYMON: mysql_field_count() fileds is %ld\n",row_field);
		}
		if(i == 0)
		{
			if((memaloc(&datap,row_count)) !=0)
			{
				ret = -6;
				GET_LOG(LogLevel[4],ERROR,"ERROR:memaloc() error:%s\n","ERROR");
				printf("MYMON: memaloc() error:%s\n","ERROR");
				return ret;
			}
		}
		if(handle_data(query_res,row_count,row_field,fd,datap,i,t)!=0)
		{
			ret = -5;
			GET_LOG(LogLevel[4],ERROR,"ERROR:handle_data() error:%s\n","ERROR");
			printf("MYMON: handle_data() error:%s\n","ERROR");
			return ret;
		}

		/*
		 * void mysql_free_result(MYSQL_RES *result)
		 * no result
		 */
		mysql_free_result(query_res);
		sleep(t);
		i++;
	}
	/*
	 * void mysql_close(MYSQL *mysql)
	 * no result
	 */

	/*hear free datap*/
		{
			freedatap(datap,row_count);
		}
	mysql_close(&conn_ptr);
	return ret;
}


int freedatap(MDATA* *datap,uint64_t row_count)
{
	int ret = 0;
	uint64_t i = 0;
	if(datap == NULL)
		{
			return ret;
		}
	GET_LOG(LogLevel[3],INFO,"INFO:freedatap()  check previous line is error?:%d\n",ret);
	for(i=0;i<row_count;i++)
		{
			xfree ((*(datap+i))->statname);
			xfree((*(datap+i))->newdata);
			xfree((*(datap+i))->olddata);
			xfree(*(datap+i));
		}
	xfree(datap);
	return ret;
}
int memaloc(MDATA** *datap,uint64_t row_count)
{
	int ret=0;
	MDATA* *mp=NULL;
	uint64_t i=0;

	if(datap == NULL)
	{
		ret = -5;
		GET_LOG(LogLevel[4],ERROR,"ERROR:memaloc()  datap=NULL?:%d\n",ret);
		printf("MYMON: memaloc()  datap=NULL?:%d\n",ret);
		return ret;
	}
	if((mp = (MDATA**)calloc(1,row_count*sizeof(MDATA*))) == NULL)
	{
		ret = -3;
		GET_LOG(LogLevel[4],ERROR,"ERROR:memaloc() memory malloc **p error:%d\n",ret);
		printf("MYMON: memaloc() memory malloc **p error:%d\n",ret);
		return ret;
	}
	for(i=0;i<row_count;i++)
	{
		if((*(mp+i)=(MDATA*)calloc(1,sizeof(MDATA))) == NULL)
		{
			ret=-4;
			GET_LOG(LogLevel[4],ERROR,"ERROR:handle_data() memory malloc *p error:%d\n",ret);
			printf("MYMON: MYMON:handle_data() memory malloc *p error:%d\n",ret);
			freedatap(mp,row_count);
			return ret;
		}
	}
	*datap = mp;
	return ret;
	
}


int handle_data(MYSQL_RES *result,uint64_t row_count,unsigned int row_field,FILE* fd,MDATA* *datap,int seq,int t)
{
	uint64_t i = 0;
	MYSQL_ROW sqlrow_data;
	int ret = 0;
	D_STROE* last_data=NULL;//used last data store
	int lenth=0;//last data lenth

	//tmp
	int tmp = 0;
    D_STROE* tmp_last_data = NULL;

	if(result == NULL||datap == NULL)
	{
		ret = -2;
		GET_LOG(LogLevel[4],ERROR,"ERROR:handle_data() result == NULL datap == NULL :%s\n","ERROR");
		printf("MYMON:handle_data() result == NULL datap == NULL :%s\n","ERROR");
		return ret;
	}

	for(i=0;i<row_count;i++)//control true result return
	{
		/*
		 * MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)
		 * A MYSQL_ROW structure for the next row. NULL if there are no more rows to retrieve or if an error occurred
		 */
		if((sqlrow_data=mysql_fetch_row(result)) == NULL) //control mysql_fetch_row error
		{
			ret = -1;
			GET_LOG(LogLevel[4],ERROR,"ERROR:mysql_fetch_row() error:%s\n","ERROR");
			printf("MYMON: mysql_fetch_row() error:%s\n","ERROR");
			return ret;
		}
		{
			unsigned long *lengths;
			lengths = mysql_fetch_lengths(result);
			xfree((*(datap+i))->statname);
			if(((*(datap+i))->statname = (char*)calloc(1,lengths[0]+10)) == NULL)
			{
				ret = -4;
				GET_LOG(LogLevel[4],ERROR,"ERROR:handle_data() statname loop mem alloc error:%s\n","ERROR");
				printf("MYMON: handle_data() statname loop mem alloc error:%s\n","ERROR");
				return ret;

			}
			strcpy((*(datap+i))->statname,sqlrow_data[0] ? sqlrow_data[0] : "NULL");
			xfree((*(datap+i))->olddata);
			(*(datap+i))->olddata = (*(datap+i))->newdata;
			//no free newdata pointer
			if(((*(datap+i))->newdata =  (char*)calloc(1,lengths[1]+10)) == NULL)
			{
				ret = -5;
				GET_LOG(LogLevel[4],ERROR,"ERROR:handle_data() newdata loop mem alloc error:%s\n","ERROR");
				printf("MYMON: handle_data() newdata loop mem alloc error:%s\n","ERROR");
				return ret;
			}
			strcpy((*(datap+i))->newdata,sqlrow_data[1] ? sqlrow_data[1] : "NULL");
			if(debug == 1)
				{
			       if((!strcmp((*(datap+i))->statname,"NULL")) || (!strcmp((*(datap+i))->newdata,"NULL")) )
			       	{
			       		GET_LOG(LogLevel[1],DEBUG,"DEBUG:handle_data() statname or data IS NULL:[%s]\n",(*(datap+i))->statname);
						GET_LOG(LogLevel[1],DEBUG,"DEBUG:handle_data() statname or data IS NULL:[%s]\n",(*(datap+i))->newdata);
			       	}
				}
		}

	}
	if(seq == 0)//olddata have no data
		{
			if(debug == 1)
				{
			GET_LOG(LogLevel[1],DEBUG,"DEBUG:handle_data() first time is info %s\n","no old data");
			printf("MYMON: handle_data() first time is info %s\n","no old data");
				}
		}
	if(seq != 0 ) //newdata and olddata have data
		{
			if(sub_store_data(datap,&last_data,row_count,&lenth) != 0 ) //store data to array
				{
					ret = -6;
					GET_LOG(LogLevel[4],ERROR,"ERROR:handle_data() sub_store_data() error:%s\n","ERROR");
					printf("MYMON: handle_data() sub_store_data() error %s\n","ERROR");
					return ret;
				}
		if(debug == 1)
			{
			tmp_last_data = last_data;
			for(tmp =0;tmp<lenth;tmp++)
				{
					GET_LOG(LogLevel[1],DEBUG,"DEBUG:handle_data() find data is [%s]",(tmp_last_data)->statname);
					GET_LOG(LogLevel[1],DEBUG,"[%lld] \n",(tmp_last_data)->data);
					tmp_last_data++;
				}
			}


			
			if(print_data(last_data,lenth,fd,t) != 0)  //store data from array
				{
					ret = -7;
					GET_LOG(LogLevel[4],ERROR,"ERROR:handle_data() print_data() error:%s\n","ERROR");
					printf("MYMON: handle_data() print_data() error %s\n","ERROR");
					return ret;
				}
			
		}
	//used end free last data array 
	xfree(last_data);
	return ret;
}


int sub_store_data(MDATA* *datap,D_STROE* *last_data,uint64_t row_count,int* lenth)
{
	D_STROE* tmp_p = NULL;
	int ret = 0;
	uint64_t i = 0;

	char* *dyd_d  = NULL;
	char* *std_d = NULL;
	int64_t a = 0;
	int64_t b = 0;

	if(datap == NULL || last_data == NULL )
		{
		    ret = -1;
			GET_LOG(LogLevel[4],ERROR,"ERROR:sub_store_data() datap == NULL || fail_data == NULL %s\n","ERROR");
			printf("MYMON: sub_store_data() datap == NULL || fail_data == NULL %s\n","ERROR");
			return ret;
		}
	
	if((tmp_p = (D_STROE*)calloc(1,(sizeof(stdata)+sizeof(dydata))*sizeof(D_STROE))) == NULL)
		{
			ret = -2;
			GET_LOG(LogLevel[4],ERROR,"ERROR:sub_store_data()  mem alloc error:%s\n","ERROR");
			printf("MYMON: sub_store_data()  mem alloc error:%s\n","ERROR");
			return ret;
		}
	*last_data = tmp_p;

	for(i=0;i<row_count;i++)
		{
			dyd_d = dydata; //re pointer to dydata
	        std_d = stdata; //re pointer to dydata
			if(strlen((*(datap+i))->statname)>MAXSTATNAME)
				{
					ret = -3;
					GET_LOG(LogLevel[4],ERROR,"ERROR:sub_store_data()  statname>MAXSTATNAME:%s\n","ERROR");
					printf("MYMON: sub_store_data()  statname>MAXSTATNAME:%s\n","ERROR");
					goto er;
				}
			while(*dyd_d)
				{
					if(debug == 3)
						{
							printf("dyd [%s] [%s] [%d] [%d]\n",*dyd_d,(*(datap+i))->statname,strlen(*dyd_d),strlen((*(datap+i))->statname));
						}
					if(!strcmp(*dyd_d,(*(datap+i))->statname))
						{
							
							if(debug == 1)
								{
									GET_LOG(LogLevel[1],DEBUG,"DEBUG:sub_store_data() find data is %s  \n",*dyd_d);
								}
							strcpy(tmp_p->statname,*dyd_d);
							if( !strcmp((*(datap+i))->olddata,"NULL") || !strcmp((*(datap+i))->newdata,"NULL")) //check "NULL" DATA ?
								{
									GET_LOG(LogLevel[2],INFO,"INFO:sub_store_data() data %s is [NULL] \n",*dyd_d);
									tmp_p->data = ERRORDATA;
								}							
							if(sscanf((*(datap+i))->olddata,"%lld",&a)  == EOF ||  sscanf((*(datap+i))->newdata,"%lld",&b) == EOF)
								{
									ret = -4;
									GET_LOG(LogLevel[4],ERROR,"ERROR:sub_store_data() sscanf() error %s\n","ERROR");
									printf("MYMON: sub_store_data() sscanf() error:%s\n","ERROR");
									goto er;
								}
							tmp_p->data = b-a; //new - old
							tmp_p++;
							(*lenth)++;
							break;
						}
					dyd_d++;
				}
			while(*std_d)
				{
					if(debug == 3)
						{
							printf("std [%s] [%s] [%d] [%d]\n",*std_d,(*(datap+i))->statname,strlen(*std_d),strlen((*(datap+i))->statname));
						}
					if(!strcmp(*std_d,(*(datap+i))->statname))
						{
							
						     if(debug == 1)
								{
									GET_LOG(LogLevel[1],DEBUG,"DEBUG:sub_store_data() find data is %s \n",*std_d);
								}
							strcpy(tmp_p->statname,*std_d);	
							if(!strcmp((*(datap+i))->newdata,"NULL")) //check "NULL" DATA ?
								{
									GET_LOG(LogLevel[2],INFO,"INFO:sub_store_data() data %s is [NULL] \n",*std_d);
									tmp_p->data = ERRORDATA;
								}
							if(sscanf((*(datap+i))->olddata,"%lld",&tmp_p->data) == EOF)
								{
									ret = -4;
									GET_LOG(LogLevel[4],ERROR,"ERROR:sub_store_data() sscanf() error %s\n","ERROR");
									printf("MYMON: sub_store_data() sscanf() error:%s\n","ERROR");
									goto er;
								}
							tmp_p++;
							(*lenth)++;
							break;
						}
				std_d++;
				}
		}
	return ret;

	er:
		xfree(tmp_p);
		*last_data= NULL;
		return ret;
}

int print_data(D_STROE* last_data,int lenth,FILE* fd,int t) 
{
	time_t rawtime;  
	struct tm * timeinfo = NULL;
	int ret = 0;
	FILE* outfile = NULL;
	if(write_file == 1)
		{  
			outfile=fd;
		}
	else
		{
			outfile=stdout;
		}
	
	if(time (&rawtime) == -1)
		{
			ret = -2;
			GET_LOG(LogLevel[4],ERROR,"ERROR:print_data() time() error %s\n","ERROR");
            printf("MYMON: ERROR:print_data() time() error:%s\n","ERROR");
			return ret;
		}
	timeinfo = localtime ( &rawtime );
	if (timeinfo == NULL )
		{
			ret = -1;
			GET_LOG(LogLevel[4],ERROR,"ERROR:print_data() localtime () error %s\n","ERROR");
			printf("MYMON: ERROR:print_data() localtime () error:%s\n","ERROR");
			return ret;
		}
	mprintf(outfile,"------------------------------------------MYMON--------------------------------------------\n");
	mprintf(outfile,"SysTime: %s",asctime (timeinfo));
	mprintf(outfile,
		    "%s%-11ld%s%-8ld%s%-8ld%s%-8ld\n",
			"Uptime:",findva("Uptime",last_data,lenth),
			"Connections:",findva("Connections",last_data,lenth) ,
			"Aborted_clients:",findva("Aborted_clients",last_data,lenth),
			"Max_used_connections:",findva("Max_used_connections",last_data,lenth));
	mprintf(outfile,"--------------------------------------MYSQLINFO-------------------------------------------\n");
	mprintf(outfile,"[###Threads info and qc info:]\n");
	mprintf(outfile,
		    "%s%-12ld%s%-14ld%s%-13ld\n",
		   "Threads_connected:",findva("Threads_connected",last_data,lenth),
           "Threads_running:",findva("Threads_running",last_data,lenth),
           "Qcache_free_memory:",findva("Qcache_free_memory",last_data,lenth)
			);
	mprintf(outfile,
		    "%s%-18ld%s%-12ld%s%-17ld\n",
		   "Qcache_hits:",findva("Qcache_hits",last_data,lenth),
		   "Qcache_not_cached:",findva("Qcache_not_cached",last_data,lenth),
		   "Qcache_inserts:",findva("Qcache_inserts",last_data,lenth)
		  );
	mprintf(outfile,"[###Table read info:]\n");
	mprintf(outfile,
		    "%s%-11ld%s%-13ld%s%-14ld\n",
			"Handler_read_first:",findva("Handler_read_first",last_data,lenth),
			"Handler_read_key:",findva("Handler_read_key",last_data,lenth),
			"Handler_read_next:",findva("Handler_read_next",last_data,lenth)
			);
	
	mprintf(outfile,
		    "%s%-12ld%s%-13ld%s%ld\n",
			"Handler_read_last:",findva("Handler_read_last",last_data,lenth),
			"Handler_read_rnd:",findva("Handler_read_rnd",last_data,lenth),
			"Handler_read_rnd_next:",findva("Handler_read_rnd_next",last_data,lenth)
			);	
	mprintf(outfile,"[###Sorts and temp tables info:]\n");
	mprintf(outfile,
		    "%s%-20ld%s%ld\n",
		    "Sort_rows:",findva("Sort_rows",last_data,lenth),
			"Sort_merge_passes:",findva("Sort_merge_passes",last_data,lenth)
			);	
	mprintf(outfile,
	 	    "%s%-11ld%s%ld\n",
			"Created_tmp_tables:",findva("Created_tmp_tables",last_data,lenth),
			"Created_tmp_disk_tables:",findva("Created_tmp_disk_tables",last_data,lenth)
			);	
	mprintf(outfile,"[###Table join info:]\n");
    mprintf(outfile,
		    "%s%-13ld%s%ld\n",
			"Select_full_join:",findva("Select_full_join",last_data,lenth),
			"Select_scan:",findva("Select_scan",last_data,lenth)
			);		 
	 
	  mprintf(outfile,"[###Open table cache and binlog cache info:]\n");
	  mprintf(outfile,
	 	    "%s%-8ld%s%ld\n",
			"Table_open_cache_hits:",findva("Table_open_cache_hits",last_data,lenth),
			"Table_open_cache_misses:",findva("Table_open_cache_misses",last_data,lenth)
			);	
	  mprintf(outfile,
	 	    "%s%-13ld%s%ld\n",
			"Binlog_cache_use:",findva("Binlog_cache_use",last_data,lenth),
			"Binlog_cache_disk_use:",findva("Binlog_cache_disk_use",last_data,lenth)
			);	
	 mprintf(outfile,"[###Hanler_xxx and Com_xxx info:]\n");

	 mprintf(outfile,
	 	    "%s%-15ld%s%-15ld%s%ld\n",
			"Handler_delete:",findva("Handler_delete",last_data,lenth),
			"Handler_update:",findva("Handler_update",last_data,lenth),
			"Handler_write:",findva("Handler_write",last_data,lenth)
			);
	 mprintf(outfile,
	 	    "%s%-15ld%s%-ld\n",
			"Handler_commit:",findva("Handler_commit",last_data,lenth),
			"Handler_rollback:",findva("Handler_rollback",last_data,lenth)
			);
	  mprintf(outfile,
			 "%s%-19ld%s%ld\n",
			 "Com_commit:",findva("Com_commit",last_data,lenth),
			 "Com_rollback:",findva("Com_rollback",last_data,lenth)
			 );
	mprintf(outfile,
		    "%s%-8ld%s%-8ld%s%-8ld%s%-8ld\n",
			"U(counts):",findva("Com_update",last_data,lenth),
			"D(counts):",findva("Com_delete",last_data,lenth),
			"I(counts):",findva("Com_insert",last_data,lenth),
			"S(counts):",findva("Com_select",last_data,lenth)
			); 	 
    mprintf(outfile,
		   "--------------------------------------INNODBINFO------------------------------------------\n");
    mprintf(outfile,"[###Innodb_lock info:]\n");
	mprintf(outfile,
		    "%s%-17ld%s%ld\n",
			"Innodb_row_lock_time:",findva("Innodb_row_lock_time",last_data,lenth),
			"Innodb_row_lock_waits:",findva("Innodb_row_lock_waits",last_data,lenth)
			);  
	mprintf(outfile,
		    "%s%ld\n",
			"Innodb_row_lock_current_waits:",findva("Innodb_row_lock_current_waits",last_data,lenth)
			);  
    mprintf(outfile,"[###Innodb_buffer_pool info:]\n");

	mprintf(outfile,"%s%-10ld%s%ld\n","Innodb_buf_pool_pages_total:",findva("Innodb_buffer_pool_pages_total",last_data,lenth)
		    ,"Innodb_buf_pool_pages_dirty:",findva("Innodb_buffer_pool_pages_dirty",last_data,lenth));
	
	mprintf(outfile,"%s%-16ld%s%ld\n","Innodb_buf_pages_data:",findva("Innodb_buffer_pool_pages_data",last_data,lenth)
		    ,"Innodb_buf_pages_free:",findva("Innodb_buffer_pool_pages_free",last_data,lenth));
	
	mprintf(outfile,
		    "%s%-9ld%s%ld\n",
			"Innodb_buffer_pool_wait_free:",findva("Innodb_buffer_pool_wait_free",last_data,lenth),
			"Innodb_buffer_pool_pages_misc:",findva("Innodb_buffer_pool_pages_misc",last_data,lenth)
			);  
	mprintf(outfile,
		    "%s%-11ld%s%ld\n",
			"Innodb_dblwr_pages_written:",findva("Innodb_dblwr_pages_written",last_data,lenth),
			"Innodb_dblwr_writes:",findva("Innodb_dblwr_writes",last_data,lenth)
			); 

	mprintf(outfile,
		    "%s%ld\n",
			"Innodb_buffer_pool_pages_flushed:",findva("Innodb_buffer_pool_pages_flushed",last_data,lenth)
			);   
	mprintf(outfile,
		     "%s%-10ld %s%-10ld %s%-10ld\n",
		     "read_ahead:",findva("Innodb_buffer_pool_read_ahead",last_data,lenth),
		     "ahead_evict:",findva("Innodb_buffer_pool_read_ahead_evicted",last_data,lenth),
		     "ahead_rnd:",findva("Innodb_buffer_pool_read_ahead_rnd",last_data,lenth));
	mprintf(outfile,"[###Innodb_log info:]\n");
	mprintf(outfile,
		     "%s%-20ld%s%ld\n",
			"Innodb_log_writes:",findva("Innodb_log_writes",last_data,lenth),
			"Innodb_log_waits:",findva("Innodb_log_waits",last_data,lenth)
			);  	
	mprintf(outfile,
		    "%s%-9ld%s%ld\n",
			"Innodb_os_log_pending_fsyncs:",findva("Innodb_os_log_pending_fsyncs",last_data,lenth),
			"Innodb_os_log_pending_writes:",findva("Innodb_os_log_pending_writes",last_data,lenth)
			);  
	mprintf(outfile,
		    "%s%ld\n",
			"Innodb_os_log_written:",findva("Innodb_os_log_written",last_data,lenth)
		   );  
	mprintf(outfile,"[###Innodb_data read/writes info:]\n");
    mprintf(outfile,
		    "%s%-20ld%s%ld\n",
			"Innodb_data_reads:",findva("Innodb_data_reads",last_data,lenth),
			"Innodb_data_read:",findva("Innodb_data_read",last_data,lenth)
			);  
    mprintf(outfile,
		    "%s%-19ld%s%ld\n",
			"Innodb_data_writes:",findva("Innodb_data_writes",last_data,lenth),
			"Innodb_data_written:",findva("Innodb_data_written",last_data,lenth)
			);  
	mprintf(outfile,
		    "%s%-19ld%s%ld\n",
		    "Innodb_data_fsyncs:",findva("Innodb_data_fsyncs",last_data,lenth),
		    "Innodb_data_pending_fsyncs:",findva("Innodb_data_pending_fsyncs",last_data,lenth)
		    );
	mprintf(outfile,
		    "%s%ld\n",
			"Innodb_buffer_pool_reads(physics reads):",findva("Innodb_buffer_pool_reads",last_data,lenth)
		   );  
	mprintf(outfile,
		    "%s%ld\n",
			"Innodb_buffer_pool_read_requests(logic reads):",findva("Innodb_buffer_pool_read_requests",last_data,lenth)
		   );  

	mprintf(outfile,"[###Innodb_data rows info:]\n");
	mprintf(outfile,
		    "%s%-8ld%s%-8ld%s%-8ld%s%-8ld\n",
			"U(rows):",findva("Innodb_rows_updated",last_data,lenth),
			"D(rows):",findva("Innodb_rows_deleted",last_data,lenth),
			"I(rows):",findva("Innodb_rows_inserted",last_data,lenth),
			"S(rows):",findva("Innodb_rows_read",last_data,lenth)
			); 	 

	mprintf(outfile,"---------------------------------------TOTALINFO------------------------------------------\n");
		{
			int64_t qps =(int64_t) (findva("Com_update",last_data,lenth)+findva("Com_delete",last_data,lenth)+findva("Com_insert",last_data,lenth)+findva("Com_select",last_data,lenth))/(int64_t)(t);
			int64_t rms  = (int64_t)(findva("Innodb_rows_updated",last_data,lenth)+findva("Innodb_rows_deleted",last_data,lenth)+findva("Innodb_rows_inserted",last_data,lenth))/(int64_t)(t);

			if((findva("Innodb_buffer_pool_read_requests",last_data,lenth)+findva("Innodb_buffer_pool_reads",last_data,lenth)+(int64_t)1)<1)
				{
					ret = -1;
					GET_LOG(LogLevel[4],ERROR,"ERROR:print_data() Innodb_buffer_pool_read_requests+Innodb_buffer_pool_reads+1 must>0 %s\n","ERROR");
					printf("MYMON: print_data() Innodb_buffer_pool_read_requests+Innodb_buffer_pool_reads+1 must>0 %s\n","ERROR");
					return ret;
				}
			float bch = (((findva("Innodb_buffer_pool_read_requests",last_data,lenth)+(int64_t)1)*1.0)/((findva("Innodb_buffer_pool_read_requests",last_data,lenth)+findva("Innodb_buffer_pool_reads",last_data,lenth)+(int64_t)1)*1.0));
	mprintf(outfile,
		    "%s%-20.2f%s%-20ld%s%-20ld\n",
			"bch:",bch,
	        "rms:",rms,
			"qps:",qps
			);	
		}
	return ret;
}

int64_t findva(const char* ins,D_STROE* last_data,int lenth)
{
	int i=0;
	for(i=0;i<lenth;i++)
		{
			if(!strcmp(last_data->statname,ins))
				{
					return last_data->data;
					break;
				}
			last_data++;
		}
	return (int64_t)(-1);
}


