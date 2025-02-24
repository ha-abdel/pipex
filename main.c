#include "libft/get_next_line.h"
#include "libft/libft.h"
#include <fcntl.h>
#include <stdio.h>

int main()
{
    int fd= open("infile.txt", O_RDWR);
    char *line = get_next_line(fd);
    while(line)
    {
        if(ft_strncmp(line, "is", ft_strlen(line)))
        {
            break;
        }
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
}