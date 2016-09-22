#include <stdio.h>  
#include <string.h>  
#include <mysql.h>  
#include <stdlib.h>  

int main(void)  
{  
    MYSQL *mysqlconn = 0;  
    MYSQL_RES *mysqlresult = 0;  
    MYSQL_ROW mysqlrow;  
    int num_row = 0;
    char *server = "192.168.17.200";  
    char *user = "root";  
    char *password = "topwalk"; /*  */  
    char *database = "TOPMTP"; 

    mysqlconn = mysql_init(NULL);  

    /* mysqlconnect to database */  
    if (!mysql_real_connect(mysqlconn, server, user, password, database, 0, NULL, 0)) 
    {  
        fprintf(stderr, "%s\n", mysql_error(mysqlconn));  
        exit(1);  
    }  

    /* send SQL query */  
    if (mysql_real_query(mysqlconn, "show tables", strlen("show tables"))) 
    {  
        fprintf(stderr, "%s\n", mysql_error(mysqlconn));  
        exit(1);  
    }  

    mysqlresult = mysql_store_result(mysqlconn);  
    if( mysqlresult )
    {
        num_row = mysql_num_row(mysqlresult);
    }

    /* output table name */  
    printf("MySQL Tables in mysql database:\n");  
    while ((mysqlrow = mysql_fetch_row(mysqlresult)) != NULL)  
        printf("%s \n", mysqlrow[0]);  

    /* close mysqlconnection */  
    mysql_free_result(mysqlresult);  
    mysql_close(mysqlconn);  

    return 1;
}  
