#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sqlite3.h>

#define DATABASE "my.db"

int callback_func(void *arg, int f_num, char **f_value, char **f_name)
{
    char *query_result = NULL;
    int i = 1;

    query_result = (char *)arg;
    
    for( i = 0; i < f_num; ++i )
    {
        fprintf(stdout, "[i=%d] f_name=[%s], f_value=[%s]\n", i, f_name[i], f_value[i]);
    }

    sprintf(query_result, "callback_func end");

    return 0;
}
int main(int argc, const char *argv[])
{
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    char sqlstr[1024] = {0};
    char query_result[1024] = {0};

    /*打开数据库*/
    if( SQLITE_OK != sqlite3_open(DATABASE, &db) )
    {
        fprintf(stderr, "sqlite3(%s) open failed, errmsg=[%s]\n", DATABASE, sqlite3_errmsg(db));
        return -1;
    }

    while( 1 )
    {
        /*插入数据, 如果数据库在忙，则等待1秒重新插入，如果插入失败，则报错退出*/
        memset(sqlstr, 0x00, sizeof(sqlstr));
        snprintf(sqlstr, sizeof(sqlstr), "insert into user values('wangqs', 10)");
        int ret = sqlite3_exec(db, sqlstr, NULL, NULL, &errmsg);
        if( SQLITE_BUSY == ret )
        {
            sleep(1);
            continue;
        }
        else if( SQLITE_OK != ret )
        {
            fprintf(stderr, "sqlite3 [%s] run error, errmsg=[%s]\n", sqlstr, errmsg);
            sqlite3_free(errmsg);
            return -1;
        }
        else
        {
            break;
        }
    }
    
    /*第一种查询方式：查询数据，并打印查询结果*/
    fprintf(stderr, "########################################## first #######################################\n");
    memset(sqlstr, 0x00, sizeof(sqlstr));
    snprintf(sqlstr, sizeof(sqlstr), "select * from user");
    ret = sqlite3_exec(db, sqlstr, callback_func, (void*)query_result, &errmsg);
    if( SQLITE_OK != ret )
    {
        fprintf(stderr, "sqlite3 [%s] run error, errmsg=[%s]\n", errmsg);
        sqlite3_free(errmsg);
    }

    fprintf(stdout, "query_result=[%s]\n", query_result);

    /*第二种查询方式：查询数据，并打印查询结果*/
    fprintf(stderr, "########################################## second #######################################\n");
    char **result = NULL;
    int nrow = 0, ncolumn = 0;
    ret = sqlite3_get_table(db, sqlstr, &result, &nrow, &ncolumn, &errmsg);
    if( SQLITE_OK != ret )
    {
        fprintf(stderr, "sqlite3 [%s] run error, errmsg=[%s]\n", errmsg);
        sqlite3_free(errmsg);
    }

    if( 0 == nrow )
    {
        fprintf(stderr, "there is nothing in [%s]\n", sqlstr);
    }
    else 
    {
        fprintf(stderr, "nrow=[%d], ncolumn=[%d]\n", nrow, ncolumn);
        int i = 0, j = 0;
        for( i = 0; i <= nrow; ++i )
        {
            for( j = 0; j < ncolumn; ++j )
            {
                fprintf(stderr, "[%d][%s] \n", j, result[i*2+j]);
            }
            fprintf(stderr, "\n");
        }
    }

    sqlite3_free_table(result);

    /*关闭数据库*/
    ret = sqlite3_close(db);
    fprintf(stderr, "sqlite3_close, ret=%d\n", ret);
    if( SQLITE_OK != ret )
    {
        fprintf(stderr, "sqlite3_close error\n");
        return -1;
    }

    return 0;
}
