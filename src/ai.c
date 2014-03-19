

int Prime, 	E_prime;	//primes nuestro y del enemigo respectivamente
int prime[4];				//se carga el valor del prime que forman nuestras fichas
int e_prime[4];			//se carga el valor del prime que forman las fichas del enemigo


void print_board(char board[])
{
	int i;
	for(i = 0;i<26;i++){
		if((i-1)%6 == 0)
			printf( "  " );
		printf("[%2d]",board[i]);
	}
	printf("\n");
	
}

#define BL_D1	 5
#define BL_D2	 4
#define BL_D3	 1
#define BL_H	 3
#define BL_I	25

#define PR_K1 -1
#define PR_K2 -2
#define PR_K3 -3
#define PR_K4 -4

#define PR_Q1 19
#define PR_Q2 21
#define PR_Q3 22
#define PR_Q4 25

#define PR_P1 100
#define PR_P2 104
#define PR_P3 108
#define PR_P4 125


#define MIN_FULL_PRIME 1800


int max_height( char board[26] )
{//retorna la altura de la pila mas alta multiplicado por una constante para aumentar un poco la unfluencia
	int i,h_max=0;
	for( i=1; i<25 ;i++ )
		if( board[i]<0 && -board[i] > h_max )
			h_max = -board[i];
	
	return -h_max*400;
}

int eval_blots( char board[26], char bar[2] )
{//retorna un valor dependiendo da la situación del tablero con respecto a los blots

	/*  Se restan puntos por cada blot negro de acuerdo a su posición en el tablero.
		Y por cada blot negro se restan puntos por cada ficha enemiga que este mas adelante en el tablero, 
		esto tambien depende de que tan cerca estén esta fichas.
	*/
	int puntaje = 0;//varible a retornar
	int i,j=0,d=0;
	int prime_max = prime[0];//prime_max será el maximo prime encontrado
	int i_max = 0;
	int e_prime_max = e_prime[0];//e_prime_max será el maximo prime enemigo encontrado
	int e_i_max = 0;
	int i_inicio, i_fin;
	
	//se busca los mayores primes
	for( i=1; i<4 ;i++ ){
		if( e_prime_max < e_prime[i] ){
			e_prime_max = e_prime[i];
			e_i_max = i;
		}
		if( prime_max < prime[i] ){
			prime_max = prime[i];
			i_max = i;
		}
	}
	
	//el valor de i_inicio depende de si es que se formó o no un prime enemigo completo
	if( e_prime_max >= MIN_FULL_PRIME ){
		i_inicio = 6+(4-e_i_max);
	}else{
		i_inicio = 1;
	}
	
	//el valor de i_fin depende de si es que se formó o no un prime propio completo
	if( prime_max >= MIN_FULL_PRIME ){
		i_fin = 16 + i_max;
	}else{
		i_fin = 25;
	}
	
	for( i=i_inicio; i<25 ;i++ ){//se recorre el tablero desde i_inicio
		
		if( board[i] == BLACK ){//se buscan las ficas negras
			
			puntaje -= i*BL_I;//por cada blot negro se restan puntos dependiendo de que tan adelantado este en el tablero
			
			if( bar[0] ){//hay fichas en la barra
				if( bar[0]==1 ){//hay una sola ficha en la barra
					for( j=i+1; ( i<16 )?( j<i_fin ):( j<25 ) ;j++ ){//se recorre el tablero desde el blot hasta i_fin
						if( board[j] > 0 ){//se buscan las fichas blancas
							d = j-i;//la distancia "d" es la diferencia entre el blot y la ficha blanca a analizar
							//se restan puntos dependiendo de la situacion
							if( i < 19 ){
								if( d <= 6 ){
									puntaje -= (24-d)*BL_D2 + board[j]*BL_H ;//con esta ficha el enemigo puede comer el blot negro usando los dos dados 
								}else{
									puntaje -= (24-d)*BL_D3 + board[j]*BL_H ;//con esta ficha el enemigo no puede comer el blot negro usando dos dados
								}
							}else{
								puntaje -= (24-d)*BL_D1 + board[j]*BL_H ;//con esta ficha el enemigo puede comer el blot negro usando uno o dos de sus dados 
							}
						}
					}
				}else{//hay mas de una ficha
					puntaje -= (25-d)*BL_D3 + board[j]*BL_H ;//con esta ficha el enemigo no puede comer el blot negro usando dos dados
				}
				
				
			}else{//no hay fichas en la barra
				
				for( j=i+1; ( i<16 )?( j<i_fin ):( j<25 ) ;j++ ){//se recorre el tablero desde el blot hasta i_fin
					if( board[j] > 0 ){//se buscan las fichas blancas
						d = j-i;//la distancia "d" es la diferencia entre el blot y la ficha blanca a analizar
						//se restan puntos dependiendo de la situacion
						if( d <= 6 ){
							puntaje -= (24-d)*BL_D1 + board[j]*BL_H;//con esta ficha el enemigo puede comer el blot negro usando uno o dos de sus dados 
						}else if( d <= 12 ){
							puntaje -= (24-d)*BL_D2 + board[j]*BL_H;//con esta ficha el enemigo puede comer el blot negro los dos dados 
						}else{
							puntaje -= (24-d)*BL_D3 + board[j]*BL_H;//con esta ficha el enemigo no puede comer el blot negro usando dos dados
						}
					}
				}
				
			}
			
		}
		
	}
	return puntaje*4;//se retorna el puntaje pero multiplicado por 4 para incrementar su influencia
}

int is_race(char board[])
{//si las fichas estan cruzadas, se esta en modo carrera
	int i;
	for(i=1;i<25 && board[i]>=0 ;i++);
	
	for(;i<25;i++)
		if(board[i]>0)
			return FALSE;
	return TRUE;
}

void eval_prime(char board[])
{
	int i;
	
	for(i = 0; i < 4;i++)
		prime[i] = e_prime[i] = 0;
	
	/*
	el calculo de cada prime (bloqueos para que una ficha no pueda escapar) se hace de la siguiente manera
	
	se revisan los primes mas probables: 
		grupos de 6 desde la posición 1 hasta la 9
		en orden descendente de amenaza (primes que amenazan a la inteligencia artificial)
			4 - 9 //prime1
			3 - 8 //prime2
			2 - 7 //prime3
			1 - 6 //prime4
	el resultado para la funcion de evaluacion de estos prime es la suma de todos los primes

	tambien se considera como algo positivo que la posicion de la inteligencia artificial tenga primes formados
	en este caso... se consideran los primes en grupos de 6 de manera análoga desde la posicion 24 hasta la posición 16
	
	la forma de puntuar un prime es la siguiente formula:
	
	(h*q*i) + p en caso de que la posición no este vacía:
	siendo 	h -> altura de la pila en esa posicion
			q -> valor por cada ficha
			i -> posicion de la pila en el tablero
			p -> valor de que este tomada la posicion
	en caso de que la posicion este vacia, se considera esa posicion como tomada por una ficha nuestra
	
	
	la justificacion de esta formula es la siguiente
	el prime es algo que muchas veces define la partida, bloquear al oponente es algo muy bueno en la estrategia,
	ya que deja a uno con la posibilidad de dominar el juego...
	
	el prime mas facil de formar (para el jugador)
	*/
	
	//peligroso para nosotros (se carga en las variables del enemigo)
	for(i=4;i<=9;i++)
		e_prime[0] += board[i]?( board[i]*PR_Q1*i + (((board[i])>0)?(PR_P1):(-PR_P1)) ):(i*PR_K1 - PR_P1/2);
	
	for(i=3;i<=8;i++)
		e_prime[1] += board[i]?( board[i]*PR_Q2*i + (((board[i])>0)?(PR_P2):(-PR_P2)) ):(i*PR_K2 - PR_P2/2);
	
	for(i=2;i<=7;i++)
		e_prime[2] += board[i]?( board[i]*PR_Q3*i + (((board[i])>0)?(PR_P3):(-PR_P3)) ):(i*PR_K3 - PR_P3/2);
	
	for(i=1;i<=6;i++)
		e_prime[3] += board[i]?( board[i]*PR_Q4*i + (((board[i])>0)?(PR_P4):(-PR_P4)) ):(i*PR_K4 - PR_P4/2);
	
	//peligroso para el enemigo (se carga en nuestras variables)
	for(i=16;i<=21;i++)
		prime[0] += board[i]?((-board[i])*PR_Q1*(25-i) + (((board[i])<0)?(PR_P1):(-PR_P1)) ):((25-i)*PR_K1  - PR_P1);
	
	for(i=17;i<=22;i++)
		prime[1] += board[i]?((-board[i])*PR_Q2*(25-i) + (((board[i])<0)?(PR_P2):(-PR_P2)) ):((25-i)*PR_K2 - PR_P2);
	
	for(i=18;i<=23;i++)
		prime[2] += board[i]?((-board[i])*PR_Q3*(25-i) + (((board[i])<0)?(PR_P3):(-PR_P3)) ):((25-i)*PR_K3 - PR_P3);
	
	for(i=19;i<=24;i++)
		prime[3] += board[i]?((-board[i])*PR_Q4*(25-i) + (((board[i])<0)?(PR_P4):(-PR_P4)) ):((25-i)*PR_K4 - PR_P4);
		
	Prime = prime[0] + prime[1] + prime[2] + prime[3];
	E_prime = e_prime[0] + e_prime[1] + e_prime[2] + e_prime[3];
}



int E(char board[],char bar[])
{
	int i;
	int value = 0;
	for(i=1;i<13;i++){
		if(board[i]<=BLACK);
			value-=abs(board[i])*E_prime/(i<=6?25:30);
	}
	if(!is_race(board)){
		eval_prime(board);
		value += eval_blots(board,bar);
		value +=max_height(board);
		value += Prime;
		value -= E_prime;
		value += 400*abs(board[25]);
		value += 700*abs(bar[0]);
	}else{
		value = 10000*abs(board[25]);
	}
	
	
	return value;
}


void ai_translator_in(char real_board[], char real_bar[], char translated_board[], char translated_bar[])
{
	/*
		nuestras funciones de inteligencia artificial asumen el sentido 0 a 25 para las movidas,
		segun las normas establecidas, esto corresponde al jugador con turno 2 (en el caso de este programa con fichas negras)
		
		es por esto que en caso de que la inteligencia artificial asuma el papel de jugador con fichas blancas
		es necesaria una traducción del estado del tablero, a un estado equivalente, pero con el programa en control de las fichas negras
		
	*/
	if(color_PC == WHITE){//se necesita traducir el tablero al tipo de movimiento soportado por la inteligencia artificial
		//se intercambian los valores de las posiciones en el tablero, asi como las identificaciones de las fichas
		//igualmente se intercambia la barra
		
		for(int i = 0;i<26;i++)
			translated_board[i] = -1*real_board[25-i];
		translated_bar[1] = -abs(real_bar[0]);
		translated_bar[0] = -abs(real_bar[1]);
	}else{//la traduccion no es necesaria, por lo que se copian directamente los valores de la barra y el tablero
		for(int i = 0;i<26;i++)
			translated_board[i] = real_board[i];
		translated_bar[1] = real_bar[1];
		translated_bar[0] = real_bar[0];
	}
}

void ai_translator_out(DADO_JUGADA *moves)
{	
	/*
		funcion inversa de la funcion de traduccion "in"
		
		a diferencia de la funcion "in" de traducción, ya que la salida de la inteligencia artificial es una jugada
		del tipo DADO_JUGADA, la traducción en este caso corresponde a la jugada elegida por la inteligencia, en caso de que 
		ésta controle las fichas blancas
		
		solo es necesaria la traduccion de la parte que define el origen de cada movida, ya que el valor de los dados es universal (para los dos jugadores)
		
		por lo tanto se cambia el indice al indice respectivo
		
		en caso de que el valor de origen sea la barra, se cambia al valor de la barra correspondiente al jugador con fichas blancas
	*/
	if(color_PC == WHITE){
		if(moves->origen[0] != -1)
			moves->origen[0] = moves->origen[0] == 27? 26:25-moves->origen[0];
		if(moves->origen[1] != -1)
			moves->origen[1] = moves->origen[1] == 27? 26:25-moves->origen[1];
		if(moves->origen[2] != -1)
			moves->origen[2] = moves->origen[2] == 27? 26:25-moves->origen[2];
		if(moves->origen[3] != -1)
			moves->origen[3] = moves->origen[3] == 27? 26: 25-moves->origen[3];
	}
}

int play_move(int dice, int src, char board[], char bar[],int *undo_bar)
{
	/*
		verifica la jugada que intenta realizar la inteligencia artificial, en caso de ser válida, aplica esa jugada a la copia del tablero
		que es utilizada por la IA, modificando variables que serviran luego para deshacer la jugada realizada por esta funcion
		
		en caso de que la jugada no sea valida, retorna FALSE y no se modifica la copia del tablero
	*/
	if(valid_move(dice,src,board,bar,BLACK) == NOT_A_VALID_MOVE)//la jugada no es valida
		return FALSE;
	else{//la jugada es realizable
		if(bar[1])//si hay algo en la barra
			bar[1]-=BLACK;//se saca una ficha de la barra
		else//la barra esta vacia
			board[src]-=BLACK;//se saca una ficha de la posicion origen del tablero
		if(board[get_dest(dice,src,board,BLACK)] == WHITE){//si se comio una ficha
			board[get_dest(dice,src,board,BLACK)] = BLACK;//se reemplaza la ficha por una ficha nuestra
			bar[0]+=WHITE;//la barra enemiga recibe una ficha
			*undo_bar = TRUE;//se indica que la barra enemiga debe actualizarse a la hora de deshacer esta movida
		}else{//no se come ficha alguna
			board[get_dest(dice,src,board,BLACK)] += BLACK;//se aumenta la cantidad de fichas en el destino
			*undo_bar = FALSE;//la barra enemiga no debe actualizarse al deshacer esta jugada
		}
	}
	return TRUE;//la jugada pudo aplicarse
	
}

void undo_move(int dice, int src, char board[], char bar[],int *undo_bar)
{
	/*
		deshace la jugada aplicada por play_move de la copia del tablero que utiliza la IA
	*/
	if(src == 27)//si el origen era la barra
		bar[1]+=BLACK;//agregamos una ficha nuestra a la barra
	else//si el origen era el tablero
		board[src]+=BLACK;//agregamos una ficha nuestra a su posicion original en el tablero
	if(*undo_bar){//si la barra del enemigo debe actualizarse
		board[get_dest(dice,src,board,BLACK)] = WHITE;//en la posicion destino reponemos la ficha que comimos antes
		bar[0]-=WHITE;//quitamos la ficha que antes fue enviada a la barra enemiga
		*undo_bar = FALSE;//la barra ya fue actualizada
	}else{//no debe actualizarse la barra
		board[get_dest(dice,src,board,BLACK)] -= BLACK;//quitamos una ficha nuestra de la posicion destino en el tablero
	}
}

void print_moves(DADO_JUGADA *moves){
	printf("\n%d\t%d\n",moves->dado[0],moves->origen[0]);
	printf("%d\t%d\n",moves->dado[1],moves->origen[1]);
	printf("%d\t%d\n",moves->dado[2],moves->origen[2]);
	printf("%d\t%d\n",moves->dado[3],moves->origen[3]);
}

void ai_selection(DADO_JUGADA *moves, char real_board[], char real_bar[])
{

	/*
		prueba todas las jugadas posibles que tiene la IA con el tablero actual de la partida, y aplica la función de evaluación 
		a cada jugada, en caso de ser superior a la jugada que es considerada actualmente como la más conveniente, se reemplaza 
		la jugada por la que resulte con mayor valor segun la funcion de evaluacion
		
		al terminar el proceso, queda seleccionada la mejor jugada (con el criterio de la funcion E)
	
	*/
	int e;
	char board[26];
	char bar[2];
	//print_bar(real_bar);
	
	ai_translator_in(real_board,real_bar,board,bar);//se traduce el tablero en caso de ser necesario
	
	//print_bar(bar);
	int actual_value = -1000000;//valor exageradamente negativo, para que la primera jugada valga mas en la funcion E
	int i,j,k,l;//contadores y banderas 
	int undo_1,undo_2,undo_3,undo_4;//variables que indican que se debe actualizar las barras del enemigo
	
	//la jugada inicial es una jugada nula
	moves->dado[0] = moves->dado[1] = moves->dado[2] = moves->dado[3] = -1;
	moves->origen[0] = moves->origen[1] = moves->origen[2] = moves->origen[3] = -1;
	
	
	/*
		la siguiente seccion es un poco complicada, debido a la gran cantidad de ciclos anidados y condiciones
		
		a manera de simplificar el entendimiento, la idea de seleccion es la siguiente:
		
		ante la vision de que la recursividad no nos permitía eliminar algunas pruebas innecesarias (caso de la barra por ejemplo)
		decidimos que la version iterativa de seleccion convenía más en el ahorro de tiempo y recursos.
		
		la estructura basica de cada ciclo es la siguiente
		PSEUDOCODIGO
		
		
		caso con un solo dado disponible
			
			for( cada pila i){
				if( hay barra )
					i = barra(ultimo valor del ciclo)
				aplicamos la jugada con origen i al tablero
					hacemos competir la jugada i con la jugada max actual
				devolvemos el tablero a antes de la jugada i
			}
		caso con dos dados disponibles
		
			for( cada pila i){  CICLO EXTERNO DADO 1
				if( hay barra )
					i = barra(ultimo valor del ciclo)
				aplicamos la jugada con origen i al tablero
					
					for(cada pila j){
						aplicamos la jugada con origen j al tablero
							hacemos competir la jugada i j con la jugada max actual
						devolvemos el tablero a antes de la jugada j
					}
				devolvemos el tablero a antes de la jugada i
			}
		
		y así sucesivamente se van agregando ciclos a los casos
	*/
	if(!double_turn){//no es turno doble
		//primero probamos las jugadas con el dado 1 seguidas con el dado 2
		if(is_dice_available(dice_1,board,bar,BLACK)){							//el dado 1 esta habilitado
			for(i = 1;i<=24;i++){												//para cada posicion se trata de jugar 
				if(bar[1]){														//hay fichas en la barra
					i = 27;														//nos vemos obligados a utilizar la barra como origen
				}
				if(play_move(dice_1,i,board,bar,&undo_1)){						//en caso de que la primera jugada se haya podido aplicar
					if(is_dice_available(dice_2,board,bar,BLACK)){						//el dado 2 esta disponible
						for(j = 1;j<=24;j++){											//para cada posicion se trata de jugar 
							if(bar[1]){													//hay fichas en la barra
								j = 27;													//nos vemos obligados a utilizar la barra como origen
							}
							if(play_move(dice_2,j,board,bar,&undo_2)){					//en caso de que la  segunda jugada se haya podido aplicar
								if(actual_value < (e = E(board,bar))){								//se hacen competir las jugadas y en caso de que gane la funcion de evaluacion, se reemplaza el maximo anterior
									actual_value = e;
									moves->dado[0] = dice_1;
									moves->origen[0] = i;
									moves->dado[1] = dice_2;
									moves->origen[1] = j;
								}
								undo_move(dice_2,j,board,bar,&undo_2);					//deshacemos la segunda jugada aplicada
							}
						}
					}else{																//el dado 2 no esta disponible
						if(actual_value < (e = E(board,bar))){										//se hacen competir las jugadas y en caso de que gane la funcion de evaluacion, se reemplaza el maximo anterior
							actual_value = e;
							moves->dado[0] = dice_1;
							moves->origen[0] = i;
							moves->dado[1] = -1;
							moves->origen[1] = -1;
						}
					}
					undo_move(dice_1,i,board,bar,&undo_1);						//deshacemos la primera jugada aplicada
				}
			}
		}
		//ahora probamos las jugadas con el dado 2 seguidas con el dado 1
		if(is_dice_available(dice_2,board,bar,BLACK)){							//el dado 2 esta habilitado
			for(i = 1;i<=24;i++){												//para cada posicion se trata de jugar 
				if(bar[1]){														//hay fichas en la barra
					i = 27;														//nos vemos obligados a utilizar la barra como origen
				}
				if(play_move(dice_2,i,board,bar,&undo_1)){						//en caso de que la primera jugada se haya podido aplicar
					if(is_dice_available(dice_1,board,bar,BLACK)){						//el dado 1 esta habilitado
						for(j = 1;j<=24;j++){											//para cada posicion se trata de jugar 
							if(bar[1]){													//hay fichas en la barra
								j = 27;													//nos vemos obligados a utilizar la barra como origen
							}
							if(play_move(dice_1,j,board,bar,&undo_2)){					//en caso de que la segunda jugada se haya podido aplicar
								if(actual_value < (e = E(board,bar))){								//se hacen competir las jugadas y en caso de que gane la funcion de evaluacion, se reemplaza el maximo anterior
									actual_value = e;
									moves->dado[0] = dice_2;
									moves->origen[0] = i;
									moves->dado[1] = dice_1;
									moves->origen[1] = j;
									
								}
								undo_move(dice_1,j,board,bar,&undo_2);					//deshacemos la segunda jugada aplicada
							}
						}
					}else{																//el dado 1 no esta disponible
						if(actual_value < (e = E(board,bar))){										//se hacen competir las jugadas y en caso de que gane la funcion de evaluacion, se reemplaza el maximo anterior
							actual_value = e;
							moves->dado[0] = dice_2;
							moves->origen[0] = i;
							moves->dado[1] = -1;
							moves->origen[1] = -1;
						}
					}
					undo_move(dice_2,i,board,bar,&undo_1);						//deshacemos la primera jugada aplicada
				}
			}
		}
	
	}else{//turno doble
		if(is_dice_available(dice_1,board,bar,BLACK)){				//el dado 1 esta habilitado(en el caso de doble turno, verificamos siempre el mismo dado, ya que los dos dados deben tener el mismo valor)
			for(i = 1;i<=24;i++){									//para cada posicion se trata de jugar 
				if(bar[1]){											//hay fichas en la barra
					i = 27;											//nos vemos obligados a utilizar la barra como origen
				}
				if(play_move(dice_1,i,board,bar,&undo_1)){							//en caso de que la primera jugada se haya podido aplicar
					if(is_dice_available(dice_1,board,bar,BLACK)){					//el dado 1 esta habilitado
						for(j = 1;j<=24;j++){										//para cada posicion se trata de jugar 
							if(bar[1]){												//hay fichas en la barra
								j = 27;												//nos vemos obligados a utilizar la barra como origen
							}
							if(play_move(dice_1,j,board,bar,&undo_2)){								//en caso de que la segunda jugada se haya podido aplicar
								if(is_dice_available(dice_1,board,bar,BLACK)){						//el dado 1 esta habilitado
									for(k = 1;k<=24;k++){											//para cada posicion se trata de jugar 
										if(bar[1]){													//hay fichas en la barra
											k = 27;													//nos vemos obligados a utilizar la barra como origen
										}
										if(play_move(dice_1,k,board,bar,&undo_3)){									//en caso de que la tercera jugada se haya podido aplicar
											if(is_dice_available(dice_1,board,bar,BLACK)){							//el dado 1 esta habilitado
												for(l = 1;l<=24;l++){												//para cada posicion se trata de jugar 
													if(bar[1]){														//hay fichas en la barra
														l = 27;														//nos vemos obligados a utilizar la barra como origen
													}
													if(play_move(dice_1,l,board,bar,&undo_4)){								//en caso de que la cuarta jugada se haya podido aplicar
														if(actual_value < (e = E(board,bar))){									//se hacen competir las jugadas y en caso de que gane la funcion de evaluacion, se reemplaza el maximo anterior
															actual_value = e;
															moves->dado[0] = dice_1;
															moves->origen[0] = i;
															moves->dado[1] = dice_1;
															moves->origen[1] = j;
															moves->dado[2] = dice_1;
															moves->origen[2] = k;
															moves->dado[3] = dice_1;
															moves->origen[3] = l;
														}
														undo_move(dice_2,l,board,bar,&undo_4);								//deshacemos la cuarta jugada aplicada
													}
												}
											}else{																	//el dado no esta disponible
												if(actual_value < (e = E(board,bar))){									//se hacen competir las jugadas y en caso de que gane la funcion de evaluacion, se reemplaza el maximo anterior
													actual_value = e;
													moves->dado[0] = dice_1;
													moves->origen[0] = i;
													moves->dado[1] = dice_1;
													moves->origen[1] = j;
													moves->dado[2] = dice_1;
													moves->origen[2] = k;
													moves->dado[3] = -1;
													moves->origen[3] = -1;
												}
											}
											undo_move(dice_2,k,board,bar,&undo_3);									//deshacemos la tercera jugada aplicada
										}
									}
								}else{																//el dado no esta disponible
									if(actual_value < (e = E(board,bar))){								//se hacen competir las jugadas y en caso de que gane la funcion de evaluacion, se reemplaza el maximo anterior
										actual_value = e;
										moves->dado[0] = dice_1;
										moves->origen[0] = i;
										moves->dado[1] = dice_1;
										moves->origen[1] = j;
										moves->dado[2] = -1;
										moves->origen[2] = -1;
										moves->dado[3] = -1;
										moves->origen[3] = -1;
									}
								}
								undo_move(dice_2,j,board,bar,&undo_2);								//deshacemos la segunda jugada aplicada
							}
						}
					}else{															//el dado  no esta disponible
						if(actual_value < (e = E(board,bar))){							//se hacen competir las jugadas y en caso de que gane la funcion de evaluacion, se reemplaza el maximo anterior
							actual_value = e;
							moves->dado[0] = dice_1;
							moves->origen[0] = i;
							moves->dado[1] = -1;
							moves->origen[1] = -1;
							moves->dado[2] = -1;
							moves->origen[2] = -1;
							moves->dado[3] = -1;
							moves->origen[3] = -1;
						}
					}
					undo_move(dice_2,i,board,bar,&undo_1);							//deshacemos la primera jugada aplicada
				}
			}
		}
	}
	ai_translator_out(moves);//se traduce la jugada seleccionada, en caso de ser necesario
}


