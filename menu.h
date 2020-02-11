#ifndef MENU_H
#define MENU_H

typedef enum type_menu
{
	T_VOID, SUBMENU, ACTION
} enum_menu;

typedef struct action
{
	char* desc;
	void (*fct)();
} action_t;

typedef struct item
{
	enum_menu type;
	union
	{
		struct menu* sub;
		action_t* act;
	} uni_t;
} item_t;

typedef struct menu menu_t;
struct menu
{
	menu_t* parent;
	char* desc;
	item_t ens[9];
};

// --- Saisie sécurisée ---
void delBuffer();
int scanf2(char* str,int len);
long scanfLong(int len);
double scanfDouble(int len);
// --- MENU ---
menu_t* createMenu(const char* text);
void addMenuAction(menu_t* m, const char* text, void(*f)());
void addSubMenu(menu_t* m, menu_t* sm);
void deleteMenu(menu_t* m);
void launchMenu(menu_t* m);

#endif