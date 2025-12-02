#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "json.h"
#include "parser.h"

//공백제거용 함수,만약 공백이 아니더라도 자기자신을 반환 | 반환:마지막으로 읽은 문자
int _PARSER_NEXTCHAR(FILE* file) {
    int ch = fgetc(file);
    if (ch<33) while ((ch = fgetc(file)) != EOF&&ch<33);//유효 문자까지 이동
    return ch;
}


//앞에 "가 나왔을때 => 반환된 문자 뒤에 위치한 공백을 지우고 :나 , 까지 감 | TYPE_VALUE=0 반환:문자열이 유효하지 않음
PARSER_SINGLEDATA _PARSER_STR(FILE* file) {
    int ch;
    int STR_INDEX = 0;
    int slashes = 0;
    //
    long pos = ftell(file);
    bool ended = false;
    int length = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch < 0 || ch>127) continue;//아스키 이외의 문자는 처리x
        if (slashes % 2 == 0 && ch == '\"') {
            ended = true;
            break;
        }
        else {
            length++;
        }
        if (ch == '\\') {
            slashes += 1;
        }
        else slashes = 0;
    }
    fseek(file, pos, SEEK_SET);

    if (ended == false) return (PARSER_SINGLEDATA) { T_NONE, NULL, ch };
    char* STR = malloc((length + 1) * sizeof(char));
    for (int i = 0;i != length;i++) {
        ch = fgetc(file);
        if (ch>127) {
            i -= 1;
            continue;
        }else if(ch==0) return (PARSER_SINGLEDATA) { T_NONE, NULL, ch };
        //문자열 읽기
        STR[i] = ch;
    }
    STR[length] = 0;//문자열 종료
    ch = fgetc(file);//뒤에 "제거
    ch = _PARSER_NEXTCHAR(file);
    return (PARSER_SINGLEDATA) { T_STRING, STR, ch };
}


//앞에 :가 나왔을때 => 반환된 문자 뒤에 위치한 공백을 지우고 :나 , 까지 감 | TYPE_VALUE=0 반환:문자열이 유효하지 않음
PARSER_SINGLEDATA _PARSER_EXTRACTION(FILE* file) {
    int ch;
    int INT_INDEX = 0;//'\0'을 제외한 글자수
    char* INT = malloc(110);
    bool isfloat = false;
    while ((ch = fgetc(file)) != EOF) {
        if (ch<33) continue; //제어문자 무시
        if (ch == '\"') {
            //문자열
            PARSER_SINGLEDATA str = _PARSER_STR(file);//thischar= : or ,
            char* getstr = str.value;
            return (PARSER_SINGLEDATA) { T_STRING, getstr, str.thischar };
        }
        else if (ch == '-') {//음수부호
            if (INT_INDEX == 0) {
                INT[INT_INDEX++] = ch;
            }
            else return (PARSER_SINGLEDATA) { T_NONE, 0, ch };//마이너스 부호가 첫번째 위치가 아닌 곳에 존재
        }
        else if (ch > 47 && ch < 58) {//정수
            INT[INT_INDEX++] = ch;
        }
        else if (ch == '.') {//소수점
            if (isfloat) return (PARSER_SINGLEDATA) { T_NONE, 0, ch };//소수점 2개이므로 오류
            isfloat = true;
            INT[INT_INDEX++] = ch;
        }
        else if (ch == '[') {//array 열기
            return (PARSER_SINGLEDATA) { T_ARRAY, 0, ch };//객체,배열은 외부에서 처리
        }
        else if (ch == '{') {//object 열기
            return (PARSER_SINGLEDATA) { T_OBJECT, 0, ch };//객체,배열은 외부에서 처리
        }
        else {
            //bool 판별
            if (INT_INDEX == 0) {
                //느슨한 판별(앞 한글자만 일치하면 됌)
                if (ch == 't') {
                    bool* t = malloc(sizeof(bool));
                    *t = true;
                    while ((ch = fgetc(file)) != EOF) {//남은 글자 제거
                        if (ch == ',' || ch == ']' || ch == '}') {
                            return (PARSER_SINGLEDATA) { T_BOOL, t, ch };
                        }
                        else if (ch == ':') return (PARSER_SINGLEDATA) { 0, 0, ch };//키값은 string만 가능하므로 오류
                    }
                    return (PARSER_SINGLEDATA) { 0, 0, ch };//끝을 찾을 수 없음
                }
                if (ch == 'f') {
                    bool* t = malloc(sizeof(bool));
                    *t = false;
                    while ((ch = fgetc(file)) != EOF) {//남은 글자 제거
                        if (ch == ',' || ch == ']' || ch == '}') {
                            return (PARSER_SINGLEDATA) { T_BOOL, t, ch };
                        }
                        else if (ch == ':') return (PARSER_SINGLEDATA) { 0, 0, ch };//키값은 string만 가능하므로 오류
                    }
                    return (PARSER_SINGLEDATA) { 0, 0, ch };//끝을 찾을 수 없음
                }
                if (ch == 'n') {//null
                    while ((ch = fgetc(file)) != EOF) {//남은 글자 제거
                        if (ch == ',' || ch == ']' || ch == '}') {
                            return (PARSER_SINGLEDATA) { T_NULL, NULL, ch };
                        }
                        else if (ch == ':') return (PARSER_SINGLEDATA) { 0, 0, ch };//키값은 string만 가능하므로 오류
                    }
                    return (PARSER_SINGLEDATA) { 0, 0, ch };//끝을 찾을 수 없음
                }
            }
            break;
        }
    }
    if (INT_INDEX == 0) return (PARSER_SINGLEDATA) { T_NONE, 0, ch };//유효한 문자가 없음
    if (!isfloat) {//정수(int) 반환
        INT[INT_INDEX] = '\0';
        int* v = malloc(sizeof(int));
        *v = atoi(INT);
        return (PARSER_SINGLEDATA) { T_INT, v, ch };
    }
    else {//실수(float) 반환 (유효숫자 7개까지만 가능)
        int i = INT_INDEX - 1;//실제 index
        if (INT[i] == '.') {//마지막 글자가 .임->json 오류
            return (PARSER_SINGLEDATA) { T_NONE, 0, ch };
        }
        float* v = malloc(sizeof(float));
        bool isdeleted = false;
        for (;i > 7;i--) {//소수점 포함이므로 8자리(유효숫자:7)까지만 반영
            if (INT[i] == '.') isdeleted = true;
            INT[i] = '\0';
        }
        if (isdeleted || INT[i] == '.') {//소수점이 사라졌거나 마지막 글자가 .임
            INT[--i] = '.';
            INT[++i] = '0';
            INT[++i] = '\0';
        }
        else INT[++i] = '\0';
        INT_INDEX = i;
        //printf("FLOAT : %s\n", INT);
        *v = (float)atof(INT);
        return (PARSER_SINGLEDATA) { T_FLOAT, v, ch };
    }
}

//앞에 { 가 나왔을때
//array 와의 차이점: 키값이 존재, }로 끝남
JSON_COMPONENTS* _PARSER_OBJ(FILE* file, char* tagkey) {
    JSON_ELEMENT* element = malloc(sizeof(JSON_ELEMENT));
    JSON_ELEMENT* element_CONST = element;//가장 처음 원소값
    JSON_COMPONENTS* components = new_JSON_OBJECT(tagkey, NULL, NULL);
    PARSER_SINGLEDATA v;//하나의 값을 가져오는 원소

    char* key = NULL;

    char ch = _PARSER_NEXTCHAR(file);
    if (ch == '}') {//빈 오브젝트
        //비었으므로 안쓰는 값을 헤체
        free(element_CONST);
        element = NULL;
        return components;
    }
    int testloop=0;
    while (1) {
        //printf("%d: %c",++testloop,ch);
        if (ch == '\"') {
            if (key == NULL) {
                //이 문자열은 tag(key)
                v = _PARSER_STR(file);
                if (v.TYPE_VALUE == T_NONE) return NULL;
                key = v.value;
                ch = v.thischar;
            }
            else {
                //이 문자열은 value-> JSON_STRING
                v = _PARSER_STR(file);
                if (v.TYPE_VALUE == T_NONE) return NULL;
                JSON_COMPONENTS* res = new_JSON_STRING(key, v.value, NULL);
                ch = v.thischar;
                //
                components->linked = res;
                components = res;
                key = NULL;
            }
            continue;
        }
        else if (ch == ':') {
            if (key == NULL) return NULL;
            v = _PARSER_EXTRACTION(file);
            ch = v.thischar;
            JSON_COMPONENTS* res;
            JSON_ELEMENT* tempel;
            switch (v.TYPE_VALUE)
            {
            case T_NULL:
                res = new_JSON_NULL(key, NULL);
                //v.value는 null임
                //
                tempel = new_JSON_ELEMENT(res, NULL);
                element->linked = tempel;
                element = tempel;
                key = NULL;
                break;
            case T_INT:
                res = new_JSON_INT(key, *(int*)v.value, NULL);
                free(v.value);//v.value는 res에서 복사되었고 더이상 쓸모가 없으므로 해체
                //
                tempel = new_JSON_ELEMENT(res, NULL);
                element->linked = tempel;
                element = tempel;
                key = NULL;
                break;
            case T_FLOAT:
                res = new_JSON_FLOAT(key, *(float*)v.value, NULL);
                free(v.value);//v.value는 res에서 복사되었고 더이상 쓸모가 없으므로 해체
                //
                tempel = new_JSON_ELEMENT(res, NULL);
                element->linked = tempel;
                element = tempel;
                key = NULL;
                break;
            case T_BOOL:
                res = new_JSON_BOOL(key, *(bool*)v.value, NULL);
                free(v.value);//v.value는 res에서 복사되었고 더이상 쓸모가 없으므로 해체
                //
                tempel = new_JSON_ELEMENT(res, NULL);
                element->linked = tempel;
                element = tempel;
                key = NULL;
                break;
            case T_STRING:
                res = new_JSON_STRING(key, v.value, NULL);//string값은 참조로 전달되기 때문에 해체x
                //
                tempel = new_JSON_ELEMENT(res, NULL);
                element->linked = tempel;
                element = tempel;
                key = NULL;
                break;
            case T_ARRAY://배열
                res = _PARSER_ARR(file, key);
                tempel = new_JSON_ELEMENT(res, NULL);
                element->linked = tempel;
                element = tempel;
                key = NULL;
                //res는 file의 해당 오브젝트의]까지 읽음-> 쉼표 읽고 다음 반복에서 다음 유효문자가 읽히도록 함
                ch = fgetc(file);//, 바로 앞 문자
                if (ch<33) {//제어문자 밑 공백이라면
                    int nextchar = _PARSER_NEXTCHAR(file);
                    ch = nextchar;
                }
                if (ch != ',' && ch != '}' && ch != ']') return NULL;//다음 문자가 쉼표가 아님->json형식 오류
                break;
            case T_OBJECT://객체
                res = _PARSER_OBJ(file, key);
                tempel = new_JSON_ELEMENT(res, NULL);
                element->linked = tempel;
                element = tempel;
                key = NULL;
                //res는 file의 해당 오브젝트의}까지 읽음-> 쉼표 읽고 다음 반복에서 다음 유효문자가 읽히도록 함
                ch = fgetc(file);//, 바로 앞 문자
                if (ch<33) {//제어문자 밑 공백이라면
                    int nextchar = _PARSER_NEXTCHAR(file);
                    ch = nextchar;
                }
                if (ch != ',' && ch != '}' && ch != ']') return NULL;//다음 문자가 쉼표가 아님->json형식 오류
                break;
            default:
                return NULL;//유효하지 않은 typeid
            }
        }
        if (ch == ',') {
            //다음 key값을 받을 차례
            ch = fgetc(file);//, 바로 앞 문자
            if (ch<33) {//제어문자 밑 공백이라면
                int nextchar = _PARSER_NEXTCHAR(file);
                ch = nextchar;
            }
            if (ch != '\"') return NULL;//다음 키값이 string이 아님->json형식 오류
            continue;
        }
        if (ch == '}') {
            //컴포넌트의 첫번째 값은 null이므로 두번째 값을 앞당김
            element = element_CONST->linked;
            components->value = element;
            free(element_CONST);//null값을 담고있던 초기화용 데이터해체
            return components;
        }

    }
    return NULL;
}

//앞에 [ 가 나왔을때
JSON_COMPONENTS* _PARSER_ARR(FILE* file, char* tagkey) {
    JSON_ELEMENT* element = malloc(sizeof(JSON_ELEMENT));
    JSON_ELEMENT* element_CONST = element;//가장 처음 원소값
    JSON_COMPONENTS* components = new_JSON_ARRAY(tagkey, NULL, NULL);
    PARSER_SINGLEDATA v;//하나의 값을 가져오는 원소

    int ch = _PARSER_NEXTCHAR(file);
    if (ch == ']') {//빈 배열
        //비었으므로 안쓰는 값을 헤체
        free(element_CONST);
        element = NULL;
        return components;
    }
    ungetc(ch, file);
    while (1) {
        v = _PARSER_EXTRACTION(file);
        ch = v.thischar;
        JSON_COMPONENTS* res;
        JSON_ELEMENT* tempel;
        switch (v.TYPE_VALUE)
        {
        case T_NULL:
            res = new_JSON_NULL(NULL, NULL);
            //
            tempel = new_JSON_ELEMENT(res, NULL);
            element->linked = tempel;
            element = tempel;
            //v.value는 null
            break;
        case T_INT:
            res = new_JSON_INT(NULL, *(int*)v.value, NULL);
            //
            tempel = new_JSON_ELEMENT(res, NULL);
            element->linked = tempel;
            element = tempel;
            free(v.value);//v.value는 res에서 복사되었고 더이상 쓸모가 없으므로 해체
            break;
        case T_FLOAT:
            res = new_JSON_FLOAT(NULL, *(float*)v.value, NULL);
            free(v.value);//v.value는 res에서 복사되었고 더이상 쓸모가 없으므로 해체
            //
            tempel = new_JSON_ELEMENT(res, NULL);
            element->linked = tempel;
            element = tempel;
            break;
        case T_BOOL:
            res = new_JSON_BOOL(NULL, *(bool*)v.value, NULL);
            free(v.value);//v.value는 res에서 복사되었고 더이상 쓸모가 없으므로 해체
            //
            tempel = new_JSON_ELEMENT(res, NULL);
            element->linked = tempel;
            element = tempel;
            break;
        case T_STRING:
            res = new_JSON_STRING(NULL, v.value, NULL);//string값은 참조로 전달되기 때문에 해체x
            //
            tempel = new_JSON_ELEMENT(res, NULL);
            element->linked = tempel;
            element = tempel;
            break;
        case T_ARRAY://배열
            res = _PARSER_ARR(file, NULL);
            tempel = new_JSON_ELEMENT(res, NULL);
            element->linked = tempel;
            element = tempel;
            //res는 file의 해당 배열의]까지 읽음-> 쉼표 읽고 다음 반복에서 다음 유효문자가 읽히도록 함
            ch = fgetc(file);//, 바로 앞 문자
            if (ch<33) {//제어문자 밑 공백이라면
                int nextchar = _PARSER_NEXTCHAR(file);
                ch = nextchar;
            }
            break;
        case T_OBJECT://객체
            res = _PARSER_OBJ(file, NULL);
            tempel = new_JSON_ELEMENT(res, NULL);
            element->linked = tempel;
            element = tempel;
            //res는 file의 해당 배열의]까지 읽음-> 쉼표 읽고 다음 반복에서 다음 유효문자가 읽히도록 함
            ch = fgetc(file);//, 바로 앞 문자
            if (ch<33) {//제어문자 밑 공백이라면
                int nextchar = _PARSER_NEXTCHAR(file);
                ch = nextchar;
            }
            break;
        default:
            return NULL;//유효하지 않은 typeid
        }
        if (ch == ']') {
            //컴포넌트의 첫번째 값은 null이므로 두번째 값을 앞당김
            element = element_CONST->linked;
            components->value = element;
            free(element_CONST);//null값을 담고있던 초기화용 데이터해체
            return components;
        }
    }
    return NULL;
}

JSON_COMPONENTS* PARSER_PARSE(char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        printf("파일을 읽을 수 없음\n");
        return NULL;
    }
    JSON_COMPONENTS* json;
    int ch;//받은 char(유니코드 기준으로 char가 아닌 int)
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '{') {
            //object로 시작
            json = _PARSER_OBJ(file, NULL);
            fclose(file);
            return json;
        }
        else if (ch == '[') {
            //array로 시작
            json = _PARSER_ARR(file, NULL);
            fclose(file);
            return json;
        }
    }
    fclose(file);
    return NULL;//끝낼 때까지 오브젝트 또는 배열의 시작이 안나옴
}

void PARSER_PRINT_VALUE(JSON_COMPONENTS* value) {
    if(value==NULL){
        printf("[ NULL ]\n");
        return;
    }
    if (value->tag != NULL) {
        switch (value->TYPE_VALUE) {
        case T_NULL:
            printf("%s:\x1b[1;34mNULL\033[0m\n", value->tag);
            break;
        case T_INT:
            printf("%s:\x1b[1;32m[INT]\033[0m%d\n", value->tag, *(int*)value->value);
            break;
        case T_FLOAT:
            float v = *(float*)value->value;
            printf("%s:\x1b[1;32m[FLOAT]\033[0m%.7g", value->tag, v);
            if (v == (int)v) printf(".0");
            printf("\n");
            break;
        case T_BOOL:
            printf("%s:\x1b[1;32m[BOOL]\033[0m%d\n", value->tag, *(bool*)value->value);
            break;
        case T_STRING:
            printf("%s:\x1b[1;32m[STRING]\033[0m%s\n", value->tag, (char*)value->value);
            break;
        case T_ARRAY://배열
            printf("%s:\033[0;33m[ARRAY][\033[0m\n", value->tag);
            _PARSER_PRINT_ARR(value, 0);
            break;
        case T_OBJECT://객체
            printf("%s:\033[38;2;255;128;0m[OBJECT]\033[38;2;255;128;0m{\033[0m\n", value->tag);
            _PARSER_PRINT_OBJ(value, 0);
            break;
        default:
            printf("ERROR\n");
            break;
        }
    }
    else {
        switch (value->TYPE_VALUE)
        {
        case T_NULL:
            printf("\x1b[1;34mNULL\033[0m\n");
            break;
        case T_INT:
            printf("\x1b[1;32m[INT]\033[0m%d\n", *(int*)value->value);
            break;
        case T_FLOAT:
            float v = *(float*)value->value;
            printf("\x1b[1;32m[FLOAT]\033[0m%.7g", v);
            if (v == (int)v) printf(".0");
            printf("\n");
            break;
        case T_BOOL:
            printf("\x1b[1;32m[BOOL]\033[0m%d\n", *(bool*)value->value);
            break;
        case T_STRING:
            printf("\x1b[1;32m[STRING]\033[0m%s\n", (char*)value->value);
            break;
        case T_ARRAY://배열
            printf("\033[0;33m[ARRAY][\033[0m\n");
            _PARSER_PRINT_ARR(value, 0);
            break;
        case T_OBJECT://객체
            printf("\033[38;2;255;128;0m[OBJECT]\033[38;2;255;128;0m{\033[0m\n");
            _PARSER_PRINT_OBJ(value, 0);
            break;
        default:
            printf("ERROR\n");
            break;
        }
    }
}

void _PARSER_PRINT_ARR(JSON_COMPONENTS* array, int level) {
    JSON_ELEMENT* v = array->value;
    while (1) {
        if (v == NULL) {
            for (int i = level;i > 0;i--) {
                printf("    ");
            }
            printf("\033[0;33m]\033[0m\n");
            return;
        }
        for (int i = level + 1;i > 0;i--) {
            printf("    ");
        }
        JSON_COMPONENTS* value = v->value;
        if (value != NULL) {
            switch (value->TYPE_VALUE)
            {
            case T_NULL:
                printf("\x1b[1;34mNULL\033[0m\n");
                break;
            case T_INT:
                printf("\x1b[1;32m[INT]\033[0m%d\n", *(int*)value->value);
                break;
            case T_FLOAT:
                float v = *(float*)value->value;
                printf("\x1b[1;32m[FLOAT]\033[0m%.7g", v);
                if (v == (int)v) printf(".0");
                printf("\n");
                break;
            case T_BOOL:
                printf("\x1b[1;32m[BOOL]\033[0m%d\n", *(bool*)value->value);
                break;
            case T_STRING:
                printf("\x1b[1;32m[STRING]\033[0m%s\n", (char*)value->value);
                break;
            case T_ARRAY://배열
                printf("\033[0;33m[ARRAY][\033[0m\n");
                _PARSER_PRINT_ARR(value, level + 1);
                break;
            case T_OBJECT://객체
                printf("\033[38;2;255;128;0m[OBJECT]\033[38;2;255;128;0m{\033[0m\n");
                _PARSER_PRINT_OBJ(value, level + 1);
                break;
            default:
                printf("NULL\n");
                break;
            }
        }
        v = v->linked;
    }
    return;
}

void _PARSER_PRINT_OBJ(JSON_COMPONENTS* obj, int level) {
    JSON_ELEMENT* v = obj->value;
    while (1) {
        if (v == NULL) {
            for (int i = level;i > 0;i--) {
                printf("    ");
            }
            printf("\033[38;2;255;128;0m}\033[0m\n");
            return;
        }
        for (int i = level + 1;i > 0;i--) {
            printf("    ");
        }
        JSON_COMPONENTS* value = v->value;
        if (value != NULL) {
            switch (value->TYPE_VALUE)
            {
            case T_NULL:
                printf("%s:\x1b[1;34mNULL\033[0m\n",value->tag);
                break;
            case T_INT:
                printf("%s:\x1b[1;32m[INT]\033[0m%d\n", value->tag, *(int*)value->value);
                break;
            case T_FLOAT:
                float v = *(float*)value->value;
                printf("%s:\x1b[1;32m[FLOAT]\033[0m%.7g", value->tag, v);
                if (v == (int)v) printf(".0");
                printf("\n");
                break;
            case T_BOOL:
                printf("%s:\x1b[1;32m[BOOL]\033[0m%d\n", value->tag, *(bool*)value->value);
                break;
            case T_STRING:
                printf("%s:\x1b[1;32m[STRING]\033[0m%s\n", value->tag, (char*)value->value);
                break;
            case T_ARRAY://배열
                printf("%s:\033[0;33m[ARRAY][\033[0m\n", value->tag);
                _PARSER_PRINT_ARR(value, level + 1);
                break;
            case T_OBJECT://객체
                printf("%s:\033[38;2;255;128;0m[OBJECT]\033[38;2;255;128;0m{\033[0m\n", value->tag);
                _PARSER_PRINT_OBJ(value, level + 1);
                break;
            default:
                printf("NULL\n");
                break;
            }
        }
        v = v->linked;
    }
    return;
}

void PARSER_PRINT(JSON_COMPONENTS* json) {
    if (json == NULL) return;
    if (json->TYPE_VALUE == T_ARRAY) {//JSON_ARRAY
        printf("\033[0;33m[ARRAY][\033[0m\n");
        _PARSER_PRINT_ARR(json, 0);
    }
    else if(json->TYPE_VALUE==T_OBJECT){//JSON_OBJECT
        //head출력
        printf("\033[38;2;255;128;0m[OBJECT]{\033[0m\n");
        _PARSER_PRINT_OBJ(json, 0);
    }
    return;
}



void _PARSER_SAVE_ARR(FILE *file,JSON_COMPONENTS* array, int level,bool end){
JSON_ELEMENT* v = array->value;
    while (1) {
        if (v == NULL) {
            for (int i = level;i > 0;i--) {
                fputs("    ",file);
            }
            if(!end){
                //콤마
                fputs("],\n",file);
            }else fputs("]\n",file);
            return;
        }
        for (int i = level + 1;i > 0;i--) {
            fputs("    ",file);
        }
        JSON_COMPONENTS* value = v->value;
        char *txt;
        bool isc = v->linked!=NULL;//콤마를 찍어야되는지에 대한 여부
        if (value != NULL) {
            switch (value->TYPE_VALUE)
            {
            case T_NULL:
                if(isc){
                    fputs("null,\n",file);
                }else fputs("null\n",file);
                break;
            case T_INT:
                txt = malloc(sizeof(char)*13);//int의 끝이 10자리임(부호1자리,\0 1자리)
                sprintf(txt,"%d",*(int*)value->value);
                fputs(txt,file);
                if(isc){
                    fputs(",\n",file);
                }else fputc('\n',file);
                free(txt);
                break;
            case T_FLOAT:
                txt = malloc(sizeof(char)*11);//float는 유효숫자 7까지(부호1자리,. 1자리,\0 1자리)
                sprintf(txt,"%.7g",*(float*)value->value);
                fputs(txt,file);
                if(isc){
                    fputs(",\n",file);
                }else fputc('\n',file);
                free(txt);
                break;
            case T_BOOL:
                if(*(bool*)value->value==false){
                    if(isc){
                        fputs("false,\n",file);
                    }else fputs("false\n",file);
                }else{
                    if(isc){
                        fputs("true,\n",file);
                    }else fputs("true\n",file);
                }
                break;
            case T_STRING:
                fputc('\"',file);
                fputs((char*)value->value,file);
                if(isc){
                    fputs("\",\n",file);
                }else fputs("\"\n",file);
                break;
            case T_ARRAY://배열
                fputs("[\n",file);
                _PARSER_SAVE_ARR(file,value, level + 1,!isc);
                break;
            case T_OBJECT://객체
                fputs("{\n",file);
                _PARSER_SAVE_OBJ(file,value, level + 1,!isc);
                break;
            default:
                break;
            }
        }
        v = v->linked;
    }
    return;
}

void _PARSER_SAVE_OBJ(FILE *file,JSON_COMPONENTS* obj, int level,bool end){
JSON_ELEMENT* v = obj->value;
    while (1) {
        if (v == NULL) {

            for (int i = level;i > 0;i--) {
                fputs("    ",file);
            }
            if(!end){
                //콤마
                fputs("},\n",file);
            }else fputs("}\n",file);
            return;
        }
        for (int i = level + 1;i > 0;i--) {
            fputs("    ",file);
        }
        JSON_COMPONENTS* value = v->value;
        char *txt;
        bool isc = v->linked!=NULL;//콤마를 찍어야되는지에 대한 여부
        if (value != NULL) {
            switch (value->TYPE_VALUE){
            case T_NULL:
                fputc('\"',file);
                fputs(value->tag,file);
                fputs("\":",file);
                if(isc){
                    fputs("null,\n",file);
                }else fputs("null\n",file);
                break;
            case T_INT:
                fputc('\"',file);
                fputs(value->tag,file);
                fputs("\":",file);
                txt = malloc(sizeof(char)*13);//int의 끝이 10자리임(부호1자리,\0 1자리)
                sprintf(txt,"%d",*(int*)value->value);
                fputs(txt,file);
                if(isc){
                    fputs(",\n",file);
                }else fputc('\n',file);
                free(txt);
                break;
            case T_FLOAT:
                fputc('\"',file);
                fputs(value->tag,file);
                fputs("\":",file);
                txt = malloc(sizeof(char)*11);//float는 유효숫자 7까지(부호1자리,. 1자리,\0 1자리)
                sprintf(txt,"%.7g",*(float*)value->value);
                fputs(txt,file);
                if(isc){
                    fputs(",\n",file);
                }else fputc('\n',file);
                free(txt);
                break;
            case T_BOOL:
                fputc('\"',file);
                fputs(value->tag,file);
                fputs("\":",file);
                if(*(bool*)value->value==false){
                fputs("false",file);
                }else fputs("true",file);
                if(isc){
                    fputs(",\n",file);
                }else fputc('\n',file);
                break;
            case T_STRING:
                fputc('\"',file);
                fputs(value->tag,file);
                fputs("\":\"",file);
                fputs((char*)value->value,file);
                if(isc){
                    fputs("\",\n",file);
                }else fputs("\"\n",file);
                break;
            case T_ARRAY://배열
                fputc('\"',file);
                fputs(value->tag,file);
                fputs("\":",file);
                fputs("[\n",file);
                _PARSER_SAVE_ARR(file,value, level + 1,!isc);
                break;
            case T_OBJECT://객체
                fputc('\"',file);
                fputs(value->tag,file);
                fputs("\":",file);
                fputs("{\n",file);
                _PARSER_SAVE_OBJ(file,value, level + 1,!isc);
                break;
            default:
                break;
            }
        }
        v = v->linked;
    }
    return;
}

void PARSER_SAVE(char*path, JSON_COMPONENTS* json){
    FILE *file = fopen(path,"w");
    if(file==NULL){
        printf("파일 불러오기 실패");
        return;
    }
    if (json == NULL){
        fclose(file);
        return;
    }
    if (json->TYPE_VALUE == T_ARRAY) {//JSON_ARRAY
        fputs("[\n",file);
        _PARSER_SAVE_ARR(file,json, 0,json->linked==NULL);
    }else if(json->TYPE_VALUE==T_OBJECT){//JSON_OBJECT
        //head출력
        fputs("{\n",file);
        _PARSER_SAVE_OBJ(file,json, 0,json->linked==NULL);
    }else{
        fclose(file);
        return;
    }
    fclose(file);
    return;
}
