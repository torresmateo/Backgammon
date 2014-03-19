#ifndef FICHA_H
#define FICHA_H

#include <allegro.h>

class checker{
	public:
		static bool drag;//indica que alguna de las fichas esta bajo control del mouse
		checker(int k, BITMAP *checker_bmp,BITMAP *cursor_bmp, int a, int b, int i);
		void set_x(int a);
		void set_y(int a);
		void set_stack(int a);
		void set_paint_me(bool value);
		int get_stack(void);
		int get_kind(void);
		int get_x(void);
		int get_y(void);
		bool get_paint_me(void);
		int clicked(BITMAP *buffer,BITMAP *cursor, char board[]);
		void paint(BITMAP *buffer, bool paint = false);
		void unpaint(BITMAP *buffer,BITMAP *board);
	private:
		int kind;//tipo de ficha (blanca o negra)
		bool being_draged;//la ficha actual esta bajo control del mouse
		bool paint_me;//la ficha actual esta bajo control del mouse
		BITMAP *bmp;//imagen de la ficha
		BITMAP *bmp_cursor;//imagen del cursor en control de la ficha
		int x;//posicion x de la ficha
		int y;//posicion y de la ficha
		int stack;//identifica la pila del tablero en donde se encuentra la ficha; 
};
#endif
#ifndef BORDER
	#define BORDER 25
#endif

