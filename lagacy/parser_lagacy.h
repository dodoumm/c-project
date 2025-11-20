//#include "json.h"

typedef struct P_T{
    int level;
    void* value;
    struct P_T* front;
    struct P_T* linked;
} PARSER_TOP;


char * PARSER_SERIALIZE(char*json_string);

PARSER_TOP _PARSER_NEWTOP(int level,void*value,PARSER_TOP*front,PARSER_TOP*linked);

int _PARSER_GETSPACE(JSON_VALUE*value);

int PARSER_OPEN(char*filepath);

int PARSER_SAVE();

int PARSER_CLOSE();