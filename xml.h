#ifndef XML_H
#define XML_H

typedef struct xattribute_s
{
	char* name;
	char* val;
	struct xattribute_s* next;
} xattribute_t;

typedef enum type_e
{
	NONE, RAW, SON
} enum_t;

typedef struct xelement_s
{
	enum_t type;
	char* name;
	xattribute_t* attributes;
	struct xelement_s* parent;
	struct xelement_s* bro;
	union
	{
		char* raw;
		struct xelement_s* son;
	} content;
} xelement_t;

xelement_t* create_xelement(const char* name);
xattribute_t* add_xattribute(xelement_t* e, const char* name, const char* value);
void add_sub_xelement(xelement_t* e, xelement_t* s);
void add_raw(xelement_t* e, const char* r);
void delete_xelement(xelement_t* e);
void save_xelement(FILE* fd, xelement_t* e);
void save_xml(const char* fname, xelement_t* e);
void print_xelement(xelement_t* e);
// --- Lecture de caractères ---
char next_char(FILE* fd);
void check_next_char(FILE* fd, char c);
bool is_next_char(FILE* fd, char c, bool cons);
char* next_word(FILE* fd);
void check_next_word(FILE* fd, const char* w);
char* next_string(FILE* fd);
char* next_raw(FILE* fd);
// --- Fonctions fournies par l'intitulé du projet ---
void load_xelement_raw(FILE* fd, xelement_t* e);
void load_xelement_sub(FILE* fd, xelement_t* e);
void load_xelement_content(FILE* fd, xelement_t* e);
xelement_t* load_xelement(FILE* fd, const char* end_tag);
xelement_t* load_xml(const char* fname);

#endif