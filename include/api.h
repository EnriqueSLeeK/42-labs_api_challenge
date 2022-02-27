#ifndef API
# define API

# include "libs.h"
# include "struct.h"

void	handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data);
void	logging(struct mg_str *method, struct mg_str *endpoint, struct mg_http_header *headers, int stat_code);
void	create_log_dir(void);

#endif
