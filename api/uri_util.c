#include "api_method.h"

void  parse_uri_param(int n_param, char **param_buff, const char *uri, int len)
{
  char  *buff;
  char  *token;

  asprintf(&buff, "%.*s", len, uri);
  token = strtok(buff, "/");
  for (int i = 0; i < n_param; i++)
  {
    token = strtok(NULL, "/");
    asprintf(&param_buff[i], "%s", token);
  }
  free(buff);
}
