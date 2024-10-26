#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureManager::TextureManager() {}
TextureManager::~TextureManager() {}

int TextureManager::getWidth() { return width; }
int TextureManager::getHeight() { return height; }
int TextureManager::getNrChannels() { return nrChannels; }
unsigned char* TextureManager::getData() { return data; }
void TextureManager::load(const char* path) {
    data = stbi_load(path, &width, &height, &nrChannels, 0);
}
void TextureManager::load(const char* path, int desiredChannels) {
    data = stbi_load(path, &width, &height, &nrChannels, desiredChannels);
}
void TextureManager::unload() {
    stbi_image_free(data);
    data = nullptr;
}
void TextureManager::flipVertically(bool flip) {
    stbi_set_flip_vertically_on_load(flip);
}