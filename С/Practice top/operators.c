#include <stdio.h>
int main(void){
    int a, b;
    a = -5;
    b = 5;
    if (a>=0){
        printf("%d", a);
    }
    else{
        printf("%u %d", b, a);
    }
}
