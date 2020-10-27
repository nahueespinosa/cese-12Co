#include <stdio.h>
#include <unistd.h>

int main( void ) {

    if( fork() == 0 ) {
        printf("Hola desde el hijo!\n");
        sleep(20);
    } else {
        printf("Hola desde el padre!\n");
        sleep(40);
    }
    return 0;
}