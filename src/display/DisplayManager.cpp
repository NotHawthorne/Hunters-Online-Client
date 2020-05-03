#include "../../includes/display.h"

DisplayManager::DisplayManager()
{
	initscr();
	cbreak();
	noecho();
	start_color();
	curs_set(0);

	mainwin = newwin(LINES / 2, (COLS / 2) + 2, LINES / 4, COLS / 4);
	keypad(mainwin, true);
	curmenu = new LoginMenu(mainwin);
	test.push_back(curmenu);
	box(mainwin, 0, 0);
	wrefresh(mainwin);
	refresh();
}

DisplayManager::~DisplayManager()
{
	unpost_menu(curmenu->menu);
	free_menu(curmenu->menu);
	for (int i = 0; i != 2; i++)
		free_item(curmenu->menu_items[i]);
	endwin();
	curs_set(1);
}

void DisplayManager::update()
{
	if (state != last_state)
	{
		//this should completely clear up any display through
		//the deconstructor
		if (curmenu)
			delete curmenu;
		switch (state)
		{
			case LOGIN:
				curmenu = new LoginMenu(mainwin);
				break ;
			case NOTIFY:
				//create notification box
				break ;
			case MAIN_WIN:
				curmenu = new GameMenu(mainwin);
				//draw the main game window, starting listener
				//and client
				break ;
			default:
				break ;
		}
		last_state = state;
	}
}
void DisplayManager::changeState(int new_state) { state = new_state; }
int	DisplayManager::getCh() { return (wgetch(mainwin)); }
void	DisplayManager::refresh() { wrefresh(mainwin); }
