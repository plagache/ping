#ifndef FT_PING_H
# define FT_PING_H

# include <stdint.h>
# include <stddef.h>

# define FALSE 0
# define TRUE 1
# define SUCCESS        0
# define FAILURE        1
# define PING_OPTIONS   "vh"


/*
**  Structure des options de ping
*/


const char ping_options[3] = "vh";

typedef struct		s_options_ping
{
        unsigned int option_v: 1;
        unsigned int option_h: 1;
}					t_options_ping;


#endif
