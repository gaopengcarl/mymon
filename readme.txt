һ���˹���չʾ��ʽ�� 
    ���߽�show global status��һЩ��Ҫ��ֵ�����û�����������ʹ������ϵĴ�ӡ��ǰ̨���ߴ�ӡ��
ָ���ļ��У����ڰ���DBA����������ϻ���ƿ���жϡ�
�����˹�������ԭ��
   ��show global status��ֵ�����û������������¼��ǰһ�ε�ֵΪoldvalue,�µ�ֵΪnewvalue���洢
���ڴ��У�Ȼ���ж��Ƿ�Ϊ�ۼ�ֵ���ǵ�ǰֵ��������ۼ�ֵ��newvalue-oldvalue�����Ϊ��ǰֵ��
�����������֤���������ȷ��
���������ص㣺

1������˺�ֻ��ҪuseageȨ��
2��Զ�����ӹ���
3�������û���Ҫ������д�뵽ָ���ļ�
4��������־��¼��������������־
5��5.6 5.7֧�֡�û�в���5.5
6������Դ�뿪Դ
7�����Ը���������ʱ�����������Ȼ��Ҫ��ϵ����

ע�⣺
WINDOWS��֧�֣�����汾ֻ��LINUX 64BIT.
���Ҵ˹�������MYSQL���Ӷ�̬���ļ�

�ġ�������ʹ�ð���
[root@testmy 6]# ./mymon 
Author: gaopeng QQ:22389860 Mail: gaopp_200217@163.com 
Info:This tool collect data from 'show global status' and show interval (-t) seconds data change
help DBA to diagnose performance problem,when error or (-d) run log is at current dir name 
is mymon.log.
WIN32 platform is not support
Useage:./mymon [-u username] [-p password] -S socketfile/{-h hosname -P port} -t seconds -n times [-l logfile] [-d]
[-?]           :help info;
[-u username]  :connect to mysqld username,when no username requisite is optional;
[-p password]  :connect to mysqld password,when no password requisite is optional;
[-S socketfile]:connect to mysqld socket file;
[-h hostname]  :connect to mysqld host;
[-P port]      :connect to mysqld port;
                socketfile or [hostname port]  must;
[-t seconds]   :seconds of interval,this is necessary;
[-n times]     :how many times you watch,this is necessary;
[-l logfile]   :when you want to write information to a logfile provide a logfile name
                when a logfile provide no information at foreground,is optional;
[-d]           :more running log at logfile,otherwise error log in mymon.log;

   ע�Ȿ��������-S����ָ��socketfile���ɣ�Զ��ʹ�� -h -P���� ʹ��IP�Ͷ˿����� ,-l ����ֱ�Ӱ����д�뵽ָ�����ļ���
-d����debugģʽһ�㲻�ñ����߻��¼һ����־�ڵ�ǰĿ¼������Ϊ��mymon.log������������־����д�����¼����־��
����-d �����������Ϣ������һ�㲻��Ҫ�ã���Ҫ���ҵ��Դ����ʱ���õ�.��Ȼ-t �� -n�������˵���Ǽ����������
�����Ĵ�����ʹ������������ֵΪ-1�����ֵû�вɼ���.
   һ��ֻ��Ҫ������һ̨�������Ϸ��ϱ����ߣ�ʹ��Զ���������ӵ���η��������ɣ��磺
   ./mymon -h 192.168.190.91 -P3306 -t 5 -n 100 -ummon2 -pgelc123 -l /root/mysqlog/20170123mysql.log
   ��ʾ���ӵ�������192.168.190.91 3306�˿� 5�����һ�� ���100�Σ�����������ļ�/root/mysqlog/20170123mysql.log
   ����ע��SysTime: Sun Jan 22 16:13:30 2017 ��������г���ϵͳ�ĵ�ǰʱ�䣬����Uptime��Ȼ����MYSQLD��������ʱ��

�ġ������ú�ʹ��
��ȡ��
��һ���������4���汾
5.7 �����汾      mymon5.7c?
5.7 percona�汾  mymon5.7p
5.6 �����汾       mymon5.6c
5.6 perconna�汾 mymon5.6p
�ٶ��������£�
http://pan.baidu.com/s/1jIa6OfG


ʹ�ã�
���غ��ϴ��� LINUX����ϵͳ,
mkdir mon
cp mymon5.7p ./mon/
chmod 755 mymon5.7p
Ȼ������ҵ���Ӧ�Ŀ��ļ�λ��һ���ڰ�װλ�õ�libĿ¼�£��磺
[root@testmy 9]# ./mymon5.7c
./mymon5.7c: error while loading shared libraries: libmysqlclient.so.20: cannot open shared object file: No such file or directory
[root@testmy 9]# export LD_LIBRARY_PATH=/mysqldata/mysql5.7/lib
[root@testmy 9]# ./mymon5.7c
Author: gaopeng QQ:22389860 Mail: gaopp_200217@163.com 
�Ϳ����������У�˳��˵һ��5.6Ϊ so.18 5.7Ϊso.20���ϵ�Ϊ5.6��ǰ�汾

����MYSQL����û�Ȩ�ޣ���ز���ҪʲôȨ�ޣ�ֻҪ����һ���û�����
mysql> create user moni@'%' identified by 'gelc123';
Query OK, 0 rows affected (0.22 sec)
mysql> show grants for  moni@'%' ;
+----------------------------------+
| Grants for moni@%                |
+----------------------------------+
| GRANT USAGE ON *.* TO 'moni'@'%' |
+----------------------------------+
1 row in set (0.06 sec)

��Ҳ��С���˹��ǣ���Ϊ����˻�����û��ʲôȨ�ޣ���������ĳ��򣬿��Խ�����ŵ�Զ�����ӵ��������ݿ��磺
./mymon5.6c -umoni -pgelc123 -h192.168.190.93 -P3307 -n 1 -t 1 -l /tmp/mysqlmon.log

�����������Ȳ�����Ȩ�޶���Ҳ�����������ݿ�����������ҿ��Լ��й�������һ��ע�⣺
SysTime: Sun Jan 22 16:13:30 2017 ��������г���ϵͳ�ĵ�ǰʱ�䣬����Uptime��Ȼ����MYSQLD��������ʱ��


�塢�˹���չʾ���ݣ�
		չʾ�����ݲ����ǹ̶������ڸ���������Խ��м򵥵ļ���ָ�꼴�ɣ���ǰ���Ϊ���£�
------------------------------------------MYMON--------------------------------------------
SysTime: Sun Jan 22 16:13:30 2017
Uptime:6963226    Connections:1       Aborted_clients:0       Max_used_connections:1319    
--------------------------------------MYSQLINFO-------------------------------------------
[###Threads info and qc  and :]
Threads_connected:252         Threads_running:4             Qcache_free_memory:0            
Qcache_hits:0                 Qcache_not_cached:0           Qcache_inserts:0                
[###Table read info:]
Handler_read_first:211        Handler_read_key:8804         Handler_read_next:42346         
Handler_read_last:0           Handler_read_rnd:73           Handler_read_rnd_next:717694
[###Sorts and temp tables info:]
Sort_rows:0                   Sort_merge_passes:0
Created_tmp_tables:163        Created_tmp_disk_tables:152
[###Table join info:]
Select_full_join:1            Select_scan:207
[###Open table cache and binlog cache info:]
Table_open_cache_hits:4938    Table_open_cache_misses:0
Binlog_cache_use:820          Binlog_cache_disk_use:0
[###Hanler_xxx and Com_xxx info:]
Handler_delete:34574          Handler_update:675            Handler_write:1364
Handler_commit:5833           Handler_rollback:10
Com_commit:156                Com_rollback:5
U(counts):631     D(counts):159     I(counts):47      S(counts):4955    
--------------------------------------INNODBINFO------------------------------------------
[###Innodb_lock info:]
Innodb_row_lock_time:0                Innodb_row_lock_waits:0
Innodb_row_lock_current_waits:38
[###Innodb_buffer_pool info:]
Innodb_buf_pool_pages_total:1965840   Innodb_buf_pool_pages_dirty:19154
Innodb_buf_pages_data:1940433         Innodb_buf_pages_free:8116
Innodb_buffer_pool_wait_free:0        Innodb_buffer_pool_pages_misc:17291
Innodb_dblwr_pages_written:1188       Innodb_dblwr_writes:80
Innodb_buffer_pool_pages_flushed:2249
[###Innodb_log info:]
Innodb_log_writes:796                 Innodb_log_waits:0
Innodb_os_log_pending_fsyncs:0        Innodb_os_log_pending_writes:0
Innodb_os_log_written:44073472
[###Innodb_data read/writes info:]
Innodb_data_reads:684                 Innodb_data_read:11206656
Innodb_data_writes:2067               Innodb_data_written:63603200
Innodb_data_fsyncs:1017               Innodb_data_pending_fsyncs:0
Innodb_buffer_pool_reads(physics reads):423
Innodb_buffer_pool_read_requests(logic reads):2869823
[###Innodb_data rows info:]
U(rows):675     D(rows):34575   I(rows):194     S(rows):767094  
---------------------------------------TOTALINFO------------------------------------------
Bytes_received:911280         Bytes_sent:8653956

�������ֵ˵����
(�����֣����������Լ�����⣬���������ָ��)

����
��ǰֵ���������show global status�ĵ�ǰֵ���
�ۼ�ֵ������������һ�ε�ֵ��ȥ��һ�ε�ֵ��������Ϊ�û�ָ��������

��һ���֣�
SysTime: Sun Jan 22 16:13:30 2017
Uptime:6963226    Connections:1       Aborted_clients:0       Max_used_connections:1319    

Uptime:The number of seconds the MySQL server has been running.(��ǰֵ��� ��λ��)
Connections:The number of connection attempts (successful or not) to the MySQL server.(�ۼ�ֵ������ ��λ����)
Aborted_clients:The number of connections that were aborted because the client died without closing the connection
                properly.(�ۼ�ֵ����������λ������)
Max_used_connections:The maximum number of connections that have been in use simultaneously since the server started.
                     (��ǰֵ�������λ������)
�ڶ����֣�
[###Threads info and qc  and :]
Threads_connected:252         Threads_running:4             Qcache_free_memory:0            
Qcache_hits:0                 Qcache_not_cached:0           Qcache_inserts:0        

Threads_connected��The number of currently open connections.(��ǰֵ��� ��λ������)
Threads_running��The number of threads that are not sleeping.(��ǰֵ��� ��λ������)
Qcache_free_memory:The amount of free memory for the query cache.(��ǰֵ��� ��λ�ֽ���)
Qcache_hits:The number of query cache hits.(�ۼ�ֵ������ ��λ����)
Qcache_not_cached:The number of noncached queries.(�ۼ�ֵ������ ��λ����)
Qcache_inserts:The number of queries added to the query cache.(�ۼ�ֵ������ ��λ����)

�������֣�
������ȫ����(�ۼ�ֵ������ ��λ����)
[###Table read info:]
Handler_read_first:211        Handler_read_key:8804         Handler_read_next:42346         
Handler_read_last:0           Handler_read_rnd:73           Handler_read_rnd_next:717694

Handler_read_first:
         /*�����е�һ����¼�����Ĵ���������ϸߣ���������������ִ�д���ȫ����ɨ�裻���磬SELECT * order by id���ٶ�id����������
         * eg:
         * +----+-------------+-------+------------+-------+---------------+---------+---------+------+------+----------+-------+
         * | id | select_type | table | partitions | type  | possible_keys | key     | key_len | ref  | rows | filtered | Extra |
         * +----+-------------+-------+------------+-------+---------------+---------+---------+------+------+----------+-------+
         * |  1 | SIMPLE      | testj | NULL       | index | NULL          | PRIMARY | 276     | NULL |   19 |   100.00 | NULL  |
         * +----+-------------+-------+------------+-------+---------------+---------+---------+------+------+----------+-------+
         */
Handler_read_key:
         /*����������һ�е�������������ϸߣ�˵����ѯ�ͱ��������ȷ����ֵ������ref����constҲ������range��ֻ�Ǵ�������KEY����ʹ��������
         * eg:
         * mysql> explain select * from testshared3 where id>1;
         * +----+-------------+-------------+------------+-------+---------------+------+---------+------+------+----------+-----------------------+
         * | id | select_type | table       | partitions | type  | possible_keys | key  | key_len | ref  | rows | filtered | Extra                 |
         * +----+-------------+-------------+------------+-------+---------------+------+---------+------+------+----------+-----------------------+
         * |  1 | SIMPLE      | testshared3 | NULL       | range | id            | id   | 5       | NULL |    1 |   100.00 | Using index condition |
         * +----+-------------+-------------+------------+-------+---------------+------+---------+------+------+----------+-----------------------+
         */
Handler_read_next:
         /*��������˳�����һ�е�������,���������Ψһ����ʹ��Ψһ������ǰ׺���߷�Χ���ᴥ�����ֵ,�򵥵�˵ֻ��Ψһ���в��������ֵ
         * ���ֵ����Ƚϴ�˵����Ҫ����������һ��key�Ĵ����ǳ���
         *
         *eg:
         *mysql> explain select * from testshared3 where id=1;
         *+----+-------------+-------------+------------+------+---------------+------+---------+-------+--------+----------+-------+
         *| id | select_type | table       | partitions | type | possible_keys | key  | key_len | ref   | rows   | filtered | Extra |
         *+----+-------------+-------------+------------+------+---------------+------+---------+-------+--------+----------+-------+
         *|  1 | SIMPLE      | testshared3 | NULL       | ref  | id            | id   | 5       | const | 523260 |   100.00 | NULL  |
         *+----+-------------+-------------+------------+------+---------------+------+---------+-------+--------+----------+-------+
         *
         *| Handler_read_key      | 49      |
         *| Handler_read_next     | 897698  |
         */
Handler_read_last:
        /*
         * ��ѯ���������һ��������������������ʹ��ORDER BYʱ�����������ȷ���ʹ�õ�һ������������֮��˳������ɨ����������ʹ��ORDER BY DESCʱ��
         * ���������ȷ���ʹ�����һ������������ ֮����ǰɨ��������
         * ���磺select �� order by id desc limit 10;
         */

Handler_read_rnd,Handler_read_rnd_next:������ֵ�ϴ������������ȫ��ɨ�輰type=ALL

��һ��������Դ������յ�ǰ���ݿ�ȫ��ɨ�������ʹ�õĴ�����  
���Ĳ��֣�
[###Sorts and temp tables info:]
Sort_rows:0                   Sort_merge_passes:0
Created_tmp_tables:163        Created_tmp_disk_tables:152

Sort_rows:The number of sorted rows.(�ۼ�ֵ������ ��λ����)
Sort_merge_passes:The number of merge passes that the sort algorithm has had to do.(�ۼ�ֵ������ ��λ����)
Created_tmp_tables:The number of internal temporary tables created by the server while executing statements.(�ۼ�ֵ������ ��λ����)
Created_tmp_disk_tables:The number of internal on-disk temporary tables created by the server while executing statements.(�ۼ�ֵ������ ��λ����)

��������һ����˵�����������ʱ��ʹ����������԰���DBA�����ж�

���岿�֣�
[###Table join info:]
Select_full_join:1            Select_scan:207

Select_full_join:The number of joins that perform table scans because they do not use indexes. 
                 If this value is not 0, you should carefully check the indexes of your tables
                 (�ۼ�ֵ������ ��λ����)
Select_scan:The number of joins that did a full scan of the first table. (�ۼ�ֵ������ ��λ����)

������ֵ��˵����jion��ʱ���Ƿ�ʹ�õ����������ر���Select_full_join��Ӧ�ñȽϵͣ�����˵����������û��ʹ�õ�����
Ӧ���������ӣ��Ż���䣬һ������Select_scan�Ƚϸ�����������Ϊ���������û��ν������ʹ��ȫ��ɨ���������ķ�ʽ

�������֣�
[###Open table cache and binlog cache info:]
Table_open_cache_hits:4938    Table_open_cache_misses:0
Binlog_cache_use:820          Binlog_cache_disk_use:0

Table_open_cache_hits:The number of hits for open tables cache lookups.(�ۼ�ֵ������ ��λ����)
Table_open_cache_misses:The number of misses for open tables cache lookups. (�ۼ�ֵ������ ��λ����)
Binlog_cache_use: The number of transactions that used the binary log cache. (�ۼ�ֵ������ ��λ������)
Binlog_cache_disk_use:The number of transactions that used the temporary binary log cache but that exceeded the value 
                      of binlog_cache_size and used a temporary file to store statements from the transaction.(�ۼ�ֵ������ ��λ������)
�����ⲿ��˵����binlog_cache_size�� table_open_cache�Ƿ����

���߲��֣�
������ȫ����(�ۼ�ֵ������ ��λ����)
[###Hanler_xxx and Com_xxx info:]
Handler_delete:34574          Handler_update:675            Handler_write:1364
Handler_commit:5833           Handler_rollback:10
Com_commit:156                Com_rollback:5
U(counts):631     D(counts):159     I(counts):47      S(counts):4955    

U(counts):631     D(counts):159     I(counts):47      S(counts):4955  
��һ����Com_update\Com_delete\Com_insert\Com_select�ļ�д

Handler_xxx:��һ��ֵ˵��������Ӧ��������Ĵ���
Com_xxx:��һ��ֵ˵������Ӧ������Ӧ��䷢���Ĵ���

����Handler_commit �� Com_commit�ⲻͬ������������ύ���Զ��ύ���÷���commit������
commit�Ǵ����˵ģ���ôHandler_commit���Ӷ�Com_rollback���䣬�ֱ����ֶ�ִ��һ��commit
Com_commit����Handler_commit���䣬��Ϊ��Ȼ������commit�������Ҫ����ʵ�����£�

mysql> show status like '%commit%';
+----------------+-------+
| Variable_name  | Value |
+----------------+-------+
| Com_commit     | 1     |
| Com_xa_commit  | 0     |
| Handler_commit | 4     |
+----------------+-------+
3 rows in set (0.01 sec)

mysql> commit;
Query OK, 0 rows affected (0.00 sec)

mysql> show status like '%commit%';
+----------------+-------+
| Variable_name  | Value |
+----------------+-------+
| Com_commit     | 2     |
| Com_xa_commit  | 0     |
| Handler_commit | 4     |
+----------------+-------+
3 rows in set (0.00 sec)

�ڰ˲��֣�
[###Innodb_lock info:]
Innodb_row_lock_time:0                Innodb_row_lock_waits:0
Innodb_row_lock_current_waits:38

Innodb_row_lock_time:The total time spent in acquiring row locks for InnoDBtables, in milliseconds.(�ۼ�ֵ������ ��λ����)
Innodb_row_lock_waits:The number of times operations on InnoDB tables had to wait for a row lock.(�ۼ�ֵ������ ��λ����)
Innodb_row_lock_current_waits:The number of row lockscurrently being waited for by operations on InnoDB tables.(��ǰֵ��� ��λ����)

������һ�����Ƕ�innodb ����״̬�ļ��

�ھŲ��֣�

[###Innodb_buffer_pool info:]
Innodb_buf_pool_pages_total:1965840   Innodb_buf_pool_pages_dirty:19154
Innodb_buf_pages_data:1940433         Innodb_buf_pages_free:8116
Innodb_buffer_pool_wait_free:0        Innodb_buffer_pool_pages_misc:17291
Innodb_dblwr_pages_written:1188       Innodb_dblwr_writes:80
Innodb_buffer_pool_pages_flushed:2249
read_ahead:0          ahead_evict:0          ahead_rnd:0       

Innodb_buf_pool_pages_total:��Innodb_buffer_pool_pages_data The total size of the InnoDB buffer pool, in pages.(��ǰֵ��� ��λҳ)
Innodb_buf_pool_pages_dirty:��Innodb_buffer_pool_pages_dirty The current number of dirty pagesin the InnoDB buffer pool.(��ǰֵ��� ��λҳ)
Innodb_buf_pages_data:��Innodb_buffer_pool_pages_data The number of pagesin the InnoDB buffer poolcontaining data. The number includes both dirtyand
                      clean pages.(��ǰֵ��� ��λҳ)
Innodb_buf_pages_free:��Innodb_buffer_pool_pages_free The number of free pagesin the InnoDB buffer pool.(��ǰֵ��� ��λҳ)
Innodb_buffer_pool_wait_free:Normally, writes to the InnoDB buffer poolhappen in the background. When InnoDBneeds to read or
                             create a pageand no clean pages are available, InnoDBflushes some dirty pagesfirst and waits for that
                             operation to finish. This counter counts instances of these waits.(�ۼ�ֵ������ ��λ����)

Innodb_buffer_pool_pages_misc:The number of pages in the InnoDB buffer pool that are busy because they have
                              been allocated for administrative overhead, such as row locks or the adaptive hash
                              index. This value can also be calculated as Innodb_buffer_pool_pages_total ?
                              Innodb_buffer_pool_pages_free ? Innodb_buffer_pool_pages_data.(��ǰֵ��� ��λҳ)

Innodb_dblwr_pages_written:The number of pages that have been written to the doublewrite buffer.(�ۼ�ֵ������ ��λҳ)
Innodb_dblwr_writes:The number of doublewrite operations that have been performed. (�ۼ�ֵ������ ��λ����)
Innodb_buffer_pool_pages_flushed:The number of requests to flush pages from the InnoDB buffer pool.(�ۼ�ֵ������ ��λ����)

�ⲿ����innodb_buffer_pool���ص��������ҳ,����ҳ,����ҳ����Ϣ,�Լ�ˢ���̵Ĵ���

��ʮ���֣�
[###Innodb_log info:]
Innodb_log_writes:796                 Innodb_log_waits:0
Innodb_os_log_pending_fsyncs:0        Innodb_os_log_pending_writes:0
Innodb_os_log_written:44073472


Innodb_log_writes:The number of physical writes to the InnoDB redo logfile.(�ۼ�ֵ������ ��λ����)
Innodb_log_waits:The number of times that the log buffer was too small and a wait was required for it to be flushed before
                continuing.(�ۼ�ֵ������ ��λ����)
Innodb_os_log_pending_fsyncs:The number of pending fsync() operations for the InnoDB redo logfiles.(�ۼ�ֵ������ ��λ����)
Innodb_os_log_pending_writes:The number of pending writes to the InnoDB redo logfiles.(�ۼ�ֵ������ ��λ����)
Innodb_os_log_written:The number of bytes written to the InnoDB redo logfiles.(�ۼ�ֵ������ ��λ�ֽ�)

�ⲿ��˵���� innodb redo д����Ϣ����д�˶������ݣ����ٴεȴ������Innodb_os_log_pending_writes��Innodb_os_log_pending_fsyncs
��Ϊ0��˵��������I/O�ȴ�����Innodb_log_waits��Ϊ0����innodb_log_buffer_size �Ƿ��С��
ע�⣺����innodb_flush_method������� redoʼ��ʹ��kernel buffer��

��ʮһ���֣�
[###Innodb_data read/writes info:]
Innodb_data_reads:684                 Innodb_data_read:11206656
Innodb_data_writes:2067               Innodb_data_written:63603200
Innodb_data_fsyncs:1017               Innodb_data_pending_fsyncs:0
Innodb_buffer_pool_reads(physics reads):423
Innodb_buffer_pool_read_requests(logic reads):2869823

Innodb_data_reads:The total number of data reads.(�ۼ�ֵ������ ��λ����)
Innodb_data_read:The amount of data read since the server was started(�ۼ�ֵ������ ��λ�ֽ���)
Innodb_data_writes:The total number of data writes.(�ۼ�ֵ������ ��λ����)
Innodb_data_written:The amount of data written so far, in bytes.(�ۼ�ֵ������ ��λ�ֽ���)
Innodb_data_fsyncs:The number of fsync()operations so far. The frequency of fsync()calls is influenced by the setting
                   of the innodb_flush_method configuration option.(�ۼ�ֵ������ ��λ����)
Innodb_data_pending_fsyncs:The current number of pending fsync()operations. The frequency of fsync()calls is influenced by
                           the setting of the innodb_flush_method configuration option.(�ۼ�ֵ������ ��λ����)
Innodb_buffer_pool_reads(physics reads):The number of logical reads that InnoDB could not satisfy from the buffer pool, and had to read 
                                        directly from disk.(�ۼ�ֵ������ ��λ����)
Innodb_buffer_pool_read_requests(logic reads):The number of logical read requests.(�ۼ�ֵ������ ��λ����)
read_ahead:��Innodb_buffer_pool_read_ahead:The number of pages read into the InnoDB buffer pool by the read-ahead background thread.(�ۼ�ֵ������ ��λҳ)
ahead_evict:��Innodb_buffer_pool_read_ahead_evicted:The number of pagesread into the InnoDB buffer poolby the read-aheadbackground thread that were
                                      subsequently evictedwithout having been accessed by queries.(�ۼ�ֵ������ ��λҳ)
ahead_rnd:��Innodb_buffer_pool_read_ahead_rnd:The number of ��random��read-aheads initiated by InnoDB. This happens when a query scans a large
                                  portion of a table but in random order.(�ۼ�ֵ������ ��λ����)

��һ���ֿ��Կ����߼���ȡ�������ʣ�Ԥ��Ч�ʵȣ�ע��Innodb_data_pending_fsyncs��Innodb_data_fsyncs���������O_DIRECT,��ôinnodb_buffer���ڴ�
���ݲ��������kernel buffer����ôfsyncs pending��������ٵöࡣ���������ֵ�ϴ�ע�����I/O

��ʮ�����֣�
������ȫ����(�ۼ�ֵ������ ��λ����)
U(rows):675     D(rows):34575   I(rows):194     S(rows):767094  

U:Innodb_rows_updated
D:Innodb_rows_deleted
I:Innodb_rows_inserted
S:Innodb_rows_read

�ⲿ�������ķ�Ӧ��DML��SELECT��������ע���Com_xxx��Handler_xxx������

��ʮ�����֣�
Bytes_received:911280         Bytes_sent:8653956

ûʲô��˵���Լ����ĵ�

�ߡ��ڴ�������
    ʹ�ã�
valgrind --tool=memcheck --leak-check=full  ./mymon5.7c -S/mysqldata/mysql5.7sla/mysqld3307.sock -t 1 -n 10
    
==5120== LEAK SUMMARY:
==5120==      definitely lost: 0 bytes in 0 blocks
==5120==      indirectly lost: 0 bytes in 0 blocks
==5120==      possibly lost: 232 bytes in 2 blocks
==5120==      still reachable: 8,176 bytes in 2 blocks
==5120==      suppressed: 0 bytes in 0 blocks
==5120== Reachable blocks (those to which a pointer was found) are not shown.
==5120== To see them, rerun with: --leak-check=full --show-reachable=yes
==5120== 
==5120== For counts of detected and suppressed errors, rerun with: -v
==5120== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 6 from 6)
���Կ���û���ڴ����









������ֵ��
1����fun.h��д����Ҫ��ص�ָ����ַ���
   �ж��Ƕ�̬�;�̬����
2����fun.c���ҵ�print_data���������Լ�
   ����Ҫ�޸Ľ��漴��