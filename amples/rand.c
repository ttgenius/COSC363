
#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;
    for (i = 0; i < 1000; i++) {
        printf("%d", (float)(rand()%100)/1000);
    }
}
