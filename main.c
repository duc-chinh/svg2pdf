#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#include "args.h"
#include "menu.h"
#include "xml.h"
#include "pdf.h"
#include "svg.h"

char* fname = NULL;
xelement_t* svg = NULL;
image_t* img = NULL;

void main_open()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Ouvrir\n\n");
	if(fname != NULL)
	{
		int user = 0;
		while(user < 1 || user > 2)
		{
			fprintf(stderr,"/!\\ Le fichier '%s' est déjà ouvert !\n\n",fname);
			fprintf(stdout,"1 - Menu principal\n2 - Fermer le fichier (sans sauvegarder)\n\n");
			fprintf(stdout,"Choix: ");
			user = scanfLong(3);
			if(user < 1 || user > 2) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		}
		if(user == 1) return;
		fprintf(stdout,"==========\nFermeture du fichier...\n");
		sleep(1);
		delete_xelement(svg);
		image_delete(img);
		fprintf(stdout,"Fichier '%s' fermé.\n",fname);
		free(fname);
		fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
		getchar();
		system("clear");
		fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
		fprintf(stdout,"Ouvrir\n\n");
	}
	fname = (char*) malloc(100*sizeof(char));
	char* ext = NULL;
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	bool same = false;
	
	while(same == false)
	{
		fprintf(stdout,"Saisir le nom du fichier SVG: ");
		scanf2(fname,100);
		ext = strchr(fname,'.');
		if(ext != NULL && strcmp(ext,".svg") == 0) same = true;
		else fprintf(stderr,"/!\\ Le nom saisi n'est pas un fichier SVG !\n==========\n");
	}
	
	svg = svg_load(fname);
	img = image_convert(svg);
	fprintf(stdout,"==========\nOuverture du fichier...\n");
	sleep(1);
	fprintf(stdout,"Fichier '%s' ouvert.\n",fname);
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();

	return;
}

void main_save()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Enregistrer\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else
	{
		int cpt = 0;
		char* ext = NULL; bool same = false;
		// Choix de l'extension
		while(cpt < 1 || cpt > 2)
		{
			fprintf(stdout,"Enregistrer sous...\n\n");
			fprintf(stdout,"1 - SVG\n2 - PDF\n\n");
			fprintf(stdout,"Choix: ");
			cpt = scanfLong(3);
			if(cpt < 1 || cpt > 2) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		}
		if(cpt == 1) // En SVG
		{
			cpt = 0;
			while(cpt < 1 || cpt > 2)
			{
				fprintf(stdout,"==========\nConserver le nom actuel ?\n\n");
				fprintf(stdout,"1 - Oui\n2 - Non\n\n");
				fprintf(stdout,"Choix ? ");
				cpt = scanfLong(3);
				if(cpt < 1 || cpt > 2) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n");
			}
			if(cpt == 2)
			{
				while(same == false)
				{
					fprintf(stdout,"==========\nSaisir le nouveau nom SVG: ");
					scanf2(fname,100);
					ext = strchr(fname,'.');
					if(ext != NULL && strcmp(ext,".svg") == 0) same = true;
					else fprintf(stderr,"/!\\ Le nom saisi n'a pas pour extension '.svg' !\n==========");
				}
			}
			svg_save(svg,fname);
		}
		else if(cpt == 2) // En PDF
		{
			while(same == false)
			{
				fprintf(stdout,"==========\nSaisir le nom PDF: ");
				scanf2(fname,100);
				ext = strchr(fname,'.');
				if(ext != NULL && strcmp(ext,".pdf") == 0) same = true;
				else fprintf(stderr,"/!\\ Le nom saisi n'a pas pour extension '.pdf' !\n==========");
			}
			pdf_t* p = pdf_convert(svg);
			pdf_save(fname,p);
			free(p);
		}
		fprintf(stdout,"==========\nSauvegarde en cours...\n");
		sleep(1);
		fprintf(stdout,"Fichier '%s' sauvegardé.\n",fname);
	}
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();

	return;
}

void main_info()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Information\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else
	{
		fprintf(stdout,"Nom du fichier: '%s'\n",fname);
		xattribute_t* a = (*svg).attributes;
		while(a != NULL)
		{
			if(strcmp((*a).name,"width") == 0) fprintf(stdout,"Longueur de l'image: %s px\n",(*a).val);
			else if(strcmp((*a).name,"height") == 0) fprintf(stdout,"Hauteur de l'image: %s px\n",(*a).val);

			a  = (*a).next;
		}
		int nb = 0;
		fprintf(stdout,"Nombre de figures: %d\n",nb_figures(svg,nb));
		nb = 0;
		fprintf(stdout,"Nombre de groupes: %d\n",nb_groups(svg,nb));
		if(nb_figures(svg,nb) != 0)
		{
			fprintf(stdout,"==========\nListe des figures:\n\n");
			menu_figures(svg,0);
		}
		fprintf(stdout,"==========\n");
		fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
		getchar();
	}
	return;
}

void main_display()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Affichage\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else
	{
		fprintf(stdout,"/!\\ L'image n'affiche ni les textes, ni les images présents dans le fichier SVG.\n\n");
		sleep(1);
		paint(img);
	}
	fprintf(stdout,"==========\n");
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

bool cont = true;

void main_quit()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Quitter\n\n");
	int user = 0;
	while(user < 1 || user > 2)
	{
		fprintf(stdout,"/!\\ Toute progression non sauvegardée sera perdue. Êtes-vous sûr(e) de vouloir quitter ?\n\n");
		fprintf(stdout,"1 - Oui\n2 - Non\n\n");
		fprintf(stdout,"Choix: ");
		user = scanfLong(3);
		if(user < 1 || user > 2) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
	}
	if(user == 1)
	{
		fprintf(stdout,"Fermeture du programme...\n");
		sleep(1);
		system("clear");
		cont = false;
	}

	return;
}

void main_back()
{
	return;
}

void main_add_circle()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Ajouter cercle\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else
	{
		int r = 0;
		char* color = (char*) malloc(10*sizeof(char));
		if(color == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}

		// Saisie du rayon
		while(r <= 0)
		{
			fprintf(stdout,"Saisir le rayon du cercle (en px): ");
			r = scanfLong(3);
			if(r <= 0) fprintf(stderr,"/!\\ Le rayon ne peut être inférieur ou égal à 0.\n==========\n");
		}

		// Saisie de la couleur
		while(1)
		{
			char* c = (char*) malloc(10*sizeof(char));
			if(c == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
			fprintf(stdout,"==========\nSaisir la couleur du cercle: ");
			scanf2(c,10);
			if(svg_color(c) == true)
			{
				strcpy(color,c);
				free(c);
				break;
			}
			else(free(c));
		}
		xelement_t* circ = svg_circle(0,0,r,color);
		add_sub_xelement(svg,circ);
		free(color);
		image_delete(img);
		img = image_convert(svg);
		fprintf(stdout,"==========\nLe cercle a été ajouté.\n");
	}
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_add_rect()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Ajouter rectangle\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else
	{
		int w = 0; int h = 0;
		char* color = (char*) malloc(10*sizeof(char));
		if(color == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}

		// Saisie de la longueur
		while(w <= 0)
		{
			fprintf(stdout,"Saisir la longueur du rectangle (en px): ");
			w = scanfLong(3);
			if(w <= 0) fprintf(stderr,"/!\\ La longueur ne peut être inférieure ou égale à 0.\n==========\n");
		}

		// Saisie de la hauteur
		while(h <= 0)
		{
			fprintf(stdout,"==========\nSaisir la hauteur du rectangle (en px): ");
			h = scanfLong(3);
			if(h <= 0) fprintf(stderr,"/!\\ La hauteur ne peut être inférieure ou égale à 0.\n==========\n");
		}

		// Saisie de la couleur
		while(1)
		{
			char* c = (char*) malloc(10*sizeof(char));
			if(c == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
			fprintf(stdout,"==========\nSaisir la couleur du rectangle: ");
			scanf2(c,10);
			if(svg_color(c) == true)
			{
				strcpy(color,c);
				free(c);
				break;
			}
			else(free(c));
		}
		xelement_t* rect = svg_rect(0,0,w,h,color);
		add_sub_xelement(svg,rect);
		free(color);
		image_delete(img);
		img = image_convert(svg);
		fprintf(stdout,"==========\nLe rectangle a été ajouté.\n");
	}
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_add_line()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Ajouter ligne\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else
	{
		int x1; int y1; int x2; int y2; int stroke = 0;
		char* color = (char*) malloc(10*sizeof(char));
		if(color == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}

		// Saisie des points
		do
		{
			fprintf(stdout,"Saisie des points de la ligne.\n");
			fprintf(stdout,"/!\\ Toute saisie autre qu'un nombre sera considérée comme 0.\n");
			fprintf(stdout,"Saisir le 1er pt d'abcisse x1 (en px): ");
			x1 = scanfLong(3);
			fprintf(stdout,"Saisir le 1er pt d'ordonnée y1 (en px): ");
			y1 = scanfLong(3);
			fprintf(stdout,"Saisir le 2ème pt d'abcisse x2 (en px): ");
			x2 = scanfLong(3);
			fprintf(stdout,"Saisir le 2ème pt d'ordonnée y2 (en px): ");
			y2 = scanfLong(3);
			if(sqrt(pow(x2-x1,2)+pow(y2-y1,2)) == 0) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		} while(sqrt(pow(x2-x1,2)+pow(y2-y1,2)) == 0);

		// Saisie de l'épaisseur
		while(stroke <= 0)
		{
			fprintf(stdout,"==========\nSaisir l'épaisseur de la ligne: ");
			stroke = scanfLong(3);
			if(stroke <= 0) fprintf(stderr,"/!\\ L'épaisseur ne peut être inférieure ou égale à 0.\n");
		}

		// Saisie de la couleur
		while(1)
		{
			char* c = (char*) malloc(10*sizeof(char));
			if(c == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
			fprintf(stdout,"==========\nSaisir la couleur de la ligne: ");
			scanf2(c,10);
			if(svg_color(c) == true)
			{
				strcpy(color,c);
				free(c);
				break;
			}
			else(free(c));
		}
		xelement_t* line = svg_line(x1,x2,y1,y2,color,stroke);
		add_sub_xelement(svg,line);
		free(color);
		image_delete(img);
		img = image_convert(svg);
		fprintf(stdout,"==========\nLa ligne a été ajoutée.\n");
	}
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_add_text()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Ajouter texte\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else
	{
		char* font = NULL; int size = 0;
		char* color = (char*) malloc(10*sizeof(char));
		char* str = (char*) malloc(280*sizeof(char));
		if(color == NULL || str == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
		int cpt = 0;
		
		// Saisie de la police
		while(cpt < 1 || cpt > 2)
		{
			fprintf(stdout,"La police du texte en...\n\n");
			fprintf(stdout,"1 - Times New Roman\n2 - Helvetica\n\n");
			fprintf(stdout,"Choix ? ");
			cpt = scanfLong(3);
			if(cpt < 1 || cpt > 2) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		}
		if(cpt == 1) asprintf(&font,"Times");
		else if(cpt == 2) asprintf(&font,"Helvetica");
		int type = 0;
		while(type < 1 || type > 4)
		{
			fprintf(stdout,"==========\nChoix du type de police:\n");
			fprintf(stdout,"1 - Normal\n2 - Gras\n3 - Italique\n4 - Gras & italique\n\n");
			fprintf(stdout,"Choix ? ");
			type = scanfLong(3);
			if(type < 1 || type > 4) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n");
		}
		if(cpt == 1 && type == 1) asprintf(&font,"%s-Roman",font);
		else if(type == 2) asprintf(&font,"%s-Bold",font);
		else if(cpt == 1 && type == 3) asprintf(&font,"%s-Italic",font);
		else if(cpt == 1 && type == 4) asprintf(&font,"%s-BoldItalic",font);
		else if(cpt == 2 && type == 3) asprintf(&font,"%s-Oblique",font);
		else if(cpt == 2 && type == 4) asprintf(&font,"%s-BoldOblique",font);

		// Saisie de la taille
		while(size <= 0)
		{
			fprintf(stdout,"==========\nSaisie de la taille de la police: ");
			size = scanfLong(3);
			if(size <= 0) fprintf(stderr,"/!\\ La taille ne doit pas être inférieure ou égale à 0.\n");
		}

		// Saisie de la couleur
		while(1)
		{
			char* c = (char*) malloc(10*sizeof(char));
			if(c == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
			fprintf(stdout,"==========\nSaisir la couleur de la ligne: ");
			scanf2(c,10);
			if(svg_color(c) == true)
			{
				strcpy(color,c);
				free(c);
				break;
			}
			else(free(c));
		}

		// Saisie du texte
		fprintf(stdout,"==========\nSaisie du texte à introduire: ");
		scanf2(str,280);

		xelement_t* text = svg_text(0,0,font,size,color,str);
		add_sub_xelement(svg,text);
		free(color); free(font); free(str);
		image_delete(img);
		img = image_convert(svg);
		fprintf(stdout,"==========\nLe texte a été ajouté.\n");
	}
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_add_image()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Ajouter image\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else
	{
		char* ext = NULL; bool same = false;
		char* href = (char*) malloc(140*sizeof(char));
		if(href == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}

		// Saisie de la référence
		while(same == false)
		{
			fprintf(stdout,"Saisie du nom de l'image JPG: ");
			scanf2(href,140);
			ext = strchr(href,'.');
			if(ext != NULL && strcmp(ext,".jpg") == 0) same = true;
			else fprintf(stderr,"/!\\ Le nom de l'image n'est pas un fichier JPG !\n==========\n");
		}

		// Récupération de la taille
		FILE* fp = NULL;
		fp = fopen(href,"rb");
		if(fp == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'ouvrir le fichier '%s'. Retour au menu principal...\n",href);
			sleep(2);
			return;
		}
		fseek(fp,0,SEEK_END);
		int len = ftell(fp);
		rewind(fp);
		char* jpg = (char*) malloc(len*sizeof(char));
		if(jpg == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
		fread(jpg,len,1,fp);
		fclose(fp);
		size_t w; size_t* ptr_w = &w;
		size_t h; size_t* ptr_h = &h;
		size_t bpc; size_t* ptr_bpc = &bpc;
		const char* cs; const char** ptr_cs = &cs;
		jpeg_info(jpg, ptr_w, ptr_h, ptr_bpc, ptr_cs);

		xelement_t* image = svg_image(0,0,w,h,"none",href);
		add_sub_xelement(svg,image);
		image_delete(img);
		img = image_convert(svg);
		fprintf(stdout,"==========\nL'image '%s' a été ajoutée.\n",href);
		free(jpg); free(href);
	}
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();

	return;
}

void main_delete_figure()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Supprimer\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else if(nb_figures(svg,0) == 0)
	{
		fprintf(stderr,"/!\\ Le fichier ne contient pas de figures !\n");
	}
	else
	{
		int cpt = 0;
		int user = 0;
		do
		{
			cpt = menu_figures(svg,cpt);
			fprintf(stdout,"\nChoix: ");
			user = scanfLong(3);
			if(user < 1 || user > cpt) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		} while(user < 1 || user > cpt);
		cpt = 0;
		delete_figure(svg,cpt,user);
		image_delete(img);
		img = image_convert(svg);	
	}
	fprintf(stdout,"==========\n");
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_translate()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Translater\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else if(nb_figures(svg,0) == 0)
	{
		fprintf(stderr,"/!\\ Le fichier ne contient pas de figures !\n");
	}
	else
	{
		int cpt = 0;
		int user = 0; int x = 0; int y = 0;
		do
		{
			cpt = menu_figures(svg,cpt);
			fprintf(stdout,"\nChoix: ");
			user = scanfLong(3);
			if(user < 1 || user > cpt) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		} while(user < 1 || user > cpt);
		cpt = 0;
		while(x <= 0)
		{
			fprintf(stdout,"==========\nSaisir le point d'abcisse de translation (en px): ");
			x = scanfLong(6);
			if(x <= 0) fprintf(stderr,"/!\\ Le point ne doit pas être inférieur ou égal à 0.\n==========\n");
		}
		while(y <= 0)
		{
			fprintf(stdout,"==========\nSaisir le point d'ordonnée de translation (en px): ");
			y = scanfLong(6);
			if(y <= 0) fprintf(stderr,"/!\\ Le point ne doit pas être inférieur ou égal à 0.\n==========\n");
		}
		char* trans = NULL;
		asprintf(&trans,"translate(%d %d)",x,y);
		transform_figure(svg,cpt,user,trans);
		image_delete(img);
		img = image_convert(svg);
		free(trans);
	}
	fprintf(stdout,"==========\n");
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_rotate()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Tourner\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else if(nb_figures(svg,0) == 0)
	{
		fprintf(stderr,"/!\\ Le fichier ne contient pas de figures !\n");
	}
	else
	{
		int cpt = 0;
		int user = 0; double x = 0;
		do
		{
			cpt = menu_figures(svg,cpt);
			fprintf(stdout,"\nChoix: ");
			user = scanfLong(3);
			if(user < 1 || user > cpt) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		} while(user < 1 || user > cpt);
		cpt = 0;
		while(x == 0 || x > 359.99 || x < -359.99)
		{
			fprintf(stdout,"==========\nSaisir l'angle de rotation: ");
			x = scanfDouble(10);
			if(x == 0 || x > 359.99 || x < -359.99) fprintf(stderr,"/!\\ L'angle doit être entre -359.99 et 359.99, et différent de 0.\n==========\n");
		}
		char* trans = NULL;
		asprintf(&trans,"rotate(%f)",x);
		transform_figure(svg,cpt,user,trans);
		image_delete(img);
		img = image_convert(svg);
		free(trans);
	}
	fprintf(stdout,"==========\n");
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_scale()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Etirer\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else if(nb_figures(svg,0) == 0)
	{
		fprintf(stderr,"/!\\ Le fichier ne contient pas de figures !\n");
	}
	else
	{
		int cpt = 0;
		int user = 0; double x = 0;
		do
		{
			cpt = menu_figures(svg,cpt);
			fprintf(stdout,"\nChoix: ");
			user = scanfLong(3);
			if(user < 1 || user > cpt) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		} while(user < 1 || user > cpt);
		cpt = 0;
		while(x == 0)
		{
			fprintf(stdout,"==========\nSaisir le ratio d'étirement: ");
			x = scanfDouble(10);
			if(x == 0) fprintf(stderr,"/!\\ Le ratio doit être différent de 0.\n==========\n");
		}
		char* trans = NULL;
		asprintf(&trans,"scale(%f)",x);
		transform_figure(svg,cpt,user,trans);
		image_delete(img);
		img = image_convert(svg);
		free(trans);
	}
	fprintf(stdout,"==========\n");
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_group()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Grouper\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else if(nb_figures(svg,0) == 0)
	{
		fprintf(stderr,"/!\\ Le fichier ne contient pas de figures !\n");
	}
	else
	{
		xelement_t* g = create_xelement("g");
		while(1)
		{
			int cpt = 0; int user = 0;
			do
			{
				fprintf(stdout,"Sélectionner la/les figure(s) à ajouter dans un groupe:\n\n");
				cpt = menu_figures(svg,cpt);
				fprintf(stdout,"%d - Fin de sélection\n",cpt+1);
				fprintf(stdout,"\nChoix: ");
				user = scanfLong(3);
				if(user < 0 || user > cpt+1) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
			} while(user < 0 || user > cpt+1);
			if(user == cpt+1) break;
			cpt = 0;
			group_figure(svg,g,cpt,user);
			fprintf(stdout,"==========\n");
			fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
			getchar();
			system("clear");
			fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
			fprintf(stdout,"Grouper\n\n");
		}
		fprintf(stdout,"==========\nAjout du groupe dans le fichier '%s' en cours...\n",fname);	
		sleep(1);
		add_sub_xelement(svg,g);
		image_delete(img);
		img = image_convert(svg);
		fprintf(stdout,"Groupe ajouté.\n");
	}
	fprintf(stdout,"==========\n");
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void main_degroup()
{
	system("clear");
	fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
	fprintf(stdout,"Dégrouper\n\n");
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert !\n");
	}
	else if(nb_groups(svg,0) == 0)
	{
		fprintf(stderr,"/!\\ Le fichier ne contient pas de groupes !\n");
	}
	else
	{
		int cpt = 0;
		int user = 0;
		do
		{
			cpt = menu_groups(svg,cpt);
			if(cpt == 0) break;
			fprintf(stdout,"\nChoix: ");
			user = scanfLong(3);
			if(user < 1 || user > cpt) fprintf(stderr,"/!\\ Erreur lors de la saisie.\n==========\n");
		} while(user < 1 || user > cpt);
		cpt = 0;
		delete_group(svg,cpt,user);
		//image_delete(img);
		//img = image_convert(svg);	
	}
	fprintf(stdout,"==========\n");
	fprintf(stdout,"Appuyez sur 'Entrée' pour continuer.\n");
	getchar();
	return;
}

void opt_open(const char* name)
{
	if(fname != NULL)
	{
		fprintf(stderr,"/!\\ Un fichier est déjà ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	char* ext = NULL;
	ext = strrchr(name,'.');
	if(strcmp(ext,".svg") == 0)
	{
		fname = (char*) malloc(strlen(name)*sizeof(char));
		if(fname == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
		strcpy(fname,name);
		svg = svg_load(fname);
		fprintf(stdout,"Fichier '%s' ouvert.\n",fname);
	}
	else
	{
		fprintf(stderr,"/!\\ Le fichier n'est pas une extension '.svg' ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	return;
}

void opt_save(const char* name)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	char* ext = NULL;
	ext = strrchr(name,'.');
	if(strcmp(ext,".svg") == 0)
	{
		svg_save(svg,name);
	}
	else if(strcmp(ext,".pdf") == 0)
	{
		pdf_t* p = pdf_convert(svg);
		pdf_save(name,p);
		free(p);
	}
	else
	{
		fprintf(stderr,"/!\\ Le nom n'est ni une extension '.svg', ni une extension '.pdf' ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	strcpy(fname,name);
	fprintf(stdout,"Fichier '%s' sauvegardé.\n",fname);

	return;
}

void opt_circle(int r)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	xelement_t* circ = svg_circle(0,0,r,"black");
	add_sub_xelement(svg,circ);
	fprintf(stdout,"Ajout du cercle de rayon %dpx réussi.\n",r);

	return;
}

void opt_rect(const char* dim)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	char* w = (char*) malloc(10*sizeof(char));
	char* h = (char*) malloc(10*sizeof(char));
	if(w == NULL || h == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du prgramme.\n");
		exit(1);
	}
	int i = 0; bool next = false;
	while(dim[i] != '\0')
	{
		if(dim[i] == ' ')
		{
			next = true;
			i++;
		}
		if(next == false) w[i] = dim[i];
		else h[i] = dim[i];
		i++;
	}
	xelement_t* rect = svg_rect(0,0,strtol(w,NULL,10),strtol(h,NULL,10),"black");
	add_sub_xelement(svg,rect);
	fprintf(stdout,"Ajout du rectangle de dimension %spx*%spx réussi.\n",w,h);

	free(h);
	free(w);
	return;
}

void opt_line(int l)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	int x = pow(l/2,2);
	xelement_t* line = svg_line(-x,0,x,0,"black",20);
	add_sub_xelement(svg,line);
	fprintf(stdout,"Ajout de la ligne de %dpx de longueur réussi.\n",l);
	
	return;
}

void opt_text(const char* text)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	char* t = (char*) malloc(strlen(text)*sizeof(char));
	if(t == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	if(text[0] != '"' || text[strlen(text)-2] != '"')
	{
		fprintf(stderr,"/!\\ Le texte n'est pas entre guillemets ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	int i = 1;
	while(text[i] != '"')
	{
		if(text[i] == '\0') break;
		t[i] = text[i];
		i++;
	}
	xelement_t* str = svg_text(0,0,"Helvetica",20,"black",t);
	add_sub_xelement(svg,str);
	fprintf(stdout,"Ajout du texte %s réussi.\n",text);

	free(t);
	return;
}

void opt_jpg(const char* jpg)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	FILE* fp = NULL;
	fp = fopen(jpg,"rb");
	if(fp == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'ouvrir le fichier '%s'. Retour au menu principal...\n",jpg);
		sleep(2);
		return;
	}
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	rewind(fp);
	char* img = (char*) malloc(len*sizeof(char));
	if(img == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	fread(img,len,1,fp);
	fclose(fp);
	size_t w; size_t* ptr_w = &w;
	size_t h; size_t* ptr_h = &h;
	size_t bpc; size_t* ptr_bpc = &bpc;
	const char* cs; const char** ptr_cs = &cs;
	jpeg_info(jpg, ptr_w, ptr_h, ptr_bpc, ptr_cs);

	xelement_t* image = svg_image(0,0,w,h,"none",jpg);
	add_sub_xelement(svg,image);

	fprintf(stdout,"Ajout de l'image '%s' réussi.\n",jpg);
	free(img);
	return;
}

void opt_delete_figure(int user)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	delete_figure(svg,0,user);

	return;
}

void opt_scale(const char* transform)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	int total = 0;
	total = nb_figures(svg,total);
	char* user = (char*) malloc(5*sizeof(char));
	char* scale = (char*) malloc(50*sizeof(char));
	if(user == NULL || scale == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	int i = 0; bool next = false;
	while(transform[i] != '\0')
	{
		if(transform[i] == ' ')
		{
			next = true;
			i++;
		}
		if(next == false) user[i] = transform[i];
		else scale[i] = transform[i];
		i++;
	}
	int n = strtol(user,NULL,10);
	if(n <= 0 || n > total)
	{
		fprintf(stderr,"/!\\ La valeur indiquée n'a pas de figure ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	float s = strtod(scale,NULL);
	if(s <= 0)
	{
		fprintf(stderr,"/!\\ Le ratio ne peut être inférieur ou égal à 0. Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	asprintf(&scale,"scale(%f)",s);
	transform_figure(svg,0,n,scale);

	free(user);
	free(scale);
	return;
}

void opt_rotate(const char* transform)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	int total = 0;
	total = nb_figures(svg,total);
	char* user = (char*) malloc(5*sizeof(char));
	char* rotate = (char*) malloc(50*sizeof(char));
	if(user == NULL || rotate == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	int i = 0; bool next = false;
	while(transform[i] != '\0')
	{
		if(transform[i] == ' ')
		{
			next = true;
			i++;
		}
		if(next == false) user[i] = transform[i];
		else rotate[i] = transform[i];
		i++;
	}
	int n = strtol(user,NULL,10);
	if(n <= 0 || n > total)
	{
		fprintf(stderr,"/!\\ La valeur indiquée n'a pas de figure ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	float r = strtod(rotate,NULL);
	if(r <= -360 || r <= 360)
	{
		fprintf(stderr,"/!\\ L'angle doit être entre ]-360,360[. Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	asprintf(&rotate,"rotate(%f)",r);
	transform_figure(svg,0,n,rotate);

	free(user);
	free(rotate);
	return;
}

void opt_translate(const char* transform)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	int total = 0;
	total = nb_figures(svg,total);
	char* user = (char*) malloc(5*sizeof(char));
	char* translate = (char*) malloc(50*sizeof(char));
	if(user == NULL || translate == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	int i = 0; bool next = false;
	while(transform[i] != '\0')
	{
		if(transform[i] == ' ')
		{
			next = true;
			i++;
		}
		if(next == false) user[i] = transform[i];
		else translate[i] = transform[i];
		i++;
	}
	int n = strtol(user,NULL,10);
	if(n <= 0 || n > total)
	{
		fprintf(stderr,"/!\\ La valeur indiquée n'a pas de figure ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	asprintf(&translate,"translate(%s)",translate);
	transform_figure(svg,0,n,translate);

	free(user);
	free(translate);
	return;
}

void opt_group(const char* list)
{
	if(fname == NULL)
	{
		fprintf(stderr,"/!\\ Aucun fichier n'est ouvert ! Fermeture du programme...\n");
		sleep(1);
		exit(1);
	}
	xelement_t* g = create_xelement("g");
	char* user = (char*) malloc(5*sizeof(char));
	if(user == NULL)
	{
		fprintf(stderr,"/!\\ Impossibler d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	int i = 0; int nb = 0;
	while(list[i] != '\0')
	{
		if(list[i] == ' ')
		{
			user[nb] = '\0';
			group_figure(svg,g,0,strtol(user,NULL,10));
			nb = 0;
		}
		user[nb] = list[i];
		i++; nb++;
	}
	add_sub_xelement(svg,g);
	fprintf(stdout,"L'ajout du groupe est réussie.\n");

	return;
}

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		fprintf(stdout,"========== CONVERTISSEUR DE FICHIERS SVG EN PDF ==========\n\n");
		fprintf(stdout,"Ouverture du programme avec options.\n\n");
		option_t* opt = NOOPTION;
		opt = opt_string(opt, "-i", opt_open);
		opt = opt_string(opt, "-o", opt_save);
		opt = opt_int(opt, "-c", opt_circle);
		opt = opt_string(opt, "-r", opt_rect);
		opt = opt_int(opt,"-l",opt_line);
		opt = opt_string(opt,"-x",opt_text);
		opt = opt_string(opt,"-j",opt_jpg);
		opt = opt_int(opt,"-d",opt_delete_figure);
		opt = opt_string(opt,"-s",opt_scale);
		opt = opt_string(opt,"-r",opt_rotate);
		opt = opt_string(opt,"-t",opt_translate);
		opt = opt_string(opt,"-g",opt_group);
		/*
		opt = opt_int(opt,"-u",opt_degroup);
		*/
		
		process_options(opt, argc, argv);
		
		opt_delete(opt);
	}
	else
	{	
		menu_t* m;
		menu_t* sm;
		m = createMenu("Menu principal");

		sm = createMenu("Fichier");
		addSubMenu(m,sm);
		addMenuAction(sm,"Ouvrir",main_open);
		addMenuAction(sm,"Enregistrer",main_save);
		addMenuAction(sm,"Information",main_info);
		addMenuAction(sm,"Affichage",main_display);
		addMenuAction(sm,"Retour au menu principal",main_back);

		sm = createMenu("Figure de base");
		addSubMenu(m,sm);
		addMenuAction(sm,"Ajouter cercle",main_add_circle);
    	addMenuAction(sm,"Ajouter rectangle",main_add_rect);
    	addMenuAction(sm,"Ajouter ligne",main_add_line);
    	addMenuAction(sm,"Ajouter texte",main_add_text);
    	addMenuAction(sm,"Ajouter image",main_add_image);
    	addMenuAction(sm,"Suppprimer",main_delete_figure);
    	addMenuAction(sm,"Retour au menu principal",main_back);

		sm = createMenu("Transformation et groupe");
		addSubMenu(m,sm);
		addMenuAction(sm,"Translater",main_translate);
    	addMenuAction(sm,"Tourner",main_rotate);
    	addMenuAction(sm,"Etirer",main_scale);
    	addMenuAction(sm,"Grouper",main_group);
    	addMenuAction(sm,"Dégrouper",main_degroup);
    	addMenuAction(sm,"Retour au menu principal",main_back);

    	addMenuAction(m,"Quitter",main_quit);

		while(cont) launchMenu(m);

		deleteMenu(m);
	}
	if(svg != NULL) delete_xelement(svg);
	if(img != NULL) image_delete(img);
	if(fname != NULL) free(fname);
	return 0;
}
