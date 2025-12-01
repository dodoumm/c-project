#pragma once
typedef struct
{
    //1:int 2:float 3:bool 4:string 5:array 6:object | 7:element | 10:array_close 11:object_close
    unsigned char TYPE_VALUE;
    void* value; //기본 타입중 하나 | array나 object일 경우 JSON_ELEMENT
    int thischar;
} PARSER_SINGLEDATA;

int _PARSER_NEXTCHAR(FILE* file);

PARSER_SINGLEDATA _PARSER_STR(FILE* file);

PARSER_SINGLEDATA _PARSER_EXTRACTION(FILE* file);

JSON_COMPONENTS* _PARSER_OBJ(FILE* file, char* tagkey);

JSON_COMPONENTS* _PARSER_ARR(FILE* file, char* tagkey);

JSON_COMPONENTS* PARSER_PARSE(char* path);

void PARSER_PRINT_VALUE(JSON_COMPONENTS* value);

void _PARSER_PRINT_ARR(JSON_COMPONENTS* array, int level);

void _PARSER_PRINT_OBJ(JSON_COMPONENTS* obj, int level);

void PARSER_PRINT(JSON_COMPONENTS* json);


void _PARSER_SAVE_ARR(FILE *file,JSON_COMPONENTS* array, int level,bool end);

void _PARSER_SAVE_OBJ(FILE *file,JSON_COMPONENTS* obj, int level,bool end);

void PARSER_SAVE(char*path, JSON_COMPONENTS* json);