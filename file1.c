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
  printf("fd = %d\n", fd);
  while(read(fd, &a, 1) && a != '\n')
    {
      if (a == ' ' && b >='0' && b <= '9')
	sizex++;
      b = a;
    }
  if (b == ' ')
    sizex--;
  close(fd);
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
  printf("fd = %d\n", fd);
  while(read(fd, &a, 1) == 1)
    {
      if (a == '\n' && b != '\n')
	sizey++;
      b = a;
    }
  if (b == '\n')
    sizey--;
  close(fd);
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
  if (!nbrtable)
    {
      printf("couldnt allocate memory");
      return NULL;
    }
  i = 0;
  while(i < get_y(file))
    {
      nbrtable[i++] = (int*)malloc(sizeof(int) * get_x(file));
      if (!nbrtable[i - 1])
	{
	  printf("couldnt allocate memory1");
	  return NULL;
	}
    }
  nbrtable[i] = 0;
  i = 0;
  fd = open(file, O_RDONLY);
  printf("fd = %d\n", fd);
  while (i < get_y(file))
    getnline(&(nbrtable[i++]), fd);
  close(fd);
  return nbrtable;
}

int ***init_coordtable(int y, int x)
{
  int i;
  int j;
  int ***coordtable;

  i = 0;
  coordtable = (int***)malloc(sizeof(int**) * (y + 1));
  if (!coordtable)
    {
      printf("couldnt allocate memory2");
      return NULL;
    }
  while (i < y)
    {
      j = 0;
      coordtable[i] = (int**)malloc(sizeof(int*) * (x + 1));
      if (!coordtable[i])
	{
	  printf("couldnt allocate memory3");
	  return NULL;
	}
      while (j < x)
	{
	  coordtable[i][j] = (int*)malloc(sizeof(int) * 2);
	  if (!coordtable[i][j])
	    {
	      printf("couldnt allocate memory4");
	      return NULL;
	    }
	  j++;
	}
      i++;
    }
  return coordtable;
}

int ***get_coordtable(int ** table, int y, int x)
{
  int ***coordtable;
  int i;
  int j;

  i = 0;
  coordtable = init_coordtable(y, x);
  while (i < y)
    {
      j = 0;
      while (j < x)
	{
	  coordtable[i][j][0] =  (550/sqrt(x*x + y*y))*(j - i + y - 0.5);
	  coordtable[i][j][1] = 50 + (600/sqrt(x*x+y*y))*(-0.11*table[i][j] + 0.5*j + 0.5*i);
	  j++; 
	}
      i++;
    }
  write(1, "wee\n", 4);
  return coordtable;
}

void draw_line(int coords[4], void *mlx, void *win)
{
  int i;
  float j;
  float ratio;

  ratio = ((float)coords[2] - (float)coords[3])/((float)coords[0] - (float)coords[1]);
  i = coords[1] - coords[0];
  j = (float)coords[3];
  while(i != 0)
    {
      mlx_pixel_put(mlx, win, i + coords[0], (int)j, 0xFF0000);
      i = (coords[1] > coords[0]) ? i - 1 : i + 1;
      j -= ratio;
    }
}

void draw_coordtable(void *mlx, int ***coordtable, int y, int x, void *win)
{
  int i;
  int j;
  int coordinates[4];

  i = 0;

  if (!mlx)
    write(1, "why?", 4);
  sleep(0);
  write(1, "mlx not null anymore hehe", 25);
  if (!win)
    write(1,"win null, wtf?", 13);
  write(1,"win not null", 12);
  while (i < y)
    {
      j = 0;
      while (j < x)
	{
	  //coordinates[0] = 25 + (650 / (coordtable[0][x - 1][0] - coordtable[y - 1][0][0]))*coordtable[i][j][0];
	  //coordinates[2] = 25 + (650 / (coordtable[y - 1][x - 1][1] - coordtable[0][0][1]))*coordtable[i][j][1];
	  coordinates[0] = coordtable[i][j][0];
	  coordinates[2] = coordtable[i][j][1];
	  if (j < x-1)
	    {
	      coordinates[1] = coordtable[i][j + 1][0];
	      coordinates[3] = coordtable[i][j + 1][1];

	      //	      coordinates[1] = 25 + (650 / (coordtable[0][x - 1][0] - coordtable[y - 1][0][0]))*coordtable[i][j+1][0];
	      //coordinates[3] = 25 + (650 / (coordtable[y - 1][x - 1][1] - coordtable[0][0][1]))*coordtable[i][j+1][1];
	      draw_line(coordinates, mlx, win);
	      //printf("j > 0 i = %d  j = %d\n", i, j); 
	    }
	  if (i > 0)
	    {
	      //  coordinates[1] = 25 + (650 / (coordtable[0][x - 1][0] - coordtable[y - 1][0][0]))*coordtable[i - 1][j][0];
              //coordinates[3] = 25 + (650 / (coordtable[y - 1][x - 1][1] - coordtable[0][0][1]))*coordtable[i - 1][j][1];
	      coordinates[1] = coordtable[i - 1][j][0];
	      coordinates[3] = coordtable[i - 1][j][1];
	      draw_line(coordinates, mlx, win);
	      //printf("i > 0 i = %d j = %d\n", i, j);
	    }
	  mlx_pixel_put(mlx, win, coordtable[i][j][0], coordtable[i][j][1], 0x00FFFF);
	  j++;
	}
      i++;
      }
}

void draw(void *mlx, int **table, int y, int x, void *win)
{
  int ***coordtable;

  coordtable = get_coordtable(table, y, x);
  draw_coordtable(mlx, coordtable, y, x, win);
}

int main(int argc, char **argv)
{
  int **table;
  int i;
  int j;
  void *mlx;
  void *win;

  i = 0;
  j = 0;
  mlx = mlx_init();
  win = mlx_new_window(mlx, 800, 700, "fdf");
  if (!mlx)
    write(1, "wtf", 3);
  printf("%d   %d\n", get_y(argv[1]), get_x(argv[1]));
  table = get_numbers_table(argv[1]);
  write(1, "getting here\n", 13);
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
  draw(mlx, table, i, j, win);
  sleep(15);
  return (0);
}
