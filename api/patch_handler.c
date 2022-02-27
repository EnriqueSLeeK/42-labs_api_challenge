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

static int  patch_param_sql(char *stm, int n_param, void *data, void *uri, char *str, struct mg_connection *c)
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
    mg_http_reply(c, 500, JSON, "{\"status\":\"Patch error\"}");
  return (500);
}

static int one_param(char *stm, int n_param, void *uri, char *param, struct mg_connection *c)
{
    return (patch_param_sql(stm, n_param, NULL, uri, param, c));
}

static int two_param(char *stm, int n_param, void *uri, char *param_p, char *param_t, struct mg_connection *c)
{
    return (patch_param_sql(stm, n_param, param_p, uri, param_t, c));
}

int patch_handler(struct mg_connection *c, struct mg_http_message *hm)
{
  char              *buff_p;
  char              *buff_s;
  t_content_creator creator;
  t_video           video;
  int               quant;
  int               flag;

  if (mg_http_match_uri(hm , "/channel/*"))
  {
    quant = json_key_quant(hm -> body.ptr, hm -> body.len);
    if (row_exist("SELECT * FROM content_creator WHERE creator_id=$1", hm, 1) &&
         quant < 3)
    {
      if (quant == 2)
        return (put_handler(c, hm));
      if (mjson_get_number(hm -> body.ptr, hm -> body.len, "$.creator_id", &creator.creator_id))
      {
        asprintf(&buff_s, "%d", creator.creator_id);
        return (one_param("UPDATE content_creator SET creator_id=$1 WHERE creator_id=$2",
            2, hm, buff_s, c));
      }
      else if (mjson_get_string(hm -> body.ptr, hm -> body.len, "$.channel", creator.channel, sizeof(creator.channel)) != -1)
      {
        asprintf(&buff_s, "%s", creator.channel);
        return (one_param("UPDATE content_creator SET channel=$1 WHERE creator_id=$2",
            2, hm, buff_s, c));
      }
      mg_http_reply(c, 400, JSON, "{\"status\":\"Not formated well\"}");
    }
    else
      mg_http_reply(c, 404, JSON, "{\"status\":\"row doesn't exists\"}");
    return (404);
  }
  else if (mg_http_match_uri(hm, "/channel/*/*"))
  {
    flag = 0;
    quant = json_key_quant(hm -> body.ptr, hm -> body.len);
    if (row_exist("SELECT * FROM video WHERE id=$2 AND creator_id=$1", hm, 2) &&
        quant < 4)
    {
      if (quant == 3)
        return (put_handler(c, hm));

      if (mjson_get_number(hm -> body.ptr, hm -> body.len, "$.id", &video.id))
        flag |= 1;
      if (mjson_get_number(hm -> body.ptr, hm -> body.len, "$.creator_id", &video.creator_id))
        flag |= 2;
      if (mjson_get_string(hm -> body.ptr, hm -> body.len, "$.video_name", video.video_name, sizeof(video.video_name)) != -1)
        flag |= 4;

      if (quant == 2)
      {
        if (1 & flag && 2 & flag)
        {
          asprintf(&buff_s, "%d", video.id);
          asprintf(&buff_p, "%d", video.creator_id);
          return (two_param("UPDATE video SET id=$1, creator_id=$2 WHERE id=$3 AND creator_id=$4",
                4, hm, buff_s, buff_p, c));
        }
        else if (1 & flag && 4 & flag)
        {
          asprintf(&buff_s, "%d", video.id);
          asprintf(&buff_p, "%s", video.video_name);
          return (two_param("UPDATE video SET id=$1, name=$2 WHERE id=$3 AND creator_id=$4",
                4, hm, buff_s, buff_p, c));
        }
        else if (2 & flag && 4 & flag)
        {
          asprintf(&buff_s, "%d", video.creator_id);
          asprintf(&buff_p, "%s", video.video_name);
          return (two_param("UPDATE video SET creator_id=$1, name=$2 WHERE id=$3 AND creator_id=$4",
                4, hm, buff_s, buff_p, c));
        }
      }
      else if (quant == 1)
      {
        if (1 & flag)
        {
          asprintf(&buff_s, "%d", video.id);
          return (one_param("UPDATE video SET id=$1 WHERE id=$2 AND creator_id=$3",
              3, hm, buff_s, c));
        }
        else if (2 & flag)
        {
          asprintf(&buff_s, "%d", video.creator_id);
          return (one_param("UPDATE video SET creator_id=$1 WHERE id=$2 AND creator_id=$3",
              3, hm, buff_s, c));
        }
        else if (4 & flag)
        {
          asprintf(&buff_s, "%s", video.video_name);
          return (one_param("UPDATE video SET name=$1 WHERE id=$2 AND creator_id=$3",
              3, hm, buff_s, c));
        }
      }
      mg_http_reply(c, 400, JSON, "{\"status\":\"Not properly formatted\"}");
      return (400);
    }
    else
      mg_http_reply(c, 404, JSON, "{\"status\":\"row doesn't exists\"}");
    return (404);
  }
  else
    mg_http_reply(c, 500, JSON, INVALID_URI);
  return (500);
}
