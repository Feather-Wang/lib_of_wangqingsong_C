#ifndef	_MOD_MYSQL_H_
#define	_MOD_MYSQL_H_

#include <mysql/mysql.h>
#include "udeftype.h"

#define DB_PATH "/topconf/topvp"
#define FUNCTION_FAILED    -1
#define FUNCTION_SUCCESS    1
#define FUNCTION_NO         0

bool	get_db_property(char *ip, char *port, char *user, char *pwd);
bool	modmysql_isopen(void);
bool	modmysql_open(const char *dbname, int nConn);
void	modmysql_close(void);
void	modmysql_return_conn(MYSQL *mysql);
MYSQL*	modmysql_get_freeconn(void);
int     modmysql_real_query(char *mysql_cmd, MYSQL_RES **result, int *num, int is_insert);
#endif	//_MOD_MYSQL_H_

