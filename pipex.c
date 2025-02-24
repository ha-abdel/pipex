#include "piepx.h"

void    fill_command(t_data **data, int ac, char **av)
{
    int i = 2;
    if((*data)->here_doc)
        i = 3;
    while (i < ac - 2)
    {
        
    }
    
}

int main(int ac, char **av)
{
    t_data *data;

    if (ac < 5)
        exit(1);
    data = malloc(sizeof(t_data));
    if (ft_strncmp(av[1], "here_doc", 8) == 0)
    {
        data->here_doc = 1;
        data->limiter = av[2];
    }
    fill_commands(&data, ac, av);


    int infile = open("infile", O_RDONLY);
    
}