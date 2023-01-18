NAME = ft_ping

SRC = 	ft_ping.c\
		ft_parsing.c\

SRCS = $(addprefix srcs/,$(SRCS))

OBJECT = $(addprefix out/,$(SRC:.c=.o))

LIBDIR = libft

LIBA = $(LIBDIR)/libft.a

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

static: fclean
	intercept-build-14 make

$(NAME): $(OBJECT)
	make -s -C $(LIBDIR)
	$(CC) $(CFLAGS) -I includes -I libft/includes -o $(NAME) $(OBJECT) $(LIBA)
	printf "$(LNECLR)$(GREEN)make ping done$(WHITE)\n"

out/%.o: srcs/%.c includes/ft_ping.h
	mkdir -p out
	printf "$(LNECLR)$(NAME): $<"
	$(CC) $(CFLAGS) -I includes -I libft/includes -o $@ -c $<

clean:
	$(RM) -rf out
	make -s -C $(LIBDIR) clean
	printf "$(PURPLE)clean ping done$(WHITE)\n"

fclean:
	$(RM) -rf out $(NAME)
	make -s -C $(LIBDIR) fclean
	printf "$(PURPLE)fclean ping done$(WHITE)\n"

re: fclean all

.PHONY: fclean clean re FORCE
.SILENT: fclean clean re FORCE $(NAME) $(OBJECT)
