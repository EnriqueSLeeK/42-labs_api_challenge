#include "api_method.h"
#include <sys/stat.h>
#include <unistd.h>

void  get_requester(struct mg_str *req, struct mg_http_header *headers);

void  logging(struct mg_str *method, struct mg_str *endpoint, struct mg_http_header *headers, int stat_code)
{
  FILE          *fd;
  struct mg_str req;
  time_t        curr_time;
  char          *buff;

  fd = fopen("Logs/log", "a");

  if (fd != NULL)
  {
    buff = 0;
    get_requester(&req, headers);
    time(&curr_time);
    asprintf(&buff, "%.*s,%.*s,%.*s,%d,%s",
        method -> len, method -> ptr,
        endpoint -> len, endpoint -> ptr,
        req.len, req.ptr,
        stat_code,
        ctime(&curr_time));
    fputs(buff, fd);
    fclose(fd);
    free(buff);
  }
}

void  get_requester(struct mg_str *req, struct mg_http_header *headers)
{
  for (int i = 0; headers[i].name.len != 0 && i < MG_MAX_HTTP_HEADERS; i++)
  {
    if (mg_vcmp(&headers[i].name, "User-Agent") == 0)
    {
      req -> ptr = headers[i].value.ptr;
      req -> len = headers[i].value.len;
      break ;
    }
  }
}

void  create_log_dir(void)
{
  struct stat st = {0};

  if (stat("./Logs", &st) == -1)
  {
    mkdir("./Logs", 0755);
  }
}
