/*************************************************************************
  > File Name: fun.h
  > Author: gaopeng QQ:22389860 all right reserved
  > Mail: gaopp_200217@163.com
  > Created Time: Wed 08 Feb 2017 03:01:55 PM CST
 ************************************************************************/
/*
 * static data means not requires new_data-old_data type is 1 
 */
/*
 * static data 
 */

static char* stdata[] = 
{
	"Uptime",
	"Threads_connected",
	"Threads_running",
	"Qcache_free_memory",
	"Innodb_buffer_pool_pages_free",
	"Innodb_buffer_pool_pages_total",
	"Innodb_buffer_pool_pages_misc",
	"Innodb_buffer_pool_pages_dirty",
	"Max_used_connections",
	"Innodb_buffer_pool_bytes_data",
	"Innodb_row_lock_current_waits",
	"Innodb_buffer_pool_pages_data",
	NULL
};

/*
 * dyminc data
 */
static char* dydata[] =
{
	"Connections",    
	"Aborted_clients",//The number of connections that were aborted because the client died without closing the connection properly.<numbers>
	"Qcache_not_cached",//The number of noncached queries (not cacheable, or not cached due to the query_cache_typesetting).<counts>
	"Qcache_hits",   //The number of query cache hits. <counts>
	"Qcache_inserts",//The number of queries added to the query cache. <counts>
	/*索引中第一条记录被读的次数。如果较高，它表明服务器正执行大量全索引扫描；例如，SELECT * order by id，假定id列有索引。
	 * eg:
	 * +----+-------------+-------+------------+-------+---------------+---------+---------+------+------+----------+-------+
	 * | id | select_type | table | partitions | type  | possible_keys | key     | key_len | ref  | rows | filtered | Extra |
	 * +----+-------------+-------+------------+-------+---------------+---------+---------+------+------+----------+-------+
	 * |  1 | SIMPLE      | testj | NULL       | index | NULL          | PRIMARY | 276     | NULL |   19 |   100.00 | NULL  |
	 * +----+-------------+-------+------------+-------+---------------+---------+---------+------+------+----------+-------+
	 */
	"Handler_read_first",//*
	/*根据索引读一行的请求数。如果较高，说明查询和表的索引正确。本值可能是ref或者const也可能是range，只是代表索引KEY命中使用了索引
	 * eg:
	 * mysql> explain select * from testshared3 where id>1;
	 * +----+-------------+-------------+------------+-------+---------------+------+---------+------+------+----------+-----------------------+
	 * | id | select_type | table       | partitions | type  | possible_keys | key  | key_len | ref  | rows | filtered | Extra                 |
	 * +----+-------------+-------------+------------+-------+---------------+------+---------+------+------+----------+-----------------------+
	 * |  1 | SIMPLE      | testshared3 | NULL       | range | id            | id   | 5       | NULL |    1 |   100.00 | Using index condition |
	 * +----+-------------+-------------+------------+-------+---------------+------+---------+------+------+----------+-----------------------+
	 */
	"Handler_read_key",//*
	/*按照索引顺序读下一行的请求数,如果索引不唯一或者使用唯一索引的前缀或者范围都会触发这个值,简单的说只有唯一命中不触发这个值
	 * 这个值如果比较大说明需要连续访问下一个key的次数非常多
	 *
	 *eg:
	 *mysql> explain select * from testshared3 where id=1;
	 *+----+-------------+-------------+------------+------+---------------+------+---------+-------+--------+----------+-------+
	 *| id | select_type | table       | partitions | type | possible_keys | key  | key_len | ref   | rows   | filtered | Extra |
	 *+----+-------------+-------------+------------+------+---------------+------+---------+-------+--------+----------+-------+
	 *|  1 | SIMPLE      | testshared3 | NULL       | ref  | id            | id   | 5       | const | 523260 |   100.00 | NULL  |
	 *+----+-------------+-------------+------------+------+---------------+------+---------+-------+--------+----------+-------+
	 *| Handler_read_key      | 49      |
	 *| Handler_read_next     | 897698  |
	 */
	"Handler_read_next",//*
	/*
	 * 查询读索引最后一个索引键的请求数。当使用ORDER BY时，服务器优先发出使用第一个索引的请求，之后顺序往后扫描索引。当使用ORDER BY DESC时，服务器优先发出使用最后一个索引的请求， 之后向前扫描索引。
	 * 例如：select … order by id desc limit 10;
	 */
	"Handler_read_last",
	"Handler_read_rnd", //*full scan <counts>
	"Handler_read_rnd_next",//*full scan <counts>
	"Select_full_join",//The number of joins that perform table scans because they do not use indexes. If this value is not 0, youshould carefully check the indexes of your tables,
	"Select_scan",  //The number of joins that did a full scan of the first table.<counts>
	"Bytes_received",//The number of bytes received from all clients.<bytes>
	"Bytes_sent", //The number of bytes sent to all clients.<bytes>
	//Hanler_xxx is real oper to handle
	"Handler_commit", //The number of internal COMMIT statements.<counts>
	"Handler_rollback",//The number of requests for a storage engine to perform a rollback operation.<counts>
	"Handler_delete", //The number of times that rows have been deleted from tables.<counts>
	"Handler_update",//The number of requests to update a row in a table.<counts>
	"Handler_write",//The number of requests to insert a row in a table.<counts>
	// Com_xxx statement counter variables indicate the number of times each xxx statement has been executed <counts>
	// this Com only statement counter
	"Com_rollback",
	"Com_commit",
	"Com_update",
	"Com_delete",
	"Com_insert",
	"Com_select",
	"Com_lock_tables",  //lock table.<counts>
	"Com_unlock_tables", //lock table unlock.<counts>
	"Created_tmp_tables",     //The number of internal temporary tables created by the server while executing statements <tables>
	"Created_tmp_disk_tables",//The number of internal on-disk temporary tables created by the server while executing statements. <tables>
	"Sort_rows",//The number of sorted rows.<rows>
	"Sort_merge_passes",//The number of merge passes that the sort algorithm has had to do. <counts>
	"Table_open_cache_hits", //The number of hits for open tables cache lookups. <counts>
	"Table_open_cache_misses",//The number of misses for open tables cache lookups. <counts>
	"Binlog_cache_disk_use",//The number of transactions that used the temporary binary log cache but that exceeded the value ofbinlog_cache_size and used a temporary file to store statements from the transaction.<transactions>
	"Binlog_cache_use",//The number of transactions that used the binary log cache. <transactions>
	/*innodb values*/
	"Innodb_row_lock_time", //The total time spent in acquiring row locksfor InnoDBtables, in milliseconds <milliseconds>
	"Innodb_row_lock_waits",//The number of times operations on InnoDBtables had to wait for a row lock <counts>
	"Innodb_log_writes",//The number of physical writes to the InnoDB redo logfile <counts>
	"Innodb_log_waits",//The number of times that the log buffer was too small and a wait was required for it to be flushed before continuing.<counts>
	"Innodb_buffer_pool_wait_free",    //The number of free pages in the InnoDB buffer pool <pages> 
	"Innodb_buffer_pool_pages_flushed",//The number of requests to flush pages from the InnoDB buffer pool <pages>
	"Innodb_dblwr_pages_written",//The number of pages that have been written to the doublewrite buffer.<pages>
	"Innodb_dblwr_writes",//The number of doublewrite operations that have been performed.<counts>
	"Innodb_data_fsyncs",//add 17121 The number of fsync()operations so far. The frequency of fsync()calls is influenced by the setting of the innodb_flush_methodconfiguration option <counts>
	"Innodb_data_pending_fsyncs",//add 17121 The current number of pending fsync()operations. The frequency of fsync()calls is influenced by the setting of the innodb_flush_methodconfiguration option.
	"Innodb_os_log_pending_fsyncs", //The number of pending fsync() operations for the InnoDB redo logfiles.<counts>
	"Innodb_os_log_pending_writes", //The number of pending writes to the InnoDB redo logfiles.<counts>
	"Innodb_os_log_written", //The number of bytes written to the InnoDB redo logfiles.<bytes>
	"Innodb_data_reads", //The total number of data reads.<counts>
	"Innodb_data_read",  //The amount of data read since the server was started. <bytes>
	"Innodb_data_writes",//The total number of data writes.<counts>
	"Innodb_data_written",//The amount of data written so far, in bytes. <bytes>
	"Innodb_rows_deleted",//The number of rows deleted from InnoDBtables.<rows>
	"Innodb_rows_inserted",//The number of rows inserted into InnoDBtables.<rows>
	"Innodb_rows_read",//The number of rows read from InnoDBtables.<rows>
	"Innodb_rows_updated",//The number of rows updated in InnoDBtables.<rows>
	"Innodb_buffer_pool_reads",//The number of logical reads that InnoDB could not satisfy from the buffer pool, and had to read directly from disk.<counts>
	"Innodb_buffer_pool_read_requests",//The number of logical read requests.<counts>
	"Innodb_buffer_pool_read_ahead",//add 17123 The number of pages read into the InnoDB buffer pool by the read-ahead background thread.<pages>
	"Innodb_buffer_pool_read_ahead_evicted",//add 17123 The number of pages read into the InnoDB buffer pool by the read-ahead background thread.<pages>
	"Innodb_buffer_pool_read_ahead_rnd",//add  17123 The number of ��random��read-aheads initiated by InnoDB. This happens when a query scans a large portion of a table but in random order.<counts>
	NULL
};
