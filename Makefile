PROG := api_server
PROG_CLI := tablefly

#CC := `curl-config --cc`
LIBS := -lpq
CC := gcc
FLAGS := -I./include

FILE_A := main_server.c handler.c get_handler.c post_handler.c\
		  put_handler.c delete_handler.c patch_handler.c craft_send.c\
		  json_util.c uri_util.c log_util.c
DIR_API_SRC := ./api/

FILE_SQL := ./sql/init_db.c ./sql/util_db.c

FILE_CLI := main.c cli_util.c init_cli.c
DIR_CLI := ./log_cli/

SRC_CLI := $(addprefix $(DIR_CLI), $(FILE_CLI))

SRC_A := $(addprefix $(DIR_API_SRC), $(FILE_A))

SRC_MONGOOSE := ./mongoose/mongoose.c
SRC_MJSON := ./mjson/mjson.c

api: $(PROG)

cli: $(PROG_CLI)

all: api cli

$(PROG): $(SRC_S) $(SRC_MONGOOSE)
	$(CC) $(FLAGS) -g -o $@ $(SRC_A) $(FILE_SQL) $(SRC_MONGOOSE) $(SRC_MJSON) $(LIBS)

$(PROG_CLI): $(SRC_CLI)
	$(CC) $(FLAGS) -g -o $@ $(SRC_CLI)

clean_api:
	rm $(PROG)

clean_cli:
	rm $(PROG_CLI)

clean_all:
	rm $(PROG) $(PROG_CLI)

.PHONY: clean all cli api
