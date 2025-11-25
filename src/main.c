#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//#include <ctype.h>
#include "json.h"
#include "parser.h"

int main(){
    JSON*data= testjson();
    JSON*filedata= PARSER_PARSE("resources/2.json");
    if(filedata==NULL){
        printf("ERROR:json파일을 읽는데 문제가 발생하였습니다.\n");
    }
    PARSER_PRINT(filedata);
    //PARSER_PRINT(data);
    printf("ㅡㅡㅡFINDㅡㅡㅡ\n");
    float t = 21474.5539868;
    printf("float : %.12g\n",t);
    printf("floatS : %.10g\n",atof("2147483647.0"));
    JSON_COMPONENTS *findt = JSON_FIND_FLOAT(29.45,filedata->value,false);
    if(findt==NULL){
        printf("CANT FIND\n");
    }else printf("FIND : %s : %.6g\n",findt->tag,*(float*)findt->value);
    return 0;
}