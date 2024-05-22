#include "runtime.h"
ResourceRef Runtime::load_sound_by_name(std::string sound){
    if(m_sound_table.contains(sound)){
        return m_sound_table[sound];
    }
    std::string name = "../resources/"+sound;
    Sound t = LoadSound(name.c_str());
    if(!IsSoundReady(t)){
        return ResourceRef();
    }
    Sound * te = new Sound;
    *te = t;
    ResourceRef r = m_sounds.emplace(te);
    m_sound_table.emplace(sound, r);
    return r;
}
Sound * Runtime::get_sound(ResourceRef ref){
    return m_sounds.get(ref);
}
void Runtime::unload_sound(std::string sound){
    if(!m_sound_table.contains(sound)){
        return;
    }
    ResourceRef rf = m_sound_table[sound];
    m_sounds.remove(rf);
}
void cleanup(Sound * sound){
    //printf("unloaded sound");
    UnloadSound(*sound);
}