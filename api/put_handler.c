#include "api_method.h"
#include "sql.h"

static int  row_exist(char *stm, struct mg_http_message *hm, int n_param)
{
  PGresult  *res;

  res = exec_param_sql(stm, PGRES_TUPLES_OK, hm, n_param, 0);
  if (res != NULL)
  {
    PQclear(res);
    return (1);
  }
  return (0);
}

static int  put_param_sql(char *stm, int n_param, void *data, void *uri, char *str, struct mg_connection *c)
{
  PGresult  *res;

  res = exec_param_ext(stm, PGRES_COMMAND_OK, data, uri, str, n_param);
  if (res != NULL)
  {
    PQclear(res);
    mg_http_reply(c, 200, NULL, "");
    return (200);
  }
  else
    mg_http_reply(c, 500, JSON, "{\"status\":\"Put error\"}");
  return (500);
}

int put_handler(struct mg_connection *c, struct mg_http_message *hm)
{
  t_video           video;
  t_content_creator creator;

  if (mg_http_match_uri(hm , "/channel/*"))
  {
    if (row_exist("SELECT * FROM content_creator WHERE creator_id=$1", hm, 1))
    {
      if (json_key_quant(hm -> body.ptr, hm -> body.len) < 3 &&
          mjson_get_number(hm -> body.ptr, hm -> body.len, "$.creator_id", &creator.creator_id) && 
          mjson_get_string(hm -> body.ptr, hm -> body.len, "$.channel", creator.channel, sizeof(creator.channel)) != -1)
      {
        return (put_param_sql("UPDATE content_creator SET creator_id=$1, channel=$2 WHERE creator_id=$3",
              3, &creator, hm, NULL, c));
      }
      else
        mg_http_reply(c, 400, JSON, "{\"status\":\"Not properly formatted\"}");
    }
    else
      mg_http_reply(c, 400, JSON, "{\"status\":\"There is no such element or a constraint conflict occurred\"}");
    return (400);
  }
  else if (mg_http_match_uri(hm, "/channel/*/*"))
  {
    if (row_exist("SELECT * FROM video WHERE creator_id=$1 AND id=$2", hm, 2))
    {
      if (json_key_quant(hm -> body.ptr, hm -> body.len) < 4 &&
          mjson_get_number(hm -> body.ptr, hm -> body.len, "$.id", &video.id) &&
          mjson_get_number(hm -> body.ptr, hm -> body.len, "$.creator_id", &video.creator_id) &&
          mjson_get_string(hm -> body.ptr, hm -> body.len, "$.video_name", video.video_name, sizeof(video.video_name)) != -1)
      {
        return (put_param_sql("UPDATE video SET id=$1, creator_id=$2, name=$3 WHERE creator_id=$4 AND id=$5",
              5, &video, hm, NULL, c));
      }
      else
        mg_http_reply(c, 400, JSON, "{\"status\":\"Not properly formatted\"}");
    }
    else
      mg_http_reply(c, 400, JSON, "{\"status\":\"There is no such element or a constraint conflict occurred\"}");
    return (400);
  }
  else
    mg_http_reply(c, 500, JSON, INVALID_URI);
  return (500);
}
