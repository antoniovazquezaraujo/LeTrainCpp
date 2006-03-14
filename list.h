/*
  list.h
  The object container 
  -------------------------------------
*/
typedef struct iterator * Iterator;
typedef struct list * List;
typedef void * Listdata;

List        l_new(void); 
void        l_free(List l);
void        l_add(List l, Listdata data);
void        l_del(List l, Listdata data);
int         l_contains(List l, Listdata data);
Listdata    l_get(List list, int index);

Iterator    i_new(List l);
void        i_free(Iterator i);
int         i_hasnext(Iterator i);
Listdata    i_next(Iterator i);
void        i_reset(Iterator i, List list);
