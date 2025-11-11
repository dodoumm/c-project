#include <stdlib.h>
#include <stdio.h>
#include "window.h"

typedef struct{
    int width;
    int height;
    int pixelquality;//화질(1:기본 , 2,3,4 높을수록 화질 낮아짐(2:2*2당 1픽셀))
    int speed;//영상 재생 속도
} Window;


static Window* window=NULL;

void window_init(int width, int height, int pixelquality, int speed){
    printf("ff");
}

void window_free(){
}