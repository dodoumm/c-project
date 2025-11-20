//#ifndef JSON.H
//#define JSON.H

typedef struct{
    void*value;
} JSON_VALUE;

//type5
typedef struct JSEL{
    void*value;
    struct JSEL*linked;
    unsigned char type;
} JSON_ELEMENT;

JSON_ELEMENT *new_JSON_ELEMENT(void*value,JSON_ELEMENT*linked);

//type7
typedef struct{
    JSON_ELEMENT* value;
    void*linked;
    unsigned char type;
} JSON_OBJECT;


JSON_OBJECT *new_JSON_OBJECT(JSON_ELEMENT*value,void*linked);

//type8
typedef struct{
    char* tag;
    JSON_ELEMENT* value;
    void*linked;
    unsigned char type;
} JSON_OBJECT_TAGGED;


JSON_OBJECT_TAGGED *new_JSON_OBJECT_TAGGED(char*tag,JSON_ELEMENT*value,void*linked);

//type6
typedef struct{
    JSON_ELEMENT* value;
    void*linked;
    unsigned char type;
} JSON_ARRAY;

JSON_ARRAY *new_JSON_ARRAY(JSON_ELEMENT*value,void*linked);

//type9
typedef struct{
    char* tag;
    JSON_ELEMENT* value;
    void*linked;
    unsigned char type;
} JSON_ARRAY_TAGGED;

JSON_ARRAY_TAGGED *new_JSON_ARRAY_TAGGED(char*tag,JSON_ELEMENT*value,void*linked);

//type1
typedef struct{
    char* tag;
    void*linked;
    unsigned char type;
    int value;
} JSON_INT;

JSON_INT *new_JSON_INT(char*tag,int value,void*linked);


//type2
typedef struct{
    char* tag;
    void*linked;
    unsigned char type;
    float value;
} JSON_FLOAT;

JSON_FLOAT *new_JSON_FLOAT(char*tag,float value,void*linked);

//type3
typedef struct{
    char* tag;
    void*linked;
    unsigned char type;
    int value;
} JSON_BOOL;

JSON_BOOL *new_JSON_BOOL(char*tag,int value,void*linked);

//type4
typedef struct{
    char* tag;
    void*linked;
    unsigned char type;
    char* value;
} JSON_STRING;

JSON_STRING *new_JSON_STRING(char*tag,char* value,void*linked);

//casting용 구조체
typedef struct{
    void*value;
    struct JSON_ELEMENT*linked;
    unsigned char type;

} _JSON_CASTING_EL;

//일반 원소 구조체(int,float,bool,string)
typedef struct{
    char*tag;
    void*linked;
    unsigned char type;
} _JSON_CASTING_NORMAL;


JSON_VALUE* testjson();

//#endif