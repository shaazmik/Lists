#include ".\List.h"

void check_nullptr(struct Plist* list)
{
    if (list == nullptr)
    {
        FILE* log = fopen("log.txt", "a");
        assert(log != nullptr);

        fprintf(log, "List [NULL_PTR_ERROR #%d] [0x000000]\n ", ERR_NULLPTR);

        fflush(log);

        FATAL_ERROR;
    }
}


void plist_free_fill(struct Plist* list, size_t start_elemenet_index, size_t count)
{
    check_nullptr(list);

    for (int i = start_elemenet_index; i < count; i++)
    {
        list->data[i].prev = -1;
        list->data[i].next = i + 1;
        list->data[i].value = Otrava;
    }

    list->data[count].value = Otrava;
    list->data[count].prev  = -1;
    list->data[count].next  = 0;
}


void plist_constructor(struct Plist* list, size_t user_capacity)
{
    check_nullptr(list);

    if (user_capacity < 1)
    {
        printf("ERROR:%d wrong capacity\n\n", OUT_OF_RANGE_NUMBER);
        list->err = OUT_OF_RANGE_NUMBER;
        return;
    }

    list->capacity = user_capacity + 1;
    list->free_el_index = 1;
    list->sort = SORTED;
    list->err = OK;
    list->size = 0;

    list->data = (Plist_t*)calloc(1 + user_capacity, sizeof(Plist_t));

    if (list->data == nullptr)
    {
        printf("Memory error, too a lot capacity\n\n");
        list->err = ERR_HUGE_CAPACITY;
        //TODO dump
        return;
    }
    
    assert(list->data != nullptr);

    list->head = 0;
    list->tail = 0;
    list->data[0].next = 0;
    list->data[0].prev = 0;
    list->data[0].value = Otrava;    

    plist_free_fill(list, 1, user_capacity);

}


void plist_destructor(struct Plist* list)
{
    check_nullptr(list);

    free(list->data);

    list->capacity      = Otrava;
    list->err           = Otrava;
    list->free_el_index = Otrava;
    list->head          = Otrava;
    list->tail          = Otrava;
    list->size          = Otrava;

    list->data = nullptr;

}


size_t plist_insert_start(struct Plist* list, element_t value)
{
    check_nullptr(list);

    //TODO verificator + resize
    
    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next  = 0;
    list->data[pos].prev  = 0;
    list->data[pos].value = value;

    list->size++;

    list->head = pos;
    list->tail = pos;

    //verificator

    return pos;
}


size_t plist_insert_first(struct Plist* list, element_t value)
{
    check_nullptr(list);

    //TODO verificator + resize
    
    if (list->size == 0)
    {
        return(plist_insert_start(list, value));
    }

    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next  = list->head; 
    list->data[pos].prev  = list->data[list->head].prev;
    list->data[pos].value = value;

    list->data[list->head].prev = pos;

    list->size++;
    list->head = pos;
    list->sort = NOT_SORTED;

    //verificator

    return pos;
}


size_t plist_insert_last(struct Plist* list, element_t value)
{
    check_nullptr(list);

    //TODO verificator + resize

    if (list->size == 0)
    {
        return(plist_insert_start(list, value));
    }

    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next        = list->data[list->tail].next;
    list->data[pos].prev        = list->tail;
    list->data[pos].value       = value;

    list->data[list->tail].next = pos;

    list->size++;
    list->sort = NOT_SORTED;
    list->tail = pos;

    //verification 

    return pos;
}


size_t plist_insert_before(struct Plist* list, element_t value, size_t number)
{
    check_nullptr(list);

    //TODO verification + resize

    if (list->size == 0)
    {
        return(plist_insert_start(list, value));
    }

    if (number == list->head)
    {
        return(plist_insert_first(list, value));
    }

    if (number > list->capacity)
    {
        printf("Error: Wrong number, more than capacity\n\n");
        return OUT_OF_RANGE_NUMBER;    
    }

    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next  = number;
    list->data[pos].prev  = list->data[number].prev;
    list->data[pos].value = value;

    list->data[number].prev = pos;
    list->data[list->data[number].prev].next = pos;

    list->size++;
    list->sort = NOT_SORTED;

    //verificator

    return pos;
}

size_t plist_insert_after(struct Plist* list, element_t value, size_t number)
{
    check_nullptr(list);

    //TODO verificator + resize

    if (list->size == 0)
    {
        return(plist_insert_start(list, value));
    }

    if (number == list->tail)
    {
        return(plist_insert_last(list, value));
    }

    if (number > list->capacity)
    {
        printf("Error: Wrong number, more than capacity\n\n");
        return OUT_OF_RANGE_NUMBER;    
    }

    size_t pos = list->free_el_index;

    list->free_el_index = list->data[pos].next;

    list->data[pos].next  = list->data[number].next;
    list->data[pos].prev  = number;
    list->data[pos].value = value;

    list->data[number].next                  = pos;
    list->data[list->data[number].next].prev = pos;

    list->size++;
    list->sort = NOT_SORTED;

    //verification

    return pos;
}


void plist_delete_el(struct Plist* list, size_t number)
{
    check_nullptr(list);

    //TODO verification

    if (number != list->tail)
    {
        list->sort = NOT_SORTED;
    }

    if (list->size == 1)
    {
        list->head = 0;
        list->tail = 0;
    }

    if (list->tail == number)
    {
        list->data[list->data[number].prev].next = 0;
        list->tail = list->data[number].prev;
    }
    else
    {
        if (list->head == number)
        {
            list->data[list->data[number].next].prev = 0;
            list->head = list->data[number].next;
        }
        else
        {
            list->data[list->data[number].next].prev = list->data[number].prev;
            list->data[list->data[number].prev].next = list->data[number].next;
        }
    }


    list->data[number].value = Otrava;
    list->data[number].next = list->free_el_index;
    list->data[number].prev = -1;


    list->free_el_index = number;

    list->size--;

    if (verification(list) != OK)
    {
        //TODO dd
    }
}



struct Plist_t* plist_resize(struct Plist* list)
{
    check_nullptr(list);

    //TODO verificator

    Plist_t* tmp_pointer = (Plist_t*)realloc(list->data, sizeof(Plist_t) * (list->capacity + Add_capacity) );

    if (tmp_pointer == nullptr)
    {
        printf("Memory if full. New element isn't added.\n\n");

        list->err = ERR_NULLPTR;

        return nullptr;
    }
    else
    {
        list->data = tmp_pointer;

        plist_free_fill(list, list->capacity + 1, Add_capacity);

        list->free_el_index = list->capacity + 1;
        list->capacity += Add_capacity;

        return list->data;
    }

    //verificator
}

size_t verificator(struct Plist* list)
{
    check_nullptr(list);

    if (list->capacity < 0)
    {
        list->err = OUT_OF_RANGE_NUMBER;
        return OUT_OF_RANGE_NUMBER;
    }

    if ( (list->data[0].value != Otrava) || (list->data[0].prev != 0) || (list->data[0].next != 0) )
    {
        list->err = ERR_WRONG_ADDRESSING;
        return ERR_WRONG_ADDRESSING;
    }

    for (int i = 1; i <= list->capacity; i++)
    {
        if ( (list->data[i].value == Otrava) && (list->data[i].prev != -1))
        {
            list->err = ERR_WRONG_FREE_ARG;
            return ERR_WRONG_FREE_ARG;
        }
    }

    if ( (list->data[list->head].prev != 0) && (list->head != 0) )
    {
        list->err = ERR_WRONG_HEAD;
        return ERR_WRONG_HEAD;
    }

    if ( (list->data[list->tail].next != 0) && (list->tail != 0) )
    {
        list->err = ERR_WRONG_TAIL;
        return ERR_WRONG_TAIL;
    }

    if (list->capacity < list->size)
    {
        list->err = ERR_SIZE_MO_CAPACITY;
        return ERR_SIZE_MO_CAPACITY;
    }

    if (list->data == nullptr)
    {
        list->err = ERR_NULLPTR;
        return ERR_NULLPTR;
    }

    int count = 1;

    while (list->data[count].value == Otrava) count++;

    int next_index = count;

    while (next_index != list->tail && count <= list->capacity)
    {
        next_index = list->data[next_index].next;
        count++;
    }

    if (next_index != list->tail)
    {
        list->err = ERR_WRONG_ADDRESSING_TAIL;
        return ERR_WRONG_ADDRESSING_TAIL;
    }
    
    count = list->capacity;
        
    while (list->data[count].value == Otrava) count--;

    int next_index = count;

    while (next_index != list->head && count >= 0)
    {
        next_index = list->data[next_index].prev;
        count--;
    }

    if (next_index != list->head)
    {
        list->err = ERR_WRONG_ADDRESSING_HEAD;
        return ERR_WRONG_ADDRESSING;
    }

    return OK;
}

void case_err(struct Plist* list)
{
    check_nullptr(list);
    
    switch (list->err)
    {
    case OUT_OF_RANGE_NUMBER:
        plist_print_err(list, OUT_OF_RANGE_NUMBER);
        break;

    case ERR_NULLPTR:
        plist_print_err(list, ERR_NULLPTR);
        break;
    
    case ERR_HUGE_CAPACITY:
        plist_print_err(list, ERR_HUGE_CAPACITY);
        break;

    case ERR_WRONG_FREE_ARG:
        plist_print_err(list, ERR_WRONG_FREE_ARG);
        break;

    case ERR_WRONG_HEAD:
        plist_print_err(list, ERR_WRONG_HEAD);
        break;

    case ERR_WRONG_TAIL:
        plist_print_err(list, ERR_WRONG_TAIL);
        break;

    case ERR_SIZE_MO_CAPACITY:
        plist_print_err(list, ERR_SIZE_MO_CAPACITY);
        break;

    case ERR_WRONG_ADDRESSING:
        plist_print_err(list, ERR_WRONG_ADDRESSING);
        break;

    case ERR_WRONG_ADDRESSING_HEAD:
        plist_print_err(list, ERR_WRONG_ADDRESSING_HEAD);
        break;

    case ERR_WRONG_ADDRESSING_TAIL:
        plist_print_err(list, ERR_WRONG_ADDRESSING_TAIL);
        break;

    default:
        break;
    }
}


void plist_print_err(struct Plist* list, size_t error)
{
    check_nullptr(list);

    printf("List (ERROR #%x: memory cell: [0x%x] \n"
           "{\n"
            "\tsize = %u\n" 
            "\tcapacity = %u\n"
            "\thead = %u\n" 
            "\ttail = %u\n"  
            "\tdata[0x%x]\n"
            "\tfirst_free_element = %u\n"
            "}\n\n\n",  
            error, list->data, list->size,  
            list->capacity, list->head, list->tail, 
            list->data, list->free_el_index       );
}

void plist_dump(struct Plist* list, FILE* file)
{
    check_nullptr(list);
    assert(file != nullptr);

    if (list->err != OK)
    {
        case_err(list);

        fprintf(file, "List (ERROR #%x: memory cell: [0x%x] \n"
                      "{\n"
                      "\tsize = %u\n" 
                      "\tcapacity = %u\n"
                      "\thead = %u\n" 
                      "\ttail = %u\n"  
                      "\tdata[0x%x]\n"
                      "\tfirst_free_element = %u\n"
                      "}\n\n\n",  
                    list->err, list->data, list->size,  
                    list->capacity, list->head, list->tail, list->data, list->free_el_index);

    fflush(file);
    }
    else
    {
        printf("All is ok!!!\n\n\n");
    }
}






//TODO dump
//TODO linearization 
//TODO list find'ы
//TODO графический дамп