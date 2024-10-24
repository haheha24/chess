#pragma once

class StbImageLoader {
   public:
    StbImageLoader();
    ~StbImageLoader();

    int GetWidth();
    int GetHeight();
    int GetNrChannels();
    void SetData(const char* path);
    void SetData(const char* path, int desiredChannels);
    unsigned int GetData();

   private:
    int width, height, nrChannels;
    unsigned char* data = {};
};