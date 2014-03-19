#ifndef LIBDADO_H
#define LIBDADO_H


#ifndef LANZAMIENTO_H
#define LANZAMIENTO_H
typedef enum TURNO
{
	PRIMERO,
	SEGUNDO
}
TURNO;

typedef struct DADO_LANZAMIENTO
{
	TURNO	turno;
	int		numero[2];
}
DADO_LANZAMIENTO;

typedef struct DADO_JUGADA
{
	int		dado[4],
			origen[4];
}
DADO_JUGADA;
#endif // LANZAMIENTO_H


//Establece el turno
void establecer_turno(DADO_LANZAMIENTO *pl,int valTurno);




// Efectúa el lanzamiento de dos dados, y almacena los resultados en un vector con dos elementos.
int dado_lanzar( DADO_LANZAMIENTO *pLanzamiento);


// Comprueba si la ruta del dado es correcta.
// Retorna 1 si tiene éxito, 0 si falla.
int dado_establecer_ruta( const char *ruta_carpeta);


// Devuelve la ruta establecida para el dado.
// Retorna NULL si no se estableció.
// ¡ESTA CADENA ES DE SÓLO LECTURA!
const char* dado_ruta_establecida( void);


// Obtiene el último lanzamiento efectuado por los dados. Almacena un valor en cada elemento del arreglo.
int dado_ultimo_lanzamiento( DADO_LANZAMIENTO *pLanzamiento);


// obtiene la ultima jugada realizada
int dado_ultima_jugada( DADO_JUGADA *pJugada);


// Indica que terminó de jugar para hacer la comprobación del tiempo en el visor.
void dado_termine_de_jugar( const DADO_JUGADA *pJugada);


// Úsese para incializar los parámetros en una nueva partida.
void dado_nuevo_juego( void);


// Libera memoria reservada, entre otras cosas. Úselo cuando ya no necesite disponer de la ruta del dado.
void dado_finalizar( void);


#endif // LIBDADO_H
