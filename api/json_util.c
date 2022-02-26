#include "api_method.h"

int json_key_quant(const char *req_body, int req_body_len)
{
  int   koff, klen, voff, vlen, vtype, off;
  int   counter;

  counter = 0;
  for (off = 0; (off = mjson_next(req_body, req_body_len,
          off, &koff, &klen, &voff, &vlen, &vtype)) != 0;)
  {
    counter += 1;
  }
  return (counter);
}
