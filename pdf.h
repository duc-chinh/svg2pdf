#ifndef PDF_H
#define PDF_H

#include <stdlib.h>

typedef enum type_obj
{
	T_NULL, T_BOOL, T_INT, T_FLOAT, STRING, NAME, STREAM, ARRAY, DICTIONARY, REFERENCE
} enum_obj;

typedef struct object_s pdf_object_t;

typedef struct dict_s
{
	char* key;
	pdf_object_t* val;
	struct dict_s* next;
} dict_t;

struct object_s
{
	enum_obj type;
	union
	{
		bool b;
		int i;
		float f;
		char* str;
		char* name;
		struct stream_s
		{
			dict_t* info;
			void* data;
		} s;
		pdf_object_t** array;
		dict_t* dico;
		unsigned int ref;
	} obj;
};

typedef struct pdf_s
{
	pdf_object_t** tab;
} pdf_t;

pdf_object_t* pdf_null(pdf_object_t* o);
pdf_object_t* pdf_bool(pdf_object_t* o, bool b);
pdf_object_t* pdf_int(pdf_object_t* o, int i);
pdf_object_t* pdf_real(pdf_object_t* o, float f);
pdf_object_t* pdf_reference(pdf_object_t* o, unsigned int id);
pdf_object_t* pdf_name(pdf_object_t* o, const char* n);
pdf_object_t* pdf_string(pdf_object_t* o, const char* s);
pdf_object_t* pdf_stream(pdf_object_t* o, size_t sz, void* data);
pdf_object_t* pdf_array(pdf_object_t* o,size_t n);
pdf_object_t* pdf_dictionary(pdf_object_t* o);
void pdf_object_free(pdf_object_t* o);
pdf_object_t* pdf_stream_from_file(pdf_object_t* o, const char* fname);
pdf_object_t* pdf_stream_from_string(pdf_object_t* o, const char* str);
pdf_object_t* pdf_array_get(pdf_object_t* a, size_t i);
pdf_object_t* pdf_dict_get(pdf_object_t* d, const char* cle);
void pdf_fprint_object(FILE* fd, pdf_object_t* o);
// --- Représentation d'un PDF
pdf_object_t* pdf_default_fonts(pdf_object_t* f);
pdf_t* pdf_create(unsigned int nb, float w, float h);
void pdf_delete(pdf_t* p);
size_t pdf_new_id(pdf_t* p);
void pdf_save(const char* fname, pdf_t* p);
void pdf_set_content(pdf_t* p, unsigned int n, const char* instr);
bool jpeg_info(const void* i, size_t* width, size_t* height, size_t* bpc, const char** cs);
char* pdf_load_image(pdf_t* p, const char* fname);

#endif