#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <unistd.h>

#include "svg.h"
#include "xml.h"
#include "pdf.h"

// --- SVG ---
xelement_t* svg_create(size_t width, size_t height, const char* mesure)
{
	xelement_t* svg = NULL;
	svg = create_xelement("svg");
	
	char* str = (char*) malloc(20*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	add_xattribute(svg,"xmlns","http://www.w3.org/2000/svg");
	if(strcmp(mesure,"cm") == 0)
	{
		float h = (float) height/100;
		float w = (float) width/100;
		sprintf(str,"%fcm",w);
		add_xattribute(svg,"width",str);
		sprintf(str,"%fcm",h);
		add_xattribute(svg,"height",str);
	}
	else if(strcmp(mesure,"px") == 0)
	{
		int h = (int) height;
		int w = (int) width;
		sprintf(str,"%dpx",w);
		add_xattribute(svg,"width",str);
		sprintf(str,"%dpx",h);
		add_xattribute(svg,"height",str);
	}
	sprintf(str,"0 0 %d %d",(int) width,(int) height);
	add_xattribute(svg,"viewBox",str);

	free(str);

	return svg;
}

void svg_add_title(xelement_t* svg, const char* title)
{
	xelement_t* t = NULL;
	t = create_xelement("title");
	add_raw(t,title);
	add_sub_xelement(svg,t);

	return;
}

void svg_add_desc(xelement_t* svg, const char* desc)
{
	xelement_t* d = NULL;
	d = create_xelement("desc");
	add_raw(d,desc);
	add_sub_xelement(svg,d);

	return;
}

bool svg_color(char* c)
{
	bool color = false;
	if(strcmp(c,"aqua") == 0 || strcmp(c,"black") == 0 || strcmp(c,"blue") == 0 || strcmp(c,"cyan") == 0
		|| strcmp(c,"gray") == 0 || strcmp(c,"grey") == 0 || strcmp(c,"green") == 0 || strcmp(c,"lime") == 0
		|| strcmp(c,"magenta") == 0 || strcmp(c,"maroon") == 0 || strcmp(c,"navy") == 0 || strcmp(c,"olive") == 0
		|| strcmp(c,"purple") == 0 || strcmp(c,"red") == 0 || strcmp(c,"teal") == 0 || strcmp(c,"white") == 0
		|| strcmp(c,"yellow") == 0)
	{
		color = true;
	}
	else
	{
		fprintf(stderr,"/!\\ Couleur non reconnue ! Voici celles reconnues par le programme:\n");
		fprintf(stderr,"aqua, black, blue, cyan, gray, grey, ");
		fprintf(stderr,"green, lime, magenta, maroon, navy, olive, ");
		fprintf(stderr,"purple, red, teal, white, yellow.\n");
	}
	
	return color;
}

xelement_t* svg_rect(int x, int y, int width, int height, const char* color)
{
	xelement_t* r = NULL;
	r = create_xelement("rect");
	char* val = (char*) malloc(10*sizeof(char));
	if(val == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	sprintf(val,"%d",x);
	add_xattribute(r,"x",val);
	sprintf(val,"%d",y);
	add_xattribute(r,"y",val);
	sprintf(val,"%d",width);
	add_xattribute(r,"width",val);
	sprintf(val,"%d",height);
	add_xattribute(r,"height",val);
	add_xattribute(r,"fill",color);

	free(val);

	return r;
}

xelement_t* svg_circle(int cx, int cy, int r, const char* color)
{
	xelement_t* c = NULL;
	c = create_xelement("circle");
	char* val = (char*) malloc(10*sizeof(char));
	if(val == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	sprintf(val,"%d",cx);
	add_xattribute(c,"cx",val);
	sprintf(val,"%d",cy);
	add_xattribute(c,"cy",val);
	sprintf(val,"%d",r);
	add_xattribute(c,"r",val);
	add_xattribute(c,"fill",color);

	free(val);

	return c;
}

xelement_t* svg_line(int x1, int y1, int x2, int y2, const char* color, int stroke)
{
	xelement_t* l = NULL;
	l = create_xelement("line");
	char* val = (char*) malloc(5*sizeof(char));
	if(val == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	sprintf(val,"%d",x1);
	add_xattribute(l,"x1",val);
	sprintf(val,"%d",y1);
	add_xattribute(l,"y1",val);
	sprintf(val,"%d",x2);
	add_xattribute(l,"x2",val);
	sprintf(val,"%d",y2);
	add_xattribute(l,"y2",val);
	add_xattribute(l,"stroke",color);
	sprintf(val,"%d",stroke);
	add_xattribute(l,"stroke-width",val);

	free(val);

	return l;
}

xattribute_t* svg_add_transform(xelement_t* e, const char* value)
{
	xattribute_t* xat = (xattribute_t*) malloc(sizeof(xattribute_t));
	(*xat).val = (char*) malloc(strlen(value)*sizeof(char));
	(*xat).name = (char*) malloc(10*sizeof(char));
	if(xat == NULL || (*xat).val == NULL || (*xat).name == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	strcpy((*xat).val,value);
	strcpy((*xat).name,"transform");
	(*xat).next = NULL;
	if((*e).attributes == NULL)
	{
		(*e).attributes = xat;
	}
	else if(strcmp((*e).attributes->name,(*xat).name) == 0)
	{
		int i = 0; char* str = NULL;
		while((*e).attributes->val[i] != '\0')
		{
			if((*e).attributes->val[i] == value[0] && (*e).attributes->val[i+1] == value[1])
			{
				asprintf(&str,"%s",value);
				if(i != 0)
				{
					char* before = NULL;
					asprintf(&before,"%s",(*e).attributes->val);
					before[i] = '\0';
					asprintf(&str,"%s,%s",before,str);
					free(before);
				}
				char* after = NULL;
				if(i == 0) after = strchr((*e).attributes->val,',');
				else after = strrchr((*e).attributes->val,',');
				if(after != NULL) asprintf(&str,"%s,%s",str,after);
				break;
			}
			i++;
		}
		int len = 0;
		if(str == NULL) len = strlen((*e).attributes->val) + strlen(value);
		else len = strlen(str);
		(*xat).val = (char*) realloc((*xat).val,len*sizeof(char));
		if((*xat).val == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
		if(str == NULL) asprintf(&str,"%s,%s",(*e).attributes->val,value);
		strcpy((*xat).val,str);
		free(str);
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
			if(strcmp((*tmp).name,(*xat).name) == 0)
			{
				int i = 0; char* str = NULL;
				while((*tmp).val[i] != '\0')
				{
					if((*tmp).val[i] == value[0] && (*tmp).val[i+1] == value[1])
					{
						asprintf(&str,"%s",value);
						if(i != 0)
						{
							char* before = NULL;
							asprintf(&before,"%s",(*tmp).val);
							before[i] = '\0';
							asprintf(&str,"%s,%s",before,str);
							free(before);
						}
						char* after = NULL;
						if(i == 0) after = strchr((*tmp).val,',');
						else after = strrchr((*tmp).val,',');
						if(after != NULL) asprintf(&str,"%s,%s",str,after);
						break;
					}
					i++;
				}
				int len = 0;
				if(str == NULL) len = strlen((*e).attributes->val) + strlen(value);
				else len = strlen(str);
				(*xat).val = (char*) realloc((*xat).val,len*sizeof(char));
				if((*xat).val == NULL)
				{
					fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
					exit(1);
				}
				if(str == NULL) asprintf(&str,"%s,%s",(*tmp).val,value);
				strcpy((*xat).val,str);
				free(str);
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

xattribute_t* svg_translate(xelement_t* e, int dx, int dy)
{
	char* str = (char*) malloc(25*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	sprintf(str,"translate(%d %d)",dx,dy);
	svg_add_transform(e,str);

	free(str);

	return (*e).attributes;
}

xattribute_t* svg_rotate(xelement_t* e, int th)
{
	char* str = (char*) malloc(15*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	sprintf(str,"rotate(%d)",th);
	svg_add_transform(e,str);

	free(str);

	return (*e).attributes;
}

xattribute_t* svg_scale(xelement_t* e, int s)
{
	char* str = (char*) malloc(15*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	sprintf(str,"scale(%d)",s);
	svg_add_transform(e,str);

	free(str);

	return (*e).attributes;
}

xelement_t* svg_text(int x, int y, const char* font, int size, const char* color, const char* str)
{
	xelement_t* t = NULL;
	t = create_xelement("text");
	char* val = (char*) malloc(5*sizeof(char));
	if(val == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	sprintf(val,"%d",x);
	add_xattribute(t,"x",val);
	sprintf(val,"%d",y);
	add_xattribute(t,"y",val);
	add_xattribute(t,"font-family",font);
	sprintf(val,"%d",size);
	add_xattribute(t,"font-size",val);
	add_xattribute(t,"fill",color);

	add_raw(t,str);

	free(val);

	return t;
}

xelement_t* svg_image(int x, int y, int width, int height, const char* ratio, const char* href)
{
	xelement_t* i = NULL;
	i = create_xelement("image");
	char* val = (char*) malloc(5*sizeof(char));
	if(val == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	add_xattribute(i,"href",href);
	sprintf(val,"%d",x);
	add_xattribute(i,"x",val);
	sprintf(val,"%d",y);
	add_xattribute(i,"y",val);
	sprintf(val,"%d",height);
	add_xattribute(i,"height",val);
	sprintf(val,"%d",width);
	add_xattribute(i,"width",val);
	add_xattribute(i,"preserveAspectRatio",ratio);

	free(val);

	return i;
}

xelement_t* svg_add_group(xelement_t* g, xelement_t* figure)
{
	add_sub_xelement(g,figure);

	return g;
}

void svg_save(xelement_t* svg, const char* fname)
{
	FILE* fp = NULL;
	fp = fopen(fname,"w");
	if(fp == NULL)
	{
		fprintf(stderr,"/!\\ Impossible de sauvegarder le fichier SVG '%s'. Fermeture du programme.\n",fname);
		exit(1);
	}
	save_xelement(fp,svg);
	fclose(fp);

	return;
}

xelement_t* svg_load(const char* fname)
{
	FILE* fp = NULL;
	fp = fopen(fname,"r");
	if(fp == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'ouvrir le fichier SVG '%s'.\n",fname);
		exit(1);
	}
	xelement_t* s = load_xelement(fp,NULL);

	fclose(fp);
	return s;
}

// --- ASCII_ART ---
void print_coord(coord_t* c)
{
	fprintf(stdout,"(%f,%f)",(*c).x,(*c).y);
	return;
}

coord_t* coordinate(double x, double y)
{
	coord_t* c = (coord_t*) malloc(sizeof(coord_t));
	if(c == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*c).x = x;
	(*c).y = y;
	return c;
}

coord_t* inv_translate_point(coord_t* p, coord_t* v)
{
	coord_t* c = (coord_t*) malloc(sizeof(coord_t));
	if(c == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*c).x = (*p).x - (*v).x;
	(*c).y = (*p).y - (*v).y;
	return c;
}

coord_t* inv_rotate_point(coord_t* p, double t)
{
	coord_t* c = (coord_t*) malloc(sizeof(coord_t));
	if(c == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*c).x = (*p).x*cos(-t) - (*p).y*sin(-t);
	(*c).y = (*p).x*sin(-t) + (*p).y*cos(-t);
	return c;
}

figure_t* circle(xelement_t* e)
{
	coord_t* c = (coord_t*) malloc(sizeof(coord_t));
	figure_t* f = (figure_t*) malloc(sizeof(figure_t));
	(*f).g = (coord_t*) malloc(sizeof(coord_t));
	(*f).specs.circ = (circle_t*) malloc(sizeof(circle_t));
	if(c == NULL || f == NULL || (*f).g == NULL || (*f).specs.circ == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	double r;
	char* transform = NULL;

	xattribute_t* a = (*e).attributes;
	while(a != NULL)
	{
		if(strcmp((*a).name,"cx") == 0) (*c).x = strtod((*a).val,NULL);
		else if(strcmp((*a).name,"cy") == 0) (*c).y = strtod((*a).val,NULL);
		else if(strcmp((*a).name,"r") == 0) r = strtod((*a).val,NULL);
		else if(strcmp((*a).name,"transform") == 0) asprintf(&transform,"%s",(*a).val);		
		a = (*a).next;
	}
	(*f).c = '+';
	(*f).g->x = (*c).x; (*f).g->y = (*c).y;
	(*f).a = 0;
	(*f).type = CIRCLE;
	(*f).specs.circ->r = r;
	if(transform != NULL)
	{
		image_transform(f,transform);	
		free(transform);
	}
	free(c);
	return f;
}

figure_t* rectangle(xelement_t* e)
{
	coord_t* c = (coord_t*) malloc(sizeof(coord_t));
	figure_t* f = (figure_t*) malloc(sizeof(figure_t));
	(*f).g = (coord_t*) malloc(sizeof(coord_t));
	(*f).specs.rect = (rectangle_t*) malloc(sizeof(rectangle_t));
	if(c == NULL || f == NULL || (*f).g == NULL || (*f).specs.rect == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	char* transform = NULL;

	xattribute_t* a = (*e).attributes;
	while(a != NULL)
	{
		if(strcmp((*a).name,"x") == 0) (*f).g->x = strtod((*a).val,NULL)/10;
		else if(strcmp((*a).name,"y") == 0) (*f).g->y = strtod((*a).val,NULL)/10;
		else if(strcmp((*a).name,"width") == 0) (*c).x = strtod((*a).val,NULL)/10;
		else if(strcmp((*a).name,"height") == 0) (*c).y = strtod((*a).val,NULL)/10;
		else if(strcmp((*a).name,"transform") == 0) asprintf(&transform,"%s",(*a).val);		
		a = (*a).next;
	}
	(*f).c = '+';
	(*f).g->x = 0; (*f).g->y = 0;
	(*f).a = 0;
	(*f).type = RECTANGLE;
	(*f).specs.rect->w = (*c).x/2;
	(*f).specs.rect->h = (*c).y/2;
	if(transform != NULL)
	{
		image_transform(f,transform);	
		free(transform);
	}
	free(c);
	return f;
}

figure_t* line(xelement_t* e)
{
	coord_t* c1 = (coord_t*) malloc(sizeof(coord_t));
	coord_t* c2 = (coord_t*) malloc(sizeof(coord_t));
	figure_t* f = (figure_t*) malloc(sizeof(figure_t));
	(*f).g = (coord_t*) malloc(sizeof(coord_t));
	(*f).specs.l = (line_t*) malloc(sizeof(line_t));
	if(c1 == NULL || c2 == NULL || f == NULL || (*f).g == NULL || (*f).specs.l == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	char* transform = NULL;

	xattribute_t* a = (*e).attributes;
	while(a != NULL)
	{
		if(strcmp((*a).name,"x1") == 0) (*c1).x = strtod((*a).val,NULL)/10;
		else if(strcmp((*a).name,"y1") == 0) (*c1).y = strtod((*a).val,NULL)/10;
		else if(strcmp((*a).name,"x2") == 0) (*c2).x = strtod((*a).val,NULL)/10;
		else if(strcmp((*a).name,"y2") == 0) (*c2).y = strtod((*a).val,NULL)/10;
		else if(strcmp((*a).name,"transform") == 0) asprintf(&transform,"%s",(*a).val);		
		a = (*a).next;
	}
	(*f).c = '+';
	(*f).g->x = 0; (*f).g->y = 0;
	(*f).a = 0;
	(*f).type = LINE;
	(*f).specs.l->len = (sqrt(pow((*c2).x-(*c1).x,2)+pow((*c2).y-(*c1).y,2)))/2;
	if(transform != NULL)
	{
		image_transform(f,transform);	
		free(transform);
	}
	free(c2);
	free(c1);
	return f;
}

figure_t* group()
{
	figure_t* f = (figure_t*) malloc(sizeof(figure_t));
	(*f).g = (coord_t*) malloc(sizeof(coord_t));
	(*f).specs.group = (group_t*) malloc(sizeof(group_t));
	if(f == NULL || (*f).g == NULL || (*f).specs.group == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*f).c = '+';
	(*f).g->x = 0; (*f).g->y = 0;
	(*f).a = 0;
	(*f).type = GROUP;
	(*f).specs.group->g = NULL;
	(*f).specs.group->nb = 0;
	return f;
}

figure_t* group_append(figure_t* g, figure_t* f)
{
	if(f != NULL)
	{
		if((*g).specs.group->nb == 0)
		{
			(*g).specs.group->g = (figure_t**) malloc(((*g).specs.group->nb+1)*sizeof(figure_t*));
			if((*g).specs.group->g == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
		}
		else
		{
			(*g).specs.group->g = (figure_t**) realloc((*g).specs.group->g,((*g).specs.group->nb+1)*sizeof(figure_t*));
			if((*g).specs.group->g == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
		}
		(*g).specs.group->nb++;
		(*g).specs.group->g[(*g).specs.group->nb-1] = f;
	}
	return g;
}

figure_t* color(figure_t* f, color_t c)
{
	(*f).c = c;
	return f;
}

figure_t* translate(figure_t* f, double dx, double dy)
{
	(*f).g->x += dx; (*f).g->y += dy;
	return f;
}

figure_t* rotate(figure_t* f, double dt)
{
	(*f).a += (dt*M_PI/180);
	return f;
}

figure_t* scale(figure_t* f, double s)
{
	(*f).g->x *= s; (*f).g->y *= s;
	(*f).a *= s;
	if((*f).type == CIRCLE)
	{
		(*f).specs.circ->r *= s;
	}
	else if((*f).type == RECTANGLE)
	{
		(*f).specs.rect->w *= s;
		(*f).specs.rect->h *= s;
	}
	else if((*f).type == LINE)
	{
		(*f).specs.l->len *= s;
	}

	return f;
}

color_t intersect(coord_t* p, figure_t* f, double grain)
{
	p = inv_translate_point(p,(*f).g);
	p = inv_rotate_point(p,(*f).a);
	if((*f).type == CIRCLE)
	{
		if(pow((*p).x,2) + pow((*p).y,2) <= (*f).specs.circ->r) return '0';
	}
	else if((*f).type == RECTANGLE)
	{
		if(fabs((*p).x) <= (*f).specs.rect->w && fabs((*p).y) <= (*f).specs.rect->h) return '+';
	}
	else if((*f).type == LINE)
	{
		if(fabs((*p).x) <= (*f).specs.l->len && fabs((*p).y) <= grain) return '*';
	}
	return 0;
}

image_t* image(unsigned int width, unsigned int height, double grain)
{
	image_t* i = (image_t*) malloc(sizeof(image_t));
	if(i == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*i).w = width;
	(*i).h = height;
	(*i).g = grain;
	(*i).f = NULL;
	(*i).nb = 0;
	return i;
}

image_t* append(image_t* img, figure_t* f)
{
	if(f != NULL)
	{
		if((*img).nb == 0)
		{
			(*img).f = (figure_t**) malloc(((*img).nb+1)*sizeof(figure_t*));
			if((*img).f == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
		}
		else
		{
			(*img).f = (figure_t**) realloc((*img).f,((*img).nb+1)*sizeof(figure_t*));
			if((*img).f == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
		}
		(*img).nb++;
		(*img).f[(*img).nb-1] = f;
	}
	return img;
}

image_t* prepend(image_t* img, figure_t* f)
{
	if((*img).nb == 0)
	{
		(*img).f = (figure_t**) malloc(((*img).nb+1)*sizeof(figure_t*));
		if((*img).f == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else
	{
		(*img).f = (figure_t**) realloc((*img).f,((*img).nb+1)*sizeof(figure_t*));
		if((*img).f == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	(*img).nb++;
	for(int i = (*img).nb-1; i>0; i--)
	{
		(*img).f[i] = (*img).f[i-1];
	}
	(*img).f[0] = f;
	return img;
}

void paint(image_t* img)
{
	for(int i = 0; i<(int)(*img).h; i++)
	{
		for(int j = 0; j<(int)(*img).w; j++)
		{
			bool inter = false;
			int n = (*img).nb;
			while(n>0)
			{
				if((*img).f[n-1]->type == GROUP)
				{
					int cpt = (*img).f[n-1]->specs.group->nb;
					while(cpt>0)
					{
						color_t c = intersect(coordinate(j,i),(*img).f[n-1]->specs.group->g[cpt-1],(*img).g);
						if(c)
						{
							inter = true;
							fprintf(stdout,"%c",c);
							break;
						}
						cpt--;
					}

				}
				else
				{
					color_t c = intersect(coordinate(j,i),(*img).f[n-1],(*img).g);
					if(c)
					{
						inter = true;
						fprintf(stdout,"%c",c);
						break;
					}
				}
				n--;
			}
			if(inter == false) fprintf(stdout," ");
		}
		fprintf(stdout,"\n");
	}
	return;
}

// --- Traitement SVG/ASCII ---
image_t* image_create_from_svg(xelement_t* svg)
{
	int* info = (int*) malloc(2*sizeof(int));
	if(info == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}

	xattribute_t* xat = (*svg).attributes;
	while(xat != NULL)
	{
		if(strcmp((*xat).name,"width") == 0) info[0] = (unsigned int) strtol((*xat).val,NULL,10)/10;
		else if(strcmp((*xat).name,"height") == 0) info[1] = (unsigned int) strtol((*xat).val,NULL,10)/10;
		xat = (*xat).next;
	}
	image_t* i = image(info[0],info[1],0.25);

	free(info);
	return i;
}

void image_transform(figure_t* f, char* transform)
{
	int cpt = 0; int n = 0;
	int len = strlen(transform)+1;
	char* str = (char*) malloc(len*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	while(transform[cpt] != '\0')
	{
		if(transform[cpt] == ',' || transform[cpt+1] == '\0')
		{
			str[n] = '\0';
			cpt++;
			int i = 0; int j = 0; int k = 0;
			char* t1 = (char*) malloc(25*sizeof(char));
			char* t2 = (char*) malloc(25*sizeof(char));
			char* t3 = (char*) malloc(25*sizeof(char));
			char** t = (char**) malloc(3*sizeof(char*));
			if(t == NULL || t1 == NULL || t2 == NULL || t3 == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
			t[0] = t1; t[1] = t2; t[2] = t3;
			while(str[i] != '\0')
			{
				if(str[i] == '(' || str[i] == ' ' || str[i] == ')')
				{
					t[j][k] = '\0'; i++; j++; k = 0;
				}
				else
				{
					t[j][k] = str[i]; i++; k++;
				}
			}
			if(strcmp(t1,"translate") == 0) translate(f,strtod(t2,NULL)/10,strtod(t3,NULL)/10);
			else if(strcmp(t1,"rotate") == 0) rotate(f,strtod(t2,NULL));
			else if(strcmp(t1,"scale") == 0) scale(f,strtod(t2,NULL));
			free(t3);
			free(t2);
			free(t1);
			free(t);
			n = 0;
		}
		else
		{
			str[n] = transform[cpt]; cpt++;	n++;
		}
	}
	free(str);

	return;
}

figure_t* image_figure(xelement_t* e)
{
	figure_t* f = NULL;
	if(strcmp((*e).name,"circle") == 0) f = circle(e);
	else if(strcmp((*e).name,"rect") == 0) f = rectangle(e);
	else if(strcmp((*e).name,"line") == 0) f = line(e);

	return f;
}

image_t* image_from_svg(image_t* i, figure_t* fig, xelement_t* svg)
{
	char* transform = NULL;
	int cpt = 0;
	xelement_t* e = (*svg).content.son;
	xattribute_t* a;
	
	while(e != NULL)
	{
		figure_t* f = NULL;
		if(strcmp((*e).name,"g") == 0)
		{
			a = (*e).attributes;
			while(a != NULL)
			{
				if(strcmp((*a).name,"transform") == 0)
				{
					asprintf(&transform,"%s",(*a).val);
					break;
				}
				a = (*a).next;
			}
			f = group();
			image_from_svg(i,f,e);
			if((*f).specs.group->g == NULL)
			{
				free(f);
			}
			else
			{
				append(i,f);
				if(transform != NULL) image_transform((*i).f[(*i).nb-1],transform);
			}
		}
		else
		{
			f = image_figure(e);
			if(strcmp((*e).parent->name,"g") == 0) group_append(fig,f);
			else if(f != NULL) append(i,f);
			else free(f);
		}
		cpt++;
		e = (*e).bro;
	}
	free(transform);

	return i;
}

image_t* image_convert(xelement_t* svg)
{
	image_t* i = image_create_from_svg(svg);
	figure_t* empty = NULL;
	i = image_from_svg(i,empty,svg);

	return i;
}

void image_delete(image_t* img)
{
	int i;
	for(i = 0; i<(*img).nb; i++)
	{
		free((*img).f[i]->g);
		if((*img).f[i]->type == CIRCLE)
		{
			free((*img).f[i]->specs.circ);
		}
		else if((*img).f[i]->type == RECTANGLE)
		{
			free((*img).f[i]->specs.rect);
		}
		else if((*img).f[i]->type == LINE)
		{
			free((*img).f[i]->specs.l);
		}
		else if((*img).f[i]->type == GROUP)
		{
			int j;
			for(j = 0; j<(*img).f[i]->specs.group->nb; j++)
			{
				if((*img).f[i]->specs.group->g[j] != NULL) free((*img).f[i]->specs.group->g[j]);
			}
			free((*img).f[i]->specs.group->g);
			free((*img).f[i]->specs.group);
		}
		free((*img).f[i]);
	}
	free((*img).f);
	free(img);

	return;
}

// --- Traitement SVG/PDF ---
pdf_t* pdf_create_from_svg(xelement_t* svg)
{
	float info[2] = {0};
	xattribute_t* xat = (*svg).attributes;
	char* w = (char*) malloc(5*sizeof(char));
	if(w == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	while(xat != NULL)
	{
		int i = 0;
		if(strcmp((*xat).name,"width") == 0)
		{
			while((*xat).val[i] != '\0')
			{
				w[i] = (*xat).val[i];
				i++;
			}
			w[i] = '\0';
			info[0] = (int) strtol(w,NULL,10);
		}
		else if(strcmp((*xat).name,"height") == 0)
		{
			while((*xat).val[i] != '\0')
			{
				w[i] = (*xat).val[i];
				i++;
			}
			w[i] = '\0';
			info[1] = (int) strtol(w,NULL,10);
		}
		xat = (*xat).next;
	}
	free(w);
	pdf_t* p = pdf_create(1,info[0],info[1]);

	return p;
}

char* pdf_transform(char* transform)
{
	int cpt = 0; int n = 0;
	int len = strlen(transform)+1;
	char* str = (char*) malloc(len*sizeof(char));
	int* tval = (int*) malloc(4*sizeof(int));
	if(str == NULL || tval == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	tval[0] = 0; tval[1] = 0; tval[2] = 0; tval[3] = 1;
	char* new = NULL;

	while(transform[cpt] != '\0')
	{
		if(transform[cpt] == ',' || transform[cpt+1] == '\0')
		{
			str[n] = '\0';
			cpt++;
			int i = 0; int j = 0; int k = 0;
			char* t1 = (char*) malloc(25*sizeof(char));
			char* t2 = (char*) malloc(25*sizeof(char));
			char* t3 = (char*) malloc(25*sizeof(char));
			char** t = (char**) malloc(4*sizeof(char*));
			if(t == NULL || t1 == NULL || t2 == NULL || t3 == NULL)
			{
				fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
				exit(1);
			}
			t[0] = t1; t[1] = t2; t[2] = t3; t[3] = '\0';
			while(str[i] != '\0')
			{
				if(str[i] == '(' || str[i] == ' ' || str[i] == ')')
				{
					t[j][k] = '\0'; i++; j++; k = 0;
				}
				else
				{
					t[j][k] = str[i]; i++; k++;
				}
			}
			if(strcmp(t1,"translate") == 0)
			{
				tval[0] = strtol(t2,NULL,10);
				tval[1] = strtol(t3,NULL,10);
			}
			else if(strcmp(t1,"rotate") == 0)
			{
				tval[2] = strtol(t2,NULL,10);
			}
			else if(strcmp(t1,"scale") == 0)
			{
				tval[3] *= strtol(t2,NULL,10);
			}
			free(t3); free(t2); free(t1); free(t); n = 0;
		}
		else
		{
			str[n] = transform[cpt]; cpt++; n++;
		}
	}
	asprintf(&new,"%d 0 0 %d %d %d cm",tval[3],tval[3],tval[0],tval[1]);
	if(tval[2] != 0)
	{
		float cx = cos(tval[2]*M_PI/180);
		float cy = sin(tval[2]*M_PI/180);
		asprintf(&new,"%s %f %f %f %f 0 0 cm",new,cx,cy,-cy,cx);
	}

	free(str); free(tval);

	return new;
}

char* pdf_color(char* c)
{
	if(strcmp(c,"aqua") == 0){ sprintf(c,"0 1 1"); }
	else if(strcmp(c,"black") == 0){ sprintf(c,"0 0 0"); }
	else if(strcmp(c,"blue") == 0){ sprintf(c,"0 0 1"); }
	else if(strcmp(c,"cyan") == 0){ sprintf(c,"0 1 1"); }
	else if(strcmp(c,"gray") == 0 || strcmp(c,"grey") == 0){ sprintf(c,"0.5 0.5 0.5"); }
	else if(strcmp(c,"green") == 0){ sprintf(c,"0 0.5 0"); }
	else if(strcmp(c,"lime") == 0){ sprintf(c,"0 1 0"); }
	else if(strcmp(c,"magenta") == 0){ sprintf(c,"1 0 1"); }
	else if(strcmp(c,"maroon") == 0){ sprintf(c,"0.5 0 0"); }
	else if(strcmp(c,"navy") == 0){ sprintf(c,"0 0 0.5"); }
	else if(strcmp(c,"olive") == 0){ sprintf(c,"0.5 0.5 0"); }
	else if(strcmp(c,"purple") == 0){ sprintf(c,"0.5 0 0.5"); }
	else if(strcmp(c,"red") == 0){ sprintf(c,"1 0 0"); }
	else if(strcmp(c,"teal") == 0){ sprintf(c,"0 0.5 0.5"); }
	else if(strcmp(c,"white") == 0){ sprintf(c,"1 1 1"); }
	else if(strcmp(c,"yellow") == 0){ sprintf(c,"1 1 0"); }
	else
	{
		fprintf(stderr,"/!\\ Couleur non reconnue ! Voici les couleurs reconnues:\n");
		fprintf(stderr,"aqua, black, blue, cyan, gray, grey, ");
		fprintf(stderr,"green, lime, magenta, maroon, navy, olive, ");
		fprintf(stderr,"purple, red, teal, white, yellow.\n");
		fprintf(stderr,"La couleur sera remplacée par la couleur noire.\n");
		sprintf(c,"0 0 0");
	}
	
	return c;
}

char* pdf_stream_line(xelement_t* xel)
{
	int* coord = (int*) malloc(6*sizeof(int));
	char* color = (char*) malloc(10*sizeof(char));
	if(coord == NULL || color == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	coord[5] = '\0';
	char* transform = NULL;

	xattribute_t* xat = (*xel).attributes;
	while(xat != NULL)
	{
		if(strcmp((*xat).name,"x1") == 0) coord[0] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"y1") == 0) coord[1] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"x2") == 0) coord[2] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"y2") == 0) coord[3] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"stroke") == 0) strcpy(color,(*xat).val);
		else if(strcmp((*xat).name,"stroke-width") == 0) coord[4] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"transform") == 0) asprintf(&transform,"%s",(*xat).val);

		xat = (*xat).next;
	}
	
	color = pdf_color(color);
	char* stream = NULL;
	if(transform != NULL) 
	{
		char* t = pdf_transform(transform);
		asprintf(&stream,"q %d w %s RG %s %d %d m %d %d l S Q",coord[4],color,t,coord[0],coord[1],coord[2],coord[3]);
		free(transform);
		free(t);
	}
	else
	{
		asprintf(&stream,"q %d w %s RG %d %d m %d %d l S Q",coord[4],color,coord[0],coord[1],coord[2],coord[3]);
	}

	free(color);
	free(coord);

	return stream;
}

char* pdf_stream_rect(xelement_t* xel)
{
	int* coord = (int*) malloc(5*sizeof(int));
	char* color = (char*) malloc(10*sizeof(char));
	if(coord == NULL || color == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	coord[4] = '\0';
	char* transform = NULL;

	xattribute_t* xat = (*xel).attributes;
	while(xat != NULL)
	{
		if(strcmp((*xat).name,"x") == 0) coord[0] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"y") == 0) coord[1] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"width") == 0) coord[2] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"height") == 0) coord[3] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"fill") == 0) strcpy(color,(*xat).val);
		else if(strcmp((*xat).name,"transform") == 0) asprintf(&transform,"%s",(*xat).val);

		xat = (*xat).next;
	}

	char* stream = NULL;
	color = pdf_color(color);
	if(transform != NULL)
	{
		char* t = pdf_transform(transform);
		asprintf(&stream,"q %s rg %s %d %d %d %d re F Q",color,t,coord[0],coord[1],coord[2],coord[3]);
		free(transform);
		free(t);
	}
	else
	{
		asprintf(&stream,"q %s %d %d %d %d re F Q",color,coord[0],coord[1],coord[2],coord[3]);
	}

	free(color);
	free(coord);

	return stream;
}

char* pdf_stream_circle(xelement_t* xel)
{
	int* coord = (int*) malloc(4*sizeof(int));
	char* color = (char*) malloc(10*sizeof(char));
	if(coord == NULL || color == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	coord[3] = '\0';
	char* transform = NULL;

	xattribute_t* xat = (*xel).attributes;
	while(xat != NULL)
	{
		if(strcmp((*xat).name,"cx") == 0) coord[0] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"cy") == 0) coord[1] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"r") == 0) coord[2] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"fill") == 0) strcpy(color,(*xat).val);
		else if(strcmp((*xat).name,"transform") == 0) asprintf(&transform,"%s",(*xat).val);

		xat = (*xat).next;
	}

	char* stream = NULL;
	color = pdf_color(color);
	if(transform != NULL)
	{
		char* t = pdf_transform(transform);
		asprintf(&stream,"q %s rg %s %d 0 0 %d 0 0 cm -1 0 m -1 0.553 -0.553 1 0 1 c 0.553 1 1 0.553 1 0 c 1 -0.553 0.553 -1 0 -1 c -0.553 -1 -1 -0.553 -1 0 c F Q",color,t,coord[2],coord[2]);
		free(transform);
		free(t);
	}
	else
	{
		asprintf(&stream,"q %s rg %d 0 0 %d 0 0 cm -1 0 m -1 0.553 -0.553 1 0 1 c 0.553 1 1 0.553 1 0 c 1 -0.553 0.553 -1 0 -1 c -0.553 -1 -1 -0.553 -1 0 c F Q",color,coord[2],coord[2]);
	}

	free(color);
	free(coord);

	return stream;
}

char* pdf_stream_image(pdf_t* p, xelement_t* xel)
{
	int* coord = (int*) malloc(3*sizeof(int));
	char* href = (char*) malloc(20*sizeof(char));
	if(coord == NULL || href == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	coord[2] = '\0';
	char* transform = NULL;

	xattribute_t* xat = (*xel).attributes;
	while(xat != NULL)
	{
		if(strcmp((*xat).name,"width") == 0) coord[0] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"height") == 0) coord[1] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"href") == 0) strcpy(href,(*xat).val);
		else if(strcmp((*xat).name,"transform") == 0) asprintf(&transform,"%s",(*xat).val);

		xat = (*xat).next;
	}

	char* stream = NULL;
	char* im = pdf_load_image(p,href);
	if(transform != NULL)
	{
		char* t = pdf_transform(transform);
		asprintf(&stream,"q %s %d 0 0 %d 0 %d cm /%s Do Q",t,coord[0],-coord[1],coord[1],im);
		free(transform);
		free(t);
	}
	else
	{
		asprintf(&stream,"q %d 0 0 %d 0 %d cm /%s Do Q",coord[0],-coord[1],coord[1],im);
	}

	free(im);
	free(href);
	free(coord);

	return stream;
}

char* font_text(char* font)
{
	char* ft = (char*) malloc(5*sizeof(char));
	if(ft == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	if(strcmp(font,"Times-Roman") == 0) strcpy(ft,"FT");
	else if(strcmp(font,"Times-Bold") == 0) strcpy(ft,"FTB");
	else if(strcmp(font,"Times-Italic") == 0) strcpy(ft,"FTI");
	else if(strcmp(font,"Times-BoldItalic") == 0) strcpy(ft,"FTBI");
	else if(strcmp(font,"Helvetica") == 0) strcpy(ft,"FV");
	else if(strcmp(font,"Helvetica-Bold") == 0) strcpy(ft,"FVB");
	else if(strcmp(font,"Helvetica-Oblique") == 0) strcpy(ft,"FVI");
	else if(strcmp(font,"Helvetica-BoldOblique") == 0) strcpy(ft,"FVBI");
	else return NULL;

	return ft;
}

char* pdf_stream_text(xelement_t* xel)
{
	int* coord = (int*) malloc(4*sizeof(int));
	char* font = (char*) malloc(25*sizeof(char));
	char* color = (char*) malloc(10*sizeof(char));
	int len = strlen((*xel).content.raw)+1;
	char* str = (char*) malloc(len*sizeof(char));
	if(coord == NULL || font == NULL || color == NULL || str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	char* transform = NULL;
	strcpy(str,(*xel).content.raw);

	xattribute_t* xat = (*xel).attributes;
	while(xat != NULL)
	{
		if(strcmp((*xat).name,"x") == 0) coord[0] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"y") == 0) coord[1] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"font-size") == 0) coord[2] = strtol((*xat).val,NULL,10);
		else if(strcmp((*xat).name,"font-family") == 0) strcpy(font,(*xat).val);
		else if(strcmp((*xat).name,"fill") == 0) strcpy(color,(*xat).val);
		else if(strcmp((*xat).name,"transform") == 0) asprintf(&transform,"%s",(*xat).val);

		xat = (*xat).next;
	}

	char* stream = NULL;
	color = pdf_color(color);
	char* ft = font_text(font);
	if(ft == NULL)
	{
		fprintf(stderr,"/!\\ Police non reconnue. Veuillez utiliser une des polices suivantes dans votre fichier SVG:\n");
		fprintf(stderr,"Times-Roman, Times-Bold, Times-Italic, Times-BoldItalic\n");
		fprintf(stderr,"Helvetica, Helvetica-Bold, Helvetica-Oblique, Helvetica-BoldOblique.\n");
		fprintf(stderr,"La police sera remplacée par 'Helvetica'.\n");
		asprintf(&ft,"FV");
	}

	if(transform != NULL)
	{
		char* t = pdf_transform(transform);
		asprintf(&stream,"q %s rg %s BT /%s %d. Tf 1 0 0 -1 0 0 Tm (%s) Tj ET Q",color,t,ft,coord[2],str);
		free(transform);
		free(t);
	}
	else
	{
		asprintf(&stream,"q %s rg BT /%s %d. Tf 1 0 0 -1 0 0 Tm (%s) Tj ET Q",color,ft,coord[2],str);
	}

	free(str);
	free(color);
	free(font);
	free(coord);

	return stream;
}

char* pdf_stream_figure(pdf_t* p, xelement_t* e)
{
	char* str = NULL;
	if(strcmp((*e).name,"circle") == 0) str = pdf_stream_circle(e);
	else if(strcmp((*e).name,"rect") == 0) str = pdf_stream_rect(e);
	else if(strcmp((*e).name,"line") == 0) str = pdf_stream_line(e);
	else if(strcmp((*e).name,"text") == 0) str = pdf_stream_text(e);
	else if(strcmp((*e).name,"image") == 0) str = pdf_stream_image(p,e);

	return str;
}

char* pdf_stream_from_svg(pdf_t* p, xelement_t* svg)
{
	char* stream = NULL;
	asprintf(&stream,"q");
	char* transform = NULL;

	int cpt = 0;
	xelement_t* e = (*svg).content.son;
	xattribute_t* a;

	while(e != NULL)
	{
		if(strcmp((*e).name,"g") == 0)
		{
			a = (*e).attributes;
			while(a != NULL)
			{
				if(strcmp((*a).name,"transform") == 0)
				{
					asprintf(&transform,"%s",pdf_transform((*a).val));
					break;
				}
				a = (*a).next;
			}

			if(transform != NULL)
			{
				if(cpt == 0) asprintf(&stream,"q %s",transform);
				else asprintf(&stream,"%s q %s",stream,transform);
			}
			else
			{
				if(cpt == 0) asprintf(&stream,"q");
				else asprintf(&stream,"%s q",stream);
			}
			asprintf(&stream,"%s %s Q",stream,pdf_stream_from_svg(p,e));
		}
		else
		{
			char* fig = NULL;
			asprintf(&fig,"%s",pdf_stream_figure(p,e));
			if(cpt == 0) asprintf(&stream,"%s",fig);
			else asprintf(&stream,"%s %s",stream,fig);
			free(fig);
		}
		cpt++;
		e = (*e).bro;
	}
	free(transform);

	return stream;
}

pdf_t* pdf_convert(xelement_t* svg)
{
	pdf_t* p = pdf_create_from_svg(svg);
	char* stream = pdf_stream_from_svg(p,svg);
	xattribute_t* a = (*svg).attributes;
	while(a != NULL)
	{
		if(strcmp((*a).name,"height") == 0)
		{
			int h = strtol((*a).val,NULL,10);
			asprintf(&stream,"q 1 0 0 -1 0 %d cm %s Q ",h,stream);
			break;
		}
		a = (*a).next;
	}
	pdf_set_content(p,0,stream);

	free(stream);
	return p;
}

// --- Fonctions du menu ---
int nb_figures(xelement_t* xel, int cpt)
{
	xelement_t* e = (*xel).content.son;
	while(e != NULL)
	{
		cpt++;
		if(strcmp((*e).name,"g") == 0) cpt = nb_figures(e,cpt);
		e = (*e).bro;
	}

	return cpt;
}

int nb_groups(xelement_t* xel, int cpt)
{
	xelement_t* e = (*xel).content.son;
	while(e != NULL)
	{
		if(strcmp((*e).name,"g") == 0) cpt = 1 + nb_groups(e,cpt);
		e = (*e).bro;
	}

	return cpt;
}

int menu_groups(xelement_t* xel, int cpt)
{
	xelement_t* e = (*xel).content.son;
	xattribute_t* a;
	if(e == NULL)
	{
		fprintf(stdout,"Le fichier ne contient pas de figures.\n");
		return 0;
	}
	while(e != NULL)
	{
		a = (*e).attributes;
		if(strcmp((*e).name,"g") == 0)
		{
			cpt++;
			fprintf(stdout,"%d - Groupe",cpt);
		}
		else if(strcmp((*e).name,"circle") == 0){ fprintf(stdout,"    Cercle"); }
		else if(strcmp((*e).name,"rect") == 0){ fprintf(stdout,"    Rectangle"); }
		else if(strcmp((*e).name,"line") == 0){ fprintf(stdout,"    Ligne"); }
		else if(strcmp((*e).name,"text") == 0){ fprintf(stdout,"    Texte"); }
		else if(strcmp((*e).name,"image") == 0){ fprintf(stdout,"    Image"); }

		if(a != NULL) fprintf(stdout,": ");
		else fprintf(stdout," ");
		while(a != NULL)
		{
			fprintf(stdout,"%s=\"%s\" ",(*a).name,(*a).val);
			a = (*a).next;
		}
		if(strcmp((*e).parent->name,"g") == 0)
		{
			if(strcmp((*e).name,"g") == 0) fprintf(stdout,"(dans groupe)");
			else fprintf(stdout,"(dans groupe)");
		}
		fprintf(stdout,"\n");
		if(strcmp((*e).name,"g") == 0)
		{
			fprintf(stdout,"=== Début du groupe ===\n");
			cpt = menu_groups(e,cpt);
			fprintf(stdout,"=== Fin du groupe ===\n");
		}

		e = (*e).bro;
	}

	return cpt;
}

int menu_figures(xelement_t* xel, int cpt)
{
	xelement_t* e = (*xel).content.son;
	xattribute_t* a;

	while(e != NULL)
	{
		a = (*e).attributes;
		cpt++;
		if(strcmp((*e).name,"g") == 0 && cpt != 1) fprintf(stdout,"\n");
		fprintf(stdout,"%d - ",cpt);
		if(strcmp((*e).name,"g") == 0)
		{
			fprintf(stdout,"Groupe");
		}
		else if(strcmp((*e).name,"circle") == 0){ fprintf(stdout,"Cercle"); }
		else if(strcmp((*e).name,"rect") == 0){ fprintf(stdout,"Rectangle"); }
		else if(strcmp((*e).name,"line") == 0){ fprintf(stdout,"Ligne"); }
		else if(strcmp((*e).name,"text") == 0){ fprintf(stdout,"Texte"); }
		else if(strcmp((*e).name,"image") == 0){ fprintf(stdout,"Image"); }

		if(a != NULL) fprintf(stdout,": ");
		else fprintf(stdout," ");
		while(a != NULL)
		{
			fprintf(stdout,"%s=\"%s\" ",(*a).name,(*a).val);
			a = (*a).next;
		}
		if(strcmp((*e).parent->name,"g") == 0)
		{
			if(strcmp((*e).name,"g") == 0) fprintf(stdout,"(dans groupe)");
			else fprintf(stdout,"(dans groupe)");
		}
		fprintf(stdout,"\n");
		if(strcmp((*e).name,"g") == 0)
		{
			fprintf(stdout,"=== Début du groupe ===\n");
			cpt = menu_figures(e,cpt);
			fprintf(stdout,"=== Fin du groupe ===\n");
		}

		e = (*e).bro;
	}

	return cpt;
}

int transform_figure(xelement_t* xel, int cpt, int nb, char* transform)
{
	xelement_t* e = (*xel).content.son;
	while(e != NULL)
	{
		cpt++;
		if(cpt == nb)
		{
			fprintf(stdout,"==========\nTransformation de: %s (figure n°%d) en cours...\n",(*e).name,cpt);
			sleep(1);
			svg_add_transform(e,transform);
			fprintf(stdout,"Transformation réussie.\n");
			break;
		}
		if(strcmp((*e).name,"g") == 0) cpt = transform_figure(e,cpt,nb,transform);

		e = (*e).bro;
	}

	return cpt;
}

int delete_figure(xelement_t* xel, int cpt, int nb)
{
	xelement_t* e = (*xel).content.son;
	xelement_t* tmp = (*xel).content.son;
	while(e != NULL)
	{
		cpt++;
		if(cpt == nb)
		{
			fprintf(stdout,"==========\nSuppression de: %s (figure n°%d) en cours...\n",(*e).name,cpt);
			sleep(1);
			if((*e).parent->content.son == e) (*e).parent->content.son = (*e).bro;
			else (*tmp).bro = (*e).bro;
			(*e).parent = NULL;
			delete_xelement(e);
			fprintf(stdout,"Suppression réussie.\n");
			break;
		}
		if(strcmp((*e).name,"g") == 0) cpt = delete_figure(e,cpt,nb);
		tmp = e;
		e = (*e).bro;
	}

	return cpt;
}

int group_figure(xelement_t* xel, xelement_t* g, int cpt, int nb)
{
	xelement_t* e = (*xel).content.son;
	xelement_t* tmp = (*xel).content.son;
	while(e != NULL)
	{
		cpt++;
		if(cpt == nb)
		{
			if((*e).parent->content.son == e) (*e).parent->content.son = (*e).bro;
			else (*tmp).bro = (*e).bro;
			(*e).parent = NULL;
			(*e).bro = NULL;
			fprintf(stdout,"==========\nAjout de '%s' (figure n°%d) dans un groupe en cours...\n",(*e).name,cpt);
			sleep(1);
			add_sub_xelement(g,e);
			fprintf(stdout,"Ajout dans le groupe réussi.\n");
			break;
		}
		if(strcmp((*e).name,"g") == 0) cpt = group_figure(e,g,cpt,nb);
		tmp = e;
		e = (*e).bro;
	}

	return cpt;
}

int delete_group(xelement_t* xel, int cpt, int nb) // Dégrouper
{
	xelement_t* e = (*xel).content.son;
	xelement_t* pt_e = (*xel).content.son;
	while(e != NULL)
	{
		if(strcmp((*e).name,"g") == 0) cpt++;
		if(cpt == nb)
		{
			fprintf(stdout,"==========\nDégroupage du groupe n°%d en cours...\n",cpt);
			sleep(1);

			xelement_t* first = (*e).content.son;
			xelement_t* last = (*e).content.son;
			while((*last).bro != NULL)
			{
				(*last).parent = (*e).parent;
				last = (*last).bro;
			}
			(*first).parent = (*e).parent;
			(*last).parent = (*e).parent;

			(*last).bro = (*e).bro;
			if((*e).parent->content.son == e) (*e).parent->content.son = first;
			else (*pt_e).bro = first;

			(*e).parent = NULL;
			delete_xelement(e);
			fprintf(stdout,"Dégroupage réussi.\n");
			break;
		}
		if(strcmp((*e).name,"g") == 0) cpt = delete_group(e,cpt,nb);
		pt_e = e;
		e = (*e).bro;
	}

	return cpt;
}
