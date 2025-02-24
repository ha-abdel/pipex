#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "libft.h"

// typedef struct s_list
// {
//     char    **cmds;
//     struct s_list *next;
    
// } t_list;


typedef struct s_data
{
    int     here_doc;
    char    *limiter;
    int     infile;
    int     outfile;
    char    **cmd;
    char    *path;
    int     nb_cmds;
} t_data;


#endif