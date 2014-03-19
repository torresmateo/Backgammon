
#include "checker.h"


#define TRANSPARENT makecol(255,0,255)

extern bool checker::drag;

bool cursor_switch = false;
checker::checker(int k,BITMAP *checker_bmp,BITMAP *cursor_bmp, int a, int b, int i)
{
	/*
	constructor: inicializa las variables de cada objeto ficha
		parametros:
			k = tipo de ficha(blaca o negra)
			checker_bmp = imagen de la ficha
			cursor_bmp = imagen del cursor en control de la ficha
			a = posicion x inicial de la ficha
			b = posicion y inicial de la ficha
			i = pila en donde se inserta inicialmente la ficha
	*/	
	kind = k;
	bmp_cursor = cursor_bmp;
	bmp = checker_bmp;
	being_draged = false;
	x = a;
	y = b;
	stack = i;
	paint_me = true;
}

int checker::clicked(BITMAP *buffer,BITMAP *cursor, char board[])
{
	/*
	prametros:
		buffer = buffer de pintado de la ventana
		cursor = imagen del cursor original del juego
		
	retorna TRUE  si la ficha esta siendo clickeada y debe ser controlada por el mouse
	retorna TRUE  si la ficha esta siendo clickeada y esta bajo control del mouse
	retorna FALSE si la ficha no esta bajo control del mouse, si se solto el click, libera al mouse del control de la ficha y dibuja 
	a la ficha en el lugar que le corresponde
	*/
	if(checker::drag && being_draged && (mouse_b & 1)){//ficha esta siendo clickeada y esta bajo control del mouse
		return TRUE;
	}
	if(mouse_x > x && 
	mouse_x < x+bmp->w && 
	mouse_y > y && 
	mouse_y < y+bmp->h && 
	getpixel(bmp,mouse_x - x,mouse_y - y) != TRANSPARENT && 
	(mouse_b & 1) && (!checker::drag || being_draged)){//ficha esta siendo clickeada y debe ser controlada por el mouse
		if(!board)
			return TRUE;
		checker::drag = true;//indicamos que se esta drageando algun objeto
		being_draged = true;//indicamos cual es el objeto drageado
		
		//cambiamos la imagen del cursor por una con el cursor y la ficha
		show_mouse(NULL);
		set_mouse_sprite(bmp_cursor);
		set_mouse_sprite_focus(bmp->w/2,bmp->h/2);
		show_mouse(screen);
		
		
		update = true;//indicamos que debe actualizarse la impresion en pantalla
		return TRUE;//indicamos que ha sido clickeada la ficha
	}else{//ficha no esta bajo control del mouse o no debe estar bajo control del mouse
		if(!checker::drag && cursor_switch){//debe devolverse el cursor a su estado original
			//se cambia la imagen del cursor con la ficha, por la imgaen que tiene simplemente un cursor
			show_mouse(NULL);
			set_mouse_sprite(cursor);
			set_mouse_sprite_focus(0,0);
			show_mouse(screen);
			
			cursor_switch = false;//el cursor no debe reestablecerse
			update = true;//indicamos que debe actualizarse la impresion en pantalla
		}
		if(checker::drag && !(mouse_b & 1)){//se acaba de hacer el release del mouse
			
			checker::drag = false;//indicamos que ya no se dragea objeto alguno
			cursor_switch = true;//indicamos que el cursor debe retomar su imagen original
			update = true;//indicamos que debe actualizarse la impresion en pantalla
		}
		if(being_draged){
			
			insert = true;//indicamos que debe verificarse la instruccion que ha indicado el mouse al utilizar este objeto
			being_draged = false;//indicamos que este objeto ha dejado de ser controlado por el mouse
			update = true;//indicamos que debe actualizarse la impresion en pantalla
		}
		
	}
	return FALSE;//la ficha no ha sido clickeada
}

void checker::paint(BITMAP *buffer, bool paint)
{
	/*
		dibuja la ficha en la posicin x,y
	*/
	if(!being_draged && paint){
		draw_sprite(buffer, bmp, x, y);
	}
}

void checker::unpaint(BITMAP *buffer, BITMAP *board)
{
	/*
		borra la ficha, dibujando la parte del tablero que corresponde a su posicion
	*/
	blit(board, buffer, x,y,x,y,bmp->w,bmp->h);
}

/*

	FUNCIONES GET Y SET
	
	devuelven el valor de la variable o cambian el valor de la variable(esto es por seguridad, ya que no es conveniente declarar publica una variable en un objeto)
	
	GET
	funciones get_xxxx()
	{
		return xxxx;
	}
	SET
	funciones set_xxxx(value)
	{
		xxxx = value;
	}
*/

int checker::get_kind(void)
{	
	return kind;
}

void checker::set_y(int a)
{
	y=a;
}

void checker::set_x(int a)
{
	x=a;
}

int checker::get_x(void)
{
	return x;
}

int checker::get_y(void)
{
	return y;
}

int checker::get_stack(void)
{
	return stack;
}

bool checker::get_paint_me(void)
{
	return paint_me;
}

void checker::set_stack(int a)
{
	stack = a;
}

void checker::set_paint_me(bool value)
{
	paint_me = value;
}


