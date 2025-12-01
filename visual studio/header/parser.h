typedef struct
{
    //1:int 2:float 3:bool 4:string 5:array 6:object | 7:element | 10:array_close 11:object_close
    unsigned char TYPE_VALUE;
    void* value; //기본 타입중 하나 | array나 object일 경우 JSON_ELEMENT
    int thischar;
} PARSER_SINGLEDATA;

int _PARSER_NEXTCHAR(FILE*file);

PARSER_SINGLEDATA _PARSER_STR(FILE*file);

PARSER_SINGLEDATA _PARSER_EXTRACTION(FILE*file);

JSON_COMPONENTS *_PARSER_OBJ(FILE*file,char *tagkey,int lastch);

JSON_COMPONENTS *_PARSER_ARR(FILE*file,char *tagkey,int lastch);

JSON_COMPONENTS *PARSER_PARSE(char*path);

int PARSER_PRINT_VALUE(JSON_COMPONENTS *value);

int _PARSER_PRINT_ARR(JSON_COMPONENTS*array,int level);

int _PARSER_PRINT_OBJ(JSON_COMPONENTS*obj,int level);

int PARSER_PRINT(JSON_COMPONENTS *json);