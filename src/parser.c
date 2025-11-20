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
    //printf("\nstring:");
    //ch=fgetc(file);
    while((ch=fgetc(file))!=EOF){
    //printf("%c",ch);
        if(slashes%2==0&&ch=='\"'){
            //문자열의 끝
            STR[STR_INDEX]='\0';
            //printf("\n %s",STR);
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
    //PARSER_SINGLEDATA res = {0,0};
    int ch;
    int INT_INDEX = 0;
    char*INT=malloc(110);
    bool isfloat=false;
    //printf("\nstr");
    while((ch=fgetc(file))!=EOF){
        if(ch==' '||ch=='\n') continue;
        //printf("%c",ch);
        if(ch=='\"'){
            //문자열
            char* getstr = _PARSER_STR(file);
            fgetc(file);
            return (PARSER_SINGLEDATA){4,getstr,ch};
        }else if(ch=='-'){
            if(INT_INDEX==0){
                INT[INT_INDEX++]=ch;
            }else return (PARSER_SINGLEDATA){0,0,ch};//마이너스 부호가 첫번째 위치가 아닌 곳에 존재
        }else if(ch>47&&ch<58){//정수
            //printf("INTED | V:%c ",ch);
            INT[INT_INDEX++]=ch;
        }else if(ch=='.'){
            if(isfloat) return (PARSER_SINGLEDATA){0,0,ch};//소수점 2개이므로 오류
            isfloat=true;
            INT[INT_INDEX++]=ch;
        }else if(ch=='['){
            //array
            return (PARSER_SINGLEDATA){5,0,ch};//객체,배열은 외부에서 처리
        }else if(ch=='{'){
            //object
            return (PARSER_SINGLEDATA){6,0,ch};//객체,배열은 외부에서 처리
        }else{
            //bool 테스트
            if(INT_INDEX==0){
                //bool
                if(ch=='t'||ch=='T'){
                    bool *t = malloc(sizeof(bool));
                    *t=true;
                    return (PARSER_SINGLEDATA){3,t,ch};
                }
                if(ch=='f'||ch=='F'){
                    bool *t = malloc(sizeof(bool));
                    *t=false;
                    return (PARSER_SINGLEDATA){3,t,ch};
                }
            }
            break;
            //return (PARSER_SINGLEDATA){0,0};//숫자가 아님
        }
    }
    if(INT_INDEX==0) return (PARSER_SINGLEDATA){0,0,ch};
    INT[INT_INDEX]='\0';
    //printf("BREAKED | INDEX:%d | data:%s",INT_INDEX,INT);
    if(!isfloat){
        //int
        int *v = malloc(sizeof(int));
        *v=atoi(INT);
        return (PARSER_SINGLEDATA){1,v,ch};
    }else{
        //float
        float *v = malloc(sizeof(float));
        *v=atof(INT);
        return (PARSER_SINGLEDATA){2,v,ch};
    }
}

//앞에 { 가 나왔을때
JSON_COMPONENTS *_PARSER_OBJ(FILE*file,JSON_COMPONENTS*before,char *tagkey){
    int ch;
    JSON_ELEMENT *el = NULL;
    JSON_COMPONENTS *obj = malloc(sizeof(JSON_COMPONENTS));
    obj->tag=tagkey;
    obj->TYPE_VALUE=6;
    obj->value=el;
    obj->TYPE_LINK=0;
    obj->linked=NULL;
    //{tagkey,6,el,NULL,0};
    //JSON_COMPONENTS *lastobj = &obj;
    char*tag = NULL;
    printf("\nobj:");
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
    JSON_ELEMENT *el = NULL;
    //JSON_COMPONENTS obj = {tagkey,5,el,NULL,0};
    JSON_COMPONENTS *obj = malloc(sizeof(JSON_COMPONENTS));
    obj->tag=tagkey;
    obj->TYPE_VALUE=5;
    obj->value=el;
    obj->TYPE_LINK=0;
    obj->linked=NULL;
        //첫번째 원소
    PARSER_SINGLEDATA v = _PARSER_EXTRACTION(file);
    //printf("\n\nTV:%d | CHAR:%c\n\n",v.TYPE_VALUE,ch);
    JSON_COMPONENTS*res;
    switch (v.TYPE_VALUE){
    case 1:
    //printf("\n\ntest1\n\n");
        res = new_JSON_INT(NULL,*(int*)v.value,NULL);
        el = malloc(sizeof(JSON_ELEMENT));
        el->value = res;
        el->TYPE_VALUE = 1;
        el->linked=NULL;
        (*obj).value=el;
        printf("  ARR_INT_F:%d\n",*(int*)res->value);
        break;
    case 2:
    //printf("\n\ntest2\n\n");
        res = new_JSON_FLOAT(NULL,*(float*)v.value,NULL);
        el = malloc(sizeof(JSON_ELEMENT));
        el->value = res;
        el->TYPE_VALUE = 2;
        el->linked=NULL;
        (*obj).value=el;
        printf("  ARR_FLOAT_F:%f\n",*(float*)res->value);
        break;
    case 3:
    //printf("\n\ntest3\n\n");
        res = new_JSON_BOOL(NULL,*(bool*)v.value,NULL);
        el = malloc(sizeof(JSON_ELEMENT));
        el->value = res;
        el->TYPE_VALUE = 3;
        el->linked=NULL;
        (*obj).value=el;
        printf("  ARR_BOOL_F:%d\n",*(bool*)res->value);
        break;
    case 4:
        res = new_JSON_STRING(NULL,(char*)v.value,NULL);
        el = malloc(sizeof(JSON_ELEMENT));
        el->value = res;
        el->TYPE_VALUE = 4;
        el->linked=NULL;
        (*obj).value=el;
        printf("  ARR_STRING_F:%s\n",(char*)res->value);
        break;
    case 5://배열
        res = _PARSER_ARR(file,res,NULL);
        JSON_ELEMENT *temp=el;
        el = malloc(sizeof(JSON_ELEMENT));
        el->value = res;
        el->TYPE_VALUE = 5;
        el->linked=temp;
        printf("    ARR_ARR_F:[]\n");
        break;
    case 6://객체
        break;
    default:
        return NULL;//유효하지 않은 typeid
    }
    //while((ch=fgetc(file))!=EOF){
    while(1){
        //if(ch==' '||ch=='\n') continue;
        /*
        if(ch=='\"'){
            //이 문자열은 value-> JSON_STRING
            JSON_COMPONENTS*res = malloc(sizeof(JSON_COMPONENTS));
            res->value=_PARSER_STR(file);
            if(res->value==NULL) return NULL;
            res->TYPE_VALUE=4;
            res->tag = NULL;
            res->TYPE_LINK=0;
            res->linked=NULL;
            //
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
                printf("  ARR_STRING:%s",res);
            }
        }
            */
            //2~끝까지 원소
            v = _PARSER_EXTRACTION(file);
            if(v.TYPE_VALUE==0) break;
            //JSON_COMPONENTS *comp;
            switch (v.TYPE_VALUE)
            {
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
                printf("  ARR_INT:%d\n",*(int*)res->value);
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
                printf("  ARR_FLOAT:%f\n",*(float*)res->value);
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
                printf("  ARR_BOOL:%d\n",*(bool*)res->value);
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
                printf("  ARR_STRING:%s\n",(char*)res->value);
                break;
            case 5://배열
                printf("    ARR_ARR:[]\n");
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
                /*
                printf("ARRAYTAG:%s|value:...\n",key);
                comp = _PARSER_ARR(file,components,key);
                components=comp;
                key=NULL;
                break;
                */
            case 6://객체
                /*
                if(key==NULL) return NULL;
                printf("OBJTAG:%s|value:...\n",key);
                comp = _PARSER_OBJ(file,components,key);
                components=comp;
                key=NULL;
                break;
                */
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
            /*
        if(ch==']'){
            (*obj).linked=before;
            (*obj).TYPE_LINK=before->TYPE_VALUE;
            break;
        }
            */
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
        printf("\nparser:");
    while((ch=fgetc(file))!=EOF){
        //printf("%c",ch);
        //if((ch==' '||ch=='\n')) continue;
        if(ch=='\"'){
            if(key==NULL){
                //이 문자열은 tag(key)
                //printf("STRINGTAG | 1-1\n");
                key = _PARSER_STR(file);
                if(key==NULL) return NULL;
                //printf("STRINGTAG | 1-2 | %s\n",key);
            }else{
                //이 문자열은 value-> JSON_STRING
                //printf("STRINGTAG | 2-1\n");
                JSON_COMPONENTS*res = malloc(sizeof(JSON_COMPONENTS));
                res->value=_PARSER_STR(file);
                if(res->value==NULL) return NULL;
                //printf("STRINGTAG | 2-2\n");
                res->TYPE_VALUE=4;
                res->tag = key;
                res->TYPE_LINK=0;
                res->linked=NULL;
                //
                components->linked=res;
                components->TYPE_LINK=4;
                components = res;
                printf("STRINGTAG:%s|value:%s\n",key,res->value);
                key=NULL;
            }
            /*
            char*txt = _PARSER_STR(file);
            if(txt==NULL) return NULL;
            if(key==NULL){
                key=txt;
            }else{
                //값
            }
                */
        continue;
        }
        if(ch==':'){
            if(key==NULL) return NULL;
            PARSER_SINGLEDATA v = _PARSER_EXTRACTION(file);
            JSON_COMPONENTS*res;
            //JSON_COMPONENTS *comp;
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
                printf("INTTAG:%s|value:%d\n",key,*(int*)(v.value));
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
                printf("FLOATTAG:%s|value:%f\n",key,*(float*)v.value);
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
                printf("BOOLTAG:%s|value:%d\n",key,*(bool*)res->value);
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
                printf("STRINGTAG:%s|value:%s\n",key,res->value);
                break;
            case 5://배열
                if(key==NULL) return NULL;
                printf("ARRAYTAG:%s|value:[parser]\n",key);
                res = _PARSER_ARR(file,components,key);
                components=res;
                key=NULL;
                break;
            case 6://객체
                if(key==NULL) return NULL;
                printf("OBJTAG:%s|value:...\n",key);
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

/*
JSON_COMPONENTS *_PARSER_BASE(FILE*file,JSON_COMPONENTS*before){
    char * tag=NULL;
    void * value=NULL;
    unsigned char type=0;

    int beforech;
    int ch;
    int STR_INDEX = 0;
    int*STR=NULL;
    while((ch=fgetc(file))!=EOF){
        if(!STR&&(ch==' '||ch=='\n')) continue;
        //str
        if(STR){
            if(STR_INDEX>98) continue;//0~99 ->100글자까지만 취급
            STR[STR_INDEX++] = (unsigned char)ch;
        }
        if(!STR&&ch==','){
            //COMPONENT의 끝
            if(tag==NULL&&value==NULL) return 1;//깨진 json
            if(((tag!=NULL)+(value==NULL))==1){
                //array 원소값
            }
        }
        if(ch=='\"'&&beforech!='\\'){
            //string
            if(!STR){
                //문자열 종료
                STR[STR_INDEX]='\0';
                STR=NULL;
                if(tag==NULL){
                    //이 문자열이 태그(key)임
                    tag = STR;
                }else{
                    //이 문자열이 값(value)임
                    tag = value;
                    type = 4;
                    break;
                }
            }else{
                //문자열 시작
                STR=malloc(sizeof(int)*110);
            }
        }
        printf("%c",ch);
        beforech = ch;
    }
}


JSON *PARSER_SERIALIZE(char* path){
    FILE* file = fopen(path,"r");
    JSON*json = malloc(sizeof(JSON));
    if(file==NULL){
        printf("파일을 읽을 수 없음");
        return json;
    }
    printf("\n");
    json->type=0;
    json->value=0;
    
    JSON_COMPONENTS *value=NULL;
    JSON_ELEMENT *element=NULL;

    bool iskey = true;
    bool isslash = false;
    char*cache[110]={1};
    int level = 0;
    int ch;
    //object형인지 array형인지 판단
    while(fgetc(file)==' ');
    int firstch = fgetc(file);
    if(firstch=="{"){
        //obj
        value = new_JSON_ARRAY(NULL,);
    }else if(firstch=="["){
        //array
    }else return NULL;//오류
    //SERIALIZE
    while((ch=fgetc(file))!=EOF){
        if(!cache&&(ch==' '||ch=='\n')) continue;
        //if(ch=="\\") isslash=true;//백슬래시이므로 다음 글자는 특수문자 취급x
        if(ch=="\""){
            if(cache!=NULL){
                cache[index]
                //문자열 종료
            }else{
                //문자열 추가(100글자까지만 취급)
                if(cache[0]!=100){
                    int index =cache[0];
                    cache[index]=(unsigned char)ch;
                }else continue;
            }
        }
        printf("%c",ch);
    }
}

*/