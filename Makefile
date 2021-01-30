
NAME = ft_ping

SRC = ft_ping.c \

SRCS = $(addprefix srcs/,$(SRCS))

OBJECT = $(addprefix out/,$(SRC:.c=.o))

CFLAGS = -Wall -Wextra -Werror
CFLAGS += -g -fsanitize=address -fno-omit-frame-pointer

CC = clang
#CC = gcc

BLUE = "\\033[36m"
RED = "\\033[31m"
WHITE = "\\033[0m"
GREEN = "\\033[32m"
PURPLE = "\\033[35m"

LNECLR = "\\33[2K\\r"

all: $(NAME)

$(NAME): $(OBJECT)
	$(CC) $(CFLAGS) -I includes -o $(NAME) $(OBJECT)
	printf "$(LNECLR)$(GREEN)make ping done$(WHITE)\n"

out/%.o: srcs/%.c
	mkdir -p out
	printf "$(LNECLR)$(NAME): $<"
	$(CC) $(CFLAGS) -I includes -o $@ -c $<

clean:
	$(RM) -rf out
	printf "$(PURPLE)clean ping done$(WHITE)\n"

fclean:
	$(RM) -rf out $(NAME)
	printf "$(PURPLE)fclean ping done$(WHITE)\n"

re: fclean all

.PHONY: fclean clean re FORCE ft_ping ping
.SILENT: fclean clean re FORCE $(NAME) $(OBJECT)
