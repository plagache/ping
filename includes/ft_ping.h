#ifndef FT_PING_H
# define FT_PING_H

# include <stdint.h>
# include <stddef.h>

# define TRUE           0
# define FALSE          1
# define FAILURE        1
# define SUCCESS        0


/*
**  Definition des Structure de Ping
*/

# define PING_OPTIONS       "vh"
# define ON                 1
# define OFF                0


typedef struct          s_options_ping
{
    unsigned char       verbose: 1;
    unsigned char       help: 1;
}                       t_options_ping;

typedef struct          s_ft_ping
{
    int                 arguments_parser;

    char                *host;

    struct addrinfo     *result;
    t_options_ping      options;

}                       t_ft_ping;

extern t_ft_ping *g_ping;

// Parsing function

int ft_lexer(int ac, char **av);
int test_option();

int is_an_option(char *argument);
int parse_option(char *opt);
int activate_options(char c);

int is_an_host(char *argument);
int parse_host(char *host);


#endif
