#include "lib.h"

#include "stdint.h"
struct DirEntry buffer[1024];
//#include "stdio.h"
int main(void) 
{
    //int total = get_total_memoryu();
    printf("Enter the name: ");
    char name[1024]={0};

    
    //memcpy(buffer, dir_entry, count * sizeof(struct DirEntry));
    //
    scans(name);
    
    int count = read_root_directory(buffer);

    int count1 = read1_root_directory(buffer);

    return 0;
}

