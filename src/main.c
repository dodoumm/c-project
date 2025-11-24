#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "parser.h"

int main(){
    JSON*data= testjson();
    JSON*filedata= PARSER_PARSE("resources/1.json");
    if(filedata==NULL){
        printf("ERROR:json파일을 읽는데 문제가 발생하였습니다.\n");
    }
    PARSER_PRINT(filedata);
    PARSER_PRINT(data);
    return 0;
}