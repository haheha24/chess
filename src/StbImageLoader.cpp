#include "StbImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

StbImageLoader::StbImageLoader() {}
StbImageLoader::~StbImageLoader() { delete data; }

int StbImageLoader::GetWidth() { return width; }
int StbImageLoader::GetHeight() { return height; }
int StbImageLoader::GetNrChannels() { return nrChannels; }
unsigned int StbImageLoader::GetData() { return *data; }
void StbImageLoader::SetData(const char* path) { data = stbi_load(path, &width, &height, &nrChannels, 0); }
void StbImageLoader::SetData(const char* path, int desiredChannels) { data = stbi_load(path, &width, &height, &nrChannels, desiredChannels); }