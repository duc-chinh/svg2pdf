#ifndef MAIN_H
#define MAIN_H

// --- Fichier ---
void main_open();
void main_save();
void main_display();
void main_quit();
void main_back();
// --- Figures de base ---
void main_add_circle();
void main_add_rect();
void main_add_line();
void main_add_text();
void main_add_image();
void main_delete_figure();
// --- Transformation et groupe ---
void main_translate();
void main_rotate();
void main_scale();
void main_group();
void main_degroup();
// Ouverture avec options
void opt_open(const char* name);
void opt_save(const char* name);
void opt_rect(const char* dim);
void opt_line(int l);
void opt_text(const char* text);
void opt_jpg(const char* jpg);
void opt_delete_figure(int user);
void opt_scale(const char* transform);
void opt_rotate(const char* transform);
void opt_translate(const char* transform);
void opt_group(const char* list);

#endif