#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include <math.h>
#include <stdarg.h>//함수의 매개변수에 서식문자 포함 string을 넣기 위해 사용


JSON_COMPONENTS* new_JSON_INT(char* tag, int value, JSON_COMPONENTS* linked) {
    int* v = malloc(sizeof(int));
    *v = value;
    JSON_COMPONENTS* data = malloc(sizeof(JSON_COMPONENTS));
    data->tag = tag;
    data->TYPE_VALUE = T_INT;
    //data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked = linked;
    data->value = v;
    return data;
};

JSON_COMPONENTS* new_JSON_FLOAT(char* tag, float value, JSON_COMPONENTS* linked) {
    float* v = malloc(sizeof(float));
    *v = value;
    JSON_COMPONENTS* data = malloc(sizeof(JSON_COMPONENTS));
    data->tag = tag;
    data->TYPE_VALUE = T_FLOAT;
    //data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked = linked;
    data->value = v;
    return data;
};

JSON_COMPONENTS* new_JSON_BOOL(char* tag, bool value, JSON_COMPONENTS* linked) {
    bool* v = malloc(sizeof(bool));
    *v = value;
    JSON_COMPONENTS* data = malloc(sizeof(JSON_COMPONENTS));
    data->tag = tag;
    data->TYPE_VALUE = T_BOOL;
    //data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked = linked;
    data->value = v;
    return data;
};

JSON_COMPONENTS* new_JSON_STRING(char* tag, char* value, JSON_COMPONENTS* linked) {
    JSON_COMPONENTS* data = malloc(sizeof(JSON_COMPONENTS));
    data->tag = tag;
    data->TYPE_VALUE = T_STRING;
    //data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked = linked;
    data->value = value;
    return data;
};

JSON_COMPONENTS* new_JSON_ARRAY(char* tag, JSON_ELEMENT* value, JSON_COMPONENTS* linked) {
    JSON_COMPONENTS* data = malloc(sizeof(JSON_COMPONENTS));
    data->tag = tag;
    data->TYPE_VALUE = T_ARRAY;
    //data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked = linked;
    data->value = value;
    return data;
};

JSON_COMPONENTS* new_JSON_OBJECT(char* tag, JSON_ELEMENT* value, JSON_COMPONENTS* linked) {
    JSON_COMPONENTS* data = malloc(sizeof(JSON_COMPONENTS));
    data->tag = tag;
    data->TYPE_VALUE = T_OBJECT;
    //data->TYPE_LINK=linked?linked->TYPE_VALUE:0;
    data->linked = linked;
    data->value = value;
    return data;

};

JSON_ELEMENT* new_JSON_ELEMENT(JSON_COMPONENTS* value, JSON_ELEMENT* linked) {
    JSON_ELEMENT* data = malloc(sizeof(JSON_ELEMENT));
    //data->TYPE_VALUE=7;
    data->linked = linked;
    data->value = value;
    return data;
};

int JSON_LENGTH(JSON_COMPONENTS* value) {
    if (value->TYPE_VALUE == T_ARRAY || value->TYPE_VALUE == T_OBJECT) {
        int length = 0;
        JSON_ELEMENT* p = value->value;
        while (p != NULL) {
            p = p->linked;
            length += 1;
        }
        return length;
    }
    else return 0;
}


JSON_COMPONENTS* JSON_FIND_KEY(JSON_COMPONENTS* value, char* key) {
    if (value == NULL) return NULL;
    if (key != NULL) {
        JSON_ELEMENT* p = value->value;
        while (p != NULL) {
            if (p->value !=NULL&&!strcmp(((JSON_COMPONENTS*)(p->value))->tag, key)) {
                return p->value;
            }
            p = p->linked;
        }
    }
    else return NULL;
}

/*
obj는 무조건 배열이어야합니다
*/
JSON_COMPONENTS* JSON_FIND_INDEX(JSON_COMPONENTS* value, int index) {
    if(value==NULL) return NULL;
    JSON_ELEMENT* p = value->value;
    for (int i = 0;i < index;i++) {
        if (p == NULL) return NULL;
        p = p->linked;
    }
    return p->value;
}

//array 반환
JSON_COMPONENTS JSON_FILTER_TYPE(JSON_COMPONENTS obj, unsigned char type) {
    JSON_COMPONENTS res = { NULL,T_ARRAY,NULL,NULL };
    if (obj.TYPE_VALUE == T_ARRAY || obj.TYPE_VALUE == T_OBJECT) {//유효한 타입
        JSON_ELEMENT* p = obj.value;
        JSON_ELEMENT* el = NULL;
        while (p != NULL) {//init el
            if (((JSON_COMPONENTS*)(p->value))->TYPE_VALUE == type) {
                el = new_JSON_ELEMENT(p->value, NULL);
                res.value = el;
                p = p->linked;
                break;
            }
            p = p->linked;
        }
        while (p != NULL) {
            if (((JSON_COMPONENTS*)(p->value))->TYPE_VALUE == type) {
                el->linked = new_JSON_ELEMENT(p->value, NULL);
                el = el->linked;
            }
            p = p->linked;
        }
        return res;
    }
    else return res;
}


//find function -> 매개변수 json은 반드시 array 또는 object여야함, 아니라면 무조건 Null반환
JSON_COMPONENTS* JSON_FIND_INT(int value, JSON_COMPONENTS* json, bool search_unlimited) {
    if(json==NULL) return NULL;
    if (json->TYPE_VALUE == T_ARRAY) {//arr
        JSON_ELEMENT* el = json->value;
        while (1) {
            if (el == NULL) return NULL;
            if (((JSON_COMPONENTS*)(el->value))->TYPE_VALUE == T_INT && *(int*)(((JSON_COMPONENTS*)el->value)->value) == value) return (JSON_COMPONENTS*)el->value;
            el = el->linked;
        }
    }
    else if (json->TYPE_VALUE == T_OBJECT) {//object
        JSON_ELEMENT* el = json->value;
        while (1) {
            if (el == NULL) return NULL;
            if (((JSON_COMPONENTS*)(el->value))->TYPE_VALUE == T_INT && *(int*)(((JSON_COMPONENTS*)el->value)->value) == value) return (JSON_COMPONENTS*)el->value;
            el = el->linked;
        }
    }
    else return NULL;
    return NULL;
};

JSON_COMPONENTS* JSON_FIND_FLOAT(float value, JSON_COMPONENTS* json, bool search_unlimited) {
    if(json==NULL) return NULL;
    if (json->TYPE_VALUE == 5) {//arr
        JSON_ELEMENT* el = json->value;
        while (1) {
            while (1) {
                if (el == NULL) return NULL;
                if (((JSON_COMPONENTS*)(el->value))->TYPE_VALUE == T_FLOAT && *(float*)(((JSON_COMPONENTS*)el->value)->value) == (float)value) return (JSON_COMPONENTS*)el->value;
                el = el->linked;
            }
        }
    }
    else if (json->TYPE_VALUE == 6) {//object
        JSON_ELEMENT* el = json->value;
        while (1) {
            while (1) {
                if (el == NULL) return NULL;
                if (((JSON_COMPONENTS*)(el->value))->TYPE_VALUE == T_FLOAT && *(float*)(((JSON_COMPONENTS*)el->value)->value) == (float)value) return (JSON_COMPONENTS*)el->value;
                el = el->linked;
            }
        }
    }
    else return NULL;
    return NULL;
};

JSON_COMPONENTS* JSON_FIND_STRING(char* value, JSON_COMPONENTS* json, bool search_unlimited) {
    if(json==NULL) return NULL;
    if (json->TYPE_VALUE == 5) {//arr
        JSON_ELEMENT* el = json->value;
        while (1) {
            while (1) {
                if (el == NULL) return NULL;
                if (((JSON_COMPONENTS*)(el->value))->TYPE_VALUE == T_STRING && !strcmp((char*)((JSON_COMPONENTS*)el->value)->value, value)) return (JSON_COMPONENTS*)el->value;
                el = el->linked;
            }
        }
    }
    else if (json->TYPE_VALUE == 6) {//object
        JSON_ELEMENT* el = json->value;
        while (1) {
            while (1) {
                if (el == NULL) return NULL;
                if (((JSON_COMPONENTS*)(el->value))->TYPE_VALUE == T_STRING && !strcmp((char*)((JSON_COMPONENTS*)el->value)->value, value)) return (JSON_COMPONENTS*)el->value;
                el = el->linked;
            }
        }
    }
    else return NULL;
    return NULL;
};

//free
void JSON_FREE(JSON_COMPONENTS* value) {
    if (value == NULL) return;
    // 배열 또는 오브젝트 처리
    if (value->TYPE_VALUE == T_ARRAY || value->TYPE_VALUE == T_OBJECT) {
        JSON_COMPONENTS* p = value->value;
        JSON_COMPONENTS* next;
        while (p != NULL) {
            next = p->linked;
            JSON_FREE(p);  // 내부 구조 재귀 제거
            p = next;
        }
    }
    else {
        //tag,value 해제
        if (value->tag) free(value->tag);
        if (value->value) free(value->value);
    }
    //자기자신 해체
    free(value);
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
                "1",
                "2"
            ]
        }
    ]
}
*/
JSON_COMPONENTS* testjson() {
    JSON_COMPONENTS* lore2 = new_JSON_STRING(NULL, "2", NULL);
    JSON_COMPONENTS* lore1 = new_JSON_STRING(NULL, "1", lore2);
    JSON_COMPONENTS* lore0 = new_JSON_STRING(NULL, "0", lore1);
    JSON_ELEMENT* lore_el2 = new_JSON_ELEMENT(lore2, NULL);
    JSON_ELEMENT* lore_el1 = new_JSON_ELEMENT(lore1, lore_el2);
    JSON_ELEMENT* lore_el0 = new_JSON_ELEMENT(lore0, lore_el1);
    JSON_COMPONENTS* lore = new_JSON_ARRAY("lore", lore_el0, NULL);

    JSON_COMPONENTS* item = new_JSON_STRING("item", "book", lore);

    JSON_ELEMENT* itemo_el1 = new_JSON_ELEMENT(lore, NULL);
    JSON_ELEMENT* itemo_el0 = new_JSON_ELEMENT(item, itemo_el1);
    JSON_COMPONENTS* itemo_0 = new_JSON_OBJECT(NULL, itemo_el0, NULL);
    JSON_COMPONENTS* itemo_1 = new_JSON_ARRAY("items", new_JSON_ELEMENT(itemo_0, NULL), NULL);
    JSON_ELEMENT* el1 = new_JSON_ELEMENT(itemo_1, NULL);

    JSON_COMPONENTS* moneyid = new_JSON_STRING("moneyid", "won", itemo_1);
    JSON_ELEMENT* el2 = new_JSON_ELEMENT(moneyid, el1);

    JSON_COMPONENTS* name = new_JSON_STRING("name", "상점", moneyid);
    JSON_ELEMENT* el3 = new_JSON_ELEMENT(name, el2);

    JSON_COMPONENTS* v = new_JSON_STRING("shopid", "shop1", name);
    JSON_ELEMENT* el4 = new_JSON_ELEMENT(v, el3);
    JSON_COMPONENTS* result = new_JSON_OBJECT(NULL, el4, NULL);

    return result;
}
