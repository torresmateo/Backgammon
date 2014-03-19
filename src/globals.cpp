#include "main_header.h"

int auto_dices = 1;//indica si se usa o no la biblioteca de lanzamiento automatico de dados
int turn = WHITE;//indica el turno actual
int color_PC = EMPTY;//indica el color que le corresponde evaluar a la inteligencia artificial
int color_ENEMY = EMPTY;//indica el color que le corresponde evaluar a la inteligencia artificial
int game_style = PL_v_PC;//indica el color que le corresponde evaluar a la inteligencia artificial(la version para el jugador humano no esta implementada)
bool update = true; //booleano que indica si hay que actualizar o no la pantalla
bool insert = false;//booleano que indica si se realiza la insercion de una ficha en alguna pila
int EOG = 0;//indica quien es el ganador
int stack_update = -1;//indica que pila hay que repintar 

int pos_update_dest = -1; //indica que pila necesita recalcular las posiciones de las fichas (destino)
int pos_update_src = -1; //indica que pila necesita recalcular las posiciones de las fichas (origen)

int dice_1 = 3;//dado 1
int dice_2 = 3;//dado 2
int last_dice = 0;//valor del ultimo dado utilizado

int laps = 0;//movidas que le quedan al jugador este turno
bool double_turn = false;//booleano que indica si el turno es doble


BITMAP *bmp_buffer = NULL;//bmp_buffer de dibujo
BITMAP *bmp_board = NULL;//bmp del tablero

//declaramos las fichas
char bar[2]={0,0};//barra central... bar[0] corresponde al jugador con fichas blancas, bar[1] corresponde al jugador con fichas negras

checker *white_checker[15];//vector de fichas blancas
checker *black_checker[15];//vector de fichas negras
checker **pc_checker;//vector de fichas de la pc
checker **enemy_checker;//vector de fichas del programa oponente

int undo_bar;//variable que indica si se debe reponer una ficha de la barra (AI)



/*BORRAR DESPUES... VARIABLES AL PEDO*/

char board2[26];
char indices[26];

