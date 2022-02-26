#include "api_method.h"
#include "sql.h"

int put_handler(struct mg_connection *c, struct mg_http_message *hm)
{
  if (mg_http_match_uri(hm , "/channel/*"))
  {
    mg_http_reply(c, 200, NULL, "");
  }
  else if (mg_http_match_uri(hm, "/channel/*/*"))
  {
    mg_http_reply(c, 200, NULL, "");
  }
  else
    mg_http_reply(c, 500, JSON, INVALID_URI);
}
