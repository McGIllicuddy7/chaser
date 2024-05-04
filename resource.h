#pragma once 
#include <vector>
#include <memory>
#include <stdint.h>
#include <stdlib.h>
struct ResourceRef{
    size_t idx;
    size_t generation;
    bool operator == (ResourceRef &other){
        return (idx == other.idx)&&(generation == other.generation);
    }
};
template <typename T >struct Resource{
    T * m_value;
    size_t m_generation;
    void reset(){
        if(m_value){
            auto v = m_value;
            cleanup(v);
            delete v;
            m_generation ++;
        }
    }
    Resource(){
        m_value= 0;
        m_generation = 0;
    }
    size_t emplace(T * value){
        reset();
        m_value = value;
        return m_generation;
    }
    void remove(size_t generation){
        reset();
    }
    T* get(size_t generation){
        if(generation != m_generation){
            return 0;
        }
        return m_value;
    }
    T*get_unchecked(){
        return m_value;
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
};