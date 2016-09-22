#include "vp_mysql.h"

// static functions
static MYSQL	*modmysql_get_new_conn(const char *dbname);
static void		modmysql_close_all_conn();

static MYSQL* modmysql_get_new_conn(const char *dbname)
{
	char ip[32] = {0};
	char port[32] = {0};
	char user[32] = {0};
	char pwd[32] = {0};

	if ( NULL == dbname )
		return NULL;

	MYSQL *mysql = mysql_init(NULL);
	if (mysql == NULL)
		return NULL;
	int recon = 1;
	mysql_options(mysql, MYSQL_OPT_RECONNECT, (char*)&recon);
	mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8");

	if (mysql_real_connect(mysql, ip, user, pwd, dbname, atoi(port), NULL, 0) == NULL)
	{
		mysql_close(mysql);
		return NULL;
	}

	return mysql;
}

static void
modmysql_close_all_conn()
{
	int i = 0;
	while (i < NUM_CONN)
	{
		if (g_conns[i].mysql != NULL)
		{
			mysql_close(g_conns[i].mysql);
			g_conns[i].mysql = NULL;
			g_conns[i].is_busy = false;
		}
		++i;
	}
}

static void*
modmysql_keep_conn(void *arg)
{
	int i = 0;
	while (g_mod_isopen)
	{
		i = 0;
		pthread_mutex_lock(&g_mutex);
		while (i < NUM_CONN)
		{
			if (g_conns[i].is_busy == false)
			{
				if (mysql_ping(g_conns[i].mysql) != 0)
				{
					mysql_close(g_conns[i].mysql);
					g_conns[i].mysql = NULL;
				}
			}
			++i;
		}
		pthread_mutex_unlock(&g_mutex);
		sleep(interval_ping);
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////
//

bool
get_db_property(char *ip, char *port, char *user, char *pwd)
{
	FILE *pf = NULL;
	char *p = NULL;
	char *q = NULL;
	char path[128] = {0};
	char buf[1024] = {0};

	sprintf(path, "%s/jdbc.properties", DB_PATH);

	if ((pf = fopen(path, "r")) == NULL)
		return false;
	if (fread(buf, 1, sizeof(buf), pf) < 0)
		return false;

	if ((p = strstr(buf, "jdbc.password")) != NULL)
		sscanf(p + strlen("jdbc.password="), "%[^\r\n]", pwd);
	if ((p = strstr(buf, "jdbc.url")) != NULL) {
		if ((q = strstr(p, "//")) != NULL)
			sscanf(q + strlen("//"), "%[^:]", ip);
		if ((p = strstr(q, ":")) != NULL) {
			sscanf(p + strlen(":"), "%[^/]", port);
		}
	}
	if ((p = strstr(buf, "jdbc.username")) != NULL)
		sscanf(p + strlen("jdbc.username="), "%[^\r\n]", user);

	return true;
}

bool
modmysql_isopen()
{
	return g_mod_isopen;
}

bool
modmysql_open(const char *dbname, int nConn)
{
	int          tret;
	pthread_t    tid;
	MYSQL *mysql = NULL;
	int i = 0;

	if (g_mod_isopen)
		return false;

	if ((nConn < 1) || (nConn > NUM_CONN_MAX))
	{
		loginf_fmt("mysql mod open failed! arg \"nConn\" should between 1 and %d", NUM_CONN_MAX);
		return false;
	}
	NUM_CONN = nConn;
	if ((g_conns = (my_conn*)malloc(NUM_CONN * sizeof(my_conn))) == NULL)
		return false;
    memset(g_conns, 0, NUM_CONN * sizeof(my_conn));
	if (mysql_server_init(0, NULL, NULL) != 0)	// mysql library init failed.
	{
		free(g_conns);
		g_conns = NULL;
		return false;
	}

	pthread_mutex_init(&g_mutex, NULL);

	g_mod_isopen = true;
	i = 0;
	while (i < NUM_CONN)
	{
		if ((mysql = modmysql_get_new_conn(dbname)) == NULL)
		{
			syslog(LOG_WARNING, "连接数据库失败!\n");
			break;
		}

		g_conns[i].mysql = mysql;
		g_conns[i].is_busy = false;
		++i;
	}
    if(i == 0)
    {
		LOGI("open db(%s) failure", dbname);
		free(g_conns);
        exit(-1);
    }

	if (i != NUM_CONN)
	{
		modmysql_close();
		free(g_conns);
		g_conns = NULL;
		return false;
	}
	tret = pthread_create(&tid, NULL, modmysql_keep_conn, NULL);
	if (tret != 0)
	{
		modmysql_close();
		free(g_conns);
		g_conns = NULL;
		return false;
	}
	pthread_detach(tid);

	return true;
}

void
modmysql_close()
{
	if ( ! g_mod_isopen)
		return;

	pthread_mutex_lock(&g_mutex);
	modmysql_close_all_conn();
	pthread_mutex_unlock(&g_mutex);

	free(g_conns);
	g_conns = NULL;
	NUM_CONN = 0;
	mysql_server_end();
	pthread_mutex_destroy(&g_mutex);

	g_mod_isopen = false;
}

MYSQL*
modmysql_get_freeconn()
{
	int i = 0;
	MYSQL *mysql = NULL;

	pthread_mutex_lock(&g_mutex);
	while (i < NUM_CONN)
	{
		if (g_conns[i].is_busy == false)
		{
			mysql = g_conns[i].mysql;
			g_conns[i].is_busy = true;
			break;
		}
		++i;
	}
	pthread_mutex_unlock(&g_mutex);
	return mysql;
}

void
modmysql_return_conn(MYSQL *mysql)
{
	int i = 0;

	pthread_mutex_lock(&g_mutex);
	while (i < NUM_CONN)
	{
		if (g_conns[i].mysql == mysql)
		{
			g_conns[i].is_busy = false;
			break;
		}
		++i;
	}
	pthread_mutex_unlock(&g_mutex);

}

/*********************************************************************
 * Function -> modmysql_real_query
 * Describe -> 获取mysql句柄，执行传入的mysql语句
 * parameter-> 
 *      mysql_cmd   -> mysql语句
 *      result      ->
 *      MYSQL_RES类型的2级指针，用来存放执行结果的结果集的地址,当不需要命令执行结果时，可以传入NULL
 *      num         -> 用来存放结果的条数，当不需要的时候可以传入NULL
 *      is_insert   ->
 *      mysql语句是否为插入操作或者修改等不会有返回结果的数据库操作, 0 or 1
 * return   ->
 *      FUNCTION_FAILED     -> 操作失败
 *      FUNCTION_SUCCESS    -> 操作成功
 *      FUNCTION_NO         ->
 *      操作成功，但是没有数据返回，select等有返回结果的操作才会有这个判断
 * **********************************************************************/
int modmysql_real_query(char *mysql_cmd, MYSQL_RES **result, int *num, int is_insert)
{
    MYSQL       *mysqlconn = NULL;
    MYSQL_RES *mysqlresult = NULL;
    int num_row = 0;

    //获取mysql句柄资源
    if( !modmysql_isopen() )
    {
        loginf_out("modmysql_real_query : mysql is not opened!");
        return FUNCTION_FAILED;
    }
    if( NULL == (mysqlconn = modmysql_get_freeconn()) )
    {
        loginf_out("modmysql_real_query : mysql gets failed!");
        return FUNCTION_FAILED;
    }

    if ( (mysql_real_query(mysqlconn, mysql_cmd, strlen(mysql_cmd))) )
    {
        loginf_out("modmysql_real_query : mysql_real_query failed!");
        modmysql_return_conn(mysqlconn);
        return FUNCTION_FAILED;
    }

    if( is_insert )
    {
        modmysql_return_conn(mysqlconn);
        return FUNCTION_SUCCESS;
    }

    //获取查找后的信息
    mysqlresult = mysql_store_result(mysqlconn);
    if (mysqlresult)
    {
        num_row = mysql_num_rows(mysqlresult);
    }

    //判断查找后的信息是否为空，如果为空，则释放资源，返回FUNCTION_NO
    if( 0 >= num_row )
    {
        loginf_out("modmysql_real_query : This mysql command don't have data!");
        mysql_free_result(mysqlresult);
        modmysql_return_conn(mysqlconn);
        return FUNCTION_NO;
    }

    if( NULL != result )
    {
        *result = mysqlresult;
    }
    else
    {
        mysql_free_result(mysqlresult);
    }

    if( NULL != num )
    {
        *num = num_row;
    }

    modmysql_return_conn(mysqlconn);
    return FUNCTION_SUCCESS;
}

