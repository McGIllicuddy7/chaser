#pragma once 
#include <vector>
#include <memory>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
struct ResourceRef{
    size_t idx;
    size_t generation;
    bool operator == (ResourceRef &other){
        return (idx == other.idx)&&(generation == other.generation);
    }
};
template <typename T >struct Resource{
    std::unique_ptr<T> m_value;
    size_t m_generation;
    void reset(){
        m_value.release();
    }
    Resource(){
        m_value.reset();
        m_generation = 0;
    }
    size_t emplace(T * value){
        reset();
        m_value.reset(value);
        return m_generation;
    }
    void remove(size_t generation){
        reset();
    }
    T* get(size_t generation){
        if(generation != m_generation){
            return 0;
        }
        return m_value.get();
    }
    T*get_unchecked(){
        return m_value.get();
    }
    size_t generation(){
        return m_generation;
    }
};
template <typename T> struct ResourceCache{
    std::vector<Resource<T>> cache; 
    size_t cache_size(){
        return cache.size();
    }
    ResourceRef emplace(T * value){
        int idx = -1;
        for(int i =0; i<cache_size(); i++){
            if( !cache[i].get_unchecked()){
                idx = i;
                break;
            }
        }
        if( idx == -1){
            idx = cache_size();
            cache.push_back(Resource<T>());
        }
        cache[idx].emplace(value);
        return ResourceRef{(size_t)idx, cache[idx].generation()};
    }
    bool remove(ResourceRef ref){
        if(ref.idx>=cache.size()){
            return false;
        }
        if (ref.generation != cache[ref.idx].generation()){
            return false;
        }
        cache[ref.idx].remove(ref.generation);
        return true;
    }
    T* get(ResourceRef ref){
        if(ref.idx>=cache.size()){
            return 0;
        }
        return cache[ref.idx].get(ref.generation);
    }    
    T* get_unchecked(size_t ref){
        if(ref>=cache.size()){
            return 0;
        }
        return cache[ref].get_unchecked();
    }
    int get_generation(size_t ref ){
        return cache[ref].generation();
    }
    void clear(){
        for(int i =0; i<cache_size(); i++){
            cache[i].reset();
        }
        cache.clear();
    }
    ~ResourceCache(){
        clear();
    } 
};
