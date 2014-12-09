#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <libpq-fe.h>
/* Para compilar: http://www.labbookpages.co.uk/software/imgProc/libPNG.html */

/* A coloured pixel. */

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

/* A picture. */
    
 typedef struct  {
    pixel_t *pixels;
    size_t width;
    size_t height;
} bitmap_t;
    
/* Given "bitmap", this returns the pixel of bitmap at the point 
   ("x", "y"). */

static pixel_t * pixel_at (bitmap_t * bitmap, int x, int y)
{
     return bitmap->pixels + bitmap->height * y + x;
}
    
/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
   success, non-zero on error. */

static int save_png_to_file (bitmap_t *bitmap, const char *path)
{
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
    /* "status" contains the return value of this function. At first
       it is set to a value which means 'failure'. When the routine
       has finished its work, it is set to a value which means
       'success'. */
    int status = -1;
    /* The following number is set by trial and error only. I cannot
       see where it it is documented in the libpng manual.
    */
    int pixel_size = 3;
    int depth = 8;

    
    fp = fopen (path, "wb");
    if (! fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }
    
    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }
    /* Set up error handling. Este if chance y lo puedo quitar */

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }
    
    /* Set image attributes. */

    png_set_IHDR (png_ptr,
                  info_ptr,
                  bitmap->width,
                  bitmap->height,
                  depth,
              	PNG_COLOR_TYPE_RGBA,   /* http://stackoverflow.com/questions/13911126/how-to-let-png-have-the-transparent-property */
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);

    /* Initialize rows of PNG. */

    row_pointers = png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
    for (y = 0; y < bitmap->height; ++y) {
        png_byte *row = png_malloc (png_ptr, sizeof (uint8_t) * bitmap->width * 4);
        row_pointers[y] = row;
		
        for (x = 0; x < bitmap->width; ++x) {		
			int hdata = 0;
            pixel_t * pixel = pixel_at (bitmap, x, y);
            *row++ = pixel->red;
			hdata += pixel-> red;
            *row++ = pixel->green;
			hdata += pixel-> green;
            *row++ = pixel->blue;
			hdata += pixel-> blue;
			if(hdata==0){
		    	*row++ = 0;
			}else{
				*row++ = 255;
			}
        }
    }
    
    /* Write the image data to "fp". */

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */

    status = 0;
    
    for (y = 0; y < bitmap->height; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);
    
 png_failure:
 png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
 png_create_write_struct_failed:
    fclose (fp);
 fopen_failed:
    return status;
}

/* Given "value" and "max", the maximum value which we expect "value"
   to take, this returns an integer between 0 and 255 proportional to
   "value" divided by "max". */

static int pix (int value, int max)
{
    if (value < 0)
        return 0;
        return (int) 255.0;
}

int main ()
{
	int x,y;
	float i, j, c_x, c_y;
	bitmap_t fruit;

	/* Inicia Postgres */
	/* Para instalar libpq-fe.h: sudo apt-get install libpq-dev */
	/* Y para compilar:
	 *
	 gcc -lm -lpng -lpq -o implPostgresql implPostgresql.c -I/usr/include/postgresql/  */

	PGconn *conn;
	PGresult *res;
	float p, q;

	conn = PQsetdbLogin("ip","5432",NULL,NULL,"database","user","password"); /* Con esta sentencia se establece la conexión (GENERAL)*/
	conn = PQsetdbLogin("db0.conabio.gob.mx","5435",NULL,NULL,"snib","postgres","conabio2008"); /* Con esta sentencia se establece la conexión */

	if (PQstatus(conn) == CONNECTION_BAD)

		printf("Unable to connect to database\n");

	else 
	{
		/* Query 1 */
	//		res = PQexec(conn, "SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_snib_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11295558.290298 3732572.964702,-10620466.456577 3732572.964702,-10620466.456577 4407664.798423,-11295558.290298 4407664.798423,-11295558.290298 3732572.964702))',900913),the_geom);");

		//http://geoportal.conabio.gob.mx/pmngr_bis?SRS=EPSG%3A900913&STYLES=&LAYERS=plantae_selector&FORMAT=image%2Fpng&VERSION=1.1.1&TRANSPARENT=TRUE&SERVICE=WMS&REQUE ST=GetMap&QTYPE=getSld&NQ=1&OPID=intersects&BBOX=-10067673.868096%2C1203424.573154%2C-8717490.200654%2C2553608.240596&WIDTH=276&HEIGHT=276

		/* Query 2 */
		//	res = PQexec(conn, "SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_snib_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-10067673.868096 1203424.573154,-8717490.200654 1203424.573154,-8717490.200654 2553608.240596,-10067673.868096 2553608.240596,-10067673.868096 1203424.573154))',900913),the_geom);");

		/* Query 3 */
//			res = PQexec(conn, "SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_snib_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-10657156.230149 1866286.482351,-10319610.313288 1866286.482351,-10319610.313288 2203832.399211,-10657156.230149 2203832.399211,-10657156.230149 1866286.482351))',900913),the_geom);");
		/* Query 4 */
	//		res = PQexec(conn, "SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_snib_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-10333368.978378 2072666.458692,-10291175.738771 2072666.458692,-10291175.738771 2114859.6983,-10333368.978378 2114859.6983,-10333368.978378 2072666.458692))',900913),the_geom);");

		/* Query 5 */
//			res = PQexec(conn, "SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_snib_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-10657156.230149 1866286.482351,-10319610.313288 1866286.482351,-10319610.313288 2203832.399211,-10657156.230149 2203832.399211,-10657156.230149 1866286.482351))',900913),the_geom);");

		/* Query 6 */
		res = PQexec(conn, "SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_snib_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);");

		if (res != NULL && PGRES_TUPLES_OK == PQresultStatus(res)){
					/* Aquí se llama a ala funci+on dibujar */

					//SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM tabla WHERE ST_Intersects(ST_GeomFromText('POLYGON((xmin ymin,xmax ymin,xmax ymax,xmin ymax,xmin ymin))',srid),the_geom);

					/* Dibuja de color el cuadrito */
				
					fruit.width = 1000;
					fruit.height = 1000;

					fruit.pixels = calloc (sizeof (pixel_t), fruit.width * fruit.height);

						/* Termina de dibujar el cuadrito */

					/* Dibuja los puntos */
					for (p = PQntuples(res)-1; p >=0; p--){

						/* Aqu+í va la conversion:
						 * |x_max -x_min| = 256
						 * |x - x_min|    = i
						 * */
						c_x = atof(PQgetvalue(res, p, 0));
						c_y = atof(PQgetvalue(res, p, 1));

						//i = 256*((abs(c_x-(xmin)))/xmax-xmin);
						//j = 256*((abs(c_y-(ymin)))/ymax-ymin);;

						/* Query 1 */
					//		i = 1000*((abs(c_x-(-11295558.290298)))/675091.833721);
					//		j = 1000*((abs(c_y-(3732572.964702)))/675091.833721);

						/* Query 2 */
	//						   	i = 1000*((abs(c_x-(-10067673.868096)))/1350183.667442);
	//						    j = 1000*((abs(c_y-(1203424.573154)))/1350183.667442);
						/* Query 3 */
				//			  i = 1000*((abs(c_x-(-10657156.230149)))/337545.916861);
				//		      j = 1000*((abs(c_y-(1866286.482351)))/337545.916861);
						/* Query 4 */
						//	   	i = 1000*((abs(c_x-(-10333368.978378)))/12406035.437070);
						//	    j = 1000*((abs(c_y-(2072666.458692)))/12406035.437070);
						/* Query 5 */
						//	   	i = 1000*((abs(c_x-(-10657156.230149)))/337545.916861);
						//	    j = 1000*((abs(c_y-(1866286.482351)))/337545.916861);
						/* Query 6 */
						i = 999*((abs(c_x-(-11320018.139346)))/1350183.667442); /* Está ajustado a 999 para que baje un pixel y se pueda dibujar toda la bolita */
						j = 999*((abs(c_y-(1203424.573154)))/1350183.667442);
						j = 1000-j; /* Para que se ajuste al cuadro pues la coordenada (0,0) está en la esquina superior izquierda.  */

						/* Pixel central */


						pixel_t * pixel_C = pixel_at (& fruit, i, j); /* Central */    /* A quí hay que restarle j a la resolución por que el (0,0 esta en la esquina superior izquierda) */
						pixel_t * pixel_D = pixel_at (& fruit, i+1, j); /* Derecha */
						pixel_t * pixel_I = pixel_at (& fruit, i-1, j); /* Izquierda */
						pixel_t * pixel_A = pixel_at (& fruit, i, j+1); /* Arriba */
						pixel_t * pixel_a = pixel_at (& fruit, i, j-1); /* abajo */
						pixel_t * pixel_aD = pixel_at (& fruit, i+1, j-1); /* abajo-Derecha */
						pixel_t * pixel_AD = pixel_at (& fruit, i+1, j+1); /* Arriba-Derecha */
						pixel_t * pixel_AI = pixel_at (& fruit, i-1, j+1); /* Arriba-Izquierda */
						pixel_t * pixel_aI = pixel_at (& fruit, i-1, j-1); /* abajo-Izquierda */
						pixel_t * pixel_D2 = pixel_at (& fruit, i+2, j); /* Derecha +2*/
						pixel_t * pixel_I2 = pixel_at (& fruit, i-2, j); /* Izquierda +2*/
						pixel_t * pixel_A2 = pixel_at (& fruit, i, j+2); /* Arriba +2*/
						pixel_t * pixel_a2 = pixel_at (& fruit, i, j-2); /* abajo +2*/

						pixel_C->red = pix (i, j);  /* Central */
						pixel_D->red = pix (i+1, j);  /* Derecha */
						pixel_I->red = pix (i-1, j);  /* Izquierda */ 
						pixel_A->red = pix (i, j+1); /* Arriba */  
						pixel_a->red = pix (i, j-1); /* abajo */
						pixel_aD->red = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->red = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->red = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->red = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_D2->red = pix (i+2, j);  /* Derecha +2*/
						pixel_I2->red = pix (i-2, j);  /* Izquierda +2*/ 
						pixel_A2->red = pix (i, j+2); /* Arriba +2*/  
						pixel_a2->red = pix (i, j-2); /* abajo +2*/

						pixel_C->green = pix (i, j);  /* Central */
						pixel_D->green = pix (i+1, j); /* Derecha */
						pixel_I->green = pix (i-1, j);  /* Izquierda */
						pixel_A->green = pix (i, j+1); /* Arriba */
						pixel_a->green = pix (i, j-1); /* abajo */
						pixel_aD->green = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->green = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->green = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->green = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_D2->green = pix (i+2, j); /* Derecha +2*/
						pixel_I2->green = pix (i-2, j);  /* Izquierda +2*/
						pixel_A2->green = pix (i, j+2); /* Arriba +2*/
						pixel_a2->green = pix (i, j-2); /* abajo +2*/

						pixel_C->blue = pix (i, j);  /* Central */
						pixel_D->blue = pix (i+1, j); /* Derecha */
						pixel_I->blue = pix (i-1, j); /* Izquierda */
						pixel_A->blue = pix (i, j+1); /* Arriba */
						pixel_a->blue = pix (i, j-1); /* abajo */
						pixel_aD->blue = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->blue = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->blue = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->blue = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_D2->blue = pix (i+2, j); /* Derecha +2*/
						pixel_I2->blue = pix (i-2, j); /* Izquierda +2*/
						pixel_A2->blue = pix (i, j+2); /* Arriba +2*/
						pixel_a2->blue = pix (i, j-2); /* abajo +2*/

						/* Termina de dibujar los puntos */

					}
	//		printf ( "%s", fruit);	/* Para salida a buffer */
				save_png_to_file (& fruit, "implPostgresql.png");
				free(fruit.pixels);
				/* Termina la función dibuja */
			//http://books.google.com.mx/books?id=gbjIzwE2NYkC&pg=PA177&lpg=PA177&dq=PQgetvalue+integer&source=bl&ots=HbeiW5vMUS&sig=UqLioZuMs7dhoPfsPUr55EvdLmM&hl=es-419&sa=X&ei=f_lsVOSmFMaiyATGqoIQ&redir_esc=y#v=onepage&q=PQgetvalue%20integer&f=false //Página 183
			PQclear(res);
		}
	}
	PQfinish(conn);
	/* Termina Postgres */
	/* Create an image. */
	/* Write the image to a file 'fruit.png'. */
	return 0;
}
