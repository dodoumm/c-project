//#ifndef JSON.H
//#define JSON.H
#include <stdbool.h>

typedef enum {
    T_NONE,//오류형 타입
    T_INT,
    T_FLOAT,
    T_BOOL,
    T_STRING,
    T_ARRAY,
    T_OBJECT,
    T_NULL
} VTYPE;//valuetype

//TYPE_ID = 1:int 2:float 3:bool 4:string 5:array 6:object 7:null
typedef struct JSC {
    char* tag; //json의 키값(없을수도 있음)
    VTYPE TYPE_VALUE;//value의 타입
    void* value; //기본 타입중 하나 | array나 object일 경우 JSON_ELEMENT
    struct JSC* linked; //JSON_COMPONENTS
} JSON_COMPONENTS;


//type5~6 | 5:array 6:object
typedef struct JSEL {
    //unsigned char TYPE_VALUE;
    JSON_COMPONENTS* value;
    struct JSEL* linked;
} JSON_ELEMENT;

JSON_COMPONENTS* new_JSON_NULL(char* tag, JSON_COMPONENTS* linked);

JSON_COMPONENTS* new_JSON_INT(char* tag, int value, JSON_COMPONENTS* linked);

JSON_COMPONENTS* new_JSON_FLOAT(char* tag, float value, JSON_COMPONENTS* linked);

JSON_COMPONENTS* new_JSON_BOOL(char* tag, bool value, JSON_COMPONENTS* linked);

JSON_COMPONENTS* new_JSON_STRING(char* tag, char* value, JSON_COMPONENTS* linked);

JSON_COMPONENTS* new_JSON_ARRAY(char* tag, JSON_ELEMENT* value, JSON_COMPONENTS* linked);

JSON_COMPONENTS* new_JSON_OBJECT(char* tag, JSON_ELEMENT* value, JSON_COMPONENTS* linked);


JSON_ELEMENT* new_JSON_ELEMENT(JSON_COMPONENTS* value, JSON_ELEMENT* linked);


//func



int JSON_LENGTH(JSON_COMPONENTS* value);

JSON_COMPONENTS* JSON_FIND_KEY(JSON_COMPONENTS* value, char* key);
JSON_COMPONENTS* JSON_FIND_INDEX(JSON_COMPONENTS* value, int index);

JSON_COMPONENTS JSON_FILTER_TYPE(JSON_COMPONENTS obj, unsigned char type);

//findfunction
JSON_COMPONENTS* JSON_FIND_INT(int value, JSON_COMPONENTS* json, bool search_unlimited);

JSON_COMPONENTS* JSON_FIND_FLOAT(float value, JSON_COMPONENTS* json, bool search_unlimited);

JSON_COMPONENTS* JSON_FIND_BOOL(bool value, JSON_COMPONENTS* json, bool search_unlimited);

JSON_COMPONENTS* JSON_FIND_STRING(char* value, JSON_COMPONENTS* json, bool search_unlimited);

//free
void JSON_FREE(JSON_COMPONENTS* value);

JSON_COMPONENTS* testjson();

//#endif