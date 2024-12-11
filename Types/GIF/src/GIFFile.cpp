#include "gif.hpp"
#include <codecvt>

using namespace GView::Type::GIF;

GIFFile::GIFFile()
{
    memset(&header, 0, sizeof(header));

    // auto& data = this->obj->GetData();
}

std::string ConvertUtf16ToUtf8(std::u16string_view utf16_view)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    return converter.to_bytes(utf16_view.data(), utf16_view.data() + utf16_view.size());
} // might need to be changed, but it works for now

bool GIFFile::Update()
{
    int errorCode;
    std::u16string_view filePath = obj->GetPath();
    std::u16string sanitizedFilepath{ filePath };
    std::string utf8FilePath = ConvertUtf16ToUtf8(filePath);

    gifFile = DGifOpenFileName(utf8FilePath.c_str(), &errorCode);
    if (gifFile == nullptr) {
        issues.push_back(GifErrorString(errorCode));
        return false;
    }

    if (DGifSlurp(gifFile) == GIF_ERROR) // read the entire GIF file
    {
        issues.push_back(GifErrorString(gifFile->Error));
        return false;
    }

    const char* versionStamp = EGifGetGifVersion(gifFile);
    std::memcpy(header.signature, versionStamp, 3);
    std::memcpy(header.version, versionStamp + 3, 3);

    return true;
}

// LZW Decompression Algorithm based on GifLib documentation: https://giflib.sourceforge.net/whatsinagif/lzw_image_data.html
bool LoadGIFToImage(Image& img, const SavedImage& savedImage, const GifFileType* parentGif)
{
    const GifImageDesc& imageDesc = savedImage.ImageDesc;

    CHECK(imageDesc.Width > 0 && imageDesc.Height > 0, false, "Invalid GIF image dimensions!");

    ColorMapObject* colorMap = imageDesc.ColorMap;
    if (!colorMap && parentGif) {
        colorMap = parentGif->SColorMap;
    }

    CHECK(colorMap != nullptr && colorMap->ColorCount > 0, false, "Missing or invalid color map!");

    CHECK(img.Create(imageDesc.Width, imageDesc.Height), false, "Failed to create image of size %dx%d!", imageDesc.Width, imageDesc.Height);

    Pixel* pixelBuffer = img.GetPixelsBuffer();
    CHECK(pixelBuffer != nullptr, false, "Failed to retrieve pixel buffer!");

    GifByteType* rasterBits = savedImage.RasterBits;
    CHECK(rasterBits != nullptr, false, "Missing raster data in the GIF!");

    GifByteType code     = rasterBits[0];
    GifByteType prevCode = code;

    GifColorType& color = colorMap->Colors[code];
    pixelBuffer[0]      = Pixel(color.Red, color.Green, color.Blue);

    for (int i = 1; i < imageDesc.Width * imageDesc.Height; ++i) {
        code = rasterBits[i];

        if (code < colorMap->ColorCount) {
            GifColorType& color = colorMap->Colors[code];
            pixelBuffer[i]      = Pixel(color.Red, color.Green, color.Blue);

            GifByteType k = code;

            if (prevCode < colorMap->ColorCount) {
            }
        } else {
            GifByteType k       = prevCode;
            GifColorType& color = colorMap->Colors[k];
            pixelBuffer[i]      = Pixel(color.Red, color.Green, color.Blue);

            if (prevCode < colorMap->ColorCount) {
            }
        }

        prevCode = code;
    }

    return true;
}

bool GIFFile::LoadImageToObject(Image& img, uint32 index)
{
    CHECK(index < gifFile->ImageCount, false, "Index out of bounds!");

    LoadGIFToImage(img, gifFile->SavedImages[index], gifFile);

    return true;
}