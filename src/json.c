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
    //printf("INTTAG:%s|value:%d\n",tag,value);
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
    //printf("FLOATTAG:%s|value:%f\n",tag,value);
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
    //printf("BOOLTAG:%s|value:%d\n",tag,value);
    return data;
};

JSON_COMPONENTS *new_JSON_STRING(char*tag,char* value,JSON_COMPONENTS*linked){
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=4;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=value;
    //printf("STRINGTAG:%s|value:%s\n",tag,value);
    return data;
};

JSON_COMPONENTS *new_JSON_ARRAY(char*tag,JSON_ELEMENT *value,JSON_COMPONENTS*linked){
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=5;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=value;
    //printf("ARRAYTAG:%s|value:[]\n",tag);
    return data;
};

JSON_COMPONENTS *new_JSON_OBJECT(char*tag,JSON_ELEMENT *value,JSON_COMPONENTS*linked){
    JSON_COMPONENTS * data = malloc(sizeof(JSON_COMPONENTS));
    data->tag=tag;
    data->TYPE_VALUE=6;
    data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked=linked;
    data->value=value;
    //printf("OBJECTTAG:%s|value:{}\n",tag);
    return data;

};

JSON_ELEMENT *new_JSON_ELEMENT(JSON_COMPONENTS*value,JSON_ELEMENT*linked){
    JSON_ELEMENT * data = malloc(sizeof(JSON_ELEMENT));
    data->TYPE_VALUE=7;
    data->linked=linked;
    data->value=value;
    //printf("ELEMENT:%s|value:...\n",value->tag);
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
    JSON_COMPONENTS *lore2 = new_JSON_STRING(NULL,"2",NULL);//{"", "1", NULL};
    JSON_COMPONENTS *lore1 = new_JSON_STRING(NULL,"1",lore2);//{"", "1", NULL};
    JSON_COMPONENTS *lore0 = new_JSON_STRING(NULL,"0",lore1);//{"", "0", &lore1};
    JSON_ELEMENT *lore_el2 = new_JSON_ELEMENT(lore2,NULL);
    JSON_ELEMENT *lore_el1 = new_JSON_ELEMENT(lore1,lore_el2);//{&lore1, NULL};
    JSON_ELEMENT *lore_el0 = new_JSON_ELEMENT(lore0,lore_el1);//{&lore0, &lore_el1};
    JSON_COMPONENTS *lore = new_JSON_ARRAY("lore",lore_el0,NULL);//{&lore_el0,NULL};

    JSON_COMPONENTS *item = new_JSON_STRING("item","book",lore);//{"item", "sss", &lore};

    JSON_ELEMENT *itemo_el1 = new_JSON_ELEMENT(lore,NULL);//{&lore,NULL};
    JSON_ELEMENT *itemo_el0 = new_JSON_ELEMENT(item,itemo_el1);//{&item,&itemo_el1};
    JSON_COMPONENTS *itemo = new_JSON_OBJECT("items",itemo_el0,NULL);//{&itemo_el0,NULL};

    JSON_COMPONENTS *moneyid = new_JSON_STRING("moneyid","won",itemo);//{"moneyid", "won", &items};
    JSON_COMPONENTS *name = new_JSON_STRING("name","상점",moneyid);//{"name", "상점", &moneyid};
    JSON_COMPONENTS *v = new_JSON_STRING("shopid","shop1",name);//{"shopid", "shop1", &name};
    
    dataset->value = v;
    //
    //item->linked;//array
    return dataset;
}
