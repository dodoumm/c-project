#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "json.h"


JSON_COMPONENTS *new_JSON_INT(char*tag,int value,JSON_COMPONENTS*linked){
    int*v = malloc(sizeof(int));
    *v=value;
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=1;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=v;
    return data;
};

JSON_COMPONENTS *new_JSON_FLOAT(char*tag,float value,JSON_COMPONENTS*linked){
    float*v = malloc(sizeof(float));
    *v=value;
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=2;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=v;
    return data;
};

JSON_COMPONENTS *new_JSON_BOOL(char*tag,bool value,JSON_COMPONENTS*linked){   
    bool*v = malloc(sizeof(bool));
    *v=value;
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=3;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=v;
    return data;
};

JSON_COMPONENTS *new_JSON_STRING(char*tag,char* value,JSON_COMPONENTS*linked){
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=4;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=value;
    return data;
};

JSON_COMPONENTS *new_JSON_ARRAY(char*tag,JSON_ELEMENT *value,JSON_COMPONENTS*linked){
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=5;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=value;
    return data;
};

JSON_COMPONENTS *new_JSON_OBJECT(char*tag,JSON_ELEMENT *value,JSON_COMPONENTS*linked){
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=6;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=value;
    return data;

};

JSON_ELEMENT *new_JSON_ELEMENT(JSON_COMPONENTS*value,JSON_ELEMENT*linked){
    JSON_ELEMENT * data = malloc(sizeof(JSON_ELEMENT));
    data->TYPE_VALUE=7;
    data->linked=linked;
    data->value=value;
    return data;
};


JSON* testjson();

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
                "1",
                "2"
            ]
        }
    ]
}
*/
JSON* testjson(){
    JSON *dataset = malloc(sizeof(JSON));
    JSON_COMPONENTS *lore2 = new_JSON_STRING(NULL,"2",NULL);
    JSON_COMPONENTS *lore1 = new_JSON_STRING(NULL,"1",lore2);
    JSON_COMPONENTS *lore0 = new_JSON_STRING(NULL,"0",lore1);
    JSON_ELEMENT *lore_el2 = new_JSON_ELEMENT(lore2,NULL);
    JSON_ELEMENT *lore_el1 = new_JSON_ELEMENT(lore1,lore_el2);
    JSON_ELEMENT *lore_el0 = new_JSON_ELEMENT(lore0,lore_el1);
    JSON_COMPONENTS *lore = new_JSON_ARRAY("lore",lore_el0,NULL);

    JSON_COMPONENTS *item = new_JSON_STRING("item","book",lore);

    JSON_ELEMENT *itemo_el1 = new_JSON_ELEMENT(lore,NULL);
    JSON_ELEMENT *itemo_el0 = new_JSON_ELEMENT(item,itemo_el1);
    JSON_COMPONENTS *itemo_0 = new_JSON_OBJECT(NULL,itemo_el0,NULL);
    JSON_COMPONENTS *itemo_1 = new_JSON_ARRAY("items",new_JSON_ELEMENT(itemo_0,NULL),NULL);
    JSON_ELEMENT*el1 = new_JSON_ELEMENT(itemo_1,NULL);

    JSON_COMPONENTS *moneyid = new_JSON_STRING("moneyid","won",itemo_1);
    JSON_ELEMENT*el2 = new_JSON_ELEMENT(moneyid,el1);

    JSON_COMPONENTS *name = new_JSON_STRING("name","상점",moneyid);
    JSON_ELEMENT*el3 = new_JSON_ELEMENT(name,el2);

    JSON_COMPONENTS *v = new_JSON_STRING("shopid","shop1",name);
    JSON_ELEMENT*el4 = new_JSON_ELEMENT(v,el3);
    JSON_COMPONENTS *result = new_JSON_OBJECT(NULL,el4,NULL);
    dataset->value = result;
    dataset->type=result->TYPE_VALUE;
    
    return dataset;
}
