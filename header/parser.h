//#include "json.h"

//JSON *PARSER_SERIALIZE(char* path);
typedef struct
{
    //1:int 2:float 3:bool 4:string 5:array 6:object | 7:element | 10:array_close 11:object_close
    unsigned char TYPE_VALUE;
    void* value; //기본 타입중 하나 | array나 object일 경우 JSON_ELEMENT
    int thischar;
} PARSER_SINGLEDATA;

char *_PARSER_STR(FILE*file);

PARSER_SINGLEDATA _PARSER_EXTRACTION(FILE*file);

JSON_COMPONENTS *_PARSER_OBJ(FILE*file,JSON_COMPONENTS*before,char *tagkey);

JSON_COMPONENTS *_PARSER_ARR(FILE*file,JSON_COMPONENTS*before,char *tagkey);

JSON *_PARSER_PARSER(char*path);