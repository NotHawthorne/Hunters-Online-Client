#include "../../includes/display.h"

Menu::Menu(WINDOW *sub) { win = sub; }
Menu::Menu() { win = NULL; }
Menu::~Menu() { }

static char	*unpad(char *s)
{
	int last = 0;
	for (int i = 0; s[i]; i++)
		if (s[i] != ' ' && (s[i] + 1))
			last = i + 1;
	if (last > 0)
		s[last] = 0;
	return (s);
}

int	Menu::handleInput(int c)
{
	switch (c)
	{
		case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			break ;
		case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			break ;
		default:
			break ;
	}
	return (1);
}

int	LoginMenu::handleInput(int c)
{
	std::hash<std::string>	hasher;
	switch (c)
	{
		case KEY_DOWN:
			menu_driver(menu, REQ_DOWN_ITEM);
			break ;
		case KEY_UP:
			menu_driver(menu, REQ_UP_ITEM);
			break ;
		case KEY_F(1):
			return (0);
			break ;
		case KEY_BACKSPACE:
			form_driver(form, REQ_DEL_CHAR);
			if (pass_field && pass_str.size())
				pass_str.erase(pass_str.size() - 1);
			break ;
		case '\t':
			form_driver(form, REQ_VALIDATION);
			form_driver(form, pass_field ? REQ_PREV_FIELD : REQ_NEXT_FIELD);
			pass_field = !pass_field;
			set_field_back(input[(int)pass_field], A_STANDOUT);
			set_field_back(input[(int)!pass_field], A_NORMAL);
			break ;
		case 10:
			form_driver(form, REQ_VALIDATION);
			if ((pass_field = !pass_field))
			{
				form_driver(form, REQ_NEXT_FIELD);
				set_field_back(input[0], A_NORMAL);
				set_field_back(input[1], A_STANDOUT);
			}
			else
			{
				
				user_str = std::string(unpad(field_buffer(input[0], 0)));
				pass_str = std::to_string(hasher(pass_str));
				return (2);
			}
			// login
			break ;
		case 127:
			form_driver(form, REQ_DEL_CHAR);
			if (pass_field && pass_str.size())
				pass_str.erase(pass_str.size() - 1);
			break ;
		default:
			form_driver(form, pass_field ? '*' : c);
			if (pass_field)
				pass_str += c;
			break ;
	}
	return (1);
}

LoginMenu::LoginMenu(WINDOW *sub)
{
	//containers for what seems to be scaling a form based
	//on a subwindow
	int	r, c;

	//set up some class variables
	type = LOGIN;
	win = sub;
	pass_field = false;
	itemcount = 3;

	// create the fields to go in to the form
	input[0] = new_field(1, (COLS / 2) - 2, 3, 1, 0, 0); 
	input[1] = new_field(1, (COLS / 2) - 2, 5, 1, 0, 0);
	input[2] = NULL;

	//set flags
	set_field_opts(input[0], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
	set_field_opts(input[1], O_VISIBLE | O_EDIT | O_ACTIVE | O_PUBLIC);

	//create form
	form = new_form(input);
	scale_form(form, &r, &c);
	set_form_win(form, sub);
	set_form_sub(form, (usrbox = derwin(sub, r, c, 1, 1)));
	keypad(usrbox, true);
	box(usrbox, 0, 0);
	set_field_back(input[0], A_STANDOUT);
	post_form(form);

	//display some information about the forms
	mvwprintw(sub, 3, 2, "username:\n");
	mvwprintw(sub, 5, 2, "password:\n");
	mvwprintw(sub, 1, 2, notification_strings[WELCOME_MSG]);
	refresh();

	//create the menu item array
	menu_items = new ITEM*[4];
	menu_items[0] = new_item(menu_item_strings[0], "");
	menu_items[1] = new_item(menu_item_strings[1], "");
	menu_items[2] = new_item(menu_item_strings[2], "");
	menu_items[3] = NULL;

	//create and style the menu
	menu = new_menu(menu_items);
	set_menu_win(menu, sub);
	set_menu_sub(menu, derwin(sub, 3, 15, 8, 1));
	set_menu_mark(menu, ">");
	post_menu(menu);

	//final refresh
	wrefresh(usrbox);
	wrefresh(sub);
}

LoginMenu::~LoginMenu()
{
	unpost_form(form);
	free_form(form);
	free_field(input[0]);
	free_field(input[1]);
	for (int i = 0; i != itemcount; i++)
		free_item(menu_items[i]);
	free_menu(menu);
}

void	LoginMenu::update()
{
	wrefresh(win);
	wrefresh(usrbox);
}
