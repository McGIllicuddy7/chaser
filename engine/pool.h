#pragma once
#include <stdlib.h>
template<typename T, size_t size> class Pool{
    T *pool;
    bool *used_list;
    int *free_list;
    int free_list_idx;
    public:
    Pool(){
        pool = (T *)calloc(size, sizeof(T));
        used_list = (bool *)calloc(size, sizeof(bool));
        free_list = (int *)calloc(size, sizeof(int));
        free_list_idx = size-1;
        for(int i =0; i<size; i++){
            free_list[i] = i;
        }
    }
    T * alloc(){
        if(free_list_idx<-1){
            return 0;
        }
        else{
            T * out = &pool[free_list[free_list_idx]];
            used_list[free_list[free_list_idx]] = true;
            free_list_idx--;
            return out;
        }
    }
    void mfree(T * to_free){
        if(to_free>=pool && to_free <&pool[size]){
            to_free->~T();
            used_list[to_free-pool] = false;
            free_list_idx++;
            free_list[free_list_idx] = to_free-pool;
        }
        else{
            assert(false);
        }
    }   
    ~Pool(){
        for(int i =0; i<size; i++){
            if(used_list[i]){
                this->mfree(&pool[i]);
            }
        }
        free(used_list);
        free(pool);
    }
};
#define POOL_ALLOC(T, pool,...)\
new (pool.alloc()) T (__VA_ARGS__)
