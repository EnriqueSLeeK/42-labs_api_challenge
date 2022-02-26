#include "api_method.h"
#include "sql.h"

int delete_handler(struct mg_connection *c, struct mg_http_message *hm)
{
  PGresult  *res;

  if (mg_http_match_uri(hm , "/channel/*"))
  {
    res = exec_param_sql("DELETE FROM content_creator WHERE creator_id=$1",
        PGRES_COMMAND_OK, hm, 1);
    if (res != NULL)
    {
      mg_http_reply(c, 200, NULL, "");
      PQclear(res);
      return (200);
    }
    else
      mg_http_reply(c, 500, JSON, "{\"status\":\"There is no element\"}");
    return (500);
  }
  else if (mg_http_match_uri(hm, "/channel/*/*"))
  {
    res = exec_param_sql("DELETE FROM video WHERE creator_id=$1 AND id=$2",
          PGRES_COMMAND_OK, hm, 2);
    if (res != NULL)
    {
      mg_http_reply(c, 200, NULL, "");
      PQclear(res);
      return (200);
    }
    else
      mg_http_reply(c, 500, JSON, "{\"status\":\"There is no element\"}");
    return (500);
  }
  else if (mg_http_match_uri(hm, "/video/*"))
  {
    res =exec_param_sql("DELETE FROM video WHERE id=$1",
        PGRES_COMMAND_OK, hm, 1);
    if (res != NULL)
    {
      mg_http_reply(c, 200, NULL, "");
      PQclear(res);
      return (200);
    }
    else
      mg_http_reply(c, 500, JSON, "{\"status\":\"There is no element\"}");
    return (500);
  }
  else
    mg_http_reply(c, 500, JSON, INVALID_URI);
  return (500);
}
