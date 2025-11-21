#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "json.h"
#include "parser.h"


//앞에 "가 나왔을때
char *_PARSER_STR(FILE*file){
    int ch;
    int STR_INDEX = 0;
    char*STR=malloc(110);
    int slashes=0;
    while((ch=fgetc(file))!=EOF){
        if(slashes%2==0&&ch=='\"'){
            //문자열의 끝
            STR[STR_INDEX]='\0';
            return STR;
        }else{
            STR[STR_INDEX++] = (unsigned char)ch;
        }
        if(ch=='\\'){
            slashes+=1;
        }else slashes=0;
    }
    return NULL;
}

//앞에 :가 나왔을때 / BOOL,FLOAT,INT
PARSER_SINGLEDATA _PARSER_EXTRACTION(FILE*file){
    int ch;
    int INT_INDEX = 0;
    char*INT=malloc(110);
    bool isfloat=false;
    while((ch=fgetc(file))!=EOF){
        if(ch==' '||ch=='\n') continue;
        if(ch=='\"'){
            //문자열
            char* getstr = _PARSER_STR(file);
            fgetc(file);
            return (PARSER_SINGLEDATA){4,getstr,ch};
        }else if(ch=='-'){//음수부호
            if(INT_INDEX==0){
                INT[INT_INDEX++]=ch;
            }else return (PARSER_SINGLEDATA){0,0,ch};//마이너스 부호가 첫번째 위치가 아닌 곳에 존재
        }else if(ch>47&&ch<58){//정수
            INT[INT_INDEX++]=ch;
        }else if(ch=='.'){//소수점
            if(isfloat) return (PARSER_SINGLEDATA){0,0,ch};//소수점 2개이므로 오류
            isfloat=true;
            INT[INT_INDEX++]=ch;
        }else if(ch=='['){//array 열기
            return (PARSER_SINGLEDATA){5,0,ch};//객체,배열은 외부에서 처리
        }else if(ch=='{'){//object 열기
            return (PARSER_SINGLEDATA){6,0,ch};//객체,배열은 외부에서 처리
        }else{
            //bool 판별
            if(INT_INDEX==0){
                //느슨한 판별(앞 한글자만 일치하면 됌)
                if(ch=='t'||ch=='T'){
                    bool *t = malloc(sizeof(bool));
                    *t=true;
                    while((ch=fgetc(file))){//남은 글자 제거
                        if(ch==','||ch==']'||ch=='}') break;
                    }
                    return (PARSER_SINGLEDATA){3,t,ch};
                }
                if(ch=='f'||ch=='F'){
                    bool *t = malloc(sizeof(bool));
                    *t=false;
                    while((ch=fgetc(file))){//남은 글자 제거
                        if(ch==','||ch==']'||ch=='}') break;
                    }
                    return (PARSER_SINGLEDATA){3,t,ch};
                }
            }
            break;
        }
    }
    if(INT_INDEX==0) return (PARSER_SINGLEDATA){0,0,ch};
    INT[INT_INDEX]='\0';
    if(!isfloat){//정수(int) 반환
        int *v = malloc(sizeof(int));
        *v=atoi(INT);
        return (PARSER_SINGLEDATA){1,v,ch};
    }else{//실수(float) 반환
        float *v = malloc(sizeof(float));
        *v=atof(INT);
        return (PARSER_SINGLEDATA){2,v,ch};
    }
}

//앞에 { 가 나왔을때
JSON_COMPONENTS *_PARSER_OBJ(FILE*file,JSON_COMPONENTS*before,char *tagkey){
    int ch;
    JSON_ELEMENT *el = NULL;
    //반환값:최상위 객체
    JSON_COMPONENTS *obj = malloc(sizeof(JSON_COMPONENTS));
    obj->tag=tagkey;
    obj->TYPE_VALUE=6;
    obj->value=el;
    obj->TYPE_LINK=0;
    obj->linked=NULL;
    char*tag = NULL;
    while((ch=fgetc(file))!=EOF){
        printf("%c",ch);
        if(ch=='\"'){
            //str
            if(tag==NULL){
                //이 문자열은 tag
                tag = _PARSER_STR(file);
                if(tag==NULL) return NULL;
            }else{
                //이 문자열은 value-> JSON_STRING
                JSON_COMPONENTS*res = malloc(sizeof(JSON_COMPONENTS));
                res->value=_PARSER_STR(file);
                if(res->value==NULL) return NULL;
                res->TYPE_VALUE=4;
                res->tag = tag;
                res->TYPE_LINK=0;
                res->linked=NULL;
                //
                if(el==NULL){
                    el = malloc(sizeof(JSON_ELEMENT));
                    el->value = res;
                    el->TYPE_VALUE = 4;
                    el->linked=NULL;
                    (*obj).value=el;
                }else{
                    JSON_ELEMENT *temp=el;
                    el = malloc(sizeof(JSON_ELEMENT));
                    el->value = res;
                    el->TYPE_VALUE = 4;
                    el->linked=temp;
                }
                tag=NULL;
            }
        }
        if(ch==':'){
            if(tag==NULL) return NULL;
            PARSER_SINGLEDATA v = _PARSER_EXTRACTION(file);
            JSON_COMPONENTS *value;
            switch (v.TYPE_VALUE)
            {
            case 1:
                value = new_JSON_INT(tag,*(int*)v.value,NULL);
                if(el==NULL){
                    el = malloc(sizeof(JSON_ELEMENT));
                    el->value = value;
                    el->TYPE_VALUE = 1;
                    el->linked=NULL;
                    (*obj).value=el;
                }else{
                    JSON_ELEMENT *temp=el;
                    el = malloc(sizeof(JSON_ELEMENT));
                    el->value = value;
                    el->TYPE_VALUE = 1;
                    el->linked=temp;
                }
                break;
            case 2:
                value = new_JSON_FLOAT(tag,*(float*)v.value,NULL);
                if(el==NULL){
                    el = malloc(sizeof(JSON_ELEMENT));
                    el->value = value;
                    el->TYPE_VALUE = 2;
                    el->linked=NULL;
                    (*obj).value=el;
                }else{
                    JSON_ELEMENT *temp=el;
                    el = malloc(sizeof(JSON_ELEMENT));
                    el->value = value;
                    el->TYPE_VALUE = 2;
                    el->linked=temp;
                }
                break;
            case 3:
                value = new_JSON_BOOL(tag,*(bool*)v.value,NULL);
                if(el==NULL){
                    el = malloc(sizeof(JSON_ELEMENT));
                    el->value = value;
                    el->TYPE_VALUE = 3;
                    el->linked=NULL;
                    (*obj).value=el;
                }else{
                    JSON_ELEMENT *temp=el;
                    el = malloc(sizeof(JSON_ELEMENT));
                    el->value = value;
                    el->TYPE_VALUE = 3;
                    el->linked=temp;
                }
                break;
            default:
                return NULL;//유효하지 않은 typeid
            }
        }
        if(ch=='['){
            _PARSER_ARR(file,obj,tag);
            //arr
        }
        if(ch=='{'){
            //obj
            _PARSER_OBJ(file,obj,tag);
            //JSON_COMPONENTS*newobj = 
        }
        if(ch=='}'){
            (*obj).linked=before;
            (*obj).TYPE_LINK=before->TYPE_VALUE;
            break;
        }
    }
    return obj;
}

//앞에 [ 가 나왔을때
JSON_COMPONENTS *_PARSER_ARR(FILE*file,JSON_COMPONENTS*before,char *tagkey){
    int ch;
    //반환값:최상위 객체의 원소값
    JSON_ELEMENT *el = NULL;
    //반환값:최상위 객체
    JSON_COMPONENTS *obj = malloc(sizeof(JSON_COMPONENTS));
    obj->tag=tagkey;
    obj->TYPE_VALUE=5;
    obj->value=el;
    obj->TYPE_LINK=0;
    obj->linked=NULL;
        //첫번째 원소
    PARSER_SINGLEDATA v = _PARSER_EXTRACTION(file);
    JSON_COMPONENTS*res;
    switch (v.TYPE_VALUE){
        case 1:
            res = new_JSON_INT(NULL,*(int*)v.value,NULL);
            el = malloc(sizeof(JSON_ELEMENT));
            el->value = res;
            el->TYPE_VALUE = 1;
            el->linked=NULL;
            (*obj).value=el;
            printf("  { ARR_INT_F:%d }\n",*(int*)res->value);
            break;
        case 2:
        //printf("\n\ntest2\n\n");
            res = new_JSON_FLOAT(NULL,*(float*)v.value,NULL);
            el = malloc(sizeof(JSON_ELEMENT));
            el->value = res;
            el->TYPE_VALUE = 2;
            el->linked=NULL;
            (*obj).value=el;
            printf("  { ARR_FLOAT_F:%f }\n",*(float*)res->value);
            break;
        case 3:
        //printf("\n\ntest3\n\n");
            res = new_JSON_BOOL(NULL,*(bool*)v.value,NULL);
            el = malloc(sizeof(JSON_ELEMENT));
            el->value = res;
            el->TYPE_VALUE = 3;
            el->linked=NULL;
            (*obj).value=el;
            printf("  { ARR_BOOL_F:%d }\n",*(bool*)res->value);
            break;
        case 4:
            res = new_JSON_STRING(NULL,(char*)v.value,NULL);
            el = malloc(sizeof(JSON_ELEMENT));
            el->value = res;
            el->TYPE_VALUE = 4;
            el->linked=NULL;
            (*obj).value=el;
            printf("  { ARR_STRING_F:%s }\n",(char*)res->value);
            break;
        case 5://배열
            res = _PARSER_ARR(file,res,NULL);
            JSON_ELEMENT *temp=el;
            el = malloc(sizeof(JSON_ELEMENT));
            el->value = res;
            el->TYPE_VALUE = 5;
            el->linked=temp;
            printf("    { ARR_ARR_F:[] }\n");
            break;
        case 6://객체
            break;
        default:
            return NULL;//유효하지 않은 typeid
    }
    while(1){
            //2~끝까지 원소
            v = _PARSER_EXTRACTION(file);
            if(v.TYPE_VALUE==0) break;
            switch (v.TYPE_VALUE){
                case 1:
                    res = new_JSON_INT(NULL,*(int*)v.value,NULL);
                    if(el==NULL){
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 1;
                        el->linked=NULL;
                        (*obj).value=el;
                    }else{
                        JSON_ELEMENT *temp=el;
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 1;
                        el->linked=temp;
                    }
                    printf("  { ARR_INT:%d }\n",*(int*)res->value);
                    break;
                case 2:
                    res = new_JSON_FLOAT(NULL,*(float*)v.value,NULL);
                    if(el==NULL){
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 2;
                        el->linked=NULL;
                        (*obj).value=el;
                    }else{
                        JSON_ELEMENT *temp=el;
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 2;
                        el->linked=temp;
                    }
                    printf("  { ARR_FLOAT:%f }\n",*(float*)res->value);
                    break;
                case 3:
                    res = new_JSON_BOOL(NULL,*(bool*)v.value,NULL);
                    if(el==NULL){
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 3;
                        el->linked=NULL;
                        (*obj).value=el;
                    }else{
                        JSON_ELEMENT *temp=el;
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 3;
                        el->linked=temp;
                    }
                    printf("  { ARR_BOOL:%d }\n",*(bool*)res->value);
                    break;
                case 4:
                    res = new_JSON_STRING(NULL,(char*)v.value,NULL);
                    if(el==NULL){
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 4;
                        el->linked=NULL;
                        (*obj).value=el;
                    }else{
                        JSON_ELEMENT *temp=el;
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 4;
                        el->linked=temp;
                    }
                    printf("  { ARR_STRING:%s }\n",(char*)res->value);
                    break;
                case 5://배열
                    printf("    { ARR_ARR:[] }\n");
                    res = _PARSER_ARR(file,res,NULL);
                    //printf("ARR_ARR_END:[]");
                    if(el==NULL){
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 5;
                        el->linked=NULL;
                        (*obj).value=el;
                    }else{
                        JSON_ELEMENT *temp=el;
                        el = malloc(sizeof(JSON_ELEMENT));
                        el->value = res;
                        el->TYPE_VALUE = 5;
                        el->linked=temp;
                    }
                    break;
                case 6://객체
                default:
                    return NULL;//유효하지 않은 typeid
            }
            
            if(v.thischar==']'){
                fgetc(file);
                printf("ARR_ARR_CLOSE:[]\n");
            (*obj).linked=before;
            (*obj).TYPE_LINK=before->TYPE_VALUE;
            break;
            }
    }
    return obj;
}

JSON *_PARSER_PARSER(char*path){
    FILE* file = fopen(path,"r");
    if(file==NULL){
        printf("파일을 읽을 수 없음");
        return NULL;
    }
    JSON*json = malloc(sizeof(JSON));
    JSON_COMPONENTS *components=malloc(sizeof(JSON_COMPONENTS));
    json->value=components;
    json->type=0;
    
    int ch;
    char*key=NULL;
    while((ch=fgetc(file))!='{');
        printf("PARSER:\n");
    while((ch=fgetc(file))!=EOF){
        if(ch=='\"'){
            if(key==NULL){
                //이 문자열은 tag(key)
                key = _PARSER_STR(file);
                if(key==NULL) return NULL;
            }else{
                //이 문자열은 value-> JSON_STRING
                JSON_COMPONENTS*res = malloc(sizeof(JSON_COMPONENTS));
                res->value=_PARSER_STR(file);
                if(res->value==NULL) return NULL;
                res->TYPE_VALUE=4;
                res->tag = key;
                res->TYPE_LINK=0;
                res->linked=NULL;
                //
                components->linked=res;
                components->TYPE_LINK=4;
                components = res;
                printf("- STRINGTAG:[%s:%s]\n",key,res->value);
                key=NULL;
            }
        continue;
        }
        if(ch==':'){
            if(key==NULL) return NULL;
            PARSER_SINGLEDATA v = _PARSER_EXTRACTION(file);
            JSON_COMPONENTS*res;
            switch (v.TYPE_VALUE)
            {
            case 1:
                res = malloc(sizeof(JSON_COMPONENTS));
                res->tag=key;
                res->value=v.value;
                res->TYPE_VALUE=v.TYPE_VALUE;
                res->TYPE_LINK=0;
                res->linked=NULL;
                //
                components->linked=res;
                components->TYPE_LINK=v.TYPE_VALUE;
                components = res;
                printf("- INTTAG:[%s:%d]\n",key,*(int*)(v.value));
                break;
            case 2:
                res = malloc(sizeof(JSON_COMPONENTS));
                res->tag=key;
                res->value=v.value;
                res->TYPE_VALUE=v.TYPE_VALUE;
                res->TYPE_LINK=0;
                res->linked=NULL;
                //
                components->linked=res;
                components->TYPE_LINK=v.TYPE_VALUE;
                components = res;
                printf("- FLOATTAG:{%s:%f}\n",key,*(float*)v.value);
                break;
            case 3:
                res = malloc(sizeof(JSON_COMPONENTS));
                res->tag=key;
                res->value=v.value;
                res->TYPE_VALUE=v.TYPE_VALUE;
                res->TYPE_LINK=0;
                res->linked=NULL;
                //
                components->linked=res;
                components->TYPE_LINK=v.TYPE_VALUE;
                components = res;
                printf("- BOOLTAG:{%s:%d}\n",key,*(bool*)res->value);
                break;
            case 4:
                res = malloc(sizeof(JSON_COMPONENTS));
                res->tag=key;
                res->value=v.value;
                res->TYPE_VALUE=v.TYPE_VALUE;
                res->TYPE_LINK=0;
                res->linked=NULL;
                //
                components->linked=res;
                components->TYPE_LINK=v.TYPE_VALUE;
                components = res;
                printf("- STRINGTAG:{%s:%s}\n",key,res->value);
                break;
            case 5://배열
                if(key==NULL) return NULL;
                printf("- ARRAYTAG:[%s:ARRAY]\n",key);
                res = _PARSER_ARR(file,components,key);
                components=res;
                key=NULL;
                break;
            case 6://객체
                if(key==NULL) return NULL;
                printf("- OBJTAG:[%s:OBJECT]\n",key);
                res = _PARSER_OBJ(file,components,key);
                components=res;
                key=NULL;
                break;
            default:
                return NULL;//유효하지 않은 typeid
            }
            key=NULL;
        }
        if(ch=='}'){
            break;
        }
    
    }
    return json;
}