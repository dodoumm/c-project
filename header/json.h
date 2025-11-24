//#ifndef JSON.H
//#define JSON.H
#include <stdbool.h>


//TYPE_ID = 1:int 2:float 3:bool 4:string 5:array 6:object
typedef struct JSC{
    char* tag; //json의 키값(없을수도 있음)
    unsigned char TYPE_VALUE;//value의 타입
    void* value; //기본 타입중 하나 | array나 object일 경우 JSON_ELEMENT
    struct JSC *linked; //JSON_COMPONENTS
    unsigned char TYPE_LINK; //0:NULL
} JSON_COMPONENTS;

typedef struct{
    JSON_COMPONENTS*value;
    unsigned char type;
} JSON;

//type5~6 | 5:array 6:object
typedef struct JSEL{
    unsigned char TYPE_VALUE;
    JSON_COMPONENTS* value;
    struct JSEL*linked;
} JSON_ELEMENT;

JSON_COMPONENTS *new_JSON_INT(char*tag,int value,JSON_COMPONENTS*linked);

JSON_COMPONENTS *new_JSON_FLOAT(char*tag,float value,JSON_COMPONENTS*linked);

JSON_COMPONENTS *new_JSON_BOOL(char*tag,bool value,JSON_COMPONENTS*linked);

JSON_COMPONENTS *new_JSON_STRING(char*tag,char* value,JSON_COMPONENTS*linked);

JSON_COMPONENTS *new_JSON_ARRAY(char*tag,JSON_ELEMENT *value,JSON_COMPONENTS*linked);

JSON_COMPONENTS *new_JSON_OBJECT(char*tag,JSON_ELEMENT *value,JSON_COMPONENTS*linked);

JSON_ELEMENT *new_JSON_ELEMENT(JSON_COMPONENTS*value,JSON_ELEMENT*linked);


JSON* testjson();

//#endif