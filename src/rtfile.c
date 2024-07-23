#include "../headers/minirt.h"

/* 
we need to see how actually this .rt file looks
and can it have also some random lines in it, or everything 
needs to be properly written.  
also can we have empty lines or not and so on.
we should use getnextline to read each line and based on line 
parse it.
we need to check if we need another split for first splitting it
based on white spaces, or it can be splited only based on space.
*/

t_amb    parse_amb(char **line)
{
    t_amb   a;

    numofel(line, 3);
    return (a);
}

t_camera    parse_camera(char **line)
{
    t_camera       c;

    numofel(line, 4);
    return (c);
}

t_light    parse_light(char **line)
{
    t_light l;

    numofel(line, 4);
    return (l);
}

t_sphere    parse_sphere(char **line)
{
    t_sphere    sp;

    numofel(line, 4);
    return (sp);
}

t_plane    parse_plane(char **line)
{
    t_plane pl;

    numofel(line, 4);
    return (pl);
}

t_cylinder    parse_cylinder(char **line)
{
    t_cylinder  cy;

    numofel(line, 6);
    return (cy);
}

t_rt    parse_rt(char *file)
{
    t_rt    rt;
    int     fd;
    char    **line;
    char    *str;
    

    fd = open_file(file);

    while (fd)
    {
        str = get_next_line(fd);

        //check if it only line with white spaces, it needs to be skipped
        line = ft_splitset(str, "  \n\t\r\v\f");
        if (ft_strcmp(line[0], "A") == 0)
            parse_amb(line);
        else if (ft_strcmp(line[0], "C") == 0)
            parse_amb(line);
        else if (ft_strcmp(line[0], "L") == 0)
            parse_amb(line);
        else if (ft_strcmp(line[0], "sp") == 0)
            parse_amb(line);
        else if (ft_strcmp(line[0], "pl") == 0)
            parse_amb(line);
        else if (ft_strcmp(line[0], "cy") == 0)
            parse_amb(line);
        else
        {
            error_msg("incorrect value in .rt file!\n", "Correct starting with: A/C/L/sp/pl/cy");
            exit(EXIT_FAILURE);
        }

        free(str);
        // free line
    }
    return (rt);
}
