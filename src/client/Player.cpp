#include "../../includes/player.h"

Character::Character(t_character_packet *p)
{
	bzero(name, 16);
	memcpy(name, p->name, strlen(p->name));
	pclass = p->pclass;
	race = p->race;
	lvl = p->level;
	hp = p->hp;
	max_hp = p->max_hp;
	mana = p->mana;
	max_mana = p->max_mana;
}
