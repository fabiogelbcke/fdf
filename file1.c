#include "fdf.h"

int get_x(char *file)
{
  int fd;
  char a;
  int sizex;
  char b;

  a = 'a';
  b = 'a';
  sizex = 1;
  fd = open(file, O_RDONLY);
  while(read(fd, &a, 1) && a != '\n')
    {
      if (a == ' ' && b >='0' && b <= '9')
	sizex++;
      b = a;
    }
  if (b == ' ')
    sizex--;
  return sizex;
}

int get_y(char *file)
{
  int fd;
  char a;
  int sizey;
  char b;

  a = 'a';
  b = 'a';
  sizey = 1;
  fd = open(file, O_RDONLY);
  while(read(fd, &a, 1) == 1)
    {
      if (a == '\n' && b != '\n')
	sizey++;
      b = a;
    }
  if (b == '\n')
    sizey--;
  return sizey;
}

void getnline(int **lineptr, int fd)
{
  int *line;
  char *textline;
  char **splitline;
  int i;

  get_next_line(fd, &textline);
  splitline = ft_strsplit(textline, ' ');
  line = *lineptr;
  i = 0;
  while (splitline[i])
    {
      line[i] = ft_atoi(splitline[i]);
      i++;
    }
}

int **get_numbers_table(char *file)
{
  int **nbrtable;
  int i;
  int fd;

  nbrtable = (int**)malloc(sizeof(int*) * (get_y(file) + 1));
  i = 0;
  while(i < get_y(file))
    nbrtable[i++] = (int*)malloc(sizeof(int) * get_x(file));
  nbrtable[i] = 0;
  i = 0;
  fd = open(file, O_RDONLY);
  while (i < get_y(file))
    getnline(&(nbrtable[i++]), fd);
  return nbrtable;
}

void draw(int **table)
{
  void *mlx;

  mlx = mlx_init();
  mlx_new_window(mlx, 512, 512, "fdf");
}

int main(int argc, char **argv)
{
  int **table;
  int i;
  int j;

  i = 0;
  j = 0;
  table = get_numbers_table(argv[1]);
  while (i < get_y(argv[1]))
    {
      j = 0;
      while (j < get_x(argv[1]))
	{
	  printf("%d ", table[i][j]);
	  j++;
	}
      printf("\n");
      i++;
    }
  draw(table);
  sleep(5);
  return (0);
}