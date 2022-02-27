#include "api_method.h"
#include "sql.h"

int get_handler(struct mg_connection *c, struct mg_http_message *hm)
{
  char *resp;

  if (mg_http_match_uri(hm, "/"))
  {
    resp = mjson_aprintf("{%Q:%Q,%Q:%Q}",
    "documentation",
    "https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK/blob/main/README.md",
    "description",
    "This is the root of the project welcome! Please read the documentation.");
    mg_http_reply(c, 200, JSON, resp);
    free(resp);
    return (200);
  }
  else if (mg_http_match_uri(hm, "/videos") ||
           mg_http_match_uri(hm, "/videos/"))
  {
    return (send_message(c, 200, JSON, 
        exec_sql("SELECT json_agg(video) FROM video",
          PGRES_TUPLES_OK), "%s"));
  }
  else if (mg_http_match_uri(hm, "/channel") ||
           mg_http_match_uri(hm, "/channel/"))
  {
    return (send_message(c, 200, JSON,
        exec_sql("SELECT json_agg(content_creator) FROM content_creator",
          PGRES_TUPLES_OK), "%s"));
  }
  else if (mg_http_match_uri(hm, "/videos/*"))
  {
    return (send_message(c, 200, JSON,
        exec_param_sql("SELECT json_agg(video) FROM video WHERE id=$1",
          PGRES_TUPLES_OK, hm, 1, 0), "%s"));
  }
  else if (mg_http_match_uri(hm, "/channel/*"))
  {
    return (send_message(c, 200, JSON,
        exec_param_sql("SELECT row_to_json(content_creator) FROM content_creator WHERE creator_id=$1",
          PGRES_TUPLES_OK, hm, 1, 0), "%s"));
  }
  else if (mg_http_match_uri(hm, "/channel/*/*"))
  {
    return (send_message(c, 200, JSON,
      exec_param_sql("SELECT row_to_json(video) FROM video WHERE creator_id=$1 AND id=$2",
          PGRES_TUPLES_OK, hm, 2, 0), "%s"));
  }
  else
    mg_http_reply(c, 500, JSON, INVALID_URI);
  return (500);
}
