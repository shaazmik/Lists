#ifndef LIST_H
#define LIST_H

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

#define Int_t

//============================================================================================

#ifdef Int_t

typedef int element_t;

#endif

#ifdef Double_t

typedef double element_t;

#endif 

//============================================================================================


#define FATAL_ERROR abort()

const size_t Add_capacity = 7;

struct Plist_t
{
    element_t value;

    size_t next;

    size_t prev = -1;
};


struct Plist
{
    size_t capacity = -1;

    size_t size = -1;

    size_t head = -1;

    size_t tail = -1;

    struct Plist_t* data; 

    size_t free_el_index;

    int sort = NOT_SORTED;

    int err = OK; 
};


enum plist_errors
{
    OK                        = 0x0001,
    OUT_OF_RANGE_NUMBER       = 0x09EA,
    ERR_NULLPTR               = 0xFFFF,
    NOT_SORTED                = 0XFF00,
    SORTED                    = 0xFF11,
    ERR_HUGE_CAPACITY         = 0x1010,
    ERR_WRONG_FREE_ARG        = 0xA1FA,
    ERR_WRONG_HEAD            = 0xAAAA,
    ERR_WRONG_TAIL            = 0xBBBB,
    ERR_SIZE_MO_CAPACITY      = 0x09EB,
    ERR_WRONG_ADDRESSING      = 0x1111,
    ERR_WRONG_ADDRESSING_HEAD = 0x2222,
    ERR_WRONG_ADDRESSING_TAIL = 0x3333,
};



const  element_t Otrava = -1337; 

//============================================================================================

void check_nullptr(struct Plist* list);

void plist_constructor(struct Plist* list, size_t user_capacity);

void plist_destructor(struct Plist* list);

void plist_free_fill(struct Plist* list, size_t start_elemenet_index, size_t count);

size_t plist_insert_start(struct Plist* list, element_t value);

size_t plist_insert_first(struct Plist* list, element_t value);

size_t plist_insert_last(struct Plist* list, element_t value);

size_t plist_insert_before(struct Plist* list, element_t value, size_t number);

size_t plist_insert_after(struct Plist* list, element_t value, size_t number);

void plist_delete_el(struct Plist* list, size_t number);

struct Plist_t* Plist_resize(struct Plist* list);

size_t verification(struct Plist* list);

#endif 