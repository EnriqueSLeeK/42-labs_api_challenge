#include "api_method.h"
#include "sql.h"

static void prepare_param(int n_param, void *aux_struct, char *param[4]);
static void param_free(int n_param, char **param);

PGconn  *get_db_conn(PGconn *connection, int flag)
{
    static PGconn   *conn;
    static int      status; 

    if (flag == CONN_RESET)
    {
        conn = 0;
        status = 0;
    }
    else if (flag == CONN_STORE)
    {
        conn = connection;
        status = 1;
    }
    else if (flag == CONN_GET && status == 1)
        return (conn);
    else
        return (NULL);
}

void    terminate_db_conn(void)
{
    PQfinish(get_db_conn(NULL, CONN_GET));
    get_db_conn(NULL, CONN_RESET);
}

PGresult *exec_sql(char const *action, ExecStatusType flag)
{
    PGresult    *res;
    PGconn      *conn;

    conn = get_db_conn(NULL, CONN_GET);
    res = PQexec(conn, action);
    if (PQresultStatus(res) != flag)
    {
        PQclear(res);
        return (NULL);
    }
    return (res);
}

PGresult *exec_param_sql(char *stm, ExecStatusType flag, void *aux_struct, int n_param)
{
    PGresult            *res;
    PGconn              *conn;
    const char          *param[n_param];

    conn = get_db_conn(NULL, CONN_GET);
    prepare_param(n_param, aux_struct, (char **)param);
    res = PQexecParams(conn, stm, n_param,
            NULL, param, NULL, NULL, 0);
    param_free(n_param, (char **)param);
    if (PQresultStatus(res) != flag ||
        (PQresultStatus(res) == flag &&
         flag == PGRES_TUPLES_OK &&
         PQntuples(res) == 0))
    {
        PQclear(res);
        return (NULL);
    }
    return (res);
}

static void param_free(int n_param, char **param)
{
    for (int i = 0; i < n_param; i++)
        free(param[i]);
}

static void prepare_param(int n_param, void *aux_struct,
        char *param[4])
{
    t_video                 *video;
    t_content_creator       *creator;
    struct mg_http_message  *hm;

    if (n_param == 3)
    {
        creator = (t_content_creator*)aux_struct;
        asprintf(&param[0], "%d", (int)creator -> creator_id);
        asprintf(&param[1], "%d", (int)creator -> followers);
        asprintf(&param[2], "%s", creator -> channel);
    }
    else if (n_param == 4)
    {
        video = (t_video*)aux_struct;
        asprintf(&param[0], "%d",  (int)video -> id);
        asprintf(&param[1], "%d",  (int)video -> creator_id);
        asprintf(&param[2], "%d",  (int)video -> likes);
        asprintf(&param[3], "%d",  (int)video -> dislikes);
    }
    else if (n_param == 1 || n_param == 2)
    {
        hm = (struct mg_http_message*) aux_struct;
        parse_uri_param(n_param, param, hm -> uri.ptr, hm -> uri.len);
    }
}
