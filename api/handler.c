#include "api.h"
#include "api_method.h"

void  handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
  const struct  mg_str* method;

  if (ev == MG_EV_HTTP_MSG)
  {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    method = (const struct mg_str*) &(hm -> method);
    if (mg_vcmp(method, "GET") == 0)
      logging(&(hm -> method), &(hm -> uri), hm -> headers, get_handler(c, hm));
    else if (mg_vcmp(method, "POST") == 0)
      logging(&(hm -> method), &(hm -> uri), hm -> headers, post_handler(c, hm));
    else if (mg_vcmp(method, "DELETE") == 0)
      logging(&(hm -> method), &(hm -> uri), hm -> headers, delete_handler(c, hm));
    else if (mg_vcmp(method, "PUT") == 0)
      logging(&(hm -> method), &(hm -> uri), hm -> headers, post_handler(c, hm));
    else if (mg_vcmp(method, "PATCH") == 0)
      logging(&(hm -> method), &(hm -> uri), hm -> headers, patch_handler(c, hm));
    else
      mg_http_reply(c, 500, NULL, "{\"status\":\"Unknown request\"");
  }
}
