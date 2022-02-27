#ifndef SQL_H
# define SQL_H

# include "libs.h"
# include "struct.h"

# define CONN_STORE 0
# define CONN_RESET 1
# define CONN_GET 2

void            terminate_db_conn(void);
PGconn          *get_db_conn(PGconn *connection, int flag);
PGresult        *exec_sql(char const *action, ExecStatusType flag);
PGresult		*exec_param_sql(char *stm, ExecStatusType flag, void *aux_struct, int n_param, int is_struct);

PGresult		*exec_param_ext(char *stm, ExecStatusType flag, void *aux_struct, void *aux_struct_2,
		char *str, int n_param);

// Initialize the connection with the database
void    init_db_conn(void);

#endif
