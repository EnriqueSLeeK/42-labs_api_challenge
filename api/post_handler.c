#include "api_method.h"
#include "sql.h"

static int  post_param_sql(char *stm, int n_param, void *data, struct mg_connection *c)
{
  PGresult *res;

  res = exec_param_sql(stm, PGRES_COMMAND_OK, data, n_param, 1);
  if (res != NULL)
  {
    PQclear(res);
    mg_http_reply(c, 201, NULL, "");
    return (201);
  }
  else
    mg_http_reply(c, 500, JSON, "{\"status\": \"Already exists or conflict\"}");
  return (500);
}

int post_handler(struct mg_connection *c, struct mg_http_message *hm)
{
  t_video           video;
  t_content_creator creator;

  if (mg_http_match_uri(hm, "/channel") ||
      mg_http_match_uri(hm, "/channel/"))
  {
    if (json_key_quant(hm -> body.ptr, hm -> body.len) == 2 &&
        mjson_get_number(hm -> body.ptr, hm -> body.len, "$.creator_id", &creator.creator_id) &&
        mjson_get_string(hm -> body.ptr, hm -> body.len, "$.channel", creator.channel, sizeof(creator.channel)) != -1)
    {
      return (post_param_sql("INSERT INTO content_creator VALUES($1, $2)", 2, &creator, c));
    }
    else
      mg_http_reply(c, 400, JSON, "{\"status\": \"Not properly formatted\"}");
    return (400);
  }
  else if (mg_http_match_uri(hm, "/videos") ||
          mg_http_match_uri(hm, "/videos/"))
  {
    if (json_key_quant(hm -> body.ptr, hm -> body.len) == 3 &&
        mjson_get_number(hm -> body.ptr, hm -> body.len, "$.id", &video.id) &&
        mjson_get_number(hm -> body.ptr, hm -> body.len, "$.creator_id", &video.creator_id) &&
        mjson_get_string(hm -> body.ptr, hm -> body.len, "$.video_name", video.video_name, sizeof(video.video_name)) != -1)
    {
      return (post_param_sql("INSERT INTO video VALUES($1, $2, $3)", 3, &video, c));
    }
    else
      mg_http_reply(c, 400, JSON, "{\"status\": \"Not properly formatted\"}");
    return (400);
  }
  else
    mg_http_reply(c, 500, JSON, INVALID_URI);
  return (500);
}
