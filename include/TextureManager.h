#pragma once

class TextureManager {
   public:
    TextureManager();
    ~TextureManager();

    int getWidth();
    int getHeight();
    int getNrChannels();
    void load(const char* path);
    void load(const char* path, int desiredChannels);
    unsigned char* getData();
    void unload();
    void flipVertically(bool flip);

   private:
    int width, height, nrChannels;
    unsigned char* data = {};
};