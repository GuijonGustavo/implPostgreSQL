#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <libpq-fe.h>
/* Para compilar: http://www.labbookpages.co.uk/software/imgProc/libPNG.html */
/*  gcc -Wall -O2 -lm -lpng -lpq -o implPostgresql implPostgresql.c -I/usr/include/postgresql/ */
/* Color del pixel */

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} pixel_t;

/* Para la imagen */
    
 typedef struct  {
    pixel_t *pixels;
    size_t width;
    size_t height;
} bitmap_t;

/* Para salvar bash */

struct mem_encode
{
   char *buffer;
   size_t size;

};

void my_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length);


void my_png_flush(png_structp png_ptr);

/* Finaliza para guardar en bash */

void usage(void);

static pixel_t * pixel_at (bitmap_t * bitmap, int x, int y);

static int save_png_to_file (bitmap_t *bitmap,const char *path, int tr);
//static int save_png_to_file (bitmap_t *bitmap, int tr);

static int pix (int value, int max);

int main(int argc, char *argv[])
{
	int radio,x,y,r,g,b, tr, larg, anch;
    float x_m, y_m, X_m, Y_m;
	float i, j, c_x, c_y;
	char * quer;
    char * rojo;
    char * verde;
    char * azul;
	char * radius;
	char * trans;
	char * ancho;
	char * largo;
	char * x_min;
	char * y_min;
	char * X_max;
	char * Y_max;
	char * ip;
	char * port;
	char * password;
	char * user;
	char * database;
	
	bitmap_t fruit;

/* Se decaran las banderas */


while ((argc > 1) && (argv[1][0] == '-'))
{
	switch (argv[1][1])
	{
		case 'q': /* Query */
			quer = &argv[1][2];
			break;
		case 'i': /* ip */
			ip = &argv[1][2];
			break;

		case 'd': /* database */
			database = &argv[1][2];
			break;
		case 'u': /* user */
			user = &argv[1][2];
			break;
		case 'p': /* password */
			password = &argv[1][2];
			break;
		case 'P': /* Puerto */
			port = &argv[1][2];
			break;
		case 'r': /* rojo */
			rojo = &argv[1][2];
			break;

		case 'g': /* verde */
			verde = &argv[1][2];
			break;

		case 'b': /* azul */
			azul =  &argv[1][2];
			break;

		case 't': /*  transparencia*/
			trans =  &argv[1][2];
			break;
		case 'R': /* Radio */
			radius = &argv[1][2];
			break;

		case 'x': /* x_min */
			x_min = &argv[1][2];
			break;
		case 'y': /* y_min */
			y_min = &argv[1][2];
			break;
		case 'X': /* X_max */
			X_max = &argv[1][2];
			break;
		case 'Y': /* Y_max */
			Y_max = &argv[1][2];
			break;
		case 'l': /* largo */
			largo = &argv[1][2];
			break;
		case 'w': /* ancho */
			ancho = &argv[1][2];
			break;
		case 'h': /* Ayuda, help */
			usage();
			break;
		default:
			printf("Wrong Argument, use -h for help: %s\n", argv[1]);
			usage();
	}

	++argv;
	--argc;
}
								
    /* Terminan las banderas */		

	/* Inicia Postgres */
	/* Para instalar libpq-fe.h: sudo apt-get install libpq-dev */
	/* Y para compilar:
	 *
	 gcc -lm -lpng -lpq -o implPostgresql implPostgresql.c -I/usr/include/postgresql/  */

	PGconn *conn;
	PGresult *res;
	float p, q;


//	conn = PQsetdbLogin("ip","5432",NULL,NULL,"database","user","password"); /* Con esta sentencia se establece la conexión (GENERAL)*/
//	conn = PQsetdbLogin("db0.conabio.gob.mx","5435",NULL,NULL,"snib","postgres","conabio2008"); /* Con esta sentencia se establece la conexión */

	conn = PQsetdbLogin(ip,port,NULL,NULL,database,user,password); /* Con esta sentencia se establece la conexión (GENERAL)*/
//	conn = PQsetdbLogin("db0.conabio.gob.mx","5435",NULL,NULL,"snib","postgres","conabio2008"); /* Con esta sentencia se establece la conexión */

//	conn = PQsetdbLogin("ip","5435",NULL,NULL,"database","user","password"); /* Con esta sentencia se establece la conexión (GENERAL)*/
//	conn = PQsetdbLogin("db02.conabio.gob.mx","5435",NULL,NULL,"geodb","postgres","conabio2008"); /* Con esta sentencia se establece la conexión */
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
//		res = PQexec(conn, "SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM geo_snib_plantae WHERE ST_Intersects(ST_GeomFromText('POLYGON((-11320018.139346 1203424.573154,-9969834.471904 1203424.573154,-9969834.471904 2553608.240596,-11320018.139346 2553608.240596,-11320018.139346 1203424.573154))',900913),the_geom);");
//
//
//  /* Nuevo query */
//
//res = PQexec(conn, "SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM lim07gw WHERE ST_Intersects(ST_Transform(ST_GeomFromText('POLYGON((-11136670.338224 1649831.71079,-10510498.202599 1649831.71079,-10510498.202599 2276003.846415,-11136670.338224 2276003.846415,-11136670.338224 1649831.71079))',900913),4326),the_geom);");
//
//
//


/* Query paa bandera */
		res = PQexec(conn, quer);

		if (res != NULL && PGRES_TUPLES_OK == PQresultStatus(res)){
					/* Aquí se llama a ala funci+on dibujar */

					//SELECT ST_X(the_geom) as x,ST_Y(the_geom) as y FROM tabla WHERE ST_Intersects(ST_GeomFromText('POLYGON((xmin ymin,xmax ymin,xmax ymax,xmin ymax,xmin ymin))',srid),the_geom);

					/* Dibuja de color el cuadrito */
					larg = atoi(largo);
					anch = atoi(ancho);
					fruit.width = anch;
					fruit.height = larg;

					fruit.pixels = calloc (sizeof (pixel_t), fruit.width * fruit.height);

						/* Termina de dibujar el cuadrito */

					/* Dibuja los puntos */
					for (p = PQntuples(res)-1; p >=0; p--){/* ************************************************************************************************ */

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
						x_m = atof(x_min);
						y_m = atof(y_min);
						X_m = atof(X_max);
						Y_m = atof(Y_max);

						i = (anch-1)*((abs(c_x-(x_m)))/(X_m-x_m)); /* Está ajustado a 999 para que baje un pixel y se pueda dibujar toda la bolita. Caso 6 se le restan 2 */
						j = (larg-1)*((abs(c_y-(y_m)))/(Y_m-y_m));
						j = larg-j; /* Para que se ajuste al cuadro pues la coordenada (0,0) está en la esquina superior izquierda.  */


				//		i = anch*((abs(c_x-(-11320018.139346)))/1350183.667442); /* Está ajustado a 999 para que baje un pixel y se pueda dibujar toda la bolita. Caso 6 se le restan 2 */
				//		j = larg*((abs(c_y-(1203424.573154)))/1350183.667442);
				//		j = larg-j; /* Para que se ajuste al cuadro pues la coordenada (0,0) está en la esquina superior izquierda.  */
/* transparencia */


//static int save_png_to_file (bitmap_t *bitmap,int tr, const char *path)
//{
//	tr = trans;
//}





						/* Fin de transparencia */



/* Colores */
								r = atoi(rojo);
								g = atoi(verde);
								b = atoi(azul);


/* Fin colores */


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
						pixel_t * pixel_a4 = pixel_at (& fruit, i-1, j+2); /*arriba j+2 */
						pixel_t * pixel_b4 = pixel_at (& fruit, i, j+2); /*arriba j+2*/
						pixel_t * pixel_c4 = pixel_at (& fruit, i+1, j+2); /* arriba j+2*/
						pixel_t * pixel_d4 = pixel_at (& fruit, i-1, j-2); /*arriba j+2 */
						pixel_t * pixel_e4 = pixel_at (& fruit, i, j-2); /*arriba j+2*/
						pixel_t * pixel_f4 = pixel_at (& fruit, i+1, j-2); /* arriba j+2*/
						pixel_t * pixel_g4 = pixel_at (& fruit, i+2, j+1); /*arriba j+2 */
						pixel_t * pixel_h4 = pixel_at (& fruit, i+2, j); /*arriba j+2*/
						pixel_t * pixel_i4 = pixel_at (& fruit, i+2, j-1); /* arriba j+2*/
						pixel_t * pixel_j4 = pixel_at (& fruit, i-2, j+1); /*arriba j+2 */
						pixel_t * pixel_k4 = pixel_at (& fruit, i-2, j); /*arriba j+2*/
						pixel_t * pixel_l4 = pixel_at (& fruit, i-2, j-1); /* arriba j+2*/
						pixel_t * pixel_a5 = pixel_at (& fruit, i+2, j+2); /* arriba j+2*/
						pixel_t * pixel_b5 = pixel_at (& fruit, i+2, j-2); /*arriba j+2 */
						pixel_t * pixel_c5 = pixel_at (& fruit, i-2, j+2); /*arriba j+2*/
						pixel_t * pixel_d5 = pixel_at (& fruit, i-2, j-2); /* arriba j+2*/
						pixel_t * pixel_a6 = pixel_at (& fruit, i-1, j+3); /* arriba j+2*/
						pixel_t * pixel_b6 = pixel_at (& fruit, i, j+3); /*arriba j+2 */
						pixel_t * pixel_c6 = pixel_at (& fruit, i+1, j+3); /*arriba j+2*/
						pixel_t * pixel_d6 = pixel_at (& fruit, i-1, j-3); /* arriba j+2*/
						pixel_t * pixel_e6 = pixel_at (& fruit, i, j-3); /*arriba j+2 */
						pixel_t * pixel_f6 = pixel_at (& fruit, i+1, j-3); /*arriba j+2*/
						pixel_t * pixel_g6 = pixel_at (& fruit, i-3, j+1); /* arriba j+2*/
						pixel_t * pixel_h6 = pixel_at (& fruit, i-3, j); /*arriba j+2 */
						pixel_t * pixel_i6 = pixel_at (& fruit, i-3, j-1); /*arriba j+2*/
						pixel_t * pixel_j6 = pixel_at (& fruit, i+3, j+1); /* arriba j+2*/
						pixel_t * pixel_k6 = pixel_at (& fruit, i+3, j); /*arriba j+2 */
						pixel_t * pixel_l6 = pixel_at (& fruit, i+3, j-1); /*arriba j+2*/
						pixel_t * pixel_a7 = pixel_at (& fruit, i+2, j+3); /*arriba j+2 */
						pixel_t * pixel_b7 = pixel_at (& fruit, i+3, j+2); /*arriba j+2*/
						pixel_t * pixel_c7 = pixel_at (& fruit, i-2, j+3); /*arriba j+2 */
						pixel_t * pixel_d7 = pixel_at (& fruit, i-3, j+2); /*arriba j+2*/
						pixel_t * pixel_e7 = pixel_at (& fruit, i+2, j-3); /*arriba j+2 */
						pixel_t * pixel_f7 = pixel_at (& fruit, i+3, j-2); /*arriba j+2*/
						pixel_t * pixel_g7 = pixel_at (& fruit, i-2, j-3); /*arriba j+2 */
						pixel_t * pixel_h7 = pixel_at (& fruit, i-3, j-2); /*arriba j+2*/

						radio = atoi(radius);

						switch ( radio ) {
							case 1:
						pixel_C->red = pix (i, j);  /* Central */
						pixel_C->green = pix (i, j);  /* Central */
						pixel_C->blue = pix (i, j);  /* Central */

						pixel_C->red = r;  /* Central */
						pixel_C->green = g;  /* Central */
						pixel_C->blue = b;  /* Central */
								break;

							case 2:	
						pixel_C->red = pix (i, j);  /* Central */
						pixel_D->red = pix (i+1, j);  /* Derecha */
						pixel_I->red = pix (i-1, j);  /* Izquierda */ 
						pixel_A->red = pix (i, j+1); /* Arriba */  
						pixel_a->red = pix (i, j-1); /* abajo */
						pixel_C->green = pix (i, j);  /* Central */
						pixel_D->green = pix (i+1, j); /* Derecha */
						pixel_I->green = pix (i-1, j);  /* Izquierda */
						pixel_A->green = pix (i, j+1); /* Arriba */
						pixel_a->green = pix (i, j-1); /* abajo */
						pixel_C->blue = pix (i, j);  /* Central */
						pixel_D->blue = pix (i+1, j); /* Derecha */
						pixel_I->blue = pix (i-1, j); /* Izquierda */
						pixel_A->blue = pix (i, j+1); /* Arriba */
						pixel_a->blue = pix (i, j-1); /* abajo */

						pixel_C->red = r;  /* Central */
						pixel_D->red = r;  /* Derecha */
						pixel_I->red = r;  /* Izquierda */ 
						pixel_A->red = r; /* Arriba */  
						pixel_a->red = r; /* abajo */
						pixel_C->green = g;  /* Central */
						pixel_D->green = g; /* Derecha */
						pixel_I->green = g;  /* Izquierda */
						pixel_A->green = g; /* Arriba */
						pixel_a->green = g; /* abajo */
						pixel_C->blue = b;  /* Central */
						pixel_D->blue = b; /* Derecha */
						pixel_I->blue = b; /* Izquierda */
						pixel_A->blue = b; /* Arriba */
						pixel_a->blue = b; /* abajo */

								break;

							case 3:
						pixel_C->red = pix (i, j);  /* Central */
						pixel_D->red = pix (i+1, j);  /* Derecha */
						pixel_I->red = pix (i-1, j);  /* Izquierda */ 
						pixel_A->red = pix (i, j+1); /* Arriba */  
						pixel_a->red = pix (i, j-1); /* abajo */
				   		pixel_aD->red = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->red = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->red = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->red = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_C->green = pix (i, j);  /* Central */
						pixel_D->green = pix (i+1, j); /* Derecha */
						pixel_I->green = pix (i-1, j);  /* Izquierda */
						pixel_A->green = pix (i, j+1); /* Arriba */
						pixel_a->green = pix (i, j-1); /* abajo */
						pixel_aD->green = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->green = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->green = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->green = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_C->blue = pix (i, j);  /* Central */
						pixel_D->blue = pix (i+1, j); /* Derecha */
						pixel_I->blue = pix (i-1, j); /* Izquierda */
						pixel_A->blue = pix (i, j+1); /* Arriba */
						pixel_a->blue = pix (i, j-1); /* abajo */
						pixel_aD->blue = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->blue = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->blue = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->blue = pix (i-1, j-1); /* abajo-Izquierda */
				
						pixel_C->red = r;  /* Central */
						pixel_D->red = r;  /* Derecha */
						pixel_I->red = r;  /* Izquierda */ 
						pixel_A->red = r; /* Arriba */  
						pixel_a->red = r; /* abajo */
				   		pixel_aD->red = r; /* abajo-Derecha */
						pixel_AD->red = r; /* Arriba-Derecha */
						pixel_AI->red = r; /* Arriba-Izquierda */
						pixel_aI->red = r; /* abajo-Izquierda */
						pixel_C->green = g;  /* Central */
						pixel_D->green = g; /* Derecha */
						pixel_I->green = g;  /* Izquierda */
						pixel_A->green = g; /* Arriba */
						pixel_a->green = g; /* abajo */
						pixel_aD->green = g; /* abajo-Derecha */
						pixel_AD->green = g; /* Arriba-Derecha */
						pixel_AI->green = g; /* Arriba-Izquierda */
						pixel_aI->green = g; /* abajo-Izquierda */
						pixel_C->blue = b;  /* Central */
						pixel_D->blue = b; /* Derecha */
						pixel_I->blue = b; /* Izquierda */
						pixel_A->blue = b; /* Arriba */
						pixel_a->blue = b; /* abajo */
						pixel_aD->blue = b; /* abajo-Derecha */
						pixel_AD->blue = b; /* Arriba-Derecha */
						pixel_AI->blue = b; /* Arriba-Izquierda */
						pixel_aI->blue = b; /* abajo-Izquierda */

		
						break;

							case 4:
						pixel_C->red = pix (i, j);  /* Central */
						pixel_D->red = pix (i+1, j);  /* Derecha */
						pixel_I->red = pix (i-1, j);  /* Izquierda */ 
						pixel_A->red = pix (i, j+1); /* Arriba */  
						pixel_a->red = pix (i, j-1); /* abajo */
						pixel_C->green = pix (i, j);  /* Central */
						pixel_D->green = pix (i+1, j); /* Derecha */
						pixel_I->green = pix (i-1, j);  /* Izquierda */
						pixel_A->green = pix (i, j+1); /* Arriba */
						pixel_a->green = pix (i, j-1); /* abajo */
						pixel_C->blue = pix (i, j);  /* Central */
						pixel_D->blue = pix (i+1, j); /* Derecha */
						pixel_I->blue = pix (i-1, j); /* Izquierda */
						pixel_A->blue = pix (i, j+1); /* Arriba */
						pixel_a->blue = pix (i, j-1); /* abajo */
						pixel_aD->red = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->red = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->red = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->red = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_aD->green = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->green = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->green = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->green = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_aD->blue = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->blue = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->blue = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->blue = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_a4->red = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->red = pix (i, j+2); /*arriba j+2*/
						pixel_c4->red = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->red = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->red = pix (i, j-2); /*arriba j+2*/
						pixel_f4->red = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->red = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->red = pix (i+2, j); /*arriba j+2*/
						pixel_i4->red = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->red = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->red = pix (i-2, j); /*arriba j+2*/
						pixel_l4->red = pix (i-2, j-1); /* arriba j+2*/
						pixel_a4->green = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->green = pix (i, j+2); /*arriba j+2*/
						pixel_c4->green = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->green = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->green = pix (i, j-2); /*arriba j+2*/
						pixel_f4->green = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->green = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->green = pix (i+2, j); /*arriba j+2*/
						pixel_i4->green = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->green = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->green = pix (i-2, j); /*arriba j+2*/
						pixel_l4->green = pix (i-2, j-1); /* arriba j+2*/
						pixel_a4->blue = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->blue = pix (i, j+2); /*arriba j+2*/
						pixel_c4->blue = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->blue = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->blue = pix (i, j-2); /*arriba j+2*/
						pixel_f4->blue = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->blue = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->blue = pix (i+2, j); /*arriba j+2*/
						pixel_i4->blue = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->blue = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->blue = pix (i-2, j); /*arriba j+2*/
						pixel_l4->blue = pix (i-2, j-1); /* arriba j+2*/

						pixel_C->red = r;  /* Central */
						pixel_D->red = r;  /* Derecha */
						pixel_I->red = r;  /* Izquierda */ 
						pixel_A->red = r; /* Arriba */  
						pixel_a->red = r; /* abajo */
						pixel_aD->red = r; /* abajo-Derecha */
						pixel_AD->red = r; /* Arriba-Derecha */
						pixel_AI->red = r; /* Arriba-Izquierda */
						pixel_aI->red = r; /* abajo-Izquierda */
						pixel_a4->red = r; /*arriba j+2 */
						pixel_b4->red = r; /*arriba j+2*/
						pixel_c4->red = r; /* arriba j+2*/
						pixel_d4->red = r; /*arriba j+2 */
						pixel_e4->red = r; /*arriba j+2*/
						pixel_f4->red = r; /* arriba j+2*/
						pixel_g4->red = r; /*arriba j+2 */
						pixel_h4->red = r; /*arriba j+2*/
						pixel_i4->red = r; /* arriba j+2*/
						pixel_j4->red = r; /*arriba j+2 */
						pixel_k4->red = r; /*arriba j+2*/
						pixel_l4->red = r; /* arriba j+2*/
						pixel_C->green = g; /* Central */
						pixel_D->green = g; /* Derecha */
						pixel_I->green = g;  /* Izquierda */
						pixel_A->green = g; /* Arriba */
						pixel_a->green = g; /* abajo */
						pixel_aD->green = g; /* abajo-Derecha */
						pixel_AD->green = g; /* Arriba-Derecha */
						pixel_AI->green = g; /* Arriba-Izquierda */
						pixel_aI->green = g; /* abajo-Izquierda */
						pixel_a4->green = g; /*arriba j+2 */
						pixel_b4->green = g; /*arriba j+2*/
						pixel_c4->green = g; /* arriba j+2*/
						pixel_d4->green = g; /*arriba j+2 */
						pixel_e4->green = g; /*arriba j+2*/
						pixel_f4->green = g; /* arriba j+2*/
						pixel_g4->green = g; /*arriba j+2 */
						pixel_h4->green = g; /*arriba j+2*/
						pixel_i4->green = g; /* arriba j+2*/
						pixel_j4->green = g; /*arriba j+2 */
						pixel_k4->green = g; /*arriba j+2*/
						pixel_l4->green = g; /* arriba j+2*/
						pixel_C->blue = b;  /* Central */
						pixel_D->blue = b; /* Derecha */
						pixel_I->blue = b; /* Izquierda */
						pixel_A->blue = b; /* Arriba */
						pixel_a->blue = b; /* abajo */
						pixel_aD->blue = b; /* abajo-Derecha */
						pixel_AD->blue = b; /* Arriba-Derecha */
						pixel_AI->blue = b; /* Arriba-Izquierda */
						pixel_aI->blue = b; /* abajo-Izquierda */
						pixel_a4->blue = b; /*arriba j+2 */
						pixel_b4->blue = b; /*arriba j+2*/
						pixel_c4->blue = b; /* arriba j+2*/
						pixel_d4->blue = b; /*arriba j+2 */
						pixel_e4->blue = b; /*arriba j+2*/
						pixel_f4->blue = b; /* arriba j+2*/
						pixel_g4->blue = b; /*arriba j+2 */
						pixel_h4->blue = b; /*arriba j+2*/
						pixel_i4->blue = b; /* arriba j+2*/
						pixel_j4->blue = b; /*arriba j+2 */
						pixel_k4->blue = b; /*arriba j+2*/
						pixel_l4->blue = b; /* arriba j+2*/
								break;

							case 5:
						pixel_C->red = pix (i, j);  /* Central */
						pixel_D->red = pix (i+1, j);  /* Derecha */
						pixel_I->red = pix (i-1, j);  /* Izquierda */ 
						pixel_A->red = pix (i, j+1); /* Arriba */  
						pixel_a->red = pix (i, j-1); /* abajo */
						pixel_C->green = pix (i, j);  /* Central */
						pixel_D->green = pix (i+1, j); /* Derecha */
						pixel_I->green = pix (i-1, j);  /* Izquierda */
						pixel_A->green = pix (i, j+1); /* Arriba */
						pixel_a->green = pix (i, j-1); /* abajo */
						pixel_C->blue = pix (i, j);  /* Central */
						pixel_D->blue = pix (i+1, j); /* Derecha */
						pixel_I->blue = pix (i-1, j); /* Izquierda */
						pixel_A->blue = pix (i, j+1); /* Arriba */
						pixel_a->blue = pix (i, j-1); /* abajo */
						pixel_aD->red = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->red = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->red = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->red = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_aD->green = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->green = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->green = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->green = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_aD->blue = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->blue = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->blue = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->blue = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_a4->red = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->red = pix (i, j+2); /*arriba j+2*/
						pixel_c4->red = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->red = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->red = pix (i, j-2); /*arriba j+2*/
						pixel_f4->red = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->red = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->red = pix (i+2, j); /*arriba j+2*/
						pixel_i4->red = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->red = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->red = pix (i-2, j); /*arriba j+2*/
						pixel_l4->red = pix (i-2, j-1); /* arriba j+2*/
						pixel_a4->green = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->green = pix (i, j+2); /*arriba j+2*/
						pixel_c4->green = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->green = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->green = pix (i, j-2); /*arriba j+2*/
						pixel_f4->green = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->green = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->green = pix (i+2, j); /*arriba j+2*/
						pixel_i4->green = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->green = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->green = pix (i-2, j); /*arriba j+2*/
						pixel_l4->green = pix (i-2, j-1); /* arriba j+2*/
						pixel_a4->blue = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->blue = pix (i, j+2); /*arriba j+2*/
						pixel_c4->blue = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->blue = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->blue = pix (i, j-2); /*arriba j+2*/
						pixel_f4->blue = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->blue = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->blue = pix (i+2, j); /*arriba j+2*/
						pixel_i4->blue = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->blue = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->blue = pix (i-2, j); /*arriba j+2*/
						pixel_l4->blue = pix (i-2, j-1); /* arriba j+2*/
						pixel_a5->red = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->red = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->red = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->red = pix (i-2, j-2); /* arriba j+2*/
						pixel_a5->green = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->green = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->green = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->green = pix (i-2, j-2); /* arriba j+2*/
						pixel_a5->blue = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->blue = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->blue = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->blue = pix (i-2, j-2); /* arriba j+2*/

						pixel_C->red = r;  /* Central */
						pixel_D->red = r;  /* Derecha */
						pixel_I->red = r;  /* Izquierda */ 
						pixel_A->red = r; /* Arriba */  
						pixel_a->red = r; /* abajo */
						pixel_aD->red = r; /* abajo-Derecha */
						pixel_AD->red = r; /* Arriba-Derecha */
						pixel_AI->red = r; /* Arriba-Izquierda */
						pixel_aI->red = r; /* abajo-Izquierda */
						pixel_a4->red = r; /*arriba j+2 */
						pixel_b4->red = r; /*arriba j+2*/
						pixel_c4->red = r; /* arriba j+2*/
						pixel_d4->red = r; /*arriba j+2 */
						pixel_e4->red = r; /*arriba j+2*/
						pixel_f4->red = r; /* arriba j+2*/
						pixel_g4->red = r; /*arriba j+2 */
						pixel_h4->red = r; /*arriba j+2*/
						pixel_i4->red = r; /* arriba j+2*/
						pixel_j4->red = r; /*arriba j+2 */
						pixel_k4->red = r; /*arriba j+2*/
						pixel_l4->red = r; /* arriba j+2*/
						pixel_a5->red = r; /* arriba j+2*/
						pixel_b5->red = r; /*arriba j+2 */
						pixel_c5->red = r; /*arriba j+2*/
						pixel_d5->red = r; /* arriba j+2*/
						pixel_C->green = g;  /* Central */
						pixel_D->green = g; /* Derecha */
						pixel_I->green = g;  /* Izquierda */
						pixel_A->green = g; /* Arriba */
						pixel_a->green = g; /* abajo */
						pixel_aD->green = g; /* abajo-Derecha */
						pixel_AD->green = g; /* Arriba-Derecha */
						pixel_AI->green = g; /* Arriba-Izquierda */
						pixel_aI->green = g; /* abajo-Izquierda */
						pixel_a4->green = g; /*arriba j+2 */
						pixel_b4->green = g; /*arriba j+2*/
						pixel_c4->green = g; /* arriba j+2*/
						pixel_d4->green = g; /*arriba j+2 */
						pixel_e4->green = g; /*arriba j+2*/
						pixel_f4->green = g; /* arriba j+2*/
						pixel_g4->green = g; /*arriba j+2 */
						pixel_h4->green = g; /*arriba j+2*/
						pixel_i4->green = g; /* arriba j+2*/
						pixel_j4->green = g; /*arriba j+2 */
						pixel_k4->green = g; /*arriba j+2*/
						pixel_l4->green = g; /* arriba j+2*/
						pixel_a5->green = g; /* arriba j+2*/
						pixel_b5->green = g; /*arriba j+2 */
						pixel_c5->green = g; /*arriba j+2*/
						pixel_d5->green = g; /* arriba j+2*/
						pixel_C->blue = b;  /* Central */
						pixel_D->blue = b; /* Derecha */
						pixel_I->blue = b; /* Izquierda */
						pixel_A->blue = b; /* Arriba */
						pixel_a->blue = b; /* abajo */
						pixel_aD->blue = b; /* abajo-Derecha */
						pixel_AD->blue = b; /* Arriba-Derecha */
						pixel_AI->blue = b; /* Arriba-Izquierda */
						pixel_aI->blue = b; /* abajo-Izquierda */
						pixel_a4->blue = b; /*arriba j+2 */
						pixel_b4->blue = b; /*arriba j+2*/
						pixel_c4->blue = b; /* arriba j+2*/
						pixel_d4->blue = b; /*arriba j+2 */
						pixel_e4->blue = b; /*arriba j+2*/
						pixel_f4->blue = b; /* arriba j+2*/
						pixel_g4->blue = b; /*arriba j+2 */
						pixel_h4->blue = b; /*arriba j+2*/
						pixel_i4->blue = b; /* arriba j+2*/
						pixel_j4->blue = b; /*arriba j+2 */
						pixel_k4->blue = b; /*arriba j+2*/
						pixel_l4->blue = b; /* arriba j+2*/
						pixel_a5->blue = b; /* arriba j+2*/
						pixel_b5->blue = b; /*arriba j+2 */
						pixel_c5->blue = b; /*arriba j+2*/
						pixel_d5->blue = b; /* arriba j+2*/
								break;
	


								case 6:


						pixel_C->red = pix (i, j);  /* Central */
						pixel_D->red = pix (i+1, j);  /* Derecha */
						pixel_I->red = pix (i-1, j);  /* Izquierda */ 
						pixel_A->red = pix (i, j+1); /* Arriba */  
						pixel_a->red = pix (i, j-1); /* abajo */
						pixel_C->green = pix (i, j);  /* Central */
						pixel_D->green = pix (i+1, j); /* Derecha */
						pixel_I->green = pix (i-1, j);  /* Izquierda */
						pixel_A->green = pix (i, j+1); /* Arriba */
						pixel_a->green = pix (i, j-1); /* abajo */
						pixel_C->blue = pix (i, j);  /* Central */
						pixel_D->blue = pix (i+1, j); /* Derecha */
						pixel_I->blue = pix (i-1, j); /* Izquierda */
						pixel_A->blue = pix (i, j+1); /* Arriba */
						pixel_a->blue = pix (i, j-1); /* abajo */
						pixel_aD->red = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->red = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->red = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->red = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_aD->green = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->green = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->green = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->green = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_aD->blue = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->blue = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->blue = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->blue = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_a4->red = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->red = pix (i, j+2); /*arriba j+2*/
						pixel_c4->red = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->red = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->red = pix (i, j-2); /*arriba j+2*/
						pixel_f4->red = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->red = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->red = pix (i+2, j); /*arriba j+2*/
						pixel_i4->red = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->red = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->red = pix (i-2, j); /*arriba j+2*/
						pixel_l4->red = pix (i-2, j-1); /* arriba j+2*/
						pixel_a4->green = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->green = pix (i, j+2); /*arriba j+2*/
						pixel_c4->green = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->green = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->green = pix (i, j-2); /*arriba j+2*/
						pixel_f4->green = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->green = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->green = pix (i+2, j); /*arriba j+2*/
						pixel_i4->green = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->green = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->green = pix (i-2, j); /*arriba j+2*/
						pixel_l4->green = pix (i-2, j-1); /* arriba j+2*/
						pixel_a4->blue = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->blue = pix (i, j+2); /*arriba j+2*/
						pixel_c4->blue = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->blue = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->blue = pix (i, j-2); /*arriba j+2*/
						pixel_f4->blue = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->blue = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->blue = pix (i+2, j); /*arriba j+2*/
						pixel_i4->blue = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->blue = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->blue = pix (i-2, j); /*arriba j+2*/
						pixel_l4->blue = pix (i-2, j-1); /* arriba j+2*/
						pixel_a5->red = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->red = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->red = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->red = pix (i-2, j-2); /* arriba j+2*/
						pixel_a5->green = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->green = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->green = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->green = pix (i-2, j-2); /* arriba j+2*/
						pixel_a5->blue = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->blue = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->blue = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->blue = pix (i-2, j-2); /* arriba j+2*/
						pixel_a6-> red = pix (i-1, j+3); /* arriba j+2*/
						pixel_b6-> red = pix (i, j+3); /*arriba j+2 */
						pixel_c6-> red = pix (i+1, j+3); /*arriba j+2*/
						pixel_d6-> red = pix (i-1, j-3); /* arriba j+2*/
						pixel_e6-> red = pix (i, j-3); /*arriba j+2 */
						pixel_f6-> red = pix (i+1, j-3); /*arriba j+2*/
						pixel_g6-> red = pix (i-3, j+1); /* arriba j+2*/
						pixel_h6-> red = pix (i-3, j); /*arriba j+2 */
						pixel_i6-> red = pix (i-3, j-1); /*arriba j+2*/
						pixel_j6-> red = pix (i+3, j+1); /* arriba j+2*/
						pixel_k6-> red = pix (i+3, j); /*arriba j+2 */
						pixel_l6-> red = pix (i+3, j-1); /*arriba j+2*/
						pixel_a6-> green = pix (i-1, j+3); /* arriba j+2*/
						pixel_b6-> green = pix (i, j+3); /*arriba j+2 */
						pixel_c6-> green = pix (i+1, j+3); /*arriba j+2*/
						pixel_d6-> green = pix (i-1, j-3); /* arriba j+2*/
						pixel_e6-> green = pix (i, j-3); /*arriba j+2 */
						pixel_f6-> green = pix (i+1, j-3); /*arriba j+2*/
						pixel_g6-> green = pix (i-3, j+1); /* arriba j+2*/
						pixel_h6-> green = pix (i-3, j); /*arriba j+2 */
						pixel_i6-> green = pix (i-3, j-1); /*arriba j+2*/
						pixel_j6-> green = pix (i+3, j+1); /* arriba j+2*/
						pixel_k6-> green = pix (i+3, j); /*arriba j+2 */
						pixel_l6-> green = pix (i+3, j-1); /*arriba j+2*/
						pixel_a6-> blue = pix (i-1, j+3); /* arriba j+2*/
						pixel_b6-> blue = pix (i, j+3); /*arriba j+2 */
						pixel_c6-> blue = pix (i+1, j+3); /*arriba j+2*/
						pixel_d6-> blue = pix (i-1, j-3); /* arriba j+2*/
						pixel_e6-> blue = pix (i, j-3); /*arriba j+2 */
						pixel_f6-> blue = pix (i+1, j-3); /*arriba j+2*/
						pixel_g6-> blue = pix (i-3, j+1); /* arriba j+2*/
						pixel_h6-> blue = pix (i-3, j); /*arriba j+2 */
						pixel_i6-> blue = pix (i-3, j-1); /*arriba j+2*/
						pixel_j6-> blue = pix (i+3, j+1); /* arriba j+2*/
						pixel_k6-> blue = pix (i+3, j); /*arriba j+2 */
						
						pixel_C->red = r;  /* Central */
						pixel_D->red = r;  /* Derecha */
						pixel_I->red = r;  /* Izquierda */ 
						pixel_A->red = r; /* Arriba */  
						pixel_a->red = r; /* abajo */
						pixel_aD->red = r; /* abajo-Derecha */
						pixel_AD->red = r; /* Arriba-Derecha */
						pixel_AI->red = r; /* Arriba-Izquierda */
						pixel_aI->red = r; /* abajo-Izquierda */
						pixel_a4->red = r; /*arriba j+2 */
						pixel_b4->red = r; /*arriba j+2*/
						pixel_c4->red = r; /* arriba j+2*/
						pixel_d4->red = r; /*arriba j+2 */
						pixel_e4->red = r; /*arriba j+2*/
						pixel_f4->red = r; /* arriba j+2*/
						pixel_g4->red = r; /*arriba j+2 */
						pixel_h4->red = r; /*arriba j+2*/
						pixel_i4->red = r; /* arriba j+2*/
						pixel_j4->red = r; /*arriba j+2 */
						pixel_k4->red = r; /*arriba j+2*/
						pixel_l4->red = r; /* arriba j+2*/
						pixel_a5->red = r; /* arriba j+2*/
						pixel_b5->red = r; /*arriba j+2 */
						pixel_c5->red = r; /*arriba j+2*/
						pixel_d5->red = r; /* arriba j+2*/
						pixel_a6-> red = r; /* arriba j+2*/
						pixel_b6-> red = r; /*arriba j+2 */
						pixel_c6-> red = r; /*arriba j+2*/
						pixel_d6-> red = r; /* arriba j+2*/
						pixel_e6-> red = r; /*arriba j+2 */
						pixel_f6-> red = r; /*arriba j+2*/
						pixel_g6-> red = r; /* arriba j+2*/
						pixel_h6-> red = r; /*arriba j+2 */
						pixel_i6-> red = r; /*arriba j+2*/
						pixel_j6-> red = r; /* arriba j+2*/
						pixel_k6-> red = r; /*arriba j+2 */
						pixel_l6-> red = r; /*arriba j+2*/
						pixel_C->green = g;  /* Central */
						pixel_D->green = g; /* Derecha */
						pixel_I->green = g;  /* Izquierda */
						pixel_A->green = g; /* Arriba */
						pixel_a->green = g; /* abajo */
						pixel_aD->green = g; /* abajo-Derecha */
						pixel_AD->green = g; /* Arriba-Derecha */
						pixel_AI->green = g; /* Arriba-Izquierda */
						pixel_aI->green = g; /* abajo-Izquierda */
						pixel_a4->green = g; /*arriba j+2 */
						pixel_b4->green = g; /*arriba j+2*/
						pixel_c4->green = g; /* arriba j+2*/
						pixel_d4->green = g; /*arriba j+2 */
						pixel_e4->green = g; /*arriba j+2*/
						pixel_f4->green = g; /* arriba j+2*/
						pixel_g4->green = g; /*arriba j+2 */
						pixel_h4->green = g; /*arriba j+2*/
						pixel_i4->green = g; /* arriba j+2*/
						pixel_j4->green = g; /*arriba j+2 */
						pixel_k4->green = g; /*arriba j+2*/
						pixel_l4->green = g; /* arriba j+2*/
						pixel_a5->green = g; /* arriba j+2*/
						pixel_b5->green = g; /*arriba j+2 */
						pixel_c5->green = g; /*arriba j+2*/
						pixel_d5->green = g; /* arriba j+2*/
						pixel_a6-> green = g; /* arriba j+2*/
						pixel_b6-> green = g; /*arriba j+2 */
						pixel_c6-> green = g; /*arriba j+2*/
						pixel_d6-> green = g; /* arriba j+2*/
						pixel_e6-> green = g; /*arriba j+2 */
						pixel_f6-> green = g; /*arriba j+2*/
						pixel_g6-> green = g; /* arriba j+2*/
						pixel_h6-> green = g; /*arriba j+2 */
						pixel_i6-> green = g; /*arriba j+2*/
						pixel_j6-> green = g; /* arriba j+2*/
						pixel_k6-> green = g; /*arriba j+2 */
						pixel_l6-> green = g; /*arriba j+2*/
						pixel_C->blue = b;  /* Central */
						pixel_D->blue = b; /* Derecha */
						pixel_I->blue = b; /* Izquierda */
						pixel_A->blue = b; /* Arriba */
						pixel_a->blue = b; /* abajo */
						pixel_aD->blue = b; /* abajo-Derecha */
						pixel_AD->blue = b; /* Arriba-Derecha */
						pixel_AI->blue = b; /* Arriba-Izquierda */
						pixel_aI->blue = b; /* abajo-Izquierda */
						pixel_a4->blue = b; /*arriba j+2 */
						pixel_b4->blue = b; /*arriba j+2*/
						pixel_c4->blue = b; /* arriba j+2*/
						pixel_d4->blue = b; /*arriba j+2 */
						pixel_e4->blue = b; /*arriba j+2*/
						pixel_f4->blue = b; /* arriba j+2*/
						pixel_g4->blue = b; /*arriba j+2 */
						pixel_h4->blue = b; /*arriba j+2*/
						pixel_i4->blue = b; /* arriba j+2*/
						pixel_j4->blue = b; /*arriba j+2 */
						pixel_k4->blue = b; /*arriba j+2*/
						pixel_l4->blue = b; /* arriba j+2*/
						pixel_a5->blue = b; /* arriba j+2*/
						pixel_b5->blue = b; /*arriba j+2 */
						pixel_c5->blue = b; /*arriba j+2*/
						pixel_d5->blue = b; /* arriba j+2*/
						pixel_a6-> blue = b; /* arriba j+2*/
						pixel_b6-> blue = b; /*arriba j+2 */
						pixel_c6-> blue = b; /*arriba j+2*/
						pixel_d6-> blue = b; /* arriba j+2*/
						pixel_e6-> blue = b; /*arriba j+2 */
						pixel_f6-> blue = b; /*arriba j+2*/
						pixel_g6-> blue = b; /* arriba j+2*/
						pixel_h6-> blue = b; /*arriba j+2 */
						pixel_i6-> blue = b; /*arriba j+2*/
						pixel_j6-> blue = b; /* arriba j+2*/
						pixel_k6-> blue = b; /*arriba j+2 */
						pixel_l6-> blue = b; /*arriba j+2*/
						pixel_l6-> blue = b; /*arriba j+2*/
								break;



								case 7:


						pixel_C->red = pix (i, j);  /* Central */
						pixel_D->red = pix (i+1, j);  /* Derecha */
						pixel_I->red = pix (i-1, j);  /* Izquierda */ 
						pixel_A->red = pix (i, j+1); /* Arriba */  
						pixel_a->red = pix (i, j-1); /* abajo */
						pixel_C->green = pix (i, j);  /* Central */
						pixel_D->green = pix (i+1, j); /* Derecha */
						pixel_I->green = pix (i-1, j);  /* Izquierda */
						pixel_A->green = pix (i, j+1); /* Arriba */
						pixel_a->green = pix (i, j-1); /* abajo */
						pixel_C->blue = pix (i, j);  /* Central */
						pixel_D->blue = pix (i+1, j); /* Derecha */
						pixel_I->blue = pix (i-1, j); /* Izquierda */
						pixel_A->blue = pix (i, j+1); /* Arriba */
						pixel_a->blue = pix (i, j-1); /* abajo */
						pixel_aD->red = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->red = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->red = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->red = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_aD->green = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->green = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->green = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->green = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_aD->blue = pix (i+1, j-1); /* abajo-Derecha */
						pixel_AD->blue = pix (i+1, j+1); /* Arriba-Derecha */
						pixel_AI->blue = pix (i-1, j+1); /* Arriba-Izquierda */
						pixel_aI->blue = pix (i-1, j-1); /* abajo-Izquierda */
						pixel_a4->red = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->red = pix (i, j+2); /*arriba j+2*/
						pixel_c4->red = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->red = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->red = pix (i, j-2); /*arriba j+2*/
						pixel_f4->red = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->red = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->red = pix (i+2, j); /*arriba j+2*/
						pixel_i4->red = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->red = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->red = pix (i-2, j); /*arriba j+2*/
						pixel_l4->red = pix (i-2, j-1); /* arriba j+2*/
						pixel_a4->green = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->green = pix (i, j+2); /*arriba j+2*/
						pixel_c4->green = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->green = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->green = pix (i, j-2); /*arriba j+2*/
						pixel_f4->green = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->green = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->green = pix (i+2, j); /*arriba j+2*/
						pixel_i4->green = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->green = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->green = pix (i-2, j); /*arriba j+2*/
						pixel_l4->green = pix (i-2, j-1); /* arriba j+2*/
						pixel_a4->blue = pix (i-1, j+2); /*arriba j+2 */
						pixel_b4->blue = pix (i, j+2); /*arriba j+2*/
						pixel_c4->blue = pix (i+1, j+2); /* arriba j+2*/
						pixel_d4->blue = pix (i-1, j-2); /*arriba j+2 */
						pixel_e4->blue = pix (i, j-2); /*arriba j+2*/
						pixel_f4->blue = pix (i+1, j-2); /* arriba j+2*/
						pixel_g4->blue = pix (i+2, j+1); /*arriba j+2 */
						pixel_h4->blue = pix (i+2, j); /*arriba j+2*/
						pixel_i4->blue = pix (i+2, j-1); /* arriba j+2*/
						pixel_j4->blue = pix (i-2, j+1); /*arriba j+2 */
						pixel_k4->blue = pix (i-2, j); /*arriba j+2*/
						pixel_l4->blue = pix (i-2, j-1); /* arriba j+2*/
						pixel_a5->red = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->red = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->red = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->red = pix (i-2, j-2); /* arriba j+2*/
						pixel_a5->green = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->green = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->green = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->green = pix (i-2, j-2); /* arriba j+2*/
						pixel_a5->blue = pix (i+2, j+2); /* arriba j+2*/
						pixel_b5->blue = pix (i+2, j-2); /*arriba j+2 */
						pixel_c5->blue = pix (i-2, j+2); /*arriba j+2*/
						pixel_d5->blue = pix (i-2, j-2); /* arriba j+2*/
						pixel_a6-> red = pix (i-1, j+3); /* arriba j+2*/
						pixel_b6-> red = pix (i, j+3); /*arriba j+2 */
						pixel_c6-> red = pix (i+1, j+3); /*arriba j+2*/
						pixel_d6-> red = pix (i-1, j-3); /* arriba j+2*/
						pixel_e6-> red = pix (i, j-3); /*arriba j+2 */
						pixel_f6-> red = pix (i+1, j-3); /*arriba j+2*/
						pixel_g6-> red = pix (i-3, j+1); /* arriba j+2*/
						pixel_h6-> red = pix (i-3, j); /*arriba j+2 */
						pixel_i6-> red = pix (i-3, j-1); /*arriba j+2*/
						pixel_j6-> red = pix (i+3, j+1); /* arriba j+2*/
						pixel_k6-> red = pix (i+3, j); /*arriba j+2 */
						pixel_l6-> red = pix (i+3, j-1); /*arriba j+2*/
						pixel_a6-> green = pix (i-1, j+3); /* arriba j+2*/
						pixel_b6-> green = pix (i, j+3); /*arriba j+2 */
						pixel_c6-> green = pix (i+1, j+3); /*arriba j+2*/
						pixel_d6-> green = pix (i-1, j-3); /* arriba j+2*/
						pixel_e6-> green = pix (i, j-3); /*arriba j+2 */
						pixel_f6-> green = pix (i+1, j-3); /*arriba j+2*/
						pixel_g6-> green = pix (i-3, j+1); /* arriba j+2*/
						pixel_h6-> green = pix (i-3, j); /*arriba j+2 */
						pixel_i6-> green = pix (i-3, j-1); /*arriba j+2*/
						pixel_j6-> green = pix (i+3, j+1); /* arriba j+2*/
						pixel_k6-> green = pix (i+3, j); /*arriba j+2 */
						pixel_l6-> green = pix (i+3, j-1); /*arriba j+2*/
						pixel_a6-> blue = pix (i-1, j+3); /* arriba j+2*/
						pixel_b6-> blue = pix (i, j+3); /*arriba j+2 */
						pixel_c6-> blue = pix (i+1, j+3); /*arriba j+2*/
						pixel_d6-> blue = pix (i-1, j-3); /* arriba j+2*/
						pixel_e6-> blue = pix (i, j-3); /*arriba j+2 */
						pixel_f6-> blue = pix (i+1, j-3); /*arriba j+2*/
						pixel_g6-> blue = pix (i-3, j+1); /* arriba j+2*/
						pixel_h6-> blue = pix (i-3, j); /*arriba j+2 */
						pixel_i6-> blue = pix (i-3, j-1); /*arriba j+2*/
						pixel_j6-> blue = pix (i+3, j+1); /* arriba j+2*/
						pixel_k6-> blue = pix (i+3, j); /*arriba j+2 */
						pixel_l6-> blue = pix (i+3, j-1); /*arriba j+2*/
						pixel_a7-> red = pix (i+2, j+3); /* arriba j+2*/
						pixel_b7-> red = pix (i+3, j+2); /*arriba j+2 */
						pixel_c7-> red = pix (i-2, j+3); /* arriba j+2*/
						pixel_d7-> red = pix (i-3, j+2); /*arriba j+2 */
						pixel_e7-> red = pix (i+2, j-3); /* arriba j+2*/
						pixel_f7-> red = pix (i+3, j-2); /*arriba j+2 */
						pixel_g7-> red = pix (i-2, j-3); /* arriba j+2*/
						pixel_h7-> red = pix (i-3, j-2); /*arriba j+2 */
						pixel_a7-> green = pix (i+2, j+3); /* arriba j+2*/
						pixel_b7-> green = pix (i+3, j+2); /*arriba j+2 */
						pixel_c7-> green = pix (i-2, j+3); /* arriba j+2*/
						pixel_d7-> green = pix (i-3, j+2); /*arriba j+2 */
						pixel_e7-> green = pix (i+2, j-3); /* arriba j+2*/
						pixel_f7-> green = pix (i+3, j-2); /*arriba j+2 */
						pixel_g7-> green = pix (i-2, j-3); /* arriba j+2*/
						pixel_h7-> green = pix (i-3, j-2); /*arriba j+2 */
						pixel_a7-> blue = pix (i+2, j+3); /* arriba j+2*/
						pixel_b7-> blue = pix (i+3, j+2); /*arriba j+2 */
						pixel_c7-> blue = pix (i-2, j+3); /* arriba j+2*/
						pixel_d7-> blue = pix (i-3, j+2); /*arriba j+2 */
						pixel_e7-> blue = pix (i+2, j-3); /* arriba j+2*/
						pixel_f7-> blue = pix (i+3, j-2); /*arriba j+2 */
						pixel_g7-> blue = pix (i-2, j-3); /* arriba j+2*/
						pixel_h7-> blue = pix (i-3, j-2); /*arriba j+2 */

						pixel_C->red = r;  /* Central */
						pixel_D->red = r;  /* Derecha */
						pixel_I->red = r;  /* Izquierda */ 
						pixel_A->red = r; /* Arriba */  
						pixel_a->red = r; /* abajo */
						pixel_aD->red = r; /* abajo-Derecha */
						pixel_AD->red = r; /* Arriba-Derecha */
						pixel_AI->red = r; /* Arriba-Izquierda */
						pixel_aI->red = r; /* abajo-Izquierda */
						pixel_a4->red = r; /*arriba j+2 */
						pixel_b4->red = r; /*arriba j+2*/
						pixel_c4->red = r; /* arriba j+2*/
						pixel_d4->red = r; /*arriba j+2 */
						pixel_e4->red = r; /*arriba j+2*/
						pixel_f4->red = r; /* arriba j+2*/
						pixel_g4->red = r; /*arriba j+2 */
						pixel_h4->red = r; /*arriba j+2*/
						pixel_i4->red = r; /* arriba j+2*/
						pixel_j4->red = r; /*arriba j+2 */
						pixel_k4->red = r; /*arriba j+2*/
						pixel_l4->red = r; /* arriba j+2*/
						pixel_a5->red = r; /* arriba j+2*/
						pixel_b5->red = r; /*arriba j+2 */
						pixel_c5->red = r; /*arriba j+2*/
						pixel_d5->red = r; /* arriba j+2*/
						pixel_a6-> red = r; /* arriba j+2*/
						pixel_b6-> red = r; /*arriba j+2 */
						pixel_c6-> red = r; /*arriba j+2*/
						pixel_d6-> red = r; /* arriba j+2*/
						pixel_e6-> red = r; /*arriba j+2 */
						pixel_f6-> red = r; /*arriba j+2*/
						pixel_g6-> red = r; /* arriba j+2*/
						pixel_h6-> red = r; /*arriba j+2 */
						pixel_i6-> red = r; /*arriba j+2*/
						pixel_j6-> red = r; /* arriba j+2*/
						pixel_k6-> red = r; /*arriba j+2 */
						pixel_l6-> red = r; /*arriba j+2*/
						pixel_a7-> red = r; /* arriba j+2*/
						pixel_b7-> red = r; /*arriba j+2 */
						pixel_c7-> red = r; /* arriba j+2*/
						pixel_d7-> red = r; /*arriba j+2 */
						pixel_e7-> red = r; /* arriba j+2*/
						pixel_f7-> red = r; /*arriba j+2 */
						pixel_g7-> red = r; /* arriba j+2*/
						pixel_h7-> red = r; /*arriba j+2 */
						pixel_C->green = g;  /* Central */
						pixel_D->green = g; /* Derecha */
						pixel_I->green = g;  /* Izquierda */
						pixel_A->green = g; /* Arriba */
						pixel_a->green = g; /* abajo */
						pixel_aD->green = g; /* abajo-Derecha */
						pixel_AD->green = g; /* Arriba-Derecha */
						pixel_AI->green = g; /* Arriba-Izquierda */
						pixel_aI->green = g; /* abajo-Izquierda */
						pixel_a4->green = g; /*arriba j+2 */
						pixel_b4->green = g; /*arriba j+2*/
						pixel_c4->green = g; /* arriba j+2*/
						pixel_d4->green = g; /*arriba j+2 */
						pixel_e4->green = g; /*arriba j+2*/
						pixel_f4->green = g; /* arriba j+2*/
						pixel_g4->green = g; /*arriba j+2 */
						pixel_h4->green = g; /*arriba j+2*/
						pixel_i4->green = g; /* arriba j+2*/
						pixel_j4->green = g; /*arriba j+2 */
						pixel_k4->green = g; /*arriba j+2*/
						pixel_l4->green = g; /* arriba j+2*/
						pixel_a5->green = g; /* arriba j+2*/
						pixel_b5->green = g; /*arriba j+2 */
						pixel_c5->green = g; /*arriba j+2*/
						pixel_d5->green = g; /* arriba j+2*/
						pixel_a6-> green = g; /* arriba j+2*/
						pixel_b6-> green = g; /*arriba j+2 */
						pixel_c6-> green = g; /*arriba j+2*/
						pixel_d6-> green = g; /* arriba j+2*/
						pixel_e6-> green = g; /*arriba j+2 */
						pixel_f6-> green = g; /*arriba j+2*/
						pixel_g6-> green = g; /* arriba j+2*/
						pixel_h6-> green = g; /*arriba j+2 */
						pixel_i6-> green = g; /*arriba j+2*/
						pixel_j6-> green = g; /* arriba j+2*/
						pixel_k6-> green = g; /*arriba j+2 */
						pixel_l6-> green = g; /*arriba j+2*/
						pixel_a7-> green = g; /* arriba j+2*/
						pixel_b7-> green = g; /*arriba j+2 */
						pixel_c7-> green = g; /* arriba j+2*/
						pixel_d7-> green = g; /*arriba j+2 */
						pixel_e7-> green = g; /* arriba j+2*/
						pixel_f7-> green = g; /*arriba j+2 */
						pixel_g7-> green = g; /* arriba j+2*/
						pixel_h7-> green = g; /*arriba j+2 */
						pixel_C->blue = b;  /* Central */
						pixel_D->blue = b; /* Derecha */
						pixel_I->blue = b; /* Izquierda */
						pixel_A->blue = b; /* Arriba */
						pixel_a->blue = b; /* abajo */
						pixel_aD->blue = b; /* abajo-Derecha */
						pixel_AD->blue = b; /* Arriba-Derecha */
						pixel_AI->blue = b; /* Arriba-Izquierda */
						pixel_aI->blue = b; /* abajo-Izquierda */
						pixel_a4->blue = b; /*arriba j+2 */
						pixel_b4->blue = b; /*arriba j+2*/
						pixel_c4->blue = b; /* arriba j+2*/
						pixel_d4->blue = b; /*arriba j+2 */
						pixel_e4->blue = b; /*arriba j+2*/
						pixel_f4->blue = b; /* arriba j+2*/
						pixel_g4->blue = b; /*arriba j+2 */
						pixel_h4->blue = b; /*arriba j+2*/
						pixel_i4->blue = b; /* arriba j+2*/
						pixel_j4->blue = b; /*arriba j+2 */
						pixel_k4->blue = b; /*arriba j+2*/
						pixel_l4->blue = b; /* arriba j+2*/
						pixel_a5->blue = b; /* arriba j+2*/
						pixel_b5->blue = b; /*arriba j+2 */
						pixel_c5->blue = b; /*arriba j+2*/
						pixel_d5->blue = b; /* arriba j+2*/
						pixel_a6-> blue = b; /* arriba j+2*/
						pixel_b6-> blue = b; /*arriba j+2 */
						pixel_c6-> blue = b; /*arriba j+2*/
						pixel_d6-> blue = b; /* arriba j+2*/
						pixel_e6-> blue = b; /*arriba j+2 */
						pixel_f6-> blue = b; /*arriba j+2*/
						pixel_g6-> blue = b; /* arriba j+2*/
						pixel_h6-> blue = b; /*arriba j+2 */
						pixel_i6-> blue = b; /*arriba j+2*/
						pixel_j6-> blue = b; /* arriba j+2*/
						pixel_k6-> blue = b; /*arriba j+2 */
						pixel_l6-> blue = b; /*arriba j+2*/
						pixel_a7-> blue = b; /* arriba j+2*/
						pixel_b7-> blue = b; /*arriba j+2 */
						pixel_c7-> blue = b; /* arriba j+2*/
						pixel_d7-> blue = b; /*arriba j+2 */
						pixel_e7-> blue = b; /* arriba j+2*/
						pixel_f7-> blue = b; /*arriba j+2 */
						pixel_g7-> blue = b; /* arriba j+2*/
						pixel_h7-> blue = b; /*arriba j+2 */
								break;

							default:	
						pixel_C->red = r;  /* Central */
						pixel_C->green = g;  /* Central */
						pixel_C->blue = b;  /* Central */
								break;
						}				/* -----  end switch  ----- */

						/* Termina de dibujar los puntos */

					}  /* *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
			int tra;
			
				tra = atoi(trans);
				
		    	save_png_to_file (& fruit, "wattie.png", tra);
		   // 	save_png_to_file (& fruit, tra);
				free(fruit.pixels);
				/* Termina la función dibuja */
			//http://books.google.com.mx/books?id=gbjIzwE2NYkC&pg=PA177&lpg=PA177&dq=PQgetvalue+integer&source=bl&ots=HbeiW5vMUS&sig=UqLioZuMs7dhoPfsPUr55EvdLmM&hl=es-419&sa=X&ei=f_lsVOSmFMaiyATGqoIQ&redir_esc=y#v=onepage&q=PQgetvalue%20integer&f=false //Página 183
			PQclear(res);
		}
	}
	PQfinish(conn);
	/* Termina Postgres */
	return 0;
}

static pixel_t * pixel_at (bitmap_t * bitmap, int x, int y)
{
     return bitmap->pixels + bitmap->height * y + x;
}

/* Para probar salida a buffer */

/* Cierro salida a buffer */


/* Escribir "bitmap" al PNG file specificado por "path"; retorna 0 en exito, diferente de cero en error. */

static int save_png_to_file (bitmap_t *bitmap,const char *path, int tr)
//static int save_png_to_file (bitmap_t *bitmap, int tr)
{
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
    /* "status" contiene el valor de retorno de esta función. La primera esta configurada al valor cual significa 'failure'. Cuando el routine hs rk, it is set to a value which means 'success'. */
    int status = -1;
    /* El siguiente número se configuró por intento y error, no se puede ver en el manual de libpng que onda con esto. */
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

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    } 
    /* Configura los atributos de la imagen */

    png_set_IHDR (png_ptr,
                  info_ptr,
                  bitmap->width,
                  bitmap->height,
                  depth,
                  PNG_COLOR_TYPE_RGBA,   /* http://stackoverflow.com/questions/13911126/how-to-let-png-have-the-transparent-property */
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);

    /* Aquí se inicailizan las filas en el PNG. */

    row_pointers = png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
    for (y = 0; y < bitmap->height; ++y) {
        png_byte *row = png_malloc (png_ptr, sizeof (uint8_t) * bitmap->width * 4);
        row_pointers[y] = row;
		
        for (x = 0; x < bitmap->width; ++x) {		
			int hdata = 0;
//			int tr;
//			tr = tra;   /* variable de transparencia. 0 es totalmente transparente y 255 es negro */
            pixel_t * pixel = pixel_at (bitmap, x, y);
            *row++ = pixel->red;
			hdata += pixel->red;
            *row++ = pixel->green;
			hdata += pixel-> green;
            *row++ = pixel->blue;
			hdata += pixel-> blue;
			if(hdata==0){
		    	*row++ = tr; /* Para controlar la transparencia. Va de 0 a 255 */
			}else{
				*row++ = 255; /* Con esto puede controlarse la transparencia de las bolitas. Pero aun no he puesto la variable, tal vez no se necesita.  */
			}
        }
    }
    
    /* Escribe la imagen data al "fp". */


    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);

	/*  static */
    struct mem_encode state; /* Funcion de bash */
    /*  initialise - put this before png_write_png() call */
    state.buffer = NULL; /* funciónde bash */
    state.size = 0; /* Funciónde bash */



    png_set_write_fn(png_ptr, &state, my_png_write_data, my_png_flush);  /* Función de bash */
	png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	  
    /* Con lo anterior se ha escrito exitosamente la routina. "status" indica el exito success. */
/*  cleanup */

 	 status = 0;

/* Para salvar en bash */

   if(state.buffer) /* Función de bash */

	   fwrite(state.buffer, state.size, 1, stdout);


     free(state.buffer); /* Función de bash */	  

/*  if my_png_flush() is not needed, change the arg to NULL */

//    png_init_io (png_ptr, fp);
//    png_set_rows (png_ptr, info_ptr, row_pointers);
//    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
//... call png_write_png() ...
	
	/*  now state.buffer contains the PNG image of size s.size bytes */
   
/* Fin Para salvar en bash*/ 	

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

/* Dado el "value" y "max", el máximo valo el cual esperamos "value" para tomar, este retorna un entero entre 0 y 255. */

static int pix (int value, int max)
{
    if (value < 0)
        return 0;
        return (int) 255.0;
}


void usage(void)
{
	printf("Usage:\n");
	printf(" -q<query>\n");
	printf(" -x<x_min>\n");
	printf(" -X<X_max>\n");
	printf(" -y<y_min>\n");
	printf(" -Y<Y_max>\n");
	printf(" -l<large>\n");
	printf(" -w<with>\n");
	printf(" -P<Port>\n");
	printf(" -i<ip>\n");
	printf(" -u<usser>\n");
	printf(" -p<password>\n");
	printf(" -d<database>\n");
	printf(" -r<color red>\n");
	printf(" -g<color green>\n");
	printf(" -b<color blue>\n");
	printf(" -t<transparency>\n");
	printf(" -R<radio>\n");
	printf(" -h<help>\n");
	exit (8);
}

/* Para salvar en bash */


void my_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
	  /*  with libpng15 next line causes pointer deference error; use libpng12 */
     struct mem_encode* p=(struct mem_encode*)png_get_io_ptr(png_ptr); /*  was png_ptr->io_ptr */
	  size_t nsize = p->size + length;
	 
	   /*  allocate or grow buffer */
	   if(p->buffer)
		      p->buffer = realloc(p->buffer, nsize);
	    else
		       p->buffer = malloc(nsize);
	
	  if(!p->buffer)
	      png_error(png_ptr, "Write Error");

  /*  copy new bytes to end of buffer */
	   memcpy(p->buffer + p->size, data, length);
	   p->size += length;


}

/*  This is optional but included to show how png_set_write_fn() is called */
void my_png_flush(png_structp png_ptr)
{
}

/* Fin para salvar en bash */
