#ifndef FT_PING_H
# define FT_PING_H

# include <stdint.h>
# include <stddef.h>

# define FALSE 1
# define TRUE 0
# define SUCCESS        0
# define FAILURE        1



/*
**  Structure des options de ping
*/

# define PING_OPTIONS       "vh"
# define ON                 1
# define OFF                0


// const char ping_options[3] = "vh";

typedef struct		s_options_ping
{
        unsigned char verbose: 1;
        unsigned char help: 1;
}					t_options_ping;

// Parsing function

int ft_lexer(int ac, char **av);
int test_option();

int is_an_option(char *argument);
int parse_option(char *opt);
int activate_options(char c);

int is_an_host(char *argument);
int parse_host(char *host);


#endif
