#include "api_method.h"
#include "sql.h"

static void prepare_p_ext(int *n_param, void *aux_struct, void *aux_struct_2, char *str, char **param);
static void prepare_param(int n_param, void *aux_struct, char **param, int is_struct);
static PGresult *check_ok(PGresult *res, ExecStatusType flag);
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
    return (check_ok(res, flag));
}

PGresult *exec_param_sql(char *stm, ExecStatusType flag, void *aux_struct, int n_param, int is_struct)
{
    PGresult            *res;
    PGconn              *conn;
    const char          *param[n_param];

    conn = get_db_conn(NULL, CONN_GET);
    prepare_param(n_param, aux_struct, (char **)param, is_struct);
    res = PQexecParams(conn, stm, n_param,
            NULL, param, NULL, NULL, 0);
    param_free(n_param, (char **)param);
    return (check_ok(res, flag));
}

PGresult *exec_param_ext(char *stm, ExecStatusType flag, void *aux_struct, void *aux_struct_2,
        char *str, int n_param)
{
    PGresult            *res;
    PGconn              *conn;
    const char          *param[n_param];

    conn = get_db_conn(NULL, CONN_GET);
    prepare_p_ext(&n_param, aux_struct, aux_struct_2, str, (char **)param);
    res = PQexecParams(conn, stm, n_param,
            NULL, param, NULL, NULL, 0);
    param_free(n_param, (char **)param);
    return (check_ok(res, flag));
}

static void param_free(int n_param, char **param)
{
    for (int i = 0; i < n_param; i++)
        free(param[i]);
}

static PGresult *check_ok(PGresult *res, ExecStatusType flag)
{
    if (PQresultStatus(res) != flag ||
        (PQresultStatus(res) == flag &&
         flag == PGRES_TUPLES_OK &&
         (PQntuples(res) == 0 ||
          strlen(PQgetvalue(res, 0, 0)) == 0)
        )
       )
    {
        PQclear(res);
        return (NULL);
    }
    return (res);
}

// -3 is for patch request and 3 for put request
static void prepare_p_ext(int *n_param, void *aux_struct, void *aux_struct_2,
        char *str, char **param)
{
    if (*n_param == 5)
    {
        prepare_param(3, aux_struct, param, 1);
        prepare_param(2, aux_struct_2, &param[3], 0);
    }
    else if (*n_param == 4)
    {
        param[0] = (char *)aux_struct;
        param[1] = str;
        prepare_param(2, aux_struct_2, &param[2], 0);
    }
    else if (*n_param == 3 && str == NULL)
    {
        prepare_param(2, aux_struct, param, 1);
        prepare_param(1, aux_struct_2, &param[2], 0);
    }
    else if (*n_param == 3 && str != NULL)
    {
        param[0] = str;
        prepare_param(2, aux_struct_2, &param[1], 0);
    }
    else if (*n_param == 2)
    {
        param[0] = str;
        prepare_param(1, aux_struct_2, &param[1], 0);
    }
}

static void prepare_param(int n_param, void *aux_struct,
        char **param, int is_struct)
{
    t_video                 *video;
    t_content_creator       *creator;
    struct mg_http_message  *hm;

    if (n_param == 2 && is_struct == 1)
    {
        creator = (t_content_creator*)aux_struct;
        asprintf(&param[0], "%d", (int)creator -> creator_id);
        asprintf(&param[1], "%s", creator -> channel);
    }
    else if (n_param == 3 && is_struct == 1)
    {
        video = (t_video*)aux_struct;
        asprintf(&param[0], "%d",  (int)video -> id);
        asprintf(&param[1], "%d",  (int)video -> creator_id);
        asprintf(&param[2], "%s",  video -> video_name);
    }
    else if ((n_param == 1 || n_param == 2) && is_struct == 0)
    {
        hm = (struct mg_http_message*) aux_struct;
        parse_uri_param(n_param, param, hm -> uri.ptr, hm -> uri.len);
    }
}
