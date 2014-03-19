

int get_dice(int x, int y)
{
	BITMAP *bmp_shape = load_bitmap("images/dice_menu_shape.bmp",NULL);
	BITMAP *bmp_shape_buffer = create_bitmap(800,600);
	clear_to_color( bmp_shape_buffer, makecol(255,255,255) );
	draw_sprite(bmp_shape_buffer,bmp_shape, (bmp_shape_buffer->w - bmp_shape->w)/2 , (bmp_shape_buffer->h - bmp_shape->h)/2);
	int color = getpixel(bmp_shape_buffer,x,y);
	
	if(color == makecol(255,0,0))
		return 1;
	if(color == makecol(255,255,0))
		return 2;
	if(color == makecol(0,0,0))
		return 3;
	if(color == makecol(0,0,255))
		return 4;
	if(color == makecol(0,255,0))
		return 5;
	if(color == makecol(0,255,255))
		return 6;
	return 0;
}		


void dice_selection()
{
	printf("se le llama a esto\n");
	BITMAP *bmp_dice_menu = load_bmp((turn == WHITE?("images/dice_menu_WHITE.bmp"):("images/dice_menu_BLACK.bmp")),NULL);
	
	BITMAP *bmp_dice = NULL;
	int sel_count = 2;
	dice_1 = dice_2 = 0;
	draw_sprite(screen,bmp_dice_menu, (screen->w - bmp_dice_menu->w)/2 , (screen->h - bmp_dice_menu->h)/2);
	printf("se le llama a ASD");
	while(sel_count){
		if(mouse_b & 1){//se clickeo el mouse... se debe reconocer que dado se reconocio
			if(!dice_1){
				dice_1 = get_dice(mouse_x,mouse_y);
				if(dice_1){
					sel_count--;
					if( turn == WHITE ){//se cargan las imagenes de los dados
						switch ( dice_1 ){
							case 1: bmp_dice = load_bitmap("images/w_dice_1.bmp", NULL);break;
							case 2: bmp_dice = load_bitmap("images/w_dice_2.bmp", NULL);break;
							case 3: bmp_dice = load_bitmap("images/w_dice_3.bmp", NULL);break;
							case 4: bmp_dice = load_bitmap("images/w_dice_4.bmp", NULL);break;
							case 5: bmp_dice = load_bitmap("images/w_dice_5.bmp", NULL);break;
							case 6: bmp_dice = load_bitmap("images/w_dice_6.bmp", NULL);break;
						}
					}else{
						switch ( dice_1 ){
							case 1: bmp_dice = load_bitmap("images/b_dice_1.bmp", NULL);break;
							case 2: bmp_dice = load_bitmap("images/b_dice_2.bmp", NULL);break;
							case 3: bmp_dice = load_bitmap("images/b_dice_3.bmp", NULL);break;
							case 4: bmp_dice = load_bitmap("images/b_dice_4.bmp", NULL);break;
							case 5: bmp_dice = load_bitmap("images/b_dice_5.bmp", NULL);break;
							case 6: bmp_dice = load_bitmap("images/b_dice_6.bmp", NULL);break;
						}
					}
					show_mouse(NULL);
					draw_sprite( screen, bmp_dice, 690, 255 );
					show_mouse(screen);
				}
			}else if(!dice_2){
				dice_2 = get_dice(mouse_x,mouse_y);
				if(dice_2){
					sel_count--;
					if( turn == WHITE ){//se cargan las imagenes de los dados
						switch ( dice_2 ){
							case 1: bmp_dice = load_bitmap("images/w_dice_1.bmp", NULL);break;
							case 2: bmp_dice = load_bitmap("images/w_dice_2.bmp", NULL);break;
							case 3: bmp_dice = load_bitmap("images/w_dice_3.bmp", NULL);break;
							case 4: bmp_dice = load_bitmap("images/w_dice_4.bmp", NULL);break;
							case 5: bmp_dice = load_bitmap("images/w_dice_5.bmp", NULL);break;
							case 6: bmp_dice = load_bitmap("images/w_dice_6.bmp", NULL);break;
						}
					}else{
						switch ( dice_2 ){
							case 1: bmp_dice = load_bitmap("images/b_dice_1.bmp", NULL);break;
							case 2: bmp_dice = load_bitmap("images/b_dice_2.bmp", NULL);break;
							case 3: bmp_dice = load_bitmap("images/b_dice_3.bmp", NULL);break;
							case 4: bmp_dice = load_bitmap("images/b_dice_4.bmp", NULL);break;
							case 5: bmp_dice = load_bitmap("images/b_dice_5.bmp", NULL);break;
							case 6: bmp_dice = load_bitmap("images/b_dice_6.bmp", NULL);break;
						}
					}
					show_mouse(NULL);
					draw_sprite( screen, bmp_dice, 743, 255 );
					show_mouse(screen);
				}
			}
			while(mouse_b);
		}
	}
}

