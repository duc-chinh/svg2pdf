#ifndef SVG_H
#define SVG_H

#include <stdlib.h>
#include <stdbool.h>
#include "xml.h"
#include "pdf.h"

typedef struct coord_s
{
	double x;
	double y;
} coord_t;

enum figure_e
{
	CIRCLE = 1, RECTANGLE, LINE, GROUP
} figure_e;

typedef struct circle_s
{
	double r;
} circle_t;

typedef struct rectangle_s
{
	double w;
	double h;
} rectangle_t;

typedef struct line_s
{
	double len;
} line_t;

typedef struct figure_s figure_t;

typedef struct group_s
{
	figure_t** g;
	int nb;
} group_t;

typedef char color_t;
struct figure_s
{
	color_t c;
	coord_t* g;
	double a;
	enum figure_e type;
	union
	{
		circle_t* circ;
		rectangle_t* rect;
		line_t* l;
		group_t* group;
	} specs;
};

typedef struct image_s
{
	unsigned int w;
	unsigned int h;
	double g;
	figure_t** f;
	int nb;
} image_t;

// --- SVG ---
xelement_t* svg_create(size_t width, size_t height, const char* mesure);
void svg_add_title(xelement_t* svg, const char* title);
void svg_add_desc(xelement_t* svg, const char* desc);
bool svg_color(char* c);
xelement_t* svg_rect(int x, int y, int width, int height, const char* color);
xelement_t* svg_circle(int cx, int cy, int r, const char* color);
xelement_t* svg_line(int x1, int y1, int x2, int y2, const char* color, int stroke);
xattribute_t* svg_add_transform(xelement_t* e, const char* value);
xattribute_t* svg_translate(xelement_t* e, int dx, int dy);
xattribute_t* svg_rotate(xelement_t* e, int th);
xattribute_t* svg_scale(xelement_t* e, int s);
xelement_t* svg_text(int x, int y, const char* font, int size, const char* color, const char* str);
xelement_t* svg_image(int x, int y, int width, int height, const char* ratio, const char* href);
xelement_t* svg_add_group(xelement_t* g, xelement_t* figure);
void svg_save(xelement_t* svg, const char* fname);
xelement_t* svg_load(const char* fname);
// --- ASCII_ART ---
void print_coord(coord_t* c);
coord_t* coordinate(double x, double y);
coord_t* inv_translate_point(coord_t* p, coord_t* v);
coord_t* inv_rotate_point(coord_t* p, double t);
figure_t* circle(xelement_t* e);
figure_t* rectangle(xelement_t* e);
figure_t* line(xelement_t* e);
figure_t* group();
figure_t* group_append(figure_t* g, figure_t* f);
figure_t* color(figure_t* f, color_t c);
figure_t* translate(figure_t* f, double dx, double dy);
figure_t* rotate(figure_t* f, double dt);
figure_t* scale(figure_t* f, double s);
color_t intersect(coord_t* p, figure_t* f, double grain);
image_t* image(unsigned int width, unsigned int height, double grain);
image_t* append(image_t* img, figure_t* f);
image_t* prepend(image_t* img, figure_t* f);
void paint(image_t* img);
// --- TRAITEMENT SVG/ASCII ---
image_t* image_create_from_svg(xelement_t* svg);
void image_transform(figure_t* f, char* transform);
figure_t* image_figure(xelement_t* e);
image_t* image_from_svg(image_t* i, figure_t* f, xelement_t* e);
image_t* image_convert(xelement_t* svg);
void image_delete(image_t* img);
// --- TRAITEMENT SVG/PDF ---
pdf_t* pdf_create_from_svg(xelement_t* svg);
char* pdf_transform(char* transform);
char* pdf_color(char* c);
char* pdf_stream_line(xelement_t* xel);
char* pdf_stream_circle(xelement_t* xel);
char* pdf_stream_image(pdf_t* p, xelement_t* xel);
char* font_text(char* font);
char* pdf_stream_text(xelement_t* xel);
char* pdf_stream_figure(pdf_t* p, xelement_t* e);
char* pdf_stream_from_svg(pdf_t* p, xelement_t* svg);
pdf_t* pdf_convert(xelement_t* svg);
// --- FONCTIONS DU MENU ---
int nb_figures(xelement_t* xel, int cpt);
int nb_groups(xelement_t* xel, int cpt);
int menu_groups(xelement_t* xel, int cpt);
int menu_figures(xelement_t* xel, int cpt);
int transform_figure(xelement_t* xel, int cpt, int nb, char* transform);
int delete_figure(xelement_t* xel, int cpt, int nb);
int group_figure(xelement_t* xel, xelement_t* g, int cpt, int nb);
int delete_group(xelement_t* xel, int cpt, int nb);

#endif