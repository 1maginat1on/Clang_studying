#include <stdio.h>
#include <locale.h>
int main(){
    int a, b;
    //setlocale(LC_ALL, ""); решает проблему с русскими буквами
    b = 3025; 
    a = 2025;
    printf("Summ %d", a+b);
    return 0;
}
