#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "parser.h"

int main(){
    JSON*data= testjson();
    //printf("testcode");
    JSON*filedata= PARSER_PARSE("resources/1.json");
    printf("\n\n\n");
    PARSER_PRINT(filedata);
    //printf("%d",_PARSER_GETSPACE(data));
    //PARSER_SERIALIZE("resources/test.json");
    return 0;
}