#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft/libft.h"

typedef struct s_cmd
{
    char    **command;
    struct s_cmd *next;
    char   *path;
    
} t_cmd;


typedef struct s_data
{
    int     here_doc;
    char    *limiter;
    int     infile;
    int     outfile;
    t_cmd    *cmds;
    int     nb_cmds;
    char   *path_line;
    int     old_fd;
} t_data;


/* FUNCTIONS */
char **free_split(char **arr, int i);
void    init_vars(t_data **data, char **av, int ac);
void    set_path_line(t_data **data, char **env);
void    free_command(t_cmd **cmd);
void    clean_all(t_data **data);
char    *get_path(t_data **data, char *command);
void    print_commands(t_data *data);
void    fill_command(t_data **data, int ac, char **av);

#endif