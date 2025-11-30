#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
#include "json.h"
#include "parser.h"


//가상의 게임에서 어느 플레이어를 밴하는 예제
void testfunc(int ban_playerid) {
    //파일에서 플레이어 데이터를 추출합니다.
    JSON_COMPONENTS* filedata = PARSER_PARSE("resources/players.json");
    PARSER_PRINT(filedata);
    //ip검사:
    JSON_COMPONENTS* p = filedata;
    while (p != NULL) {
        printf("while\n");
        p = p->linked;
        JSON_COMPONENTS* id = JSON_FIND_KEY(p, "id");
        if (id == NULL) continue;
        if (*(int*)id->value == ban_playerid) {//플레이어 찾음
            printf("NAME : %s\n", (char*)JSON_FIND_KEY(p, "name")->value);
        }
    }

}

void ttt() {
    JSON_COMPONENTS* filedata = PARSER_PARSE("resources/players.json");
    PARSER_PRINT(filedata);
    if (filedata->TYPE_VALUE == T_ARRAY) {
        JSON_ELEMENT* p = filedata->value;
        while (p != NULL) {
            char* name = (char*)(JSON_FIND_KEY(p->value, "name")->value);
            //printf("NAME:%s\n", name);
            if (!strcmp(name, "stone123")) {
                //
                PARSER_PRINT_VALUE(p->value);
                break;
            }
            p = p->linked;
        }
    }
}

int main() {
    ttt();
    //testfunc(10001);
    /*
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
    */
    return 0;
}