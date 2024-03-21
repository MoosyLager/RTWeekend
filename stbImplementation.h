#ifndef STB_IMPLEMENTATION_H
#define STB_IMPLEMENTATION_H

//! Disable strict warnings for this header from MSVC
#ifdef _MSC_VER
#pragma warning(push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb/stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <cstdlib>
#include <iostream>

class RTWImage
{
private:
    const int bytesPerPixel = 3;
    uint8_t *data;
    int imageWidth, imageHeight;
    int bytesPerScanline;

    static int Clamp(int x, int low, int high)
    {
        // Return the value clamped to the range [low,high)
        if ( x < low ) return low;
        if ( x < high ) return x;
        return high - 1;
    }

public:
    RTWImage() : data(nullptr) {}

    RTWImage(const char *imageFilename)
    {
        // Loads image data from the specified file. If the RTW_IMAGES environment variable is
        // defined, looks only in that directory for the image file. If the image was not found,
        // searches for the specified image file first from the current directory, then in the
        // images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
        // parent, on so on, for six levels up. If the image was not loaded successfully,
        // width() and height() will return 0.

        auto filename = std::string(imageFilename);
        auto imageDirectory = getenv("RTW_IMAGES");

        // hunt for the image file in some likely locations.
        if ( imageDirectory && Load(std::string(imageDirectory) + "/" + imageFilename) ) return;
        if ( Load(filename) ) return;
        if ( Load("Assets/" + filename) ) return;
        if ( Load("../Assets/" + filename) ) return;
        if ( Load("../../Assets/" + filename) ) return;
        if ( Load("../../../Assets/" + filename) ) return;
        if ( Load("../../../../Assets/" + filename) ) return;
        if ( Load("../../../../../Assets/" + filename) ) return;
        if ( Load("../../../../../../Assets/" + filename) ) return;

        std::cerr << "ERROR: Could not load image file '" << imageFilename << "'.\n";
    }

    ~RTWImage() { STBI_FREE(data); }

    bool Load(const std::string filename)
    {
        // Loads image data from the given filename. Returns true if load succeeded
        auto n = bytesPerPixel; // Dummy out parameter: original components per pixel
        data = stbi_load(filename.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);
        bytesPerScanline = imageWidth * bytesPerPixel;
        return data != nullptr;
    }

    int Width() const { return (data == nullptr) ? 0 : imageWidth; }

    int Height() const { return (data == nullptr) ? 0 : imageHeight; }

    const uint8_t *pixelData(int x, int y) const
    {
        // Return the address of the three bytes of the pixel aat x,y (or magenta if no data)
        static uint8_t magenta[] = {255, 0, 255};
        if ( data == nullptr ) return magenta;

        x = Clamp(x, 0, imageWidth);
        y = Clamp(y, 0, imageHeight);

        return data + y * bytesPerScanline + x * bytesPerPixel;
    }
};

//! Restore MSVC compiler warnings
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif