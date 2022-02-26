#include "sql.h"

static void craft_message(char **message, char *body, char *format)
{
  *message = mjson_aprintf(format, body);
}

int send_message(struct mg_connection *c, int code, char *type, PGresult *res, char *format)
{
  char  *message;

  if (res == NULL)
  {
    mg_http_reply(c, 404, type,
        "{\"doc\": \"https://github.com/42sp/42labs-selection-process-v2-EnriqueSLeeK/blob/main/README.md\"}");
    return (404);
  }
  craft_message(&message, format, PQgetvalue(res, 0, 0));
  mg_http_reply(c, code, type, "%s", message);
  PQclear(res);
  free(message);
  return (200);
}
