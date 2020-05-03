#include "../../includes/game.h"

static int	aura_load_callback(void *d, int argc, char **argv, char **colname)
{
	if (!argv || !colname)
		return (1);
	Aura	*a = new Aura(argv);
	GameManager	*m = (GameManager *)d;
	m->auras.insert(std::pair<int, Aura *>(a->id, a));
	return (0);
}

static int	item_base_load_callback(void *d, int argc, char **argv, char **colname)
{
	if (!argv || !colname)
		return (1);
	ItemBase	*ib = new ItemBase(argv);
	GameManager	*m = (GameManager *)d;
	m->item_base.insert(std::pair<int, ItemBase *>(ib->id, ib));
	return (0);
}

GameManager::GameManager()
{
	int rc = sqlite3_open("./info.db", &db);
	int ret = 0;
	if (rc)
		printf("db init failed\n");
	ret = sqlite3_exec(db, "SELECT * FROM item_base;", item_base_load_callback, (void*)this, NULL); 
	if (ret != SQLITE_OK)
		printf("item load failed\n");
	ret = sqlite3_exec(db, "SELECT * FROM auras;", aura_load_callback, (void*)this, NULL); 
	if (ret != SQLITE_OK)
		printf("item load failed\n");
}

GameManager::~GameManager()
{
	characters.clear();
	inventory.clear();
	equipment.clear();
	item_base.clear();
	auras.clear();
	container.clear();
	//auctions.clear();
	//sqlite3 close
}

int	GameManager::getCharList(t_packet_header *h)
{
	characters.clear();
	for (int i = 0; i != h->amt && !packet_in->q.empty(); i++)
	{
		Packet	*p = packet_in->q.front();
		Character	*c = new Character((t_character_packet*)p->data);
		packet_in->q.pop();
		characters.insert(std::pair<int, Character*>(characters.size(), c));
	}
	printf("GOT CHAR LIST\n");
	return (0);
}

int	GameManager::emptyQueue()
{
	for (int i = 0; i != packet_in->q.size(); i++)
	{
		Packet	*p = packet_in->q.front();
		t_packet_header	*h = (t_packet_header*)p->data;
		packet_in->q.pop();
		switch (h->cmd)
		{
			case SERVER_LOGIN_FAIL:
				return (-1);
				break;
			case SERVER_LOGIN_SUCCESS:
				getCharList((t_packet_header*)p->data);
				break ;
		}
	}
	return (0);
}
