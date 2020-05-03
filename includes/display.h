/*
** MAIN HUNTERS ONLINE DISPLAY AND USER INTERFACE DRIVER
** note to future self: look into clrtoeol() function of ncurses
*/

#ifndef DISPLAY_H
# define DISPLAY_H

# include <menu.h>
# include <sqlite3.h>
# include <map>
# include <string>
# include <signal.h>
# include <fcntl.h>
# include <curses.h>
# include <form.h>
# include <vector>
# include "packet.h"

enum notification_string_types
{
	WELCOME_MSG,
	VERIFY_MSG,
	DISCONNECT_MSG,
	INVALID_USER_MSG,
	INVALID_PASS_MSG,
	SERVER_DOWN_MSG
};

static const char *notification_strings[] = {
	"Welcome to Hunters Online v0.2.0!",
	"Please verify that you wish to register a new account.",
	"Disconnected from server.",
	"Invalid username.",
	"Invalid password.",
	"Server down."
};

static const char *menu_item_strings[] = {
	"Enter",
	"Register",
	"Exit"
};

enum	menu_types
{
	LOGIN,
	NOTIFY,
	MAIN_WIN,
	CHARACTER_LIST
};

enum	display_states
{
	DISPLAY_LOGIN,
	DISPLAY_REGISTER,
	DISPLAY_ERROR,
	DISPLAY_VERIFY,
	DISPLAY_WELCOME,
	DISPLAY_INVENTORY,
	DISPLAY_EQUIPMENT,
	DISPLAY_PLAYERS,
	DISPLAY_AUCTIONS,
	DISPLAY_SKILLS,
	DISPLAY_STATS,
	DISPLAY_WORLD
};

class	Menu
{
public:
	ITEM	**menu_items;
	MENU	*menu;
	ITEM	*cur_item;
	WINDOW	*win;
	PacketManager	*packet_man;
	int		type;
	int		itemcount;

	Menu(WINDOW *sub);
	Menu();
	~Menu();

	void	update();
	int		handleInput(int c);
};

class	LoginMenu : public Menu
{
private:
	FIELD		*input[3];
	FORM		*form;
	bool		pass_field;
public:
	WINDOW	*usrbox;
	std::string	pass_str;
	std::string	user_str;
	LoginMenu(WINDOW *sub);
	~LoginMenu();
	void		update();
	int			handleInput(int c);
};

class	ItemMenu : public Menu
{
public:
	int			selection;

	ItemMenu(WINDOW *sub);
	~ItemMenu();
	void		update();
	int			handleInput(int c);
	int			addItem(char *str);
};

class	GameMenu : public Menu
{
public:
	Menu		*display_port;
	WINDOW		*display_container;
	WINDOW		*player_card;
	WINDOW		*log;
	WINDOW		*chatbox;
	int			focus;

	GameMenu(WINDOW *sub);
	~GameMenu();

	void		update();
	int			handleInput(int c);
};

// should construct output packets for session manager
class	DisplayManager
{
private:
	int		state;
	int		last_state;
	WINDOW	*mainwin;
public:
	Menu	*curmenu;
	std::vector<Menu *> test;
	PacketManager	*packet_man;
	DisplayManager();
	~DisplayManager();
	void	update();
	void	refresh();
	void	changeState(int new_state);
	int		getCh();
};

#endif
