#include "sql.h"

static void check_init_table(PGconn *conn);

void    exit_db(PGconn *conn, PGresult *res)
{
    PQclear(res);
    PQfinish(conn);
    exit(2);
}

void    init_db_conn(void)
{
    PGconn *conn;

    conn = PQconnectdb("user=labs dbname=labs-db");

    if (PQstatus(conn) == CONNECTION_BAD)
    {
        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    get_db_conn(conn, CONN_STORE);
    check_init_table(conn);
}

static int  check_table(PGconn *conn, PGresult *res, const char *stm)
{
    res = PQexec(conn, stm);

    if (PQresultStatus(res) != PGRES_TUPLES_OK
        || PQgetvalue(res, 0, 0)[0] == 0)
    {
        PQclear(res);
        return (0);
    }
    PQclear(res);
    return (1);
}

static void check_init_table(PGconn *conn)
{
    PGresult    *res;

    if (check_table(conn, res, "SELECT to_regclass('public.content_creator');") == 0)
    {
        res = PQexec(conn, "CREATE TABLE content_creator (creator_id int PRIMARY KEY, channel text)");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
            exit_db(conn, res);
        PQclear(res);
    }
    if (check_table(conn, res, "SELECT to_regclass('public.video')") == 0)
    {
        res = PQexec(conn, "CREATE TABLE video (id int, creator_id int REFERENCES content_creator(creator_id), name text, PRIMARY KEY(id, creator_id))");
        if (PQresultStatus(res) != PGRES_COMMAND_OK)
            exit_db(conn, res);
        PQclear(res);
    }
}
