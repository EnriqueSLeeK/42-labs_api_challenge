#ifndef API_METHOD
# define API_METHOD

# include "libs.h"

# define JSON "Content-Type: application/json\r\n"
# define HTML "Content-Type: text/html\r\n"

# define INVALID_URI "{\"status\": \"Invalid endpoint\"}"

int	get_handler(struct mg_connection *c, struct mg_http_message *hm);
int	post_handler(struct mg_connection *c, struct mg_http_message *hm);
int	put_handler(struct mg_connection *c, struct mg_http_message *hm);
int	patch_handler(struct mg_connection *c, struct mg_http_message *hm);
int	delete_handler(struct mg_connection *c, struct mg_http_message *hm);

void  parse_uri_param(int n_param, char **param_buff, const char *uri, int len);

int	send_message(struct mg_connection *c, int code, char *type, PGresult *res, char *format);

// Misc
int json_key_quant(const char *req_body, int req_body_len);

#endif
