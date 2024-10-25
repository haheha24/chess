#pragma once

class StbImageLoader {
   public:
    StbImageLoader();
    ~StbImageLoader();

    int GetWidth();
    int GetHeight();
    int GetNrChannels();
    void LoadImage(const char* path);
    void LoadImage(const char* path, int desiredChannels);
    unsigned int GetData();

   private:
    int width, height, nrChannels;
    unsigned char* data = {};
};