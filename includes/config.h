#ifndef CONFIG_H
# define CONFIG_H

# define VERSION "0.2.0"

# ifdef _DEBUG
#  define PORT 4243
#  define ADDRESS "127.0.0.1"
# else
#  define PORT 4242
#  define ADDRESS "64.225.45.59"
# endif

#endif
