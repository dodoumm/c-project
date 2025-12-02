#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "json.h"
#include "parser.h"

void printarray(){
    JSON_COMPONENTS* info = PARSER_PARSE("resources/1.json");
    if(info==NULL) return;
    PARSER_PRINT(info);
}

void objectf(){
    JSON_COMPONENTS* info = PARSER_PARSE("resources/2.json");
    if(info==NULL) return;
    PARSER_PRINT(info);
    printf("ㅡㅡㅡㅡㅡFILTER INTㅡㅡㅡㅡㅡ\n");
    JSON_COMPONENTS filter = JSON_FILTER_TYPE(*info,T_INT);
    PARSER_PRINT(&filter);
    printf("ㅡㅡㅡㅡㅡFILTER STRINGㅡㅡㅡㅡㅡ\n");
    filter = JSON_FILTER_TYPE(*info,T_STRING);
    PARSER_PRINT(&filter);
}


//가상의 게임에서 어느 플레이어를 밴하는 예제
void playerban(int ban_playerid) {
    //지금까지 저장된 baninfo를 확인합니다.
    JSON_COMPONENTS* baninfo = PARSER_PARSE("resources/baninfo.json");
    //정보
    JSON_COMPONENTS* idinfo;
    JSON_COMPONENTS* ipinfo;
    JSON_COMPONENTS* didinfo;
    if (baninfo == NULL) {//파일을 읽을 수 없다면 데이터를 초기화합니다.
        idinfo = new_JSON_ARRAY("playerid", NULL, NULL);
        ipinfo = new_JSON_ARRAY("ip", NULL, NULL);
        didinfo = new_JSON_ARRAY("deviceid", NULL, NULL);
        //element
        JSON_ELEMENT* did = new_JSON_ELEMENT(didinfo, NULL);
        JSON_ELEMENT* ip = new_JSON_ELEMENT(ipinfo, did);
        JSON_ELEMENT* id = new_JSON_ELEMENT(idinfo, ip);
        baninfo = new_JSON_OBJECT(NULL, id, NULL);
        PARSER_PRINT(baninfo);
    }else{//이전 정보 불러오기
        idinfo = JSON_FIND_KEY(baninfo, "playerid");
        ipinfo = JSON_FIND_KEY(baninfo, "ip");
        didinfo = JSON_FIND_KEY(baninfo, "deviceid");
        if(idinfo==NULL||ipinfo==NULL||didinfo==NULL) return;
    }
    //파일에서 플레이어 데이터를 추출합니다.
    JSON_COMPONENTS* filedata = PARSER_PARSE("resources/players.json");
    JSON_ELEMENT* p = filedata->value;
    while (p != NULL) {
        JSON_COMPONENTS* id = JSON_FIND_KEY(p->value, "id");
        if (id == NULL) {
            p = p->linked;
            continue;
        }
        if (*(int*)id->value == ban_playerid) {//플레이어 찾음
            JSON_COMPONENTS* info = JSON_FIND_KEY(p->value, "info");
            if (info == NULL) return;
            JSON_COMPONENTS* getip = JSON_FIND_KEY(info, "ip");
            JSON_COMPONENTS* getdid = JSON_FIND_KEY(info, "deviceid");
            if (!JSON_FIND_INT(ban_playerid, idinfo, false)) {//새로운 아이디라면
                JSON_ELEMENT* temp = idinfo->value;//첫번째 id
                idinfo->value = new_JSON_ELEMENT(new_JSON_INT(NULL, ban_playerid, NULL), temp);
            }
            JSON_ELEMENT* pp; //반복문용 원소 변수
            JSON_COMPONENTS* now;//반복문용 값 변수
            pp = getip->value;
            while (pp != NULL) {
                now = pp->value;
                if (now->TYPE_VALUE == T_STRING && !JSON_FIND_STRING(now->value, ipinfo, false)) {//새로운 아이피라면
                    JSON_ELEMENT* temp = ipinfo->value;//첫번째 id
                    ipinfo->value = new_JSON_ELEMENT(new_JSON_STRING(NULL, now->value, NULL), temp);
                }
                pp = pp->linked;
            }
            pp = getdid->value;
            while (pp != NULL) {
                now = pp->value;
                if (now->TYPE_VALUE == T_STRING && !JSON_FIND_STRING(now->value, didinfo, false)) {//새로운 디바이스아이디라면
                    JSON_ELEMENT* temp = didinfo->value;//첫번째 id
                    didinfo->value = new_JSON_ELEMENT(new_JSON_STRING(NULL, now->value, NULL), temp);
                }
                pp = pp->linked;
            }
            PARSER_SAVE("resources/baninfo.json", baninfo);
            return;
        }
        p = p->linked;
    }
    printf("플레이어를 찾을 수 없습니다.");
    return;
}


int main() {
    printf("########## 예제1 ##########\n");
    printarray();
    printf("########## 예제2 ##########\n");
    objectf();
    printf("########## 예제3 ##########\n");
    playerban(100001);
    playerban(100004);
    return 0;
}