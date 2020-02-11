#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "pdf.h"

// --- Objets PDF ---
pdf_object_t* pdf_null(pdf_object_t* o)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	(*o).type = T_NULL;
	(*o).obj.b = NULL;

	return o;
}

pdf_object_t* pdf_bool(pdf_object_t* o, bool b)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	(*o).type = T_BOOL;
	(*o).obj.b = b;
	return o;
}

pdf_object_t* pdf_int(pdf_object_t* o, int i)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	(*o).type = T_INT;
	(*o).obj.i = i;

	return o;
}

pdf_object_t* pdf_real(pdf_object_t* o, float f)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	(*o).type = T_FLOAT;
	(*o).obj.f = f;
	
	return o;
}

pdf_object_t* pdf_reference(pdf_object_t* o, unsigned int id)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	(*o).type = REFERENCE;
	(*o).obj.ref = id;
	
	return o;
}

pdf_object_t* pdf_name(pdf_object_t* o, const char* n)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	int len = strlen(n)+1;
	(*o).obj.name = (char*) malloc(len*sizeof(char));
	if((*o).obj.name == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*o).type = NAME;
	strcpy((*o).obj.name,n);
	
	return o;
}

pdf_object_t* pdf_string(pdf_object_t* o, const char* s)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	int len = strlen(s)+1;
	(*o).obj.str = (char*) malloc(len*sizeof(char));
	if((*o).obj.str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*o).type = STRING;
	strcpy((*o).obj.str,s);
	
	return o;
}

pdf_object_t* pdf_stream(pdf_object_t* o, size_t sz, void* data)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	(*o).obj.s.data = (void*) malloc(sz*sizeof(void));
	(*o).obj.s.info = (dict_t*) malloc(sizeof(dict_t));
	(*o).obj.s.info->key = (char*) malloc(7*sizeof(char));
	if((*o).obj.s.data == NULL || (*o).obj.s.info == NULL || (*o).obj.s.info->key == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*o).type = STREAM;
	strcpy((*o).obj.s.info->key,"Length");
	(*o).obj.s.info->val = NULL;
	(*o).obj.s.info->val = pdf_int((*o).obj.s.info->val,sz);
	(*o).obj.s.info->next = NULL;
	(*o).obj.s.data = data;

	return o;
}

pdf_object_t* pdf_array(pdf_object_t* o, size_t n)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	(*o).obj.array = (pdf_object_t**) malloc((n+1)*sizeof(pdf_object_t));
	if((*o).obj.array == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*o).type = ARRAY;
	int i;
	for(i = 0; i<(int) n; i++) (*o).obj.array[i] = pdf_null((*o).obj.array[i]);
	(*o).obj.array[n] = '\0';

	return o;
}

pdf_object_t* pdf_dictionary(pdf_object_t* o)
{
	if(o == NULL)
	{
		o = (pdf_object_t*) malloc(sizeof(pdf_object_t));
		if(o == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
	}
	else if(o != NULL && (*o).type != T_NULL)
	{
		pdf_object_free(o);
	}

	(*o).obj.dico = (dict_t*) malloc(sizeof(dict_t));
	if((*o).obj.dico == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*o).type = DICTIONARY;
	(*o).obj.dico = NULL;
	return o;
}

void pdf_object_free(pdf_object_t* o)
{
	if((*o).type == T_BOOL || (*o).type == T_INT || (*o).type == T_FLOAT || (*o).type == REFERENCE)
	{
		(*o).obj.b = NULL;
	}
	else if((*o).type == STRING)
	{
		free((*o).obj.str);
	}
	else if((*o).type == NAME)
	{
		free((*o).obj.name);
	}
	else if((*o).type == STREAM)
	{
		dict_t* tmp = (*o).obj.s.info;
		dict_t* next;
		while(tmp != NULL)
		{
			next = (*tmp).next;
			pdf_object_free((*tmp).val);
			free((*tmp).val);
			free((*tmp).key);
			free(tmp);
			tmp = next;
		}
		free((*o).obj.s.data);
	}
	else if((*o).type == ARRAY)
	{
		int i = 0;
		while((*o).obj.array[i] != '\0')
		{
			pdf_object_free((*o).obj.array[i]);
			free((*o).obj.array[i]);
			i++;
		}
		free((*o).obj.array[i]);
		free((*o).obj.array);
	}
	else if((*o).type == DICTIONARY)
	{
		dict_t* tmp = (*o).obj.dico;
		dict_t* next;
		while(tmp != NULL)
		{
			next = (*tmp).next;
			pdf_object_free((*tmp).val);
			free((*tmp).val);
			free((*tmp).key);
			free(tmp);
			tmp = next;
		}
	}
	(*o).type = T_NULL;

	return;
}

pdf_object_t* pdf_stream_from_file(pdf_object_t* o, const char* fname)
{
	FILE* fp = NULL;
	fp = fopen(fname,"rb");
	if(fp == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'ouvrir le fichier '%s'. Fermeture du programme.\n",fname);
		exit(1);
	}
	fseek(fp,0,SEEK_END);
	int len = ftell(fp);
	char* str = (char*) malloc(len*sizeof(char));
	if(str == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	rewind(fp);
	fread(str,len,1,fp);

	o = pdf_stream(o,len,str);

	fclose(fp);
	return o;
}

pdf_object_t* pdf_stream_from_string(pdf_object_t* o, const char* str)
{
	int len = strlen(str)+1;
	char* cpy = (char*) malloc(len*sizeof(char));
	if(cpy == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	strcpy(cpy,str);
	o = pdf_stream(o,len,cpy);
	
	return o;
}

pdf_object_t* pdf_array_get(pdf_object_t* a, size_t i)
{
	if((*a).type != ARRAY)
	{
		fprintf(stderr,"/!\\ L'objet PDF n'est pas un tableau. Fermeture du programme.\n");
		exit(1);
	}

	return (*a).obj.array[i];
}

pdf_object_t* pdf_dict_get(pdf_object_t* d, const char* cle)
{
	if((*d).type != DICTIONARY)
	{
		fprintf(stderr,"/!\\ L'objet PDF n'est pas un dictionnaire. Fermeture du programme.\n");
		exit(1);
	}
	dict_t* dt = (dict_t*) malloc(sizeof(dict_t));
	int len = strlen(cle)+1;
	(*dt).key = (char*) malloc(len*sizeof(char));
	if(dt == NULL || (*dt).key == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	strcpy((*dt).key,cle);
	(*dt).val = NULL;
	(*dt).val = pdf_null((*dt).val);
	(*dt).next = NULL;

	pdf_object_t* o;
	if((*d).obj.dico == NULL)
	{
		(*d).obj.dico = dt;
		o = (*d).obj.dico->val;
	}
	else if(strcmp((*d).obj.dico->key,cle) == 0)
	{
		o = (*d).obj.dico->val;
	}
	else
	{
		dict_t* tmp = (*d).obj.dico;
		bool same = false;
		while((*tmp).next != NULL)
		{
			if(strcmp((*tmp).next->key,cle) == 0)
			{
				same = true;
				break;
			}
			tmp = (*tmp).next;
		}
		if(same == false) (*tmp).next = dt;
		o = (*tmp).next->val;
	}

	return o;
}

void pdf_fprint_object(FILE* fd, pdf_object_t* o)
{
	if((*o).type == T_NULL)
	{
		fprintf(fd,"null");
	}
	else if((*o).type == T_BOOL)
	{
		if((*o).obj.b == true) fprintf(fd,"true");
		else fprintf(fd,"false");
	}
	else if((*o).type == T_INT)
	{
		fprintf(fd,"%d",(*o).obj.i);
	}
	else if((*o).type == T_FLOAT)
	{
		fprintf(fd,"%f",(*o).obj.f);
	}
	else if((*o).type == STRING)
	{
		fprintf(fd,"(%s)",(*o).obj.str);
	}
	else if((*o).type == NAME)
	{
		fprintf(fd,"/%s",(*o).obj.name);
	}
	else if((*o).type == STREAM)
	{
		fprintf(fd,"<<\n");
		dict_t* tmp = (*o).obj.s.info;
		while(tmp != NULL)
		{
			fprintf(fd,"   /%s ",(*tmp).key);
			pdf_fprint_object(fd,(*tmp).val);
			if((*tmp).val != NULL) fprintf(fd,"\n");
			tmp = (*tmp).next;
		}
		int len = (*o).obj.s.info->val->obj.i;
		fprintf(fd,">>\nstream\n");
		fwrite((*o).obj.s.data,len,1,fd);
		fprintf(fd,"endstream");
	}
	else if((*o).type == ARRAY)
	{
		int i = 0;
		fprintf(fd,"[ ");
		while((*o).obj.array[i] != '\0')
		{
			pdf_fprint_object(fd,(*o).obj.array[i]);
			fprintf(fd," ");
			i++;
		}
		fprintf(fd,"]");
	}
	else if((*o).type == DICTIONARY)
	{
		fprintf(fd,"<<\n");
		dict_t* tmp = (*o).obj.dico;
		while(tmp != NULL)
		{
			fprintf(fd,"  /%s ",(*tmp).key);
			pdf_fprint_object(fd,(*tmp).val);
			if((*tmp).val != NULL) fprintf(fd,"\n");
			tmp = (*tmp).next;
		}
		fprintf(fd,">>");
	}
	else if((*o).type == REFERENCE)
	{
		fprintf(fd,"%d 0 R",(*o).obj.ref);
	}

	return;
}
// --- Représentation d'un PDF ---
pdf_object_t* pdf_default_fonts(pdf_object_t* f)
{
	f = pdf_dictionary(f);
	pdf_object_t* font;

	font = pdf_dictionary(pdf_dict_get(f,"FT"));
	pdf_name(pdf_dict_get(font,"Type"),"Font");
	pdf_name(pdf_dict_get(font,"Subtype"),"Type1");
	pdf_name(pdf_dict_get(font,"BaseFont"),"Times-Roman");

	font = pdf_dictionary(pdf_dict_get(f,"FTB"));
	pdf_name(pdf_dict_get(font,"Type"),"Font");
	pdf_name(pdf_dict_get(font,"Subtype"),"Type1");
	pdf_name(pdf_dict_get(font,"BaseFont"),"Times-Bold");

	font = pdf_dictionary(pdf_dict_get(f,"FTI"));
	pdf_name(pdf_dict_get(font,"Type"),"Font");
	pdf_name(pdf_dict_get(font,"Subtype"),"Type1");
	pdf_name(pdf_dict_get(font,"BaseFont"),"Times-Italic");

	font = pdf_dictionary(pdf_dict_get(f,"FTBI"));
	pdf_name(pdf_dict_get(font,"Type"),"Font");
	pdf_name(pdf_dict_get(font,"Subtype"),"Type1");
	pdf_name(pdf_dict_get(font,"BaseFont"),"Times-BoldItalic");

	font = pdf_dictionary(pdf_dict_get(f,"FV"));
	pdf_name(pdf_dict_get(font,"Type"),"Font");
	pdf_name(pdf_dict_get(font,"Subtype"),"Type1");
	pdf_name(pdf_dict_get(font,"BaseFont"),"Helvetica");

	font = pdf_dictionary(pdf_dict_get(f,"FVB"));
	pdf_name(pdf_dict_get(font,"Type"),"Font");
	pdf_name(pdf_dict_get(font,"Subtype"),"Type1");
	pdf_name(pdf_dict_get(font,"BaseFont"),"Helvetica-Bold");

	font = pdf_dictionary(pdf_dict_get(f,"FVI"));
	pdf_name(pdf_dict_get(font,"Type"),"Font");
	pdf_name(pdf_dict_get(font,"Subtype"),"Type1");
	pdf_name(pdf_dict_get(font,"BaseFont"),"Helvetica-Oblique");

	font = pdf_dictionary(pdf_dict_get(f,"FVBI"));
	pdf_name(pdf_dict_get(font,"Type"),"Font");
	pdf_name(pdf_dict_get(font,"Subtype"),"Type1");
	pdf_name(pdf_dict_get(font,"BaseFont"),"Helvetica-BoldOblique");

	return f;	
}

pdf_t* pdf_create(unsigned int nb, float w, float h)
{
	int len = nb+4;
	pdf_t* p = (pdf_t*) malloc(sizeof(pdf_t));
	(*p).tab = (pdf_object_t**) malloc((len+1)*sizeof(pdf_object_t*));
	if(p == NULL || (*p).tab == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	int i = 0;
	while(i < len)
	{
		(*p).tab[i] = NULL;
		(*p).tab[i] = pdf_dictionary((*p).tab[i]);
		if(i == 0)
		{
			pdf_name(pdf_dict_get((*p).tab[i],"Type"),"Catalog");
			pdf_reference(pdf_dict_get((*p).tab[i],"Pages"),3);
		}
		else if(i == 1)
		{
			(*p).tab[i] = pdf_default_fonts((*p).tab[i]);
		}
		else if(i == 2)
		{
			pdf_name(pdf_dict_get((*p).tab[i],"Type"),"Pages");
			pdf_int(pdf_dict_get((*p).tab[i],"Count"),nb);
			pdf_array(pdf_dict_get((*p).tab[i],"Kids"),nb);
			int cpt = 5;
			for(int j = 0; j< (int)nb; j++)
			{
				pdf_reference(pdf_array_get(pdf_dict_get((*p).tab[i],"Kids"),j),cpt);
				cpt++;
			}
		}
		else if(i >= 4 && i < (int)nb+4)
		{
			pdf_name(pdf_dict_get((*p).tab[i],"Type"),"Page");
			pdf_reference(pdf_dict_get((*p).tab[i],"Parent"),3);
			
			pdf_array(pdf_dict_get((*p).tab[i],"MediaBox"),4);
			pdf_int(pdf_array_get(pdf_dict_get((*p).tab[i],"MediaBox"),0),0);
			pdf_int(pdf_array_get(pdf_dict_get((*p).tab[i],"MediaBox"),1),0);
			pdf_real(pdf_array_get(pdf_dict_get((*p).tab[i],"MediaBox"),2),w);
			pdf_real(pdf_array_get(pdf_dict_get((*p).tab[i],"MediaBox"),3),h);

			pdf_object_t* r = pdf_dictionary(pdf_dict_get((*p).tab[i],"Resources"));
			pdf_reference(pdf_dict_get(r,"Font"),2);
			pdf_reference(pdf_dict_get(r,"XObject"),4);

			pdf_null(pdf_dict_get((*p).tab[i],"Contents"));
		}
		i++;
	}

	(*p).tab[len] = '\0';

	return p;
}

void pdf_delete(pdf_t* p)
{
	int cpt = 0;
	while((*p).tab[cpt] != '\0')
	{
		pdf_object_free((*p).tab[cpt]);
		free((*p).tab[cpt]);
		cpt++;
	}
	free(p);
}

size_t pdf_new_id(pdf_t* p)
{
	int cpt = 0;
	while((*p).tab[cpt] != '\0') cpt++;
	cpt++;
	(*p).tab = (pdf_object_t**) realloc((*p).tab,(cpt+1)*sizeof(pdf_object_t*));
	if((*p).tab == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	(*p).tab[cpt-1] = NULL;
	(*p).tab[cpt-1] = pdf_null((*p).tab[cpt-1]);
	(*p).tab[cpt] = '\0';

	return cpt-1;
}

void pdf_save(const char* fname, pdf_t* p)
{
	FILE* fp = NULL;
	fp = fopen(fname,"wb");
	if(fp == NULL)
	{
		fprintf(stderr,"/!\\ Impossible de sauvegarder le fichier PDF '%s'. Fermeture du programme.\n",fname);
		exit(1);
	}
	// Entête
	fprintf(fp,"%%PDF-1.7\n%%");
	int com = 0xE2E3CFD3;
	fwrite(&com,4,1,fp);
	fprintf(fp,"\n");

	// Corps
	int n = 0;
	int* xref = (int*) malloc(sizeof(int));
	if(xref == NULL)
	{
		fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
		exit(1);
	}
	while((*p).tab[n] != '\0')
	{
		xref = (int*) realloc(xref,(n+1)*sizeof(int));
		if(xref == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
		xref[n] = ftell(fp);
		fprintf(fp,"%d 0 obj\n",n+1);
		pdf_fprint_object(fp,(*p).tab[n]);
		fprintf(fp,"\nendobj\n\n");
		n++;
	}

	// Table de références
	int trail = ftell(fp);
	fprintf(fp,"xref\n0 %d\n",n+1);
	fprintf(fp,"0000000000 65535 f\n");
	for(int i = 0; i<n; i++)
	{
		float nb = (float) xref[i];
		int cpt = 1;
		while(nb >= 10)
		{
			nb /= 10;
			cpt++;
		}
		cpt = 10 - cpt;
		int j;
		for(j = 0; j<cpt; j++) fprintf(fp,"0");
		fprintf(fp,"%d 00000 n\n",xref[i]);
	}

	// Trailer
	fprintf(fp,"trailer\n<<\n   ");
	fprintf(fp,"/Root 1 0 R\n   /Size %d\n>>\n",n+1);
	fprintf(fp,"startxref\n%d\n%%%%EOF",trail);

	free(xref);
	fclose(fp);

	return;
}

void pdf_set_content(pdf_t* p, unsigned int n, const char* instr)
{
	int i = pdf_new_id(p);
	(*p).tab[i] = pdf_stream_from_string((*p).tab[i],instr);
	
	pdf_reference(pdf_dict_get((*p).tab[4+n],"Contents"),i+1);

	return;
}

bool jpeg_info(const void* i, size_t* width, size_t* height, size_t* bpc, const char** cs)
{
	const unsigned char* im = (const unsigned char*) i;
	if(im[0] != 0xff || im[1] != 0xd8) return false;
	im += 2;

	while(im[0] == 0xff)
	{
		unsigned char frame_id = im[1];
		if(frame_id == 0xc0) break;
		size_t frame_length = 256*im[2]+im[3];
		im += frame_length+2;
	}
	size_t h = im[5]*256+im[6];
	size_t w = im[7]*256+im[8];
	size_t b = im[4];
	const char* c;
	switch(im[9])
	{
		case 1: c = "DeviceGray"; break;
		case 3: c = "DeviceRGB"; break;
		case 4: c = "DeviceCMYK"; break;
		default: return false;
	}
	if(width) *width = w;
	if(height) *height = h;
	if(bpc) *bpc = b;
	if(cs) *cs = c;

	return true;
}

char* pdf_load_image(pdf_t* p, const char* fname)
{
	int i = pdf_new_id(p);
	(*p).tab[i] = pdf_stream_from_file((*p).tab[i],fname);
	size_t w; size_t* ptr_w = &w;
	size_t h; size_t* ptr_h = &h;
	size_t bpc; size_t* ptr_bpc = &bpc;
	const char* cs; const char** ptr_cs = &cs;
	jpeg_info((*p).tab[i]->obj.s.data, ptr_w, ptr_h, ptr_bpc, ptr_cs);

	int n;
	for(n = 1; n<=7; n++)
	{
		dict_t* d = (dict_t*) malloc(sizeof(dict_t));
		(*d).key = (char*) malloc(17*sizeof(char));
		if(d == NULL || (*d).key == NULL)
		{
			fprintf(stderr,"/!\\ Impossible d'allouer de la mémoire ! Fermeture du programme.\n");
			exit(1);
		}
		(*d).val = NULL;
		(*d).next = NULL;
		if(n == 1)
		{
			strcpy((*d).key,"Type");
			(*d).val = pdf_name((*d).val,"XObject");
		}
		else if(n == 2)
		{
			strcpy((*d).key,"Subtype");
			(*d).val = pdf_name((*d).val,"Image");
		}
		else if(n == 3)
		{
			strcpy((*d).key,"Width");
			(*d).val = pdf_int((*d).val,(int) w);
		}
		else if(n == 4)
		{
			strcpy((*d).key,"Height");
			(*d).val = pdf_int((*d).val,(int) h);
		}
		else if(n == 5)
		{
			strcpy((*d).key,"BitsPerComponent");
			(*d).val = pdf_int((*d).val,(int) bpc);
		}
		else if(n == 6) 
		{
			strcpy((*d).key,"ColorSpace");
			(*d).val = pdf_name((*d).val,cs);
		}
		else if(n == 7) 
		{
			strcpy((*d).key,"Filter");
			(*d).val = pdf_name((*d).val,"DCTDecode");
		}

		dict_t* tmp = (*p).tab[i]->obj.s.info;
		while((*tmp).next != NULL) tmp = (*tmp).next;
		(*tmp).next = d;
	}

	int cpt = 0;
	dict_t* tmp = (*p).tab[3]->obj.dico;
	if(tmp != NULL)
	{
		while((*tmp).next != NULL)
		{
			cpt++;
			tmp = (*tmp).next;
		}
	}
	char* im = NULL;
	asprintf(&im,"Im%d",cpt);
	pdf_reference(pdf_dict_get((*p).tab[3],im),i+1);

	return im;
}
