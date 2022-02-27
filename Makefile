PROG := api_server
PROG_CLI := get_log

#CC := `curl-config --cc`
LIBS := -lpq
CC := gcc
FLAGS := -I./include

FILE_A := main_server.c handler.c get_handler.c post_handler.c\
		  put_handler.c delete_handler.c patch_handler.c craft_send.c\
		  json_util.c uri_util.c log_util.c
DIR_API_SRC := ./api/

FILE_SQL := ./sql/init_db.c ./sql/util_db.c

SRC_A := $(addprefix $(DIR_API_SRC), $(FILE_A))
SRC_MONGOOSE := ./mongoose/mongoose.c
SRC_MJSON := ./mjson/mjson.c

$(PROG): $(SRC_S) $(SRC_MONGOOSE)
	$(CC) $(FLAGS) -g -o $@ $(SRC_A) $(FILE_SQL) $(SRC_MONGOOSE) $(SRC_MJSON) $(LIBS)

%: %.c
	$(CC) $(FLAGS) $(LIBS)

clean:
	rm $(PROG)

