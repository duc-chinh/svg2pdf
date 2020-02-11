#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "xml.h"

xelement_t* create_xelement(const char* name)
{
	xelement_t* xel = (xelement_t*) malloc(sizeof(xelement_t));
	int len = strlen(name);
	(*xel).name = (char*) malloc(len*sizeof(char));
	if(xel == NULL || (*xel).name == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire !\nFermeture du programme.\n");
		exit(1);
	}
	else
	{
		strcpy((*xel).name,name);
		(*xel).attributes = NULL;
		(*xel).parent = NULL;
		(*xel).bro = NULL;
		(*xel).content.raw = NULL;
		(*xel).type = NONE;
	}
	return xel;
}

xattribute_t* add_xattribute(xelement_t* e, const char* name, const char* value)
{
	xattribute_t* xat = (xattribute_t*) malloc(sizeof(xattribute_t));
	int len = strlen(name)+1;
	(*xat).name = (char*) malloc(len*sizeof(char));
	len = strlen(value)+1;
	(*xat).val = (char*) malloc(len*sizeof(char));
	if(xat == NULL || (*xat).name == NULL || (*xat).val == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire !\nFermeture du programme.\n");
		exit(1);
	}
	strcpy((*xat).name,name);
	strcpy((*xat).val,value);
	(*xat).next = NULL;
	if((*e).attributes == NULL)
	{
		(*e).attributes = xat;
	}
	else if(strcmp((*e).attributes->name,name) == 0)
	{
		(*xat).next = (*e).attributes->next;
		(*e).attributes = xat;
	}
	else
	{
		xattribute_t* tmp = (*e).attributes;
		xattribute_t* pt_tmp = (*e).attributes;
		bool same = false;
		while(tmp != NULL)
		{
			if(strcmp((*tmp).name,name) == 0)
			{
				(*xat).next = (*tmp).next;
				(*pt_tmp).next = xat;
				free(tmp);
				same = true;
				break;
			}
			pt_tmp = tmp;
			tmp = (*tmp).next;
		}
		if(same == false) (*pt_tmp).next = xat;
	}
	return (*e).attributes;
}

void add_sub_xelement(xelement_t* e, xelement_t* s)
{
	if(s->parent == NULL && ((e->parent == NULL && e->bro == NULL && e->type == NONE) || e->type == SON))
	{
		(*e).type = SON;
		(*s).parent = e;
		if((*e).content.son == NULL)
		{
			(*e).content.son = s;
		}
		else
		{
			xelement_t* tmp = (*e).content.son;
			while((*tmp).bro != NULL) tmp = (*tmp).bro;
			(*tmp).bro = s;
		}
		return;
	}
	else
	{
		if(s->parent != NULL) fprintf(stderr,"/!\\ Le sous-élément '%s' possède déjà un parent ! Fermeture du programme.\n",(*s).name);
		else fprintf(stderr,"/!\\ L'élément '%s' n'est pas vide ! Fermeture du programme.\n",(*e).name);
		exit(1);
	}
}

void add_raw(xelement_t* e, const char* r)
{
	if((e->parent == NULL && e->bro == NULL && e->type == NONE) || (*e).type == RAW)
	{
		if((*e).content.raw != NULL)
		{
			free((*e).content.raw);
			(*e).type = NONE;
		}
		int len = strlen(r)+1;
		(*e).content.raw = (char*) malloc(len*sizeof(char));
		if((*e).content.raw == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
		strcpy((*e).content.raw,r);
		(*e).type = RAW;
		return;
	}
	else
	{
		if(e->type != RAW) fprintf(stderr,"/!\\ L'élément '%s' ne contient pas déjà de contenu brut ! Fermeture du programme.\n",(*e).name);
		else fprintf(stderr,"/!\\ L'élément '%s' n'est pas vide ! Fermeture du programme.\n",(*e).name);
		exit(1);
	}
}

void delete_xelement(xelement_t* e)
{
	if((*e).parent == NULL)
	{
		if((*e).type == RAW)
		{
			free((*e).content.raw);
			(*e).type = NONE;
		}
		else if((*e).type == SON)
		{
			xelement_t* tmp = (*e).content.son;
			while((*tmp).parent != NULL)
			{
				(*tmp).parent = NULL;
				tmp = (*tmp).bro;
				if(tmp == NULL) break;
			}

			(*e).type = NONE;
		}	
			
		if((*e).attributes != NULL)
		{
			xattribute_t* tmp = (*e).attributes;
			xattribute_t* next;
			while(tmp != NULL)
			{
				next = (*tmp).next;
				free((*tmp).val);
				free((*tmp).name);
				free(tmp);
				tmp = next;
			}
		}
		free((*e).name);
		free(e);
	}
	else
	{
		fprintf(stderr,"/!\\ Suppression impossible ! L'élément '%s' possède un parent.\n",(*e).name);
		exit(1);
	}
	return;
}

void save_xelement(FILE* fd, xelement_t* e)
{
	if((*e).parent != NULL)
	{
		xelement_t* tmp = e;
		while((*tmp).parent != NULL)
		{
			fprintf(fd,"   "); // Décalage avant la balise ouvrante
			tmp = (*tmp).parent;
		}
	}
	fprintf(fd,"<%s",(*e).name);
	if((*e).attributes != NULL)
	{
		xattribute_t* tmp = (*e).attributes;
		while(tmp != NULL)
		{
			fprintf(fd," %s=\"%s\"",(*tmp).name,(*tmp).val);
			tmp = (*tmp).next;
		}
	}
	if((*e).type == NONE)
	{
		fprintf(fd,"/>\n");
	}
	else
	{
		fprintf(fd,">");
		if((*e).type == RAW)
		{
			fprintf(fd,"%s</%s>\n",(*e).content.raw,(*e).name);
		}
		else
		{
			fprintf(fd,"\n");
			xelement_t* tmp = (*e).content.son;
			while(tmp != NULL)
			{
				save_xelement(fd,tmp);
				tmp = (*tmp).bro;
			}
			if((*e).parent != NULL)
			{
				xelement_t* tmp = e;
				while((*tmp).parent != NULL)
				{
					fprintf(fd,"   "); // Décalage avant la balise fermante
					tmp = (*tmp).parent;
				}
			}
			fprintf(fd,"</%s>\n",(*e).name);
		}
	}
	return;
}

void save_xml(const char* fname, xelement_t* e)
{
	FILE* fp = NULL;
	fp = fopen(fname,"w");
	if(fp == NULL)
	{
		fprintf(stderr,"/!\\ Impossible de sauvegarder '%s'.\nFermeture du programme.\n",fname);
		exit(1);
	}
	else
	{
		//fprintf(fp,"<?xml version=\"1.0\"?>\n");
		save_xelement(fp,e);
		fclose(fp);
	}
	return;
}

void print_xelement(xelement_t* e)
{
	save_xelement(stdout,e);
	return;
}

char next_char(FILE* fd)
{
	char c = fgetc(fd);
	while(c == ' ' || c == '\n' || c == '\r' || c == '\t')
	{
		c = fgetc(fd);
		if(feof(fd)) exit(1);
	}
	return c;
}

void check_next_char(FILE* fd, char c)
{
	if(next_char(fd) != c) exit(1);
	else return;
}

bool is_next_char(FILE* fd, char c, bool cons)
{
	char s = next_char(fd);
	bool same = false;
	if(s == c) same = true;
	if(s != c || cons == false) ungetc(s,fd);
	return same;
}

char* next_word(FILE* fd)
{
	char* str = malloc(40*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire !\nFermeture du programme.\n");
		exit(1);
	}
	else
	{
		char c = 0;
		int i = 0;
		while((c = fgetc(fd)))
		{
			if(c == '<' || c == '>' || c == '/' || c == '=' || (c == ' ' && str[i-1] >= 31 && str[i-1] <= 126))
			{
				ungetc(c,fd);
				str[i] = '\0';
				break;
			}
			else if(c == ' ')
			{
				continue;
			}
			else if(feof(fd))
			{
				exit(1);
			}
			str[i] = c;
			i++;
		}
		if(i == 0) return NULL;
		else return str;
	}
}

void check_next_word(FILE* fd, const char* w)
{
	char* str = next_word(fd);
	if(strcmp(str,w) != 0)
	{
		free(str);
		exit(1);
	}
	free(str);
	return;
}

char* next_string(FILE* fd)
{
	char* str = (char*) malloc(140*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire !\nFermeture du programme.\n");
		exit(1);
	}
	else
	{
		char c = 0;
		int i = 0;
		int cpt = 0;
		while((c = fgetc(fd)))
		{
			if(c == '"' && (cpt != 2))
			{
				cpt++;
				continue;
			}
			if(cpt == 2)
			{
				ungetc(c,fd);
				str[i] = '\0';
				break;
			}
			str[i] = c;
			i++;
		}
		if(cpt == 0) exit(1);
		return str;
	}
}

char* next_raw(FILE* fd)
{
	char* str = (char*) malloc(280*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire !\nFermeture du programme.\n");
		exit(1);
	}
	else
	{
		char c = 0;
		int i = 0;
		while((c = fgetc(fd)))
		{
			if(c == '<')
			{
				ungetc(c,fd);
				str[i] = '\0';
				break;
			}
			str[i] = c;
			i++;
		}
		return str;
	}
}

// Fonctions fournies dans l'intitulé du projet
xelement_t* load_xelement(FILE* fd, const char* end_tag);
 
void load_xelement_raw(FILE* fd, xelement_t* e)
{
 	char* w = next_raw(fd);
  	check_next_char(fd, '<');
  	check_next_char(fd, '/');
  	check_next_word(fd, e->name);
  	check_next_char(fd, '>');
  	add_raw(e,w);
  	free(w);
}
 
void load_xelement_sub(FILE* fd, xelement_t* e)
{
  	xelement_t* f = load_xelement(fd, e->name);
  	if(f != NULL)
  	{
    	add_sub_xelement(e,f);
    	load_xelement_sub(fd, e);
  	}
}
 
void load_xelement_content(FILE* fd, xelement_t* e) 
{
  	if(is_next_char(fd,'<',false)) load_xelement_sub(fd, e);
  	else load_xelement_raw(fd, e);
}
 
xelement_t* load_xelement(FILE* fd, const char* end_tag)
{
	xelement_t* e = NULL;
	char c;
 
	check_next_char(fd,'<');
 
	if((end_tag) && (is_next_char(fd,'/',true)))
	{
    	check_next_word(fd,end_tag);
    	check_next_char(fd,'>');
    	return NULL;
  	}
 
	char* name = next_word(fd);
	if(name == NULL)
	{
    	fprintf(stderr, "load_xelement: tag name expected\n");
    	exit(1);
	}
	e = create_xelement(name);
	free(name);
 
	while((name = next_word(fd)) != NULL)
	{
    	check_next_char(fd,'=');
    	char* value = next_string(fd);
    	add_xattribute(e,name,value);
	}
	c = next_char(fd);
 
	if(c == '/')
	{
    	check_next_char(fd,'>');
    	return e;
  	}
 
  	if(c == '>')
  	{
    	load_xelement_content(fd, e);
    	return e;
  	}
 	int len = ftell(fd);
  	fprintf(stderr, "load_xelement: end of markup expected ('>' or '/>'), but got %c (char n°%d)\n", c,len);
  	exit(1);
}
 
xelement_t* load_xml(const char* fname)
{
  	FILE* fd = fopen(fname, "r");
  	xelement_t* e = load_xelement(fd,NULL);
  	fclose(fd);
  	return e;
}
