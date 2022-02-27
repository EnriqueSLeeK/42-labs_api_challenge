#include "api_method.h"
#include "sql.h"

static int  del_param_sql(char *stm, int n_param, struct mg_http_message *hm, struct mg_connection *c)
{
  PGresult  *res;

  res = exec_param_sql(stm, PGRES_COMMAND_OK, hm, n_param, 0);
  if (res != NULL)
  {
    mg_http_reply(c, 200, NULL, "");
    PQclear(res);
    return (200);
  }
  else
    mg_http_reply(c, 500, JSON, "{\"status\":\"There is no such element or a conflict occurred\"}");
  return (500);
}

static int  del_sql(const char *stm, struct mg_http_message *hm, struct mg_connection *c)
{
  PGresult  *res;

  res =exec_sql(stm, PGRES_COMMAND_OK);
  if (res != NULL)
  {
    mg_http_reply(c, 200, NULL, "");
    PQclear(res);
    return (200);
  }
  else
    mg_http_reply(c, 500, JSON, "{\"status\":\"There is no such element or a conflict occurred\"}");
    return (500);
}

int delete_handler(struct mg_connection *c, struct mg_http_message *hm)
{
  int       ret_code;

  if (mg_http_match_uri(hm , "/channel/*"))
  {
    ret_code = del_param_sql("DELETE FROM content_creator WHERE creator_id=$1",
        1, hm, c);
  }
  else if (mg_http_match_uri(hm, "/channel/*/*"))
  {
    ret_code = del_param_sql("DELETE FROM video WHERE creator_id=$1 AND id=$2",
        2, hm, c);
  }
  else if (mg_http_match_uri(hm, "/video/*"))
  {
    ret_code = del_param_sql("DELETE FROM video WHERE id=$1",
        1, hm, c);
  }
  else if (mg_http_match_uri(hm, "/channel/") ||
            mg_http_match_uri(hm, "/channel"))
  {
    ret_code = del_sql("DELETE FROM content_creator", hm, c);
  }
  else if (mg_http_match_uri(hm, "/videos/") ||
            mg_http_match_uri(hm, "/videos"))
  {
    ret_code = del_sql("DELETE FROM video", hm, c);
  }
  else
  {
    ret_code = 500;
    mg_http_reply(c, 500, JSON, INVALID_URI);
  }
  return (ret_code);
}
