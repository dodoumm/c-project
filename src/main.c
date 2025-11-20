#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "parser.h"

int main(){
    JSON*data= testjson();
    //printf("testcode");
    _PARSER_PARSER("resources/1.json");
    //printf("%d",_PARSER_GETSPACE(data));
    //PARSER_SERIALIZE("resources/test.json");
    return 0;
}