#include <iostream>
#include <stdio.h>
#include <allegro.h>
#include "globals.cpp"
#include "main_header.h"
#include "board.c"
#include "ai.c"
#include "manual_dices.c"
#include "checker.cpp"
#include "libdado.h"


#define TRANSPARENT makecol(255,0,255)


//timer
volatile int exit_time = FALSE;//true = salir del programa

void close_button_handler(void)
{//se ejecuta cuando se aprieta el boton X en el marco de la ventana
	exit_time = TRUE;
}

void print_move(DADO_JUGADA *move, char board[],int turn);
void print_ERROR_move( DADO_JUGADA *move );


int main()
{
	
	
	//inicializar allegro
	allegro_init();
	
	//habilitar boton de cierre
	LOCK_FUNCTION(close_button_handler);
	set_close_button_callback(close_button_handler);
	
	//dispositivos entrada
	install_timer();
	install_keyboard();
	install_mouse();
	
	//configurar propiedades de la ventana de ejecucion
	set_color_depth(desktop_color_depth());
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0);
	clear_to_color(screen, makecol(255,255,255));
	set_display_switch_mode(SWITCH_BACKAMNESIA);
	//preparamos el tablero
	char board[26];//tablero
	char folder[600] = {0};
	int i,id=0;
	int *dice;
	//strcpy(folder, "c:\\");
	//se elige la ruta de la carpeta para libdado
	file_select_ex("Carpeta de dados:",folder,"+d" , 75, 700,500);
	
	if(!folder[0]){
		rectfill(screen,100,100,700,500,makecol(255,0,0));
		textprintf_ex(screen, font, 110, 210, makecol(255, 255, 255),-1, "LA RUTA NO FUE CARGADA... EL PROGRAMA NO PUEDE CONTINUAR");
		textprintf_ex(screen, font, 110, 250, makecol(255, 255, 255),-1, "PRESIONE UNA TECLA PARA CONTINUAR (CIERRA ESTA VENTANA)");
		while(!keypressed());
		exit(0);
	}
	
	//inicio de libdado
	if(auto_dices){
		dado_establecer_ruta(folder);
		dado_nuevo_juego();
	}
	DADO_JUGADA	my_moves,enemy_moves,pl_moves;
	DADO_LANZAMIENTO my_dices,enemy_dices,pl_dices;
	
	
	board_init(board);//preparamos la posicion inicial del tablero
	
	
	//cargamos las imagenes de las fichas y del menu de seleccion de turnos
	BITMAP *bmp_wc = load_bitmap("images/wc.bmp",NULL);
	BITMAP *bmp_wc_cursor = load_bitmap("images/wc_cursor.bmp",NULL);
	BITMAP *bmp_bc = load_bitmap("images/bc.bmp",NULL);
	BITMAP *bmp_bc_cursor = load_bitmap("images/bc_cursor.bmp",NULL);
	BITMAP *bmp_opt_menu = load_bitmap("images/opt_menu.bmp",NULL);
	BITMAP *bmp_no_moves = load_bitmap("images/no_moves.bmp",NULL);
	BITMAP *bmp_style_selection = load_bitmap("images/style_selection.bmp",NULL);
	BITMAP *bmp_winner_BLACK = load_bitmap("images/winner_BLACK.bmp",NULL);
	BITMAP *bmp_winner_WHITE = load_bitmap("images/winner_WHITE.bmp",NULL);
	BITMAP *bmp_eog_ERROR = load_bitmap("images/eog_ERROR.bmp",NULL);
	
	
	//asigmanos a cada vector de fichas las posiciones iniciales correspondientes junto con las imagenes
	white_checker[0]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,625 ,25  ,24);
	white_checker[1]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,625 ,75  ,24);
	white_checker[2]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,25  ,25  ,13);
	white_checker[3]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,25  ,75  ,13);
	white_checker[4]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,25  ,125 ,13);
	white_checker[5]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,25  ,175 ,13);
	white_checker[6]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,25  ,225 ,13);
	white_checker[7]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,225 ,525 ,8);
	white_checker[8]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,225 ,475 ,8);
	white_checker[9]  = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,225 ,425 ,8);
	white_checker[10] = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,375 ,525 ,6);
	white_checker[11] = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,375 ,475 ,6);
	white_checker[12] = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,375 ,425 ,6);
	white_checker[13] = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,375 ,375 ,6);
	white_checker[14] = new checker(WHITE ,bmp_wc ,bmp_wc_cursor ,375 ,325 ,6);
	
	black_checker[0]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,625 ,525 ,1);
	black_checker[1]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,625 ,475 ,1);
	black_checker[2]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,25  ,525 ,12);
	black_checker[3]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,25  ,475 ,12);
	black_checker[4]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,25  ,425 ,12);
	black_checker[5]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,25  ,375 ,12);
	black_checker[6]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,25  ,325 ,12);
	black_checker[7]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,225 ,25  ,17);
	black_checker[8]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,225 ,75  ,17);
	black_checker[9]  = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,225 ,125 ,17);
	black_checker[10] = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,375 ,25  ,19);
	black_checker[11] = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,375 ,75  ,19);
	black_checker[12] = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,375 ,125 ,19);
	black_checker[13] = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,375 ,175 ,19);
	black_checker[14] = new checker(BLACK ,bmp_bc ,bmp_bc_cursor ,375 ,225 ,19);
	
	
	bmp_buffer = create_bitmap(800,600);//bmp_buffer de dibujo
	bmp_board = load_bitmap("images/board.bmp",NULL);//tablero vacio
	
	//copiamos el tablero al bmp_buffer
	
	blit(bmp_board, bmp_buffer,0,0,0,0,bmp_board->w,bmp_board->h);
	
	
	//imagenes para los dados
	BITMAP *bmp_scratch1 = load_bitmap("images/scratch1.bmp",NULL);
	BITMAP *bmp_scratch2 = load_bitmap("images/scratch2.bmp",NULL);
	BITMAP *bmp_dice_1 = NULL;
	BITMAP *bmp_dice_2 = NULL;
	
	BITMAP *bmp_cursor = load_bitmap("images/cursor.bmp",NULL);//cargamos la imagen del cursor
	set_mouse_sprite(bmp_cursor);//cambiamos el cursor predeterminado por la imagen que acabamos de cargar
	set_mouse_sprite_focus(0,0);//definimos que el puntero es la esquina superior izquierda de la imagen
	show_mouse(screen);
	
	checker::drag = false;//inicializamos la variable en false para liberar al mouse de todas las fichas
	
	//imprimimos el menu de seleccion de estilo de juego
	
	draw_sprite(bmp_buffer,bmp_style_selection, screen->w/2 -bmp_style_selection->w/2, screen->h/2 - bmp_style_selection->h/2);
	show_mouse(NULL);
	blit(bmp_buffer, screen, 0, 0, 0, 0, 800, 600);
	show_mouse(screen);
	while( !key[KEY_A] && !key[KEY_B] );
	if(key[KEY_A]){
		game_style = PC_v_PC;
	}else if(key[KEY_B]){
		game_style = PL_v_PC;
	}
	
	//imprimimos el menu de seleccion de turnos
	blit(bmp_board, bmp_buffer,0,0,0,0,bmp_board->w,bmp_board->h);
	draw_sprite(bmp_buffer,bmp_opt_menu, screen->w/2 - bmp_opt_menu->w/2, screen->h/2 - bmp_opt_menu->h/2);
	
	
	
	//se usan dos estructuras ficha para capturar la opcion... de esta forma... necesitamos guardar los valores para reponerlos nuevamente
	int wx = white_checker[0]->get_x();
	int wy = white_checker[0]->get_y();
	int bx = black_checker[0]->get_x();
	int by = black_checker[0]->get_y();
	white_checker[0]->set_x( screen->w/2 - bmp_wc->w -20);
	black_checker[0]->set_x( screen->w/2 + bmp_wc->w -20);
	white_checker[0]->set_y( screen->h/2 - bmp_wc->h/2 + 25);
	black_checker[0]->set_y( screen->h/2 - bmp_wc->h/2 + 25);
	
	white_checker[0]->paint(bmp_buffer,true);
	black_checker[0]->paint(bmp_buffer,true);
	
	
	show_mouse(NULL);
	blit(bmp_buffer, screen, 0, 0, 0, 0, 800, 600);
	show_mouse(screen);
	
	while(!key[KEY_ESC] && !color_PC){//bucle para opciones pre-game
		if(mouse_b & 1){
			if(white_checker[0]->clicked(bmp_buffer,bmp_cursor,NULL)){
				pc_checker = white_checker;
				enemy_checker = black_checker;
				color_PC = WHITE;
				color_ENEMY = BLACK;
				if(auto_dices){
					establecer_turno(&my_dices,PRIMERO);
					establecer_turno(&enemy_dices,PRIMERO);
					establecer_turno(&pl_dices,SEGUNDO);
				}
			}else if(black_checker[0]->clicked(bmp_buffer,bmp_cursor,NULL)){
				pc_checker = black_checker;
				enemy_checker = white_checker;
				color_PC = BLACK;
				color_ENEMY = WHITE;
				if(auto_dices){
					establecer_turno(&my_dices,SEGUNDO);
					establecer_turno(&enemy_dices,SEGUNDO);
					establecer_turno(&pl_dices,PRIMERO);
				}
			}
		}
	}
	while(key[KEY_ESC] || mouse_b);
	clear_keybuf();
	
	white_checker[0]->set_x( wx );
	black_checker[0]->set_x( bx );
	white_checker[0]->set_y( wy );
	black_checker[0]->set_y( by );
	white_checker[0]->paint(bmp_buffer,true);
	black_checker[0]->paint(bmp_buffer,true);
	
	
	blit(bmp_board, bmp_buffer,0,0,0,0,bmp_board->w,bmp_board->h);
	blit(bmp_buffer,screen,0,0,0,0,bmp_board->w,bmp_board->h);
	
	//el primer turno se tiene que hacer fuera del while.... eso es aca... por lo tanto... el turno despues va a ser BLACK (cuando comience realmente el juego)
	turn = WHITE;
	
	update = true;
	for(int i = 0;i<15;i++){//se reimprimen las fichas
		white_checker[i]->paint(bmp_buffer,white_checker[i]->get_paint_me());
		black_checker[i]->paint(bmp_buffer,black_checker[i]->get_paint_me());
	}
	
	show_mouse(NULL);//se oculta el mouse para imprimir
	blit(bmp_buffer, screen, 0, 0, 0, 0, 800, 600);//se reimprime la pantalla
	show_mouse(screen);//se muestra de nuevo el mouse
	
	/******************************************************************************************************************************************************************/
	if(color_PC == WHITE){
		if(auto_dices){
			rest(1000);
			dado_lanzar(&my_dices);//seleccion automatica de dados
			dice_1 = my_dices.numero[0];
			dice_2 = my_dices.numero[1];
		}else{
			show_mouse( NULL );
			blit(bmp_board, screen, 675, 210, 675, 210, 125, 205);
			show_mouse( screen );
			dice_selection();//seleccion manual de dados
		}
		
		if(dice_1 == dice_2)
			double_turn = true;
		else 
			double_turn = false;
		
		print_dices(bmp_dice_1,bmp_dice_2);
		update = true;
		
		board_update(board,pc_checker);
		
		ai_selection(&my_moves,board,bar);
		
		for(int j = 0; j < 4; j++){
			for(i = 0;i<15;i++){
				if(pc_checker[i]->get_stack() == my_moves.origen[j]){
					id = i;
					//mbreak;
				}
			}
			
			pc_checker[id]->unpaint(bmp_buffer,bmp_board);
			
			if(my_moves.dado[j] == abs(dice_1))
				dice = &dice_1;
			else
				dice = &dice_2;
			if(my_moves.dado[j] != -1)
				insert_checker(board,bar,pc_checker[id],dice,bmp_buffer,bmp_board);
			board_update(board,pc_checker);
		}
		if(auto_dices)
			dado_termine_de_jugar(&my_moves);//enviar jugadas
		print_moves(&my_moves);
		turn = color_ENEMY;
	}
	while(!EOG && !key[KEY_ESC] && !exit_time){
		if(turn == color_PC){//juega este programa
			if(auto_dices){
				rest(1000);
				dado_lanzar(&my_dices);//seleccion automatica de dados
				dice_1 = my_dices.numero[0];
				dice_2 = my_dices.numero[1];
			}else{
				show_mouse( NULL );
				blit(bmp_board, screen, 675, 210, 675, 210, 125, 205);
				show_mouse( screen );
				dice_selection();//seleccion manual de dados
			}
			if(dice_1 == dice_2)
				double_turn = true;
			else 
				double_turn = false;
			
			print_dices(bmp_dice_1,bmp_dice_2);
			update = true;
			
			if(game_style == PL_v_PC){
				print_move(&pl_moves,board,color_ENEMY);
			}else{
				print_move(&enemy_moves,board,color_ENEMY);
			}
			board_update(board,pc_checker);
			
			ai_selection(&my_moves,board,bar);
			
			for(int j = 0; j < 4; j++){
				for(i = 0;i<15;i++){
					if(pc_checker[i]->get_stack() == my_moves.origen[j]){
						id = i;
						//mbreak;
					}
				}
				
				pc_checker[id]->unpaint(bmp_buffer,bmp_board);
				
				if(my_moves.dado[j] == abs(dice_1))
					dice = &dice_1;
				else
					dice = &dice_2;
				if(my_moves.dado[j] != -1)
					insert_checker(board,bar,pc_checker[id],dice,bmp_buffer,bmp_board);
				board_update(board,pc_checker);
			}
			if(auto_dices)
				dado_termine_de_jugar(&my_moves);//enviar jugadas
			print_moves(&my_moves);
			turn = color_ENEMY;
		}else{//juega el oponente
			if(game_style == PL_v_PC){
				
				if(auto_dices){
					rest(1000);
					dado_lanzar(&pl_dices);//el que juega es humano
					dice_1 = pl_dices.numero[0]; 
					dice_2 = pl_dices.numero[1];
				}else{
					show_mouse( NULL );
					blit(bmp_board, screen, 675, 210, 675, 210, 125, 205);
					show_mouse( screen );
					dice_selection();//seleccion manual de dados
				}
				if(dice_1 == dice_2){
					laps = 4;
					double_turn = true;
				}else{ 
					laps = 2;
					double_turn = false;
				}
				
				print_dices(bmp_dice_1,bmp_dice_2);
				
				print_move(&my_moves,board,color_PC);
			
				for(i=0;i<4;i++){
					pl_moves.dado[i] = pl_moves.origen[i] = -1;
				}
				while(laps){//mientras dure su turno
				
					if(key[KEY_ESC]){
						exit(0);
					}
					//verificamos que haya jugadas disponibles
					if(!is_dice_available(dice_1,board,bar,turn) && !is_dice_available(dice_2,board,bar,turn)){//no hay mas jugadas disponibles para este jugador
						draw_sprite(screen,bmp_no_moves,bmp_buffer->w/2-bmp_no_moves->w/2,bmp_buffer->h/2-bmp_no_moves->h/2);
						laps = 0;
						continue;
					}
					if(turn == WHITE){//el jugador humano juega con blancas
						for(i = 0;i<15;i++){
							if(white_checker[i]->clicked(bmp_buffer,bmp_cursor,board)){//si se clickeo una de las fichas del jugador
								white_checker[i]->unpaint(bmp_buffer,bmp_board);
							}else{
								if(insert){//hay que actualizar las posiciones
									insert_checker_player(board,bar,white_checker[i],mouse_x, mouse_y,bmp_buffer,bmp_board);
									insert = false;
								}
							}
						}
					}else{//el jugador humano juega con negras
						for(i = 0;i<15;i++){
							if(black_checker[i]->clicked(bmp_buffer,bmp_cursor,board)){//si se clickeo una de las fichas del jugador
								black_checker[i]->unpaint(bmp_buffer,bmp_board);
							}else{
								if(insert){//hay que actualizar las posiciones
									insert_checker_player(board,bar,black_checker[i],mouse_x, mouse_y,bmp_buffer,bmp_board);
									insert = false;
								}
							}
						}
					}
					if(pos_update_src>=0 || pos_update_dest>=0){//se actulizan la posicion origen y destino y se imprimen las jugadas en la pantalla
						pos_updater(pos_update_src, turn == WHITE? white_checker : black_checker, board, bmp_buffer, bmp_board);
						pos_updater(pos_update_dest, turn == WHITE? white_checker : black_checker, board, bmp_buffer, bmp_board);
						
						switch(laps){
							case 0:{
								pl_moves.dado[double_turn?3:1] = last_dice;
								pl_moves.origen[double_turn?3:1] = pos_update_src;
								if(dice_1 == USED){
									draw_sprite(bmp_buffer,bmp_scratch1,680,245);
									draw_sprite(bmp_buffer,bmp_scratch2,680,245);
								}
								if(dice_2 == USED){
									draw_sprite(bmp_buffer,bmp_scratch1,730,245);
									draw_sprite(bmp_buffer,bmp_scratch2,730,245);
								}
								break;
							}
							case 1:{
								pl_moves.dado[double_turn?2:0] = last_dice;
								pl_moves.origen[double_turn?2:0] = pos_update_src;
								if(dice_1 == USED){
									draw_sprite(bmp_buffer,bmp_scratch1,680,245);
									draw_sprite(bmp_buffer,bmp_scratch2,680,245);
								}
								if(dice_2 == USED){
									draw_sprite(bmp_buffer,bmp_scratch1,730,245);
									draw_sprite(bmp_buffer,bmp_scratch2,730,245);
								}
								if(dice_1 != USED && dice_1 < 0){
									draw_sprite(bmp_buffer,bmp_scratch1,680,245);
								}
								if(dice_2 != USED && dice_2 < 0){
									draw_sprite(bmp_buffer,bmp_scratch1,730,245);
								}break;
							}
							case 2:{
								pl_moves.dado[1] = last_dice;
								pl_moves.origen[1] = pos_update_src;
								if(dice_1<0){
									draw_sprite(bmp_buffer,bmp_scratch2,680,245);
								}
								break;
							}
							case 3:{
								pl_moves.dado[0] = last_dice;
								pl_moves.origen[0] = pos_update_src;
								draw_sprite(bmp_buffer,bmp_scratch1,680,245);
								break;
							}
						}
						
						
						
						pos_update_src = pos_update_dest = -1;
						
						
						
					}
					
					if(update){
						for(int i = 0;i<15;i++){
							white_checker[i]->paint(bmp_buffer,white_checker[i]->get_paint_me());
							black_checker[i]->paint(bmp_buffer,black_checker[i]->get_paint_me());
						}
						show_mouse(NULL);
						blit(bmp_buffer, screen, 0, 0, 0, 0, 800, 600);
						show_mouse(screen);
						update = false;
					}
				}
				if(auto_dices)
					dado_termine_de_jugar(&pl_moves);
				turn = color_PC;
			}else{
				
				//juega el programa del oponente
				while(!dado_ultimo_lanzamiento(&enemy_dices));//esta jugando el oponente
				dice_1 = enemy_dices.numero[0];
				dice_2 = enemy_dices.numero[1];
				if(dice_1 == dice_2)
					double_turn = true;
				else 
					double_turn = false;
				
				print_dices(bmp_dice_1,bmp_dice_2);
				update = true;
				board_update(board,enemy_checker);
				
				dado_ultima_jugada(&enemy_moves);
			
				if(!security( dice_1, dice_2,&enemy_moves,board,bar)){
					FILE *error = fopen("error_log.txt","w");;
					for(i = 0;i<26;i++){
						if((i-1)%6 == 0)
							printf( "  " );
						fprintf(error,"[%2d]",board[i]);
					}
					fprintf(error,"\n\n");
					fprintf(error,"JUGADA\n");
					
					fprintf(error,"(%d,%d)(%d,%d)(%d,%d)(%d,%d)\n"	,enemy_moves.dado[0],enemy_moves.origen[0]
																	,enemy_moves.dado[1],enemy_moves.origen[1]
																	,enemy_moves.dado[2],enemy_moves.origen[2]
																	,enemy_moves.dado[3],enemy_moves.origen[3]);
																	
					fprintf(error,"TURNO:");
					turn > 0?fprintf(error," BLANCO (1)\n"):fprintf(error," NEGRO (-1)\n");
					fclose(error);
					
					EOG = NOT_A_VALID_MOVE;
				}
					
				
				
				for(int j = 0; j < 4; j++){
					for(i = 0;i<15;i++){
						if(enemy_checker[i]->get_stack() == enemy_moves.origen[j]){
							id = i;
							//mbreak;
						}
					}
					enemy_checker[id]->unpaint(bmp_buffer,bmp_board);
					if(enemy_moves.dado[j] == abs(dice_1))
						dice = &dice_1;
					else
						dice = &dice_2;
					if(enemy_moves.dado[j] != -1)
						insert_checker(board,bar,enemy_checker[id],dice,bmp_buffer,bmp_board);
					board_update(board,enemy_checker);
				}
				
				turn = color_PC;
			}
		}
		if(abs(board[0]) == 15)
			EOG = WHITE;
		else if(abs(board[25]) == 15)
			EOG = BLACK;
		if(pos_update_src>=0 || pos_update_dest>=0){//se actulizan la posicion origen y destino y se imprimen las jugadas en la pantalla
			pos_updater(pos_update_src, turn == WHITE? white_checker : black_checker, board, bmp_buffer, bmp_board);
			pos_updater(pos_update_dest, turn == WHITE? white_checker : black_checker, board, bmp_buffer, bmp_board);
		}
		if(update){
			for(int i = 0;i<15;i++){
				white_checker[i]->paint(bmp_buffer,white_checker[i]->get_paint_me());
				black_checker[i]->paint(bmp_buffer,black_checker[i]->get_paint_me());
			}
			show_mouse(NULL);
			blit(bmp_buffer, screen, 0, 0, 0, 0, 800, 600);
			show_mouse(screen);
			update = false;
		}
		//imprimir el turno que se jugo... con las jugadas realizadas
	}
	clear_keybuf();
	
	switch(EOG){
		case WHITE:{
			draw_sprite(screen, bmp_winner_WHITE, screen->w/2 - bmp_winner_WHITE->w/2, screen->h/2 - bmp_winner_WHITE->h/2);
			break;
		}
		case BLACK:{
			draw_sprite(screen, bmp_winner_BLACK, screen->w/2 - bmp_winner_WHITE->w/2, screen->h/2 - bmp_winner_WHITE->h/2);
			break;
		}
		case NOT_A_VALID_MOVE:{
			draw_sprite(screen, bmp_eog_ERROR, screen->w/2 - bmp_winner_WHITE->w/2, screen->h/2 - bmp_winner_WHITE->h/2);
			print_ERROR_move(&enemy_moves);
			break;
		}
		default:{
			rectfill(screen,100,100,700,500,makecol(255,0,0));
			textprintf_ex(screen, font, 110, 210, makecol(255, 255, 255),-1, "ACA NO SE TENIA QUE LLEGAR WTF!!!");
			textprintf_ex(screen, font, 110, 250, makecol(255, 255, 255),-1, "PRESIONE UNA TECLA PARA CONTINUAR (CIERRA ESTA VENTANA)");
		}
	}
	while(!keypressed());
	return 0;

}END_OF_MAIN();

void print_ERROR_move( DADO_JUGADA *move ){
	int i;
	for( i=0; i<4 ;i++ )
		textprintf_ex( screen, font, 300, 365+i*text_height(font), makecol(255, 255, 255),-1, "%d)  dado = %d   origen = %d", i, move->dado[i], move->origen[i] );
}

void print_move(DADO_JUGADA *move, char board[],int turn)
{
	textprintf_ex(bmp_buffer,font,700,310,makecol(255,255,255),-1,"(%d,%d,%d)",move->dado[0], move->origen[0], get_dest(move->dado[0],move->origen[0],board,turn));
	textprintf_ex(bmp_buffer,font,700,320,makecol(255,255,255),-1,"(%d,%d,%d)",move->dado[1], move->origen[1], get_dest(move->dado[1],move->origen[1],board,turn));
	textprintf_ex(bmp_buffer,font,700,330,makecol(255,255,255),-1,"(%d,%d,%d)",move->dado[2], move->origen[2], get_dest(move->dado[2],move->origen[2],board,turn));
	textprintf_ex(bmp_buffer,font,700,340,makecol(255,255,255),-1,"(%d,%d,%d)",move->dado[3], move->origen[3], get_dest(move->dado[3],move->origen[3],board,turn));
	update = true;
}

void print_dices(BITMAP *bmp_dice_1, BITMAP *bmp_dice_2)
{//imprime los dados en la pantalla
	if( turn == WHITE ){//se cargan las imagenes de los dados
		switch ( dice_1 ){
			case 1: bmp_dice_1 = load_bitmap("images/w_dice_1.bmp", NULL);break;
			case 2: bmp_dice_1 = load_bitmap("images/w_dice_2.bmp", NULL);break;
			case 3: bmp_dice_1 = load_bitmap("images/w_dice_3.bmp", NULL);break;
			case 4: bmp_dice_1 = load_bitmap("images/w_dice_4.bmp", NULL);break;
			case 5: bmp_dice_1 = load_bitmap("images/w_dice_5.bmp", NULL);break;
			case 6: bmp_dice_1 = load_bitmap("images/w_dice_6.bmp", NULL);break;
		}
		switch ( dice_2 ){
			case 1: bmp_dice_2 = load_bitmap("images/w_dice_1.bmp", NULL);break;
			case 2: bmp_dice_2 = load_bitmap("images/w_dice_2.bmp", NULL);break;
			case 3: bmp_dice_2 = load_bitmap("images/w_dice_3.bmp", NULL);break;
			case 4: bmp_dice_2 = load_bitmap("images/w_dice_4.bmp", NULL);break;
			case 5: bmp_dice_2 = load_bitmap("images/w_dice_5.bmp", NULL);break;
			case 6: bmp_dice_2 = load_bitmap("images/w_dice_6.bmp", NULL);break;
		}
	}else{
		switch ( dice_1 ){
			case 1: bmp_dice_1 = load_bitmap("images/b_dice_1.bmp", NULL);break;
			case 2: bmp_dice_1 = load_bitmap("images/b_dice_2.bmp", NULL);break;
			case 3: bmp_dice_1 = load_bitmap("images/b_dice_3.bmp", NULL);break;
			case 4: bmp_dice_1 = load_bitmap("images/b_dice_4.bmp", NULL);break;
			case 5: bmp_dice_1 = load_bitmap("images/b_dice_5.bmp", NULL);break;
			case 6: bmp_dice_1 = load_bitmap("images/b_dice_6.bmp", NULL);break;
		}
		switch ( dice_2 ){
			case 1: bmp_dice_2 = load_bitmap("images/b_dice_1.bmp", NULL);break;
			case 2: bmp_dice_2 = load_bitmap("images/b_dice_2.bmp", NULL);break;
			case 3: bmp_dice_2 = load_bitmap("images/b_dice_3.bmp", NULL);break;
			case 4: bmp_dice_2 = load_bitmap("images/b_dice_4.bmp", NULL);break;
			case 5: bmp_dice_2 = load_bitmap("images/b_dice_5.bmp", NULL);break;
			case 6: bmp_dice_2 = load_bitmap("images/b_dice_6.bmp", NULL);break;
		}
	}
	//se imprimen los dados en pantalla
	blit( bmp_board, bmp_buffer, 680, 245, 680, 245, 120,  150);
	draw_sprite( bmp_buffer, bmp_dice_1, 690, 255 );
	draw_sprite( bmp_buffer, bmp_dice_2, 743, 255 );
	update = true;//se indica que es necesaria la actualizacion de la pantalla
	//se libera la memoria utilizada por las imagenes
	destroy_bitmap(bmp_dice_1);
	destroy_bitmap(bmp_dice_2);

}


