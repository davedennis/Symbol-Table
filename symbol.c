#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "symbol.h"

/** @file symbol.c
 *  @brief You will modify this file and implement nine functions
 *  @details Your implementation of the functions defined in symbol.h.
 *  You may add other functions if you find it helpful. Added functions
 *  should be declared <b>static</b> to indicate they are only used
 *  within this file. The reference implementation added approximately
 *  90 lines of code to this file. This count includes lines containing
 *  only a single closing bracket (}).
 * <p>
 * @author <b>Your name</b> goes here
 */

/** size of LC3 memory */
#define LC3_MEMORY_SIZE  65536

/** Provide prototype for strdup() */
char *strdup(const char *s);

/** defines data structure used to store nodes in hash table */
typedef struct node {
  symbol_t     symbol;   /**< the data the user is interested in   */
  int          hash;     /**< hash value - makes searching faster  */
  struct node* next;     /**< linked list of symbols at same index */
} node_t;

/** defines the data structure for the hash table */
struct sym_table {
  node_t*  hash_table[SYMBOL_SIZE]; /**< head of linked list for this index */
  char**   addr_table;              /**< look up symbols by addr            */
};

/** djb hash - found at http://www.cse.yorku.ca/~oz/hash.html
 * tolower() call to make case insensitive.
 */

static int symbol_hash (const char* name) {
  unsigned char* str  = (unsigned char*) name;
  unsigned long  hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + tolower(c); /* hash * 33 + c */

  c = hash & 0x7FFFFFFF; /* keep 31 bits - avoid negative values */

  return c;
}

/** @todo implement this function */
sym_table_t* symbol_init (int lookup_by_addr) {
	sym_table_t* tab = calloc (1,sizeof(sym_table_t));
	if(lookup_by_addr > 0){
	tab -> addr_table = (char**) calloc(LC3_MEMORY_SIZE,sizeof(char*));
	}
	return 	tab;
}

/** @todo implement this function */
void symbol_term (sym_table_t* symTab) {
	symbol_reset(symTab);	
	free(symTab);
}

/** @todo implement this function */
void symbol_reset(sym_table_t* symTab) {
	for(int i = 0; i < SYMBOL_SIZE;i++){
		for(node_t* nodepointer = symTab -> hash_table[i]; nodepointer != NULL; 		nodepointer = nodepointer -> next){		
			if(nodepointer != NULL){			
			free(nodepointer -> symbol.name);			
			free(nodepointer);
			}
		}
		symTab -> hash_table[i] = NULL;
	}
	if(symTab -> addr_table !=NULL){
		for(int i = 0; i < SYMBOL_SIZE;i++){
			symTab -> addr_table[i] = NULL;
		}
	}
}

/** @todo implement this function */
int symbol_add (sym_table_t* symTab, const char* name, int addr) {
	int hash;
        int index;		
	node_t* node = symbol_search(symTab, name, &hash, &index);
	struct node* node1 = calloc (1,sizeof(node));		
	if(node == NULL){
		//struct node* pointer = (struct node*) calloc (1,sizeof(struct node));	
		node_t* hello = malloc(sizeof(node_t));
		hello->symbol.name = strdup(name);
		hello->symbol.addr = addr;
		hello->hash = hash;
		hello->next = symTab -> hash_table[index];		
		symTab->hash_table[index] = hello;
		return 1;
	}
	if(symTab -> addr_table !=NULL){
		symTab -> addr_table[addr] = node1 -> symbol.name;
	}

  return 0;
}

/** @todo implement this function */
struct node* symbol_search (sym_table_t* symTab, const char* name, int* hash, int* index) {
  *hash = symbol_hash(name);
  *index = (*hash % SYMBOL_SIZE);
  // node_t* nodepointer = symTab -> hash_table[*index];
	if(symTab -> hash_table[*index] == NULL){
		return NULL;
	}
	else{	
	for(node_t* nodepointer = symTab -> hash_table[*index]; nodepointer != NULL; 		nodepointer = nodepointer -> next){
		
		int compare = strcmp(name,nodepointer -> symbol.name);
		if(compare == 0){
			return nodepointer;
			}	
	}
	}
	
  return NULL;
}

/** @todo implement this function */
symbol_t* symbol_find_by_name (sym_table_t* symTab, const char* name) {
	int hash;
	int index;	
	node_t* node = symbol_search(symTab, name, &hash, &index);
	if(node != NULL){
		return &node -> symbol;
	}

	return NULL;
}

/** @todo implement this function */
char* symbol_find_by_addr (sym_table_t* symTab, int addr) {
  return symTab -> addr_table[addr];
}

/** @todo implement this function */
void symbol_iterate (sym_table_t* symTab, iterate_fnc_t fnc, void* data) {
	int track = 0;	
	for(int i = 0; i < SYMBOL_SIZE;i++){
		for(node_t* nodepointer = symTab -> hash_table[i]; nodepointer != NULL; 		nodepointer = nodepointer -> next){
			if(nodepointer != NULL){
				track++;
				(*fnc)(&nodepointer -> symbol, data);
			}
		
		}
	}
}

