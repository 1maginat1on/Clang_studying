#include <stdio.h>
//printf("Text") - print formated
// Variants: %d(знаковое обычное число), %u(беззнаковое)
// Example: printf("Hello %d%u", i, j)
// Variants: %f, %c(char), %s(string)
// Example: printf("Hello %s%f", "IBKS", -3.75)

//%5d - количество знаков для вывода
//%-5d 
//
//%7.3d - 2.75(3 spaces)
//("%*.*f", 7.3, 2.75)

// \n(переход на следующую строку), \r(возвращает в начало строки), \t(табуляция)
// \\- чтобы вывести слеш
// \" - просто кавычка
// %% - вывод процента



//scanf("%d", &i)

int main(void){

    printf("%*.*f", 7.3, 2.75);

}