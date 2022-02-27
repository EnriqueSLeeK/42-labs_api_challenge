#include "api.h"
#include "sql.h"

// When a SIGINT or SINTERM signal is received 
// stop the server gracefully
static int	signal_code;
void  init_signal(void *handler);
static void	signal_handler(int signal);

int main(void)
{
  struct mg_mgr mgr;

  create_log_dir();
  init_db_conn();
  mg_mgr_init(&mgr);
  init_signal(signal_handler);
  mg_http_listen(&mgr, "http://localhost:8000", handler, &mgr);
  while (signal_code == 0)
    mg_mgr_poll(&mgr, 1000);
  mg_mgr_free(&mgr);
  terminate_db_conn();
  return (0);
}

void  init_signal(void *handler)
{
  signal(SIGINT, handler);
  signal(SIGTERM, handler);
}

static void	signal_handler(int signal)
{
	signal_code = signal;
}
