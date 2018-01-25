#include <stdio.h>
#include "sfmm.h"

int main(int argc, char const *argv[]) {

    sf_mem_init();

    void *x = sf_malloc(0);
    void *y = sf_malloc(16385);
    void *z = sf_realloc(x, 16383);
    if(z || y);

    // sf_errno = 0;
    // /* void *w = */ sf_malloc(sizeof(long));
    // void *x = sf_malloc(sizeof(double) * 11);
    // void *y = sf_malloc(sizeof(char));
    // /* void *z = */ sf_malloc(sizeof(int));

    // sf_free(y);
    // sf_free(x);

    // void *x = sf_malloc(6000);
    // sf_realloc(x, 4064);

    // sf_malloc(3000);
    // sf_malloc(2500);
    // sf_malloc(4096);
    // sf_malloc(4096);
    // sf_malloc(4096);
    // sf_malloc(4096);

    // sf_header *header = (sf_header*)((char*)y - 8);
    // sf_malloc(4096);
    // sf_malloc(4096);
    // sf_malloc(4096);
    // sf_malloc(4096);

    //     /* void *u = */ sf_malloc(1);          //32
    // void *v = sf_malloc(LIST_1_MIN); //48
    // void *w = sf_malloc(LIST_2_MIN); //160
    // void *x = sf_malloc(LIST_3_MIN); //544
    // void *y = sf_malloc(LIST_4_MIN); //2080
    // /* void *z = */ sf_malloc(1); // 32

    // int allocated_block_size[4] = {48, 160, 544, 2080};

    // sf_free(v);
    // sf_free(w);
    // sf_free(x);
    // sf_free(y);

    // // First block in each list should be the most recently freed block
    // for (int i = 0; i < FREE_LIST_COUNT; i++) {
    //     sf_free_header *fh = (sf_free_header *)(seg_free_list[i].head);
    //     // cr_assert_not_null(fh, "list %d is NULL!", i);
    //     if(fh == NULL){
    //         printf("list %d is NULL\n", i+1);
    //     }
    //     // cr_assert(fh->header.block_size << 4 == allocated_block_size[i], "Unexpected free block size!");
    //     if(fh->header.block_size << 4 != allocated_block_size[i]){
    //         printf("Unexpected block size in list %d", i+1);
    //     }
    //     // cr_assert(fh->header.allocated == 0, "Allocated bit is set!");
    //     if(fh->header.allocated != 0){
    //         printf("list %d allocated bit is wrong", i+1);
    //     }
    // }

    // // There should be one free block in each list, 2 blocks in list 3 of size 544 and 1232
    // for (int i = 0; i < FREE_LIST_COUNT; i++) {
    //     sf_free_header *fh = (sf_free_header *)(seg_free_list[i].head);
    //     if (i != 2){
    //         // cr_assert_null(fh->next, "More than 1 block in freelist [%d]!", i);
    //         if(fh->next != NULL){
    //             printf("More than 1 block in freelist [%d]!", i);
    //         }
    //     }else {
    //         // cr_assert_not_null(fh->next, "Less than 2 blocks in freelist [%d]!", i);
    //         if(fh ->next == NULL){
    //             printf("Less than 2 blocks in freelist [%d]!", i);
    //         }
    //         // cr_assert_null(fh->next->next, "More than 2 blocks in freelist [%d]!", i);
    //         if(fh->next->next != NULL){
    //             printf("More than 2 blocks in freelist [%d]!", i);
    //         }
    //     }
    // }

    // sf_malloc(16);
    // sf_malloc(4096);

    // double* ptr = sf_malloc(sizeof(double));
    // double* ptr = sf_malloc(sizeof(double));

    // double *p = sf_malloc(32);
    // double *p1 = sf_malloc(32);
    // double *p2 = sf_malloc(32);

    // sf_free(p1);

    // sf_malloc(8);

    // (void)p;
    // (void)p2;

    // sf_malloc(sizeof(double));
    // sf_malloc(sizeof(double));
    // sf_malloc(sizeof(double));
    // sf_malloc(sizeof(double));

    // sf_malloc(sizeof(double));
    // sf_malloc(16352);
    // sf_malloc(16368);
    // sf_malloc(3008);
    // sf_malloc(3008);

    // sf_malloc(4000);
    // sf_malloc(4000);

    // sf_malloc(7000);

    // *ptr = 320320320e-320;

    // printf("%f\n", *ptr);

    // sf_free(ptr);

    sf_mem_fini();

    return EXIT_SUCCESS;
}
