#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "menu.h"

// --- Saisie sécurisée ---
void delete_buffer()
{
	int c = 0;
	while(c != '\n' && c != EOF) c = getchar();

	return;
}

int scanf2(char *str, int len)
{
	char *posEnter = NULL;
	if(fgets(str,len,stdin) != NULL)
	{
		posEnter = strchr(str,'\n');
		if(posEnter != NULL) *posEnter = '\0';
		else delete_buffer();
		return 1;
	}
	else
	{
		delete_buffer();
		return 0;
	}
}

long scanfLong(int len)
{
	char strNum[100] = {0};
	if(scanf2(strNum,len)) return strtol(strNum,NULL,10);
	else return 0;
}

double scanfDouble(int len)
{
	char strNum[100] = {0};
	if(scanf2(strNum,len)) return strtod(strNum,NULL);
	else return 0;
}
// --- MENU ---
menu_t* createMenu(const char* text)
{
	menu_t* menu = (menu_t*) malloc(sizeof(menu_t));
	if(menu == NULL)
	{
		printf("/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	menu->parent = NULL;
	menu->desc = (char*) text;
	for(int i = 0; i<9; i++) menu->ens[i].type = T_VOID;
	return menu;
}

void addMenuAction(menu_t* m, const char* text, void(*f)())
{
	item_t a;
	a.type = ACTION;
	a.uni_t.act = (action_t*) malloc(sizeof(action_t));
	a.uni_t.act->fct = (void*) malloc(sizeof(void*));
	if(a.uni_t.act == NULL || a.uni_t.act->fct == NULL)
	{
		printf("/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	a.uni_t.act->desc = (char*) text;
	a.uni_t.act->fct = *f;
	for(int i = 0; i<9; i++)
	{
		if(m->ens[i].type == T_VOID)
		{
			m->ens[i] = a;
			break;
		}
	}
}

void addSubMenu(menu_t* m, menu_t* sm)
{
	if(sm->parent == NULL)
	{
		for(int i = 0; i<9; i++)
		{
			if(m->ens[i].type == T_VOID)
			{
				m->ens[i].type = SUBMENU;
				m->ens[i].uni_t.sub = sm;
				m->ens[i].uni_t.sub->parent = m;
				break;
			}
		}
	}
	else
	{
		printf("/!\\ Le sous-menu '%s' possède déjà un parent !\n",sm->desc);
		exit(1);
	}
}

void deleteMenu(menu_t* m)
{
	if(m->parent == NULL)
	{
		for(int i = 0; i<9; i++)
		{
			if(m->ens[i].type == ACTION)
			{
				m->ens[i].uni_t.act->fct = NULL;
				free(m->ens[i].uni_t.act);
				m->ens[i].type = T_VOID;
			}
			else if(m->ens[i].type == SUBMENU)
			{
				m->ens[i].uni_t.sub->parent = NULL;
				deleteMenu(m->ens[i].uni_t.sub);
				m->ens[i].type = T_VOID;
			}
			else
			{
				continue;
			}
		}
	}
	free(m);
}

void launchMenu(menu_t* m)
{
	system("clear");
	int user = 0;
	fprintf(stdout,"%s\n\n",m->desc);
	int i = 0;
	while(i<9)
	{
		if(m->ens[i].type == ACTION) fprintf(stdout," %d - %s\n",i+1,m->ens[i].uni_t.act->desc);
		else if(m->ens[i].type == SUBMENU) fprintf(stdout," %d - %s\n",i+1,m->ens[i].uni_t.sub->desc);
		else break;
		i++;
	}
	printf("\n");
	while(user < 1 || user > i)
	{
		printf("Choix: ");
		user = (int) scanfLong(3);
		if(user < 1 || user > i) fprintf(stdout,"/!\\ Erreur lors de la saisie.\n==========\n");
	}
	if(m->ens[user-1].type == ACTION)
	{
		(*m->ens[user-1].uni_t.act->fct)();
	}
	else if(m->ens[user-1].type == SUBMENU)
	{
		launchMenu(m->ens[user-1].uni_t.sub);
	}
	else
	{
		fprintf(stderr,"/!\\ Cette action n'existe pas !\n==========\n");
	}
}
