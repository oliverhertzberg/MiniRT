#include "../headers/minirt.h"

static void check_args(int argc, char **argv)
{
    if (argc != 2)
    {
        error_msg("number of arguments is not valid!\n", "Usage: ./miniRT <scene_file.rt>\n");
        exit(EXIT_FAILURE);
    }
    if (ft_strchr(argv[1], '.') == NULL || ft_strcmp(ft_strchr(argv[1], '.'), ".rt") != 0)
    {
        error_msg("argument is not correct!\n", "Usage: ./miniRT <scene_file.rt>\n");
        exit(EXIT_FAILURE);
    }
}

// void print_vector(const char *name, t_vector v) {
//     printf("%s: (%f, %f, %f)\n", name, v.x, v.y, v.z);
// }

// void print_rt(t_rt *rt) {
//     int i;

//     // Print ambient light
//     printf("Ambient Light:\n");
//     printf("ID: %d\n", rt->a.id);
//     printf("Ratio: %f\n", rt->a.ratio);
//     print_vector("Color", rt->a.col);

//     // Print camera
//     printf("\nCamera:\n");
//     printf("ID: %d\n", rt->c.id);
//     print_vector("View Point", rt->c.point);
//     print_vector("Orientation Vector", rt->c.or_vec);
//     printf("FOV: %f\n", rt->c.fov);

//     // Print light
//     printf("\nLight:\n");
//     printf("ID: %d\n", rt->l.id);
//     print_vector("Point", rt->l.point);
//     printf("Brightness: %f\n", rt->l.bright);
//     print_vector("Color", rt->l.col);

//     // Print spheres
//     printf("\nSpheres: %d\n", rt->num_sp);
//     for (i = 0; i < rt->num_sp; i++) {
//         printf("\nSphere %d:\n", i);
//         printf("ID: %d\n", rt->sp[i].id);
//         print_vector("Center", rt->sp[i].center);
//         printf("Radius: %f\n", rt->sp[i].radius);
//         print_vector("Color", rt->sp[i].col);
//     }

//     // Print planes
//     printf("\nPlanes: %d\n", rt->num_pl);
//     for (i = 0; i < rt->num_pl; i++) {
//         printf("\nPlane %d:\n", i);
//         printf("ID: %d\n", rt->pl[i].id);
//         print_vector("Point", rt->pl[i].point);
//         print_vector("Normal", rt->pl[i].normal);
//         print_vector("Color", rt->pl[i].col);
//     }

//     // Print cylinders
//     printf("\nCylinders: %d\n", rt->num_cy);
//     for (i = 0; i < rt->num_cy; i++) {
//         printf("\nCylinder %d:\n", i);
//         printf("ID: %d\n", rt->cy[i].id);
//         print_vector("Center", rt->cy[i].center);
//         print_vector("Normal", rt->cy[i].normal);
//         printf("Radius: %f\n", rt->cy[i].radius);
//         printf("Height: %f\n", rt->cy[i].height);
//         print_vector("Color", rt->cy[i].col);
//     }
// }

int main(int argc, char **argv)
{
    t_rt    rt;
    int     fd;
    t_index j;

    check_args(argc, argv);
    init_rt(&rt, &j, argv);
    fd = open_file(argv[1]);
    if (parse_rt(&rt, fd, &j) == 0)
    {
        //free everything
        close(fd);
        return (1);
    }
    //print_rt(&rt);
    
    close(fd);
    // camera
    float focal_length = (WIDTH / 2) / (tanf((rt.c.fov * (PI/180))/2));
    // t_vector camera_center = {0, 0, 0};

    // viewport_u
    t_vector viewport_u = {VIEWPORT_W, 0, 0};
    // viewport_v, in our viewport y increases upward, in our graphical window
    // y increases downward, hence -VIEWPORT_H
    t_vector viewport_v = {0, -VIEWPORT_H, 0};

    // calculate the horizontal and vertical delta vectors from pixel to pixel.
    t_vector pixel_delta_u = vec_div(viewport_u, WIDTH);
    t_vector pixel_delta_v = vec_div(viewport_v, HEIGHT);
    // calculate location of upper left pixel.
    t_vector focal = {0, 0, focal_length};
    t_vector viewport_upper_left = vec_sub(rt.c.point, focal);
    viewport_upper_left = vec_sub(viewport_upper_left, vec_div(viewport_u, 2));
    viewport_upper_left = vec_sub(viewport_upper_left, vec_div(viewport_v, 2));
    t_vector pixel00_loc = vec_mult(vec_add(pixel_delta_u, pixel_delta_v), 0.5);
    pixel00_loc = vec_add(pixel00_loc, viewport_upper_left);

    t_vector color;
    int rgba;
    int x;
    int y;
    
    rt.mlx = mlx_init(WIDTH, HEIGHT, "SPHERES", false);
    rt.img = mlx_new_image(rt.mlx, WIDTH, HEIGHT);
    mlx_image_to_window(rt.mlx, rt.img, 0, 0);

    y = 0;
    while (y < HEIGHT)
    {
        x = 0;
        while (x < WIDTH)
        {
            t_vector pixel_center = vec_add(pixel00_loc, vec_mult(pixel_delta_u, x));
            pixel_center = vec_add(pixel_center, vec_mult(pixel_delta_v, y));
            t_vector ray_direction = vec_sub(pixel_center, rt.c.point);
            t_ray ray = init_ray(rt.c.point, ray_direction);
            color = ray_color(ray, rt);
            rgba = get_rgba(color.x, color.y, color.z, 255);
            set_px_col(rt.img, x, y, rgba);
            x++;
        }
        y++;
    }
    mlx_loop(rt.mlx);
    return (0);
}