#include <stdio.h>
#include <stdlib.h>
#include "json.h"

JSON_ELEMENT *new_JSON_ELEMENT(void*value,JSON_ELEMENT*linked){
    return &(JSON_ELEMENT){value,linked,5};
}
JSON_OBJECT *new_JSON_OBJECT(JSON_ELEMENT*value,void*linked){
    return &(JSON_OBJECT){value,linked,7};
}
JSON_OBJECT_TAGGED *new_JSON_OBJECT_TAGGED(char*tag,JSON_ELEMENT*value,void*linked){
    return &(JSON_OBJECT_TAGGED){tag,value,linked,8};
}
JSON_ARRAY *new_JSON_ARRAY(JSON_ELEMENT*value,void*linked){
    return &(JSON_ARRAY){value,linked,6};
}
JSON_ARRAY_TAGGED *new_JSON_ARRAY_TAGGED(char*tag,JSON_ELEMENT*value,void*linked){
    return &(JSON_ARRAY_TAGGED){tag,value,linked,9};
}
JSON_INT *new_JSON_INT(char*tag,int value,void*linked){
    return &(JSON_INT){tag,value,linked,1};
}
JSON_FLOAT *new_JSON_FLOAT(char*tag,float value,void*linked){
    return &(JSON_FLOAT){tag,value,linked,2};
}
JSON_BOOL *new_JSON_BOOL(char*tag,int value,void*linked){
    value= value?1:0;
    return &(JSON_BOOL){tag,value,linked,3};
}
JSON_STRING *new_JSON_STRING(char*tag,char* value,void*linked){
    return &(JSON_STRING){tag,value,linked,4};
}

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
JSON_VALUE* testjson(){
    JSON_VALUE *dataset = malloc(sizeof(JSON_VALUE));
    JSON_STRING *lore1 = new_JSON_STRING("","1",NULL);//{"", "1", NULL};
    JSON_STRING *lore0 = new_JSON_STRING("","0",lore1);//{"", "0", &lore1};
    JSON_ELEMENT *lore_el1 = new_JSON_ELEMENT(lore1,NULL);//{&lore1, NULL};
    JSON_ELEMENT *lore_el0 = new_JSON_ELEMENT(lore1,lore_el1);//{&lore0, &lore_el1};
    JSON_ARRAY *lore = new_JSON_ARRAY(lore_el0,NULL);//{&lore_el0,NULL};
    JSON_STRING *item = new_JSON_STRING("item","sss",lore);//{"item", "sss", &lore};
    JSON_ELEMENT *itemo_el1 = new_JSON_ELEMENT(lore,NULL);//{&lore,NULL};
    JSON_ELEMENT *itemo_el0 = new_JSON_ELEMENT(item,itemo_el1);//{&item,&itemo_el1};
    JSON_OBJECT *itemo = new_JSON_OBJECT(itemo_el0,NULL);//{&itemo_el0,NULL};
    JSON_ELEMENT *itema = new_JSON_ELEMENT(itemo,NULL);//{&itemo,NULL};
    JSON_ARRAY *items = new_JSON_ARRAY(itema,NULL);//{&itema,NULL};
    JSON_STRING *moneyid = new_JSON_STRING("moneyid","won",items);//{"moneyid", "won", &items};
    JSON_STRING *name = new_JSON_STRING("name","상점",moneyid);//{"name", "상점", &moneyid};
    JSON_STRING *shopid = new_JSON_STRING("shopid","shop1",name);//{"shopid", "shop1", &name};
    dataset->value = &shopid;
    //
    //item->linked;//array
    return dataset;
}
