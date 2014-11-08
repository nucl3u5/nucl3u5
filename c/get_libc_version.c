#include <gnu/libc-version.h>
#include <stdio.h>
const char *gnu_get_libc_version(void);

int main(void){
        const char *version = gnu_get_libc_version();
        printf("%s\n", version);
        return(0);
}
