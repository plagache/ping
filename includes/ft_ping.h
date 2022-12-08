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

t_options_ping options;


#endif
