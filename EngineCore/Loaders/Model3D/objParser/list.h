#ifndef __LIST_H
#define __LIST_H

namespace ObjParser 
{
	typedef struct
	{
		int item_count;
		int current_max_size;
		char growable;

		void **items;
		char **names;	
	} list;

	void list_make(ObjParser::list *listo, int size, char growable);
	int list_add_item(ObjParser::list *listo, void *item, char *name);
	char* list_print_items(ObjParser::list *listo);
	void* list_get_name(ObjParser::list *listo, char *name);
	void* list_get_index(ObjParser::list *listo, int indx);
	void* list_get_item(list *listo, void *item_to_find);
	int list_find(ObjParser::list *listo, char *name_to_find);
	void list_delete_index(ObjParser::list *listo, int indx);
	void list_delete_name(ObjParser::list *listo, char *name);
	void list_delete_item(ObjParser::list *listo, void *item);
	void list_delete_all(ObjParser::list *listo);
	void list_print_list(ObjParser::list *listo);
	void list_free(ObjParser::list *listo);

	void test_list();
}
#endif
