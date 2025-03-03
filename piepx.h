#ifndef PIPEX_H
#define PIPEX_H

#include "libft/get_next_line.h"
#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct s_cmd
{
    char    **command;
    struct s_cmd *next;
    char   *path;
    
} t_cmd;


typedef struct s_data
{
    int     here_doc;
    int     here_doc_fd;
    char    *limiter;
    int     infile;
    int     outfile;
    t_cmd    *cmds;
    int     nb_cmds;
    char   *path_line;
    int     old_fd;
    int     *pid;
    int     pid_index;
} t_data;


/* FUNCTIONS */
void   free_2d(char **arr);
char    *get_path(t_data **data, char *command);
void    free_command(t_cmd **cmd);
void    set_path_line(t_data **data, char **env);
t_cmd	*ft_lstnew1(char **command, char *path, char *av);
void	ft_lstadd_back1(t_cmd **lst, t_cmd *new);
t_cmd	*ft_lstlast1(t_cmd *lst);
void   open_files(t_data **data, char **av, int ac);
void    handle_here_doc(t_data **data, int fd[2]);
void    check_command(t_data **data, t_cmd *tmp);
void    handle_last_cmd(t_data **data, char **env, int fd[2], t_cmd *tmp);
void    handle_first_cmd(t_data **data, char **env, int fd[2], t_cmd *tmp);
void    child(t_data **data, char **env, int fd[2], t_cmd *tmp);
char **free_split(char **arr, int i);
void    init_vars(t_data **data, char **av, int ac);
void    set_path_line(t_data **data, char **env);
void    free_command(t_cmd **cmd);
void    clean_all(t_data **data);
char    *get_path(t_data **data, char *command);
void    print_commands(t_data *data);
void    fill_command(t_data **data, int ac, char **av);

#endif