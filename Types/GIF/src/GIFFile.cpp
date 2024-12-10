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

bool GIFFile::LoadImageToObject(Image& img, uint32 index)
{
    Buffer buf;
    auto bf = obj->GetData().GetEntireFile();
    if (bf.IsValid() == false) {
        buf = this->obj->GetData().CopyEntireFile();
        CHECK(buf.IsValid(), false, "Fail to copy Entire file");
        bf = (BufferView) buf;
    }
    CHECK(img.Create(bf), false, "");

    return true;
}