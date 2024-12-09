#include "gif.hpp"

using namespace GView::Type::GIF;

GIFFile::GIFFile()
{
    memset(&header, 0, sizeof(header));
    imageDescriptors.clear();

    auto& data = this->obj->GetData();

    uint64 offset = sizeof(Header) + sizeof(LogicalScreenDescriptor);
}

bool GIFFile::Update()
{
    // Provide the implementation for Update function
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