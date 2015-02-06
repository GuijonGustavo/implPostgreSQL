#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libpq-fe.h>
/* Para compilar: http://www.labbookpages.co.uk/software/imgProc/libPNG.html */
/*  gcc -Wall -O2 -lm -lpng -lpq -o implPostgresql implPostgresql.c -I/usr/include/postgresql/ */
/* Color del pixel */

	/* Para instalar libpq-fe.h: sudo apt-get install libpq-dev */
//       Y para compilar:
//       
//       gcc -lpng -lpq -o implPostgresql implPostgresql.c -I/usr/include/postgresql/  */
//http://books.google.com.mx/books?id=gbjIzwE2NYkC&pg=PA177&lpg=PA177&dq=PQgetvalue+integer&source=bl&ots=HbeiW5vMUS&sig=UqLioZuMs7dhoPfsPUr55EvdLmM&hl=es-419&sa=X&ei=f_lsVOSmFMaiyATGqoIQ&redir_esc=y#v=onepage&q=PQgetvalue%20integer&f=false //Página 183
//SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM tabla WHERE ST_Intersects(ST_GeomFromText('POLYGON((xmin ymin,xmax ymin,xmax ymax,xmin ymax,xmin ymin))',srid),the_geom);
//
//If you're using Ubuntu, you'll probably need to install libpng-dev. You might also need to try a different compile command:
//
//> sudo apt-get install libpng-dev
//> gcc -o makePNG makePNG.c -lm -lpng 
//> ./makePNG output.png
//
//
//
//

typedef struct
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} pixel_t;

typedef struct
{
  pixel_t *pixels;
  size_t width;
  size_t height;
} bitmap_t;

struct mem_encode
{
  char *buffer;
  size_t size;

};


char * str_replace (char * string, char * substr, char * replacement);


void my_png_write_data (png_structp png_ptr, png_bytep data,
			png_size_t length);

void my_png_flush (png_structp png_ptr);

void usage (void);

static pixel_t *pixel_at (bitmap_t * bitmap, int x, int y);

static int save_png_to_file (bitmap_t * bitmap, int tr);



int
main (int argc, char *argv[])
{
  int r, 
	  g, 
	  b,
	  tra, 
	  radio, 
	  anch, 
	  larg,
	  i, 
	  j,k,k2; 

  float x_m, 
		y_m, 
		X_m, 
		Y_m, 
		
		c_x, 
		c_y,
		p;

  char *quer, 
    *rojo,
    *verde,
    *azul,
    *radius,
    *trans,
    *ancho,
    *largo,
    *x_min,
    *y_min,
    *X_max,
    *Y_max, *ip, *port = "5435", *password, *user = "postgres", *database;

  bitmap_t fruit;

  while ((argc > 1) && (argv[1][0] == '-'))
    {
      switch (argv[1][1])
	{
	case 'q':		/* Query */
	  quer = &argv[1][2];
	  quer = str_replace(quer,"\\","");
	  quer = str_replace(quer,"\"","");
	  break;
	case 'i':		/* ip */
	  ip = &argv[1][2];
	  break;

	case 'd':		/* database */
	  database = &argv[1][2];
	  break;
	case 'u':		/* user */
	  user = &argv[1][2];
	  break;
	case 'p':		/* password */
	  password = &argv[1][2];
	  break;
	case 'P':		/* Puerto */
	  port = &argv[1][2];
	  break;
	case 'r':		/* rojo */
	  rojo = &argv[1][2];
	  break;
	case 'g':		/* verde */
	  verde = &argv[1][2];
	  break;
	case 'b':		/* azul */
	  azul = &argv[1][2];
	  break;
	case 't':		/*  transparencia */
	  trans = &argv[1][2];
	  break;
	case 'R':		/* Radio */
	  radius = &argv[1][2];
	  break;

	case 'x':		/* x_min */
	  x_min = &argv[1][2];
	  break;
	case 'y':		/* y_min */
	  y_min = &argv[1][2];
	  break;
	case 'X':		/* X_max */
	  X_max = &argv[1][2];
	  break;
	case 'Y':		/* Y_max */
	  Y_max = &argv[1][2];
	  break;
	case 'W':		/* larg */
	  largo = &argv[1][2];
	  break;
	case 'H':		/* High */
	  ancho = &argv[1][2];
	  break;
	case 'h':		/* Ayuda, help */
	  usage ();
	  break;
	default:
	  printf ("Wrong Argument, use -h for help: %s\n", argv[1]);
	  usage ();
	}

      ++argv;
      --argc;
    }

  PGconn *conn;
  PGresult *res;  
  conn = PQsetdbLogin (ip, port, NULL, NULL, database, user, password);	/* Con esta sentencia se establece la conexión (GENERAL) */

  if (PQstatus (conn) == CONNECTION_BAD){
    printf ("Unable to connect to database\n");}

  else
    {
      res = PQexec (conn, quer);

      if (res != NULL && PGRES_TUPLES_OK == PQresultStatus (res))
	{
	  /* Dibuja de color el cuadrito */
	  larg = atoi (largo);
	  anch = atoi (ancho);
	  fruit.width = anch;
	  fruit.height = larg;

	  fruit.pixels =
	    calloc (sizeof (pixel_t), fruit.width * fruit.height);

	  /* Termina de dibujar el cuadrito */

	  /* Dibuja los puntos */
	   x_m = atof (x_min);
	   y_m = atof (y_min);
	   X_m = atof (X_max);
	   Y_m = atof (Y_max);
	   r = atoi (rojo);
	   g = atoi (verde);
	   b = atoi (azul);
		  
	  for (p = PQntuples (res) - 1; p >= 0; p--)
	    {

	      /* Aqu+í va la conversion:
	       * |x_max -x_min| = 256
	       * |x - x_min|    = i
	       * */

	      c_x = atof (PQgetvalue (res, p, 0));
	      c_y = atof (PQgetvalue (res, p, 1));

	      //i = 256*((abs(c_x-(xmin)))/xmax-xmin);
	      //j = 256*((abs(c_y-(ymin)))/ymax-ymin);;

	      radio = atoi (radius);

	      i = (anch) * ((abs (c_x - (x_m))) / (X_m - x_m));	/* Está ajustado a 999 para que baje un pixel y se pueda dibujar toda la bolita. Caso 6 se le restan 2 */
	      j = (larg) * ((abs (c_y - (y_m))) / (Y_m - y_m));
	      j = larg - j;	/* Para que se ajuste al cuadro pues la coordenada (0,0) está en la esquina superior izquierda.  */

	      /* Fin de transparencia */

	      /* Colores */
	      

	      /* Fin colores */
	      /* Pixel central */

	 
	      
		  fruit.pixels[larg*j+i].red = r;	/* Central */
		  fruit.pixels[larg*j+i].green = g;	/* Central */
		  fruit.pixels[larg*j+i].blue = b;	/* Central */
		  for (k = 1; k<radio; k++){
			for(k2 = 0; k2<=k; k2++){
			if(k!=radio-1 || k2!=radio-1){
			if(larg*(j+k)+i+k2 < larg*anch){
			fruit.pixels[larg*(j+k)+i+k2].red = r;	/* Central */
			fruit.pixels[larg*(j+k)+i+k2].green = g;	/* Central */
			fruit.pixels[larg*(j+k)+i+k2].blue = b;	/* Central */
			}
			if(larg*(j+k2)+i+k < larg*anch){
			fruit.pixels[larg*(j+k2)+i+k].red = r;	/* Central */
			fruit.pixels[larg*(j+k2)+i+k].green = g;	
			fruit.pixels[larg*(j+k2)+i+k].blue = b;	
			}
			if((larg*(j-k)+i-k2 < larg*anch)&&(larg*(j-k)+i-k2 >= 0)){
			fruit.pixels[larg*(j-k)+i-k2].red = r;	/* Central */
			fruit.pixels[larg*(j-k)+i-k2].green = g;	/* Central */
			fruit.pixels[larg*(j-k)+i-k2].blue = b;	/* Central */
			}
			if((larg*(j-k2)+i-k < larg*anch)&&(larg*(j-k2)+i-k >= 0)){
			fruit.pixels[larg*(j-k2)+i-k].red = r;	/* Central */
			fruit.pixels[larg*(j-k2)+i-k].green = g;	/* Central */
			fruit.pixels[larg*(j-k2)+i-k].blue = b;	/* Central */
			}
			if((larg*(j+k)+i-k2 < larg*anch)&&(larg*(j+k)+i-k2 >= 0)){
			fruit.pixels[larg*(j+k)+i-k2].red = r;	/* Central */
			fruit.pixels[larg*(j+k)+i-k2].green = g;	/* Central */
			fruit.pixels[larg*(j+k)+i-k2].blue = b;	/* Central */
			}
			if((larg*(j-k2)+i+k < larg*anch)&&(larg*(j-k2)+i+k >= 0)){
			fruit.pixels[larg*(j-k2)+i+k].red = r;	/* Central */
			fruit.pixels[larg*(j-k2)+i+k].green = g;	
			fruit.pixels[larg*(j-k2)+i+k].blue = b;	
			}
			if((larg*(j-k)+i+k2 < larg*anch)&&(larg*(j-k)+i+k2 >= 0)){
			fruit.pixels[larg*(j-k)+i+k2].red = r;	/* Central */
			fruit.pixels[larg*(j-k)+i+k2].green = g;	/* Central */
			fruit.pixels[larg*(j-k)+i+k2].blue = b;	/* Central */
			}
			if((larg*(j+k2)+i-k < larg*anch)&&(larg*(j+k2)+i-k >= 0)){
			fruit.pixels[larg*(j+k2)+i-k].red = r;	/* Central */
			fruit.pixels[larg*(j+k2)+i-k].green = g;	/* Central */
			fruit.pixels[larg*(j+k2)+i-k].blue = b;	/* Central */
			}
			}
			}
		  }
	  
	  
	  
	    }

	  tra = atoi (trans);

	  save_png_to_file (&fruit, tra);
	  free (fruit.pixels);
	  /* Termina la función dibuja */
	  PQclear (res);
	}
    }
  PQfinish (conn);
  /* Termina Postgres */
  return 0;
}

static pixel_t *
pixel_at (bitmap_t * bitmap, int x, int y)
{
  return bitmap->pixels + bitmap->height * y + x;
}

static int
save_png_to_file (bitmap_t * bitmap, int tr)
{
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  size_t x, y;
  png_byte **row_pointers = NULL;
  /* "status" contiene el valor de retorno de esta función. La primera esta configurada al valor cual significa 'failure'. Cuando el routine hs rk, it is set to a value which means 'success'. */
  int status = -1;
  int depth = 8;

  png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL)
    {
      goto png_create_write_struct_failed;
    }

  info_ptr = png_create_info_struct (png_ptr);
  if (info_ptr == NULL)
    {
      goto png_create_info_struct_failed;
    }

  if (setjmp (png_jmpbuf (png_ptr)))
    {
      goto png_failure;
    }
  /* Configura los atributos de la imagen */

  png_set_IHDR (png_ptr, info_ptr, bitmap->width, bitmap->height, depth, PNG_COLOR_TYPE_RGBA,	/* http://stackoverflow.com/questions/13911126/how-to-let-png-have-the-transparent-property */
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  /* Aquí se inicailizan las filas en el PNG. */

  row_pointers = png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
  for (y = 0; y < bitmap->height; ++y)
    {
      png_byte *row =
	png_malloc (png_ptr, sizeof (uint8_t) * bitmap->width * 4);
      row_pointers[y] = row;

      for (x = 0; x < bitmap->width; ++x)
	{
	  int hdata = 0;
	  pixel_t *pixel = pixel_at (bitmap, x, y);
	  *row++ = pixel->red;
	  hdata += pixel->red;
	  *row++ = pixel->green;
	  hdata += pixel->green;
	  *row++ = pixel->blue;
	  hdata += pixel->blue;
	  if (hdata == 0)
	    {
	      *row++ = tr;	/* Para controlar la transparencia. Va de 0 a 255 */
	    }
	  else
	    {
	      *row++ = 255;	/* Con esto puede controlarse la transparencia de las bolitas. Pero aun no he puesto la variable, tal vez no se necesita.  */
	    }
	}
    }

  png_set_rows (png_ptr, info_ptr, row_pointers);

   /*  static */
  struct mem_encode state;	/* Funcion de bash */
  /*  initialise - put this before png_write_png() call */
  state.buffer = NULL;		/* funciónde bash */
  state.size = 0;		/* Funciónde bash */

  png_set_write_fn (png_ptr, &state, my_png_write_data, my_png_flush);	/* Función de bash */
  png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

  /* Con lo anterior se ha escrito exitosamente la routina. "status" indica el exito success. */
/*  cleanup */

  status = 0;

/* Para salvar en bash */

  if (state.buffer)		/* Función de bash */
    fwrite (state.buffer, state.size, 1, stdout);
  free (state.buffer);		/* Función de bash */

/* Fin Para salvar en bash*/

  for (y = 0; y < bitmap->height; y++)
    {
      png_free (png_ptr, row_pointers[y]);
    }
  png_free (png_ptr, row_pointers);

png_failure:
png_create_info_struct_failed:
  png_destroy_write_struct (&png_ptr, &info_ptr);
png_create_write_struct_failed:

  return status;
}

/* Dado el "value" y "max", el máximo valo el cual esperamos "value" para tomar, este retorna un entero entre 0 y 255. */


void
usage (void)
{
  printf ("Usage:\n");
  printf (" -q\"<query>\"\n");
  printf (" -x<x_min>\n");
  printf (" -X<X_max>\n");
  printf (" -y<y_min>\n");
  printf (" -Y<Y_max>\n");
  printf (" -W<Width>\n");
  printf (" -H<Height>\n");
  printf (" -P<Port>\n");
  printf (" -i\"<ip>\"\n");
  printf (" -u\"<usser>\"\n");
  printf (" -p\"<password>\"\n");
  printf (" -d\"<database>\"\n");
  printf (" -r<color red>\n");
  printf (" -g<color green>\n");
  printf (" -b<color blue>\n");
  printf (" -t<transparency>\n");
  printf (" -R<radio>\n");
  printf (" -h<help>\n");
  exit (8);
}

/* Para salvar en bash */
void
my_png_write_data (png_structp png_ptr, png_bytep data, png_size_t length)
{
  /*  with libpng15 next line causes pointer deference error; use libpng12 */
  struct mem_encode *p = (struct mem_encode *) png_get_io_ptr (png_ptr);	/*  was png_ptr->io_ptr */
  size_t nsize = p->size + length;

  /*  allocate or grow buffer */
  if (p->buffer)
    p->buffer = realloc (p->buffer, nsize);
  else
    p->buffer = malloc (nsize);

  if (!p->buffer)
    png_error (png_ptr, "Write Error");

  /*  copy new bytes to end of buffer */
  memcpy (p->buffer + p->size, data, length);
  p->size += length;
}

void
my_png_flush (png_structp png_ptr)
{
}

char * str_replace ( char *string, char *substr, char *replacement ){
  char *tok = NULL;
  char *newstr = NULL;
  char *oldstr = NULL;
  char *head = NULL;
 
  /* if either substr or replacement is NULL, duplicate string a let caller handle it */
  if ( substr == NULL || replacement == NULL ) return strdup (string);
  newstr = strdup (string);
  head = newstr;
  while ( (tok = strstr ( head, substr ))){
    oldstr = newstr;
    newstr = malloc ( strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) + 1 );
    /*failed to alloc mem, free old string and return NULL */
    if ( newstr == NULL ){
      free (oldstr);
      return NULL;
    }
    memcpy ( newstr, oldstr, tok - oldstr );
    memcpy ( newstr + (tok - oldstr), replacement, strlen ( replacement ) );
    memcpy ( newstr + (tok - oldstr) + strlen( replacement ), tok + strlen ( substr ), strlen ( oldstr ) - strlen ( substr ) - ( tok - oldstr ) );
    memset ( newstr + strlen ( oldstr ) - strlen ( substr ) + strlen ( replacement ) , 0, 1 );
    /* move back head right after the last replacement */
    head = newstr + (tok - oldstr) + strlen( replacement );
    free (oldstr);
  }
  return newstr;
}
