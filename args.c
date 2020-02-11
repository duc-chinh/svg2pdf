#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "args.h"

option_t* opt_void(option_t* l, const char* kw, void (*f)())
{
	option_t* opt = (option_t*) malloc(sizeof(option_t));
	if(opt == NULL)
	{
		fprintf(stderr,"/!\\Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	else
	{
		opt->keyword = kw;
		opt->spec = OptVoid;
		opt->fct.opt_void = f;
		opt->next = NULL;
	}
	option_t* opt_p = l;
	option_t* opt_n = NULL;
	while(opt_p != NULL)
	{
	opt_n = opt_p;
	opt_p = opt_p->next;
	}
	opt_n->next = opt;

	return l;
}

option_t* opt_int(option_t* l, const char* kw, void (*f)(int))
{
	option_t* opt = (option_t*) malloc(sizeof(option_t));
	if(opt == NULL)
	{
		fprintf(stderr,"/!\\Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	else
	{
		opt->keyword = kw;
		opt->spec = OptInt;
		opt->fct.opt_int = f;
		opt->next = NULL;
	}
	option_t* opt_p = l;
	option_t* opt_n = NULL;
	while(opt_p != NULL)
	{
		opt_n = opt_p;
		opt_p = opt_p->next;
	}
	opt_n->next = opt;

	return l;
}

option_t* opt_float(option_t* l, const char* kw, void (*f)(float))
{
	option_t* opt = (option_t*) malloc(sizeof(option_t));
	if(opt == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	else
	{
		opt->keyword = kw;
		opt->spec = OptFloat;
		opt->fct.opt_float = f;
		opt->next = NULL;
	}
	option_t* opt_p = l;
	option_t* opt_n = NULL;
	while(opt_p != NULL)
	{
		opt_n = opt_p;
		opt_p = opt_p->next;
	}
	opt_n->next = opt;

	return l;
}

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*))
{
	option_t* opt = (option_t*) malloc(sizeof(option_t));
	if(opt == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	else
	{
		opt->keyword = kw;
		opt->spec = OptString;
		opt->fct.opt_str = f;
		opt->next = NULL;
	}
	if (l == NULL)
	{
		l = opt;
	}
	else{
		option_t* opt_p = l;
		while(opt_p->next != NULL) opt_p =opt_p->next;
		opt_p->next = opt;
	}

	return l;
}

void opt_delete(option_t* l)
{
	if(l != NULL)
	{
		opt_delete(l->next);
		free(l);
	}

	return;
}

void process_options(option_t* l, int argc, char* *argv)
{
	option_t* opt;
	int i;

	for(i = 1; i < argc; i++){
 		opt = l;
		while(strcmp(opt->keyword,argv[i]) != 0 && (opt != NULL)) opt = opt->next;
		if(opt->spec == OptVoid)
		{
			opt->fct.opt_void();
		}
		else if(opt->spec == OptInt){
			i++;
			opt->fct.opt_int(strtol(argv[i],NULL,10));
		}
		else if(opt->spec == OptString){
			i++;
			opt->fct.opt_str(argv[i]);
		}
		else if(opt->spec== OptFloat){
			i++;
			opt->fct.opt_float(strtod(argv[i],NULL));
		}
		sleep(1);
	}

	return;
}
