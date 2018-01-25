/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include "sfmm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * You should store the heads of your free lists in these variables.
 * Doing so will make it accessible via the extern statement in sfmm.h
 * which will allow you to pass the address to sf_snapshot in a different file.
 */
free_list seg_free_list[4] = {
    {NULL, LIST_1_MIN, LIST_1_MAX},
    {NULL, LIST_2_MIN, LIST_2_MAX},
    {NULL, LIST_3_MIN, LIST_3_MAX},
    {NULL, LIST_4_MIN, LIST_4_MAX}
};

void sf_sbrkColleasce();
void* makeBlock(sf_header *h, int paddedSize, int allocated, int padded, int size);

int sf_errno = 0;
int sbrk_Counter = 0;
int i = 0;
int j = 0;

void *sf_malloc(size_t size) {

    if(size == 0 || size > (4*PAGE_SZ)){
        sf_errno = 22;
        return NULL;
    }

    // if(size > (4*PAGE_SZ)-16){
    //     sf_errno = 12;
    //     return NULL;
    // }

    int foundFreeBlock = 0;
    sf_free_header *list_ptr = NULL;
    Check:for(i = 0; i < FREE_LIST_COUNT; i++){
        list_ptr = seg_free_list[i].head;
        while(list_ptr != NULL){
            int block_size = ((list_ptr -> header).block_size << 4) - 16;
            if(block_size >= size){
                if(list_ptr -> prev == NULL){
                    if(list_ptr -> next == NULL){
                        seg_free_list[i].head = NULL;
                    }else{
                        seg_free_list[i].head = list_ptr -> next;
                        (seg_free_list[i].head -> prev) -> next = NULL;
                        (seg_free_list[i].head -> prev) = NULL;
                    }
                }else{
                    (list_ptr -> prev) -> next = list_ptr -> next;
                    (list_ptr -> next) -> prev = list_ptr -> prev;
                    list_ptr -> next = NULL;
                    list_ptr -> prev = NULL;
                }
                foundFreeBlock = 1;
                break;
            }else{
                list_ptr = list_ptr -> next;
            }
        }

        if(foundFreeBlock == 1){
            break;
        }
    }

    int paddedSize = size;
    sf_header *header ;

    if (foundFreeBlock == 1){
        foundFreeBlock = 0;
        int padded = 0;

        if(size % 16 != 0){
            padded = 1;
            while(paddedSize % 16 != 0){paddedSize++;};
        }

        header = &(list_ptr->header);
        int oldBlockSize = header -> block_size << 4;
        int block_size = paddedSize + 16;
        int freeBlock_Size = oldBlockSize - block_size;

        if(freeBlock_Size >= 32){
            header = makeBlock(header, block_size, 1, padded, size);

            sf_header *freeHeader = (sf_header*)(((char *)header + paddedSize + 8)+8);
            freeHeader = makeBlock(freeHeader, freeBlock_Size, 0, 0,0);

            for(j = 0; j < FREE_LIST_COUNT; j++){
                if((freeHeader -> block_size) << 4 >= seg_free_list[j].min && (freeHeader -> block_size) << 4 <= seg_free_list[j].max){
                    if(seg_free_list[j].head == NULL){
                        seg_free_list[j].head = (sf_free_header*)freeHeader;
                        seg_free_list[j].head -> next = NULL;
                        seg_free_list[j].head -> prev = NULL;
                    }else{
                        ((sf_free_header*)freeHeader) -> next = seg_free_list[j].head;
                        seg_free_list[j].head -> prev = ((sf_free_header*)freeHeader);
                        seg_free_list[j].head = (sf_free_header*)freeHeader;
                    }
                    break;
                }
            }
        }else{
            if(freeBlock_Size > 0){
                padded = 1;

            }
            header = makeBlock(header, oldBlockSize, 1, padded, size);
        }
        // sf_blockprint(header);
        // sf_snapshot();
    }else{
        if(sbrk_Counter == 4){
            sf_errno = 12;
            return NULL;
        }
        sf_sbrkColleasce();
        goto Check;
    }

    return ((char *)header) + 8;
}

void sf_sbrkColleasce(){

    sf_header *header = sf_sbrk();
    header = makeBlock(header, PAGE_SZ, 0, 0, 0);
    sbrk_Counter++;

    sf_footer *prevFooter = (sf_footer*)((char *)header - 8);
    if(sbrk_Counter > 1 && prevFooter -> allocated == 0){

        sf_free_header *list_ptr = NULL;
        int coalescingBlockSize = (prevFooter -> block_size) << 4;
        sf_header * freeHeader = (sf_header *)((char *)prevFooter + 8 - (coalescingBlockSize));

        for(i = 0; i < FREE_LIST_COUNT; i++){
            list_ptr = seg_free_list[i].head;
            while(list_ptr != NULL){
                if(freeHeader == &(list_ptr->header)){
                    if(list_ptr -> prev == NULL){
                        if(list_ptr -> next == NULL){
                            seg_free_list[i].head = NULL;
                        }else{
                            seg_free_list[i].head = list_ptr -> next;
                        }
                    }else{
                        (list_ptr -> prev) -> next = list_ptr -> next;
                        (list_ptr -> next) -> prev = list_ptr -> prev;
                        list_ptr -> next = NULL;
                        list_ptr -> prev = NULL;
                    }
                    break;

                }else{
                    list_ptr = list_ptr -> next;
                }
            }
        }

        freeHeader = makeBlock(freeHeader, PAGE_SZ + (freeHeader -> block_size <<4), 0, 0, 0);
        header = freeHeader;
    }

    for(j = 0; j < FREE_LIST_COUNT; j++){
        if((header -> block_size) << 4 >= seg_free_list[j].min && (header -> block_size) << 4 <= seg_free_list[j].max){
            if(seg_free_list[j].head == NULL){
                seg_free_list[j].head = (sf_free_header*)header;
                seg_free_list[j].head -> next = NULL;
                seg_free_list[j].head -> prev = NULL;
            }else{
                ((sf_free_header*)header) -> next = seg_free_list[j].head;
                seg_free_list[j].head -> prev = ((sf_free_header*)header);
                seg_free_list[j].head = (sf_free_header*)header;
            }
            break;
        }
    }

}

void* makeBlock(sf_header *h, int block_size, int allocated, int padded, int size){

    sf_header *header = h;
    sf_footer *footer = (sf_footer*)((char *)header + block_size - 8);

    header -> padded = padded;
    header -> allocated = allocated;
    header -> two_zeroes = 0;
    header -> block_size = (block_size) >> 4;

    footer -> padded = header -> padded;
    footer -> allocated = header -> allocated;
    footer -> two_zeroes = 0;
    footer -> block_size = header -> block_size;
    footer -> requested_size = size;

    return header;
}

void sf_free(void *ptr) {
    if(ptr == NULL){
        abort();
    }
    sf_header *header = (sf_header*)((char*)ptr - 8);
    sf_footer *footer = (sf_footer*)((char*)header + ((header->block_size) << 4) -8);

    if(header < (sf_header*)get_heap_start() || header > (sf_header*)get_heap_end()){
        abort();
    }

    if(header -> allocated == 0){
        abort();
    }

    if(footer->requested_size + 16 != (footer ->block_size) << 4){
        if(header -> padded != 1){
            abort();
        }
    }

    if(header->allocated != footer->allocated && header->padded != footer->padded){
        abort();
    }

    sf_header *nextBlockHeader = (sf_header*)((char*)footer + 8);
    if(nextBlockHeader -> allocated == 0){//Coalesce with next block
        sf_free_header *list_ptr = NULL;
        for(i = 0; i < FREE_LIST_COUNT; i++){//removing next block from linked list
            list_ptr = seg_free_list[i].head;
            while(list_ptr != NULL){
                if(nextBlockHeader == &(list_ptr->header)){
                    if(list_ptr -> prev == NULL){
                        if(list_ptr -> next == NULL){
                            seg_free_list[i].head = NULL;
                        }else{
                            seg_free_list[i].head = list_ptr -> next;
                        }
                    }else{
                        (list_ptr -> prev) -> next = list_ptr -> next;
                        (list_ptr -> next) -> prev = list_ptr -> prev;
                        list_ptr -> next = NULL;
                        list_ptr -> prev = NULL;
                    }
                    break;

                }else{
                    list_ptr = list_ptr -> next;
                }
            }
        }

        header -> block_size = ((header -> block_size <<4) + (nextBlockHeader -> block_size <<4)) >> 4;
        header -> padded = 0;
        footer = (sf_footer*)((char*)nextBlockHeader + (nextBlockHeader -> block_size <<4) -8);
        footer -> block_size = header ->block_size;
    }
    //not coalescing
    header -> allocated = 0;
    footer -> allocated = 0;
    header -> padded = 0;
    footer -> padded = 0;
    footer -> requested_size = 0;

    for(j = 0; j < FREE_LIST_COUNT; j++){
        if((header -> block_size) << 4 >= seg_free_list[j].min && (header -> block_size) << 4 <= seg_free_list[j].max){
            if(seg_free_list[j].head == NULL){
                seg_free_list[j].head = (sf_free_header*)header;
                seg_free_list[j].head -> next = NULL;
                seg_free_list[j].head -> prev = NULL;
            }else{
                ((sf_free_header*)header) -> next = seg_free_list[j].head;
                seg_free_list[j].head -> prev = ((sf_free_header*)header);
                seg_free_list[j].head = (sf_free_header*)header;
            }
            break;
        }
    }
    // sf_snapshot();
    return;
}

void *sf_realloc(void *ptr, size_t size) {

    if(ptr == NULL){
        abort();
    }

    sf_header *header = (sf_header*)((char*)ptr - 8);
    sf_footer *footer = (sf_footer*)((char*)header + ((header->block_size) << 4) -8);

    if(header < (sf_header*)get_heap_start() || header > (sf_header*)get_heap_end()){
        abort();
    }

    if(header -> allocated == 0){
        abort();
    }

    if(footer->requested_size + 16 != (footer ->block_size) << 4){
        if(header -> padded != 1){
            abort();
        }
    }

    if(header->allocated != footer->allocated && header->padded != footer->padded){
        abort();
    }

    if(size < 0 || size > (4*PAGE_SZ)-16){
        abort();
    }

    int padded = 0;
    int paddedSize = size;
    if(paddedSize % 16 != 0){
        padded = 1;
        while(paddedSize % 16 != 0){paddedSize++;}
    }
    int oldBlockSize = header->block_size << 4;
    int newBlockSize = paddedSize + 16;

    if(size == 0){
        sf_free(ptr);
        return NULL;
    }else if(newBlockSize < oldBlockSize){//changing to smaller size
        int freeBlock_Size = oldBlockSize - newBlockSize;
        if(freeBlock_Size >= 32){
            header = makeBlock(header, newBlockSize, 1, padded, size);

            sf_header *freeHeader = (sf_header*)(((char *)header + paddedSize + 8)+8);
            freeHeader = makeBlock(freeHeader, freeBlock_Size, 1, 1,0);
            sf_free((char*)freeHeader + 8);

        }else{
            if(freeBlock_Size > 0){
                padded = 1;

            }
            header = makeBlock(header, oldBlockSize, 1, padded, size);
        }
        return  (char*)header+8;

    }else if(newBlockSize == oldBlockSize){
        header = makeBlock(header, newBlockSize, 1, padded, size);
        return (char*)header + 8;
    }else{//changing to larger size
        void *x = sf_malloc(size);
        if(x != NULL){
            sf_header *reAllocHeader = (sf_header*)((char*)x - 8);
            memcpy((char*)reAllocHeader + 8, (char*)header + 8, header->block_size << 4);
            sf_free(ptr);
            return (char*)reAllocHeader + 8;
        }
        return NULL;
    }

}

