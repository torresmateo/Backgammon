#include "board.h"
#include "libdado.h"
void board_update(char board[],checker **needs_update);
void print_dices(BITMAP *bmp_dice_1, BITMAP *bmp_dice_2);


void board_init(char board[])
{
	
	/*
		inicializa el tablero para una partida nueva
	*/
	board[25] = EMPTY;
	board[24] = 2*WHITE;
	board[23] = EMPTY;//WHITE;
	board[22] = EMPTY;
	board[21] = EMPTY;
	board[20] = EMPTY;
	board[19] = 5*BLACK;
	board[18] = EMPTY;
	board[17] = 3*BLACK;
	board[16] = EMPTY;
	board[15] = EMPTY;
	board[14] = EMPTY;
	board[13] = 5*WHITE;
	board[12] = 5*BLACK;
	board[11] = EMPTY;
	board[10] = EMPTY;
	board[ 9] = EMPTY;
	board[ 8] = 3*WHITE;
	board[ 7] = EMPTY;
	board[ 6] = 5*WHITE;
	board[ 5] = EMPTY;
	board[ 4] = EMPTY;
	board[ 3] = EMPTY;
	board[ 2] = EMPTY;
	board[ 1] = 2*BLACK;
	board[ 0] = EMPTY;

		
}



int get_pos_x( int i )
{//dado un indice de la pila en el vector, retorna su posicion x del bitmap
	if( i==0 || i==25 )
		return ( 712 );//posición x de las casas
		
	if( i==26 || i==27 )
		return ( 325 );//posición x de las barras
	
	if( 12 < i ) 
		i = 25-i;//si es mayor que 12 se utiliza el complemento de i
	
	//se calcula la coordenada x para posiciones antes o después de la barra
	if( 6 < i )
		return ( BORDER + 50*(12-i) );
	else
		return ( BORDER + 50*(13-i) );
	
}


int get_stack(int x, int y)
{//dadas coordenadas en la pantalla, retorna el indice de la pila en que se encuentra
	if( y < 25 || 575 <= y || x < 25 || 775 <= x || ( 675 <= x && x < 700 ) )
		return ( -1 );//si x,y no corresponde ni al tablero ni a las casas se retorna -1
	
	if( y < 300 ){//parte superior del tablero
		
		if( 675 <= x )
			return ( BLACK_HOME );//corresponde a la casa negra
		
		if( 325 <= x && x < 375 )
			return ( WHITE_BAR );//corresponde a la barra blanca
		
		//se calcula el indice
		if( x < 325 )
			return ( 13 + (x-BORDER)/50 );
		else
			return ( 12 + (x-BORDER)/50 );
		
	}else{//parte inferior del tablero
		
		if( 675 <= x )
			return ( WHITE_HOME );//corresponde a la casa blanca
		
		if( 325 <= x && x < 375 )
			return ( BLACK_BAR );//corresponde a la barra negra
		
		//se calcula el indice
		if( x < 325 )
			return ( 12 - (x-BORDER)/50 );
		else
			return ( 13 - (x-BORDER)/50 );
		
	}
}

void update_bar_y(void)
{//actualiza la posicion vertical de los objetos que esten en la barra
	int i,x=0;
	if(turn == WHITE){//si el turno es blanco hay fichas negras que van a la barra
		for(i=0;i<15;i++)
			if(black_checker[i]->get_stack() == BLACK_BAR)
				black_checker[i]->set_y(550 - BORDER - x++*25);//dependiendo del numero de fichas de la barra se van actualizando las posiciones desde la parte inferior del bitmap
	}else{ //si el turno es negrio hay fichas blancas que van a la barra
		for(i=0;i<15;i++)
			if(white_checker[i]->get_stack() == WHITE_BAR)
				white_checker[i]->set_y(BORDER + x++*25);//dependiendo del numero de fichas de la barra se van actualizando las posiciones desde la parte superior del bitmap
	}
}

void send_to_bar(int stack)
{//actualiza la posicion de los objetos en la barra
	int i;
	if(turn == WHITE){//si el turno es para el jugador que usa las fichas blancas
		for(i=0;i<15;i++){
			//se busca la ficha que se tiene que mandar a la barra y se actualiza su posicion
			if(black_checker[i]->get_stack() == stack){
				black_checker[i]->set_stack(BLACK_BAR);
				black_checker[i]->set_x(325);
				update_bar_y();
			}
		}
	}else{//si el turno es para el jugador que usa las fichas negras
		for(i=0;i<15;i++){
		//se busca la ficha que se tiene que mandar a la barra y se actualiza su posicion
			if(white_checker[i]->get_stack() == stack){
				white_checker[i]->set_stack(WHITE_BAR);
				white_checker[i]->set_x(325);
				update_bar_y();
			}
		}
	}
}

int get_dest(int dice, int src, char board[],int turn)
{//recibiendo un valor de dado y un origen, retorna el destino correspondiente (considerando la barra y los rebotes)
	if(turn == WHITE){//si el turno es blanco
		if(src == WHITE_BAR){//si el origen es la barra
			return 25 - abs(dice);//se retorna el valor del destino (las fichas blancas van de la posicion 25 a la 0)
		}
		if(src - abs(dice) < 0){//hay rebote
			return abs(src - abs(dice));
		}else{//no hay rebote
			return src - abs(dice);
		}
	}else{
		if(src == BLACK_BAR){//si el origen es la barra
			
			return abs(dice);//se retorna el valor del destino (las fichas blancas van de la posicion 0 a la 25)
		}
		if(src + abs(dice) > 25){//hay rebote
			return (25 - (  src + abs(dice) - 25 ));
		}else{//no hay rebote
			return (src + abs(dice));
		}
	}
}


int valid_move(int dice, int src, char board[], char bar[],int turn)
{//true si es una jugada valida, NOT_A_VALID_MOVE en caso contrario
	if(turn == WHITE){//turno del jugador con fichas blancas
		if(bar[0]){//solo puede jugarse desde la barra
			if(src != WHITE_BAR)//si el origen desdeado no es la barra la jugada no puede realizarse
				return NOT_A_VALID_MOVE;
			else//sino... se cambia el valor a la posicion de partida del jugador blanco
				src = 25;
			
			
			if(board[src - abs(dice)] >= BLACK){//si en el destino hay una ficha negra o vacio.. o n fichas blancas, es valido
				return abs(src - abs(dice));
			}else{//no se puede mover al destino porque hay mas de una ficha negra
				return NOT_A_VALID_MOVE;
			}
		
		}
		if(src!=WHITE_HOME && board[src]>=WHITE){//si en el origen hay al menos una ficha blanca
			if(src - abs(dice) < 0){//hay rebote
				if(board[abs(src - abs(dice))] >= BLACK){//si en el destino hay una ficha negra o vacio.. o n fichas blancas, es valido
					return abs(src - abs(dice));
				}else{//no se puede mover al destino porque hay mas de una ficha negra
					return NOT_A_VALID_MOVE;
				}
			}else{
				if(board[src - abs(dice)] >= BLACK){//si en el destino hay una ficha negra o vacio.. o n fichas blancas, es valido
					return abs(src - abs(dice));
				}else{//no se puede mover al destino porque hay mas de una ficha negra
					return NOT_A_VALID_MOVE;
				}
			}
		}else//no puede realizarse la jugada porque no hay fichas del turno correspondiente en la pila seleccionada
			return NOT_A_VALID_MOVE;
	}else{//turno del jugador con fichas negras
	
		if(bar[1]){//solo puede jugarse desde la barra
			if(src != BLACK_BAR)//si el origen desdeado no es la barra la jugada no puede realizarse
				return NOT_A_VALID_MOVE;
			else//sino... se cambia el valor a la posicion de partida del jugador negro
				src = 0;
				
			if(board[src + abs(dice)] <= WHITE){//si en el destino hay una ficha blanca o vacio.. o n fichas negras, es valido
				return src + abs(dice);
			}else{//no se puede mover al destino porque hay mas de una ficha blanca
				return NOT_A_VALID_MOVE;
			}
		
		}
		if(src!=BLACK_HOME && board[src]<=BLACK){//si en el origen hay al menos una ficha negra
			if(src + abs(dice) > 25){//hay rebote
				if(board[ 25 - (  src + abs(dice) - 25 ) ] <= WHITE){//si en el destino hay una ficha blanca o vacio.. o n fichas negras, es valido
					return 25 - (  src + abs(dice) - 25 );
				}else{//no se puede mover al destino porque hay mas de una ficha blanca
					return NOT_A_VALID_MOVE;
				}
			}else{
				if(board[src + abs(dice)] <= WHITE){//si en el destino hay una ficha blanca o vacio.. o n fichas negras, es valido
					return src + abs(dice);
				}else{//no se puede mover al destino porque hay mas de una ficha blanca
					return NOT_A_VALID_MOVE;
				}
			}
		}else//no puede realizarse la jugada porque no hay fichas del turno correspondiente en la pila seleccionada
			return NOT_A_VALID_MOVE;
	}
}

void insert_checker(char board[], char bar[], checker *mychecker, int *dice, BITMAP *bmp_buffer, BITMAP *bmp_board)
{//mueve las fichas tanto en la pantalla como en el tablero, define que columnas deben actualizar sus posiciones para reimprimirse
	int src_stack = mychecker->get_stack();
	int dest_stack = get_dest(*dice,src_stack,board,turn);
	
	if(turn == WHITE){//el turno es del jugador blanco
		if(!bar[0]){//la barra esta vacia para el jugador blanco
			if(board[dest_stack]==BLACK){//si hay una ficha negra... esta debe ir a la barra
				send_to_bar(dest_stack);//se manda la ficha negra a la barra desde su posicion actual
				board[dest_stack]+=WHITE;//se agrega la ficha blanca a la posicion donde estaba la ficha que fue a la barra
				bar[1]+=BLACK;//se agrega una ficha negra a la representacion lógica de la barra
			}
			//actualizacion de dados
			/*
				dependiendo de si es o no soble turno, cada dado va vambiando de valor
				exite un define USED, que indica que el dado no esta disponible para volver a usarse, 
				en caso de que el turno sea simple, este valor se asigna directamente al usar el dado, 
				en caso de turno doble, antes de pasar a USED, se convierte el valor del dado a su valor negativo
				esto se hace así, porque esa cualidad de los enteros es útil en una sitiación como ésta, en que debemos
				guardar la información de que se utilizó una vez, antes de pasar a USED
			
				en cada caso el ultimo dado utilizado se rescata con la variable last_dice, para cargar la jugada en el orden correcto
			*/
			if(double_turn){//si es doble turno
				if(*dice<0){//si el dado es negativo se pone en used
					last_dice = abs(*dice);//se captura el dado utilizado
					*dice = USED;//se deja inhabilitado para más usos
				}else{//el dado se utiliza por primera vez
					last_dice = abs(*dice);//se captura el dado utilizado
					*dice *= -1;//se cambia el valor del dado a su valor negativo(se puede utilizar una vez más)
				}
			}else{//el turno es simple (cada dado se usa una sola vez)
				last_dice = abs(*dice);//se captura el dado utilizado
				*dice = USED;//se inhabilita el dado para usos posteriores
			}
				
			board[mychecker->get_stack()]-=WHITE;//se resta un blanco del origen(se suma despues al destino)
			pos_update_src = mychecker->get_stack();//se guarda el origen de la jugada (con dos fines, uno es el de mandar la jugada al visor, y el otro el de actualizar la posicion de origen en la pantalla)
				
			mychecker->set_stack(dest_stack);//se cambia el valor de posicion logica del objeto (ficha)
				
			board[mychecker->get_stack()]+=WHITE;//al destino se suma una ficha blanca
			pos_update_dest = mychecker->get_stack();//se guarda la posicion de destino, con el fin de actualizar las posiciones donde se imprimen los objetos
				
			mychecker->set_x(get_pos_x(mychecker->get_stack()));//se cambia la posicion de impresion horizontal del objeto
			update = true;//se le indica al programa que el tablero de la pantalla debe actualizarse
				
		}else{//solo se pueden mover fichas que estan en la barra
				/*identico al bloque anterior del if, estan comentadas las diferencias*/
			if(board[dest_stack]==BLACK){
				send_to_bar(dest_stack);
				board[dest_stack]+=WHITE;
				bar[1]+=BLACK;
			}
			
			if(double_turn){
				if(*dice<0){
					last_dice = abs(*dice);
					*dice = USED;
				}else{
					last_dice = abs(*dice);
					*dice *= -1;
				}
			}else{
				last_dice = abs(*dice);
				*dice = USED;
			}
			
			bar[0]-=WHITE;//en lugar de remover una ficha de la posicion de origen, se saca la ficha de la barra
			pos_update_src = mychecker->get_stack();
			
			mychecker->set_stack(dest_stack);
			
			board[mychecker->get_stack()]+=WHITE;
			pos_update_dest = mychecker->get_stack();
			
			mychecker->set_x(get_pos_x(mychecker->get_stack()));
			update = true;
		}
		
	}else{//se verifica para el jugador con turno BLACK
	
		/*analogo a los dos bloques anteriores al else, se intercambian los defines BLACK y WHITE */
		
		if(!bar[1]){//la barra esta vacia para el jugador negro
			if(board[dest_stack]==WHITE){//si hay una ficha blanca... esta debe ir a la barra
				send_to_bar(dest_stack);
				board[dest_stack]+=BLACK;
				bar[0]+=WHITE;
			}
		
			if(double_turn){
				if(*dice<0){
					last_dice = abs(*dice);
					*dice = USED;
				}else{
					last_dice = abs(*dice);
					*dice *= -1;
				}
			}else{
				last_dice = abs(*dice);
				*dice = USED;
			}
				
			board[mychecker->get_stack()]-=BLACK;
			pos_update_src = mychecker->get_stack();
				
			mychecker->set_stack(dest_stack);
				
			board[mychecker->get_stack()]+=BLACK;
			pos_update_dest = mychecker->get_stack();
				
			mychecker->set_x(get_pos_x(mychecker->get_stack()));
			update = true;
				
		}else{//solo se pueden mover fichas que estan en la barra
				
			if(board[dest_stack]==WHITE){//si hay una ficha blanca... esta debe ir a la barra
				send_to_bar(dest_stack);
				board[dest_stack]+=BLACK;
				bar[0]+=WHITE;
			}
			
			if(double_turn){
				if(*dice<0){
					last_dice = abs(*dice);
					*dice = USED;
				}else{
					last_dice = abs(*dice);
					*dice *= -1;
				}
			}else{
				last_dice = abs(*dice);
				*dice = USED;
			}
			
			bar[1]-=BLACK;
			pos_update_src = mychecker->get_stack();
			
			mychecker->set_stack(dest_stack);
			
			board[mychecker->get_stack()]+=BLACK;
			pos_update_dest = mychecker->get_stack();
			
			mychecker->set_x(get_pos_x(mychecker->get_stack()));
			update = true;
		}
	}
	
}

void dice_selector( BITMAP *bmp_buffer, BITMAP *bmp_board )
{//imprime el menu de selección del dado a utilizarse en case de ser una jugada ambigua
	
	BITMAP *bmp_cuadro = load_bitmap("images/cuadro.bmp", NULL);//se carga la imagen base del menu
	//variables donde se cargan las imagenes de los dados
	BITMAP *bmp_dice_1 = NULL;
	BITMAP *bmp_dice_2 = NULL;
	
	if( turn == WHITE ){//se cargan las imagenes de los dados para el turno blanco
		switch ( abs(dice_1) ){
			case 1: bmp_dice_1 = load_bitmap("images/w_dice_1.bmp", NULL);break;
			case 2: bmp_dice_1 = load_bitmap("images/w_dice_2.bmp", NULL);break;
			case 3: bmp_dice_1 = load_bitmap("images/w_dice_3.bmp", NULL);break;
			case 4: bmp_dice_1 = load_bitmap("images/w_dice_4.bmp", NULL);break;
			case 5: bmp_dice_1 = load_bitmap("images/w_dice_5.bmp", NULL);break;
			case 6: bmp_dice_1 = load_bitmap("images/w_dice_6.bmp", NULL);break;
		}
		switch ( abs(dice_2) ){
			case 1: bmp_dice_2 = load_bitmap("images/w_dice_1.bmp", NULL);break;
			case 2: bmp_dice_2 = load_bitmap("images/w_dice_2.bmp", NULL);break;
			case 3: bmp_dice_2 = load_bitmap("images/w_dice_3.bmp", NULL);break;
			case 4: bmp_dice_2 = load_bitmap("images/w_dice_4.bmp", NULL);break;
			case 5: bmp_dice_2 = load_bitmap("images/w_dice_5.bmp", NULL);break;
			case 6: bmp_dice_2 = load_bitmap("images/w_dice_6.bmp", NULL);break;
		}
	}else{//se cargan las imagenes de los dados para el turno negro
		switch ( abs(dice_1) ){
			case 1: bmp_dice_1 = load_bitmap("images/b_dice_1.bmp", NULL);break;
			case 2: bmp_dice_1 = load_bitmap("images/b_dice_2.bmp", NULL);break;
			case 3: bmp_dice_1 = load_bitmap("images/b_dice_3.bmp", NULL);break;
			case 4: bmp_dice_1 = load_bitmap("images/b_dice_4.bmp", NULL);break;
			case 5: bmp_dice_1 = load_bitmap("images/b_dice_5.bmp", NULL);break;
			case 6: bmp_dice_1 = load_bitmap("images/b_dice_6.bmp", NULL);break;
		}
		switch ( abs(dice_2) ){
			case 1: bmp_dice_2 = load_bitmap("images/b_dice_1.bmp", NULL);break;
			case 2: bmp_dice_2 = load_bitmap("images/b_dice_2.bmp", NULL);break;
			case 3: bmp_dice_2 = load_bitmap("images/b_dice_3.bmp", NULL);break;
			case 4: bmp_dice_2 = load_bitmap("images/b_dice_4.bmp", NULL);break;
			case 5: bmp_dice_2 = load_bitmap("images/b_dice_5.bmp", NULL);break;
			case 6: bmp_dice_2 = load_bitmap("images/b_dice_6.bmp", NULL);break;
		}
	}
	
	draw_sprite(bmp_buffer, bmp_cuadro, (screen->w)/2 - (bmp_cuadro->w)/2, (screen->h)/2 - (bmp_cuadro->h)/2);//imprime el cuadro 
	//imprime los dados que son las opciones en el caso de ambigüedad
	draw_sprite(bmp_buffer, bmp_dice_1, 325, 297);
	draw_sprite(bmp_buffer, bmp_dice_2, 430, 297);
	//se liberan los bitmap de la memoria
	destroy_bitmap (bmp_dice_1);
	destroy_bitmap (bmp_dice_2);
	destroy_bitmap (bmp_cuadro);
}
	

int is_ambiguous( char board[], char bar[], int src_stack )
{//determina si la jugada que se quiere realizar tiene mas de una forma de realizarse
	/*
		retorna TRUE en caso de que dado un origen, pueda tener el mismo destino con los dos dados, creando así 
		una instrucción ambigua
		
		en caso contrario no existe ambigüedad alguna y se retorna FALSE
		
		en caso de que los dados sean iguales, se retorna false, ya que siempre tendrán destinos compartidos sin
		que importe el origen
	*/
	if( abs(dice_1) != abs(dice_2)  &&  get_dest(dice_1, src_stack, board, turn) == get_dest(dice_2, src_stack, board, turn) )
		return TRUE;
	else
		return FALSE;
	
	
}


int insert_checker_player( char board[], char bar[], checker *mychecker, int x, int y, BITMAP *bmp_buffer, BITMAP *bmp_board )
{//funciona de intermediario para el input de los jugadores humanos, ya que en input del mouse puede llevar a instrucciones ammbiguas
	
	int dest_stack = get_stack(x,y);//pila destino
	int src_stack = mychecker->get_stack();//pila origen
	
	if( dest_stack == -1 )//indica que el origen no forma parte del tablero del juego (el release del mouse fue sobre alguno de los bordes, o fuera de la ventana)
		return FALSE;
	//bitmaps para imprimir dados (necesarios para reimprimir el tablero en caso de que haya ambigüedad)
	BITMAP *bmp_dice_1 = NULL;
	BITMAP *bmp_dice_2 = NULL;
	if( is_ambiguous( board, bar, src_stack ) ){//si la instruccion que se recibe es ambigua
		if( get_dest(dice_1, src_stack, board, turn)==dest_stack && (valid_move( dice_1, src_stack, board, bar, turn )!= NOT_A_VALID_MOVE) ){//si ademas de ambigua, la jugada es válida
			//debemos dar al usuario la opcion de elegir que dado utilizar para su movida, por lo tanto, se imprime el menu de seleccion de dado
			dice_selector( bmp_buffer, bmp_board );//menu de seleccion
			board_update(board,NULL);
			while( !key[KEY_A] && !key[KEY_B] );//se espera el input de seleccion
			
			//una vez que se haya seleccionado un dado, se reimprime el tablero sin el menu(se borra el menu de seleccion de dados)
			
			blit(bmp_board, bmp_buffer,0,0,0,0,bmp_board->w,bmp_board->h);
			print_dices(bmp_dice_1,bmp_dice_2);
			board_update(board,NULL);
			
			//luego de borrar el menu de seleccion, se procede a insertar la ficha seleccionada en el destino con el dado seleccionado
			if(key[KEY_A]){//se eligio el primer dado como instruccion correcta
				insert_checker(board, bar, mychecker, &dice_1, bmp_buffer, bmp_board);//insercion con el dado elegido (dado 1)
				laps--;
				return TRUE;
			}else if(key[KEY_B]){//se eligio el segundo dado como instruccion correcta
				insert_checker(board, bar, mychecker, &dice_2, bmp_buffer, bmp_board);//insercion con el dado elegido (dado 2)
				laps--;
				return TRUE;
			}
			return FALSE;
		}else{//en este caso, aunque la instrucción es ambigua, la jugada no es realizable, por lo que se devuelve false y la movida no se realiza (el tablero se queda como estaba antes de seleccionar la ficha)
			return FALSE;
		}
	}else{//la instrucción no es ambigua
		if( get_dest(dice_1, src_stack, board, turn)==dest_stack ){//si la instrucción la cumple el dado 1
			
			if( valid_move( dice_1, src_stack, board, bar, turn ) != NOT_A_VALID_MOVE){//si es una movida permitida
				insert_checker(board, bar, mychecker, &dice_1, bmp_buffer, bmp_board);//se inserta la ficha en el destino correspondiente
				laps--;//indicador de jugadas restantes se disminuye en uno
				return TRUE;
			}else{//no es un movida permitida, el tablero permanece como estaba antes de recibir la instrucción
				return FALSE;			
			}
			
		}else if( get_dest(dice_2, src_stack, board, turn)==dest_stack ){//si la instrucción la cumple el dado 2
			/*
				idéntico al bloque anterior, con la diferencia de que todas las instrucciones se refieren al dado 2
			*/
			if( valid_move( dice_2, src_stack, board, bar, turn ) != NOT_A_VALID_MOVE){
				insert_checker(board, bar, mychecker, &dice_2, bmp_buffer, bmp_board);
				laps--;
				return TRUE;
			}else{
				return FALSE;
			}
			
		}else{//no existe dado que cumpla con la instrucción por lo tanto no se realiza acción alguna y el tablero permanece como estaba antes de recibir la isntrucción
			return FALSE;
		}
	}
}

void pos_updater(int i, checker **list, char board[], BITMAP *buffer, BITMAP *board_bmp)
{//actualiza las posiciones de los objetos ficha... para su reimpresion en la pantalla
	int j;//iterador
	int x = 0;//indica el numero del objeto en la pila que debe obtener su posicion vertical
	if(i == 27){//se borra la barra negra en caso de ser necesario (no se realizan posteriores acciones ya que la barra adquire su posicion de manera distinta al resto del tablero)
		blit(board_bmp,buffer,325,300,325,300,50,275);
	}else if(i == 26){//barra blanca en caso de ser necesario(no se realizan posteriores acciones ya que la barra adquire su posicion de manera distinta al resto del tablero)
		blit(board_bmp,buffer,325,25,325,25,50,275);
	}else{//la posicion que debe actualizarse no es la barra, por lo que se realizan mas acciones ademas del borrado de la pila
		//se borra la pila
		blit(board_bmp,buffer,get_pos_x(i),(i<13)?((i == 0)?350:300):25,get_pos_x(i),(i<13)?((i == 0)?350:300):25,50,(i == 0 || i == 25)?((i == 0)?290:200):300);
		/*
			en caso de que hayan 5 fichas o menos, no hay solapamiento de fichas
			
			en el caso de más de 5 fichas, se calcula el solapamiento limitando la altura a la altura que ocupan 5 fichas sin solaparse
			
			EN EL CASO DE LAS CASAS, esto se reduce a la altura de 3 fichas
		*/
		if(abs(board[i])<=5){//fichas sin solapamiento (excepto por las casas)
			for(j=0;j<15 && x<abs(board[i]);j++){//se recorren las fichas hasta encontrar las que estan en la posicion que debe actualizarse
				if(list[j]->get_stack() == i){
					//se actualiza la posicion vertical de la ficha
					list[j]->set_y(i<13?(550-(BORDER + ((i==0 || i == 25)?120/abs(board[i])*x:50*x))):BORDER + ((i==0 || i == 25)?120/abs(board[i])*x:50*x));
					list[j]->unpaint(buffer, board_bmp);//se borra la ficha de donde esta actualmente(al actualizar el tablero se imprimen en el orden correcto)
					x++;//se indica que la siguiente posicion es mas arriba (o abajo), dependiendo del caso
				}
			}
		}else{
			for(j=0;j<15 && x<abs(board[i]);j++){//se recorren las fichas hasta encontrar las que estan en la posicion que debe actualizarse
				if(list[j]->get_stack() == i){
					//se actualiza la posicion vertical de la ficha
					list[j]->set_y(i<13?550-(BORDER + (((i==0 || i == 25)?120:230)/abs(board[i]))*x):BORDER + (((i==0 || i == 25)?120:230)/abs(board[i]))*x);
					list[j]->unpaint(buffer, board_bmp);//se borra la ficha de donde esta actualmente(al actualizar el tablero se imprimen en el orden correcto)
					x++;//se indica que la siguiente posicion es mas arriba (o abajo), dependiendo del caso
				}
			}
		}
		update = true;//se indica que el tablero necesita actualizarse (reimprimirse en pantalla)
	}
}


int is_dice_available( int dice, char board[], char bar[] ,int turn)
{//retorna true si hay jugadas habilitadas para un dado determinado
	int i;
	if(abs(dice)>=1 && abs(dice) <= 6){//si el valor del dado esta dentro del rango 1-6
		if( turn == BLACK ){//turno de las fichas negras
			if( bar[1] ){//si hay fichas en la barra
				if( board[WHITE_HOME+abs(dice)] <= 1 ){//si en la posición de destino hay una ficha blanca o menos (ninguna o fichas negras)
					return TRUE;//posible jugada
				}
			}else{//no hay fichas en la barra
				for( i=WHITE_HOME+1; i < BLACK_HOME ;i++ )//se recorre el tablero
					//si se encuentra una ficha negra   y   en la posición de destino hay una ficha blanca o menos (ninguna o fichas negras)
					if( board[i] < 0 && (( i+abs(dice) <= BLACK_HOME )?( board[i+abs(dice)] <= 1 ):( board[25-(i+abs(dice)-25)] <= 1 )) )
						return TRUE;//posible jugada
			}
		}else{//turno de las fichas blancas
			if( bar[0] ){//si hay fichas en la barra
				if( board[BLACK_HOME-abs(dice)] >= -1 ){//si en la posición de destino hay una ficha negra o mas (ninguna o fichas blancas)
					return TRUE;//posible jugada
				}
			}else{//no hay fichas en la barra
				for( i=WHITE_HOME+1; i < BLACK_HOME ;i++ )//se recorre el tablero
					//si se encuentra una ficha blanca   y   en la posición de destino hay una ficha negra o mas (ninguna o fichas blancas)
					if( board[i] > 0 && (( i-abs(dice) >= WHITE_HOME )?( board[i-abs(dice)] >= -1 ):( board[-i+abs(dice)] >= -1 )) ){
						return TRUE;//posible jugada
					}
			}
		}
	}
	return FALSE;
}


int security( int dice_1, int dice_2, DADO_JUGADA *move, char board[], char bar[] )
{//verifica que las jugadas recidivas del oponente sean válidas y coherentes
	int i;
	
	char board_copy[26];
	for( i=0; i<26 ;i++ )//se hace una copia del tablero 
		board_copy[i] = board[i];
	
	char bar_copy[2];
	bar_copy[0] = bar[0];
	bar_copy[1] = bar[1];
	
	int laps;//laps indica hasta cuantas movidas se pueden hacer
	int dice_1_ok, dice_2_ok;//dice_i_ok es igual a cero si el dado correspondiente ya se ha utilizado 
	if( dice_1==dice_2 ){//turno doble
		dice_1_ok = 4;//se jugará 4 veces con este dado
		dice_2_ok = FALSE;//no se jugará con este dado
		laps = 4;//hasta cuatro movidas posibles 
	}else{//turno simple
		dice_1_ok = 1;//se jugará una vez con este dado
		dice_2_ok = 1;//se jugará una vez con este dado
		laps = 2;//hasta cuatro movidas posibles 
	}
	
	int i_bar;//i_bar es el indice del vector "bar" al que le corresponde el turno 
	if( turn==WHITE )
		i_bar = 0;
	else
		i_bar = 1;
	
	for( i=0; i<laps ;i++ ){
		//si dado es nulo...
		if( move->dado[i]==-1 ){
			//si hay algun dado disponible y tiene alguna jugada valida... 
			if( ( dice_1_ok && is_dice_available(dice_1,board_copy,bar_copy,turn) ) || ( dice_2_ok && is_dice_available(dice_2,board_copy,bar_copy,turn) ) )
				return FALSE;//se retorna FALSE porque si podia jugar tenia que hacerlo
			else
				break;//se sale de for para iniciar la verificación de jugadas nulas 
			
		}else{
			//según sea el dado de la jugada, se le resta 1 a la variable que indica su uso
			if(move->dado[i] == dice_1){
				if( !dice_1_ok )
					return FALSE;//se retorna FALSE si el dado ya no está disponible
				dice_1_ok--;
			}else if(move->dado[i] == dice_2){
				if( !dice_2_ok )
					return FALSE;//se retorna FALSE si el dado ya no está disponible
				dice_2_ok--;
			}else{		
				return FALSE;//si no es igual a ninguno de los dados se retorna FALSE
			}
			//si hay fichas en la barra...
			if( bar_copy[i_bar] ){
				//si el origen no es la barra...
				if( move->origen[i]!=(i_bar+26) )
					return FALSE;
				//si no es una jugada posible...
				if( valid_move(move->dado[i],move->origen[i],board_copy,bar_copy,turn)==NOT_A_VALID_MOVE )
					return FALSE;
				//se actualiza la copia del tablero con la jugada analizada
				bar_copy[i_bar] -= turn;
				if( board_copy[get_dest(move->dado[i],move->origen[i],board_copy,turn)] == -turn )//si en el dstino hay una ficha del enemigo(se puede comer)
					board_copy[get_dest(move->dado[i],move->origen[i],board_copy,turn)] = turn;
				else
					board_copy[get_dest(move->dado[i],move->origen[i],board_copy,turn)] += turn;
				
			}else{
				//si el origen no está en el tablero...
				if( move->origen[i] < 0  ||  move->origen[i] > 24 )
					return FALSE;
				//si no es una jugada posibles...
				if( valid_move(move->dado[i],move->origen[i],board_copy,bar_copy,turn) == NOT_A_VALID_MOVE )
					return FALSE;
				//se actualiza la copia del tablero con la jugada analizada
				board_copy[(int)move->origen[i]] -= turn; 
				if( board_copy[get_dest(move->dado[i],move->origen[i],board_copy,turn)] == -turn )//si en el destino hay una ficha del enemigo(se puede comer)
					board_copy[get_dest(move->dado[i],move->origen[i],board_copy,turn)] = turn;
				else
					board_copy[get_dest(move->dado[i],move->origen[i],board_copy,turn)] += turn;				
			}
		}
		
	}
	//se analizan las jugadas restantes (las cuales deben ser nulas)
	for( ; i<4 ;i++ )
		if( move->dado[i]!=(-1) || move->origen[i]!=(-1) )
			return FALSE;//se retorna FALSE si se encuentra un dado o un origen distinto de -1
	
	return TRUE;//si el análisis llegó a este punto es porque se pasaron todas las "pruebas"
	
}

void board_update(char board[],checker **needs_update){//reimprime el tablero en pantalla y actualiza las fichas en caso de ser necesario
	if(pos_update_src>=0 || pos_update_dest>=0){//se actulizan la posicion origen y destino y se imprimen las jugadas en la pantalla
		pos_updater(pos_update_src, needs_update, board, bmp_buffer, bmp_board);//actualiza la posicion origen de la ultima jugada
		pos_updater(pos_update_dest, needs_update, board, bmp_buffer, bmp_board);//actualiza la posicion destino de la ultima jugada
	}
	if(update){//en  caso de que sea necesaria la reimpresion del tablero
		for(int i = 0;i<15;i++){//se reimprimen las fichas
			white_checker[i]->paint(bmp_buffer,white_checker[i]->get_paint_me());
			black_checker[i]->paint(bmp_buffer,black_checker[i]->get_paint_me());
		}
		
		show_mouse(NULL);//se oculta el mouse para imprimir
		blit(bmp_buffer, screen, 0, 0, 0, 0, 800, 600);//se reimprime la pantalla
		show_mouse(screen);//se muestra de nuevo el mouse
		update = false;//no es necesaria la actualizacion de la pantalla
	}
}
