#include "runtime.h"
ResourceRef Runtime::load_texture_by_name(std::string texture){
    if(m_texture_table.contains(texture)){
        return m_texture_table[texture];
    }
    std::string name = "resources/"+texture;
    Texture t = LoadTexture(name.c_str());
    if(!IsTextureReady(t)){
        return ResourceRef();
    }
    Texture * te = new Texture;
    *te = t;
    ResourceRef r = m_textures.emplace(te);
    m_texture_table.emplace(texture, r);
    return r;
}
Texture* Runtime::get_texture(ResourceRef ref){
    return m_textures.get(ref);
}
void Runtime::unload_texture(std::string name){
    if(!m_texture_table.contains(name)){
        return;
    }
    ResourceRef rf = m_texture_table[name];
    m_textures.remove(rf);
}
void cleanup(Texture * texture){
    UnloadTexture(*texture);
}