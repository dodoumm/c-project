#include <stdio.h>
#include <stdlib.h>
#include "json.h"
#include "parser.h"

static char path[150];

/*json=>struct로 변환
int PARSER_DESERIALIZE(char*json_string){

}
*/

//json 직렬화
char * PARSER_SERIALIZE(char*json_string){

}

//

PARSER_TOP _PARSER_NEWTOP(int level,void*value,PARSER_TOP*front,PARSER_TOP*linked){
    return (PARSER_TOP){level,value,front,linked};
}

int _PARSER_GETSPACE(JSON_VALUE*value){
    int spaces=0;
    //int level=1;
    PARSER_TOP top = {1,value->value,NULL,NULL};
    JSON_ELEMENT*current = NULL;
    PARSER_TOP cache ={0,0,0,0};
    //int topped = 1;
    while (1){
        //spaces+=(level*4);
        _JSON_CASTING *tp = top.value;
        printf("%d",tp->type);
        _JSON_CASTING *p  =current?current:top.value;
        //printf("\nINT detected\n");
        if(p==NULL){
            //json끝
            return spaces;
        };
        switch (p->type)
        {
        case 1:
            printf("\nINT detected\n");
            cache = _PARSER_NEWTOP(top.level,((JSON_INT*)p)->linked,&top,NULL);
            top.linked = &cache;
            top = cache;
            spaces+=(4*top.level);
            break;
        case 2:
            printf("\nFLOAT detected\n");
            cache = _PARSER_NEWTOP(top.level,((JSON_FLOAT*)p)->linked,&top,NULL);
            top.linked = &cache;
            top = cache;
            spaces+=(4*top.level);
            break;
        case 3:
            printf("\nSTRING detected\n");
            cache = _PARSER_NEWTOP(top.level,((JSON_BOOL*)p)->linked,&top,NULL);
            top.linked = &cache;
            top = cache;
            spaces+=(4*top.level);
            break;
        case 4:
            printf("\nSTRING detected\n");
            cache = _PARSER_NEWTOP(top.level,((JSON_STRING*)p)->linked,&top,NULL);
            top.linked = &cache;
            top = cache;
            spaces+=(4*top.level);
            break;
        case 5:
            spaces+=(4*top.level);
            //원소가 가리키는 다음 원소
            void* v = ((JSON_ELEMENT*)p)->linked;
            if(v==NULL){
                printf("\nELEMENT_FINAL detected\n");
                //마지막 원소
                //이때, top는 배열이나 객체
                cache = _PARSER_NEWTOP(top.level-1,top.linked,&top,NULL);
                top.linked = &cache;
                top = cache;
                current = NULL;
            }else{
                printf("\nELEMENT detected\n");
                current = v;
                _JSON_CASTING* next = current->value;
                if(next->type>5){
                    //링크된 값이 배열이나 객체
                    cache = _PARSER_NEWTOP(top.level+1,v,&top,NULL);
                    top.linked = &cache;
                    top = cache;
                    current = next->value;
                }
            }
            spaces+=(4*top.level);
            break;
        default:
            printf("\nNULL detected\n");
            return spaces;
            break;
        }
    }
    
}

//0:정상적으로 파일 불러옴 | 1:오류
int PARSER_OPEN(char*filepath){
    if(path==NULL) return 0;
    FILE* file = fopen(path, "r");
    int capped = 0;
    while(!feof(file)){

    }
    return 0;
}

int PARSER_SAVE(){

}

int PARSER_CLOSE(){
    return 0;
}