#include <stdio.h>
#include <stdlib.h>
#include "json.h"

typedef struct
{
    void*value;
} JSON_VALUE;

//type5
typedef struct JSEL{
    void*value;
    struct JSEL*linked;
} JSON_ELEMENT;

//type7
typedef struct{
    JSON_ELEMENT* value;
    void*linked;
} JSON_OBJECT;

//type6
typedef struct{
    JSON_ELEMENT* value;
    void*linked;
} JSON_ARRAY;


//type1
typedef struct{
    char* tag;
    int value;
    void*linked;
} JSON_INT;

//type2
typedef struct{
    char* tag;
    float value;
    void*linked;
} JSON_FLOAT;

//type3
typedef struct{
    char* tag;
    int value;
    void*linked;
} JSON_BOOL;

//type4
typedef struct{
    char* tag;
    char* value;
    void*linked;
} JSON_STRING;


/*
{
    "shopid": "shop1",
    "name": "상점",
    "moneyid": "won",
    "items": [
        {
            "item": "book",
            "lore": [
                "0",
                "1"
            ]
        }
    ]
}
*/
JSON_VALUE dataset;
int testjson(){
    JSON_STRING lore1 = {"", "1", NULL};
    JSON_STRING lore0 = {"", "0", &lore1};
    JSON_ELEMENT lore_el1 = {&lore1, NULL};
    JSON_ELEMENT lore_el0 = {&lore0, &lore_el1};
    JSON_ARRAY lore = {&lore_el0,NULL};
    JSON_STRING item = {"item", "sss", &lore};
    JSON_ELEMENT itemo_el1 = {&lore,NULL};
    JSON_ELEMENT itemo_el0 = {&item,&itemo_el1};
    JSON_OBJECT itemo = {&itemo_el0,NULL};
    JSON_ELEMENT itema = {&itemo,NULL};
    JSON_ARRAY items = {&itema,NULL};
    JSON_STRING moneyid = {"moneyid", "won", &items};
    JSON_STRING name = {"name", "상점", &moneyid};
    JSON_STRING shopid = {"shopid", "shop1", &name};
    dataset.value = &shopid;
    //
    item.linked;//array
    return 0;
}
