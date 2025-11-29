#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include "json.h"
#include "parser.h"

int main(){
    JSON_COMPONENTS*data= testjson();
    JSON_COMPONENTS*filedata= PARSER_PARSE("resources/1.json");
    printf("PARSED\n");
    if(filedata==NULL){
        printf("ERROR:json파일을 읽는데 문제가 발생하였습니다.\n");
    }
    PARSER_PRINT(filedata);
    //PARSER_PRINT(data);
    
    printf("ㅡㅡㅡFINDㅡㅡㅡ\n");
    //printf("JSON_LENGTH : %d",JSON_LENGTH(filedata->value));
    JSON_COMPONENTS *findt = JSON_FIND_INDEX(filedata->value,3);//JSON_FIND_FLOAT(67.0,filedata->value,false);
    if(findt==NULL){
        printf("CANT FIND\n");
    }else PARSER_PRINT_VALUE(findt);
    JSON_COMPONENTS sort = JSON_FILTER_TYPE(*filedata,T_FLOAT);
    if(sort.value==NULL){
        printf("CANT FILTER\n");
    }else{
        PARSER_PRINT(&sort);
    }
    printf("ㅡㅡㅡFREEㅡㅡㅡ\n");
    JSON_FREE(data);
    JSON_FREE(filedata);
    JSON_FREE(&sort);
    PARSER_PRINT(filedata);
    return 0;
}