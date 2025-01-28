#include "gif.hpp"

using namespace AppCUI;
using namespace AppCUI::Utils;
using namespace AppCUI::Application;
using namespace AppCUI::Controls;
using namespace GView::Utils;
using namespace GView::Type;
using namespace GView;
using namespace GView::View;

extern "C" {
PLUGIN_EXPORT bool Validate(const AppCUI::Utils::BufferView& buf, const std::string_view& extension)
{
    // Expect at least 6 bytes (header) + 7 bytes (logical screen descriptor)
    if (buf.GetLength() < sizeof(GIF::Header) + sizeof(GIF::LogicalScreenDescriptor)) {
        return false;
    }

    auto header = buf.GetObject<GIF::Header>();
    if (memcmp(header->signature, "GIF", 3) != 0) {
        return false;
    }

    if (memcmp(header->version, "87a", 3) != 0 && memcmp(header->version, "89a", 3) != 0) {
        return false;
    }

    auto lsd = buf.GetObject<GIF::LogicalScreenDescriptor>(sizeof(GIF::Header));
    if ((lsd->canvasWidth == 0) || (lsd->canvasHeight == 0)) {
        return false;
    }

    return true;
}
PLUGIN_EXPORT TypeInterface* CreateInstance()
{
    return new GIF::GIFFile;
}

void CreateImageView(Reference<GView::View::WindowInterface> win, Reference<GIF::GIFFile> gif)
{
    GView::View::ImageViewer::Settings settings;
    settings.SetLoadImageCallback(gif.ToBase<View::ImageViewer::LoadImageInterface>());
    settings.SetLoadGifImageCallback(gif.ToBase<View::ImageViewer::LoadGifImageInterface>());

    for (uint32_t i = 0; i < gif->gifFile->ImageCount; ++i) {
        const SavedImage& savedImage = gif->gifFile->SavedImages[i];

        uint32_t imageSize = savedImage.ImageDesc.Width * savedImage.ImageDesc.Height * sizeof(Pixel);

        settings.AddImage(i, imageSize);
    }

    win->CreateViewer(settings);
}

// void CreateContainerView(Reference<GView::View::WindowInterface> win, Reference<GIF::GIFFile> zip)
// {
//     ContainerViewer::Settings settings;

//     win->CreateViewer(settings);
// }

void CreateBufferView(Reference<GView::View::WindowInterface> win, Reference<GIF::GIFFile> gif)
{
    BufferViewer::Settings settings;

    AppCUI::uint64 offset = 0;

    ColorPair headerColor{ Color::DarkGreen, Color::DarkBlue };
    ColorPair logicalScreenDescColor{ Color::DarkGreen, Color::DarkBlue };
    ColorPair rasterBitsColor{ Color::DarkRed, Color::DarkBlue };
    ColorPair globalColorMapColor{ Color::Magenta, Color::DarkBlue };
    ColorPair imageDescColor{ Color::Silver, Color::Black };
    ColorPair imageColorMapColor{ Color::Pink, Color::Black };
    ColorPair extensionBlockColor{ Color::White, Color::Black };
    ColorPair trailerColor{ Color::Green, Color::Black };

    settings.AddZone(0, sizeof(GIF::Header), headerColor, "Header");
    offset += sizeof(GIF::Header);

    settings.AddZone(offset, sizeof(GIF::LogicalScreenDescriptor), logicalScreenDescColor, "Logical Screen Descriptor");
    offset += sizeof(GIF::LogicalScreenDescriptor);

    GifFileType* gifFile = gif->gifFile;

    if (gifFile->SColorMap != nullptr) {
        size_t globalColorMapSize = gifFile->SColorMap->ColorCount * sizeof(GifColorType);
        settings.AddZone(offset, globalColorMapSize, globalColorMapColor, "Global Color Map");

        offset += globalColorMapSize;
    }

    for (int i = 0; i < gifFile->ImageCount; ++i) {
        const SavedImage& savedImage = gifFile->SavedImages[i];

        settings.AddZone(offset, 10, imageDescColor, "Image Descriptor");
        offset += 10;

        if (savedImage.ImageDesc.ColorMap != nullptr) {
            size_t localColorMapSize = savedImage.ImageDesc.ColorMap->ColorCount * sizeof(GifColorType);
            settings.AddZone(offset, localColorMapSize, imageColorMapColor, "Local Color Map");

            offset += localColorMapSize;
        }

        size_t rasterBitsSize = savedImage.ImageDesc.Width * savedImage.ImageDesc.Height;
        settings.AddZone(offset, rasterBitsSize, rasterBitsColor, "Raster Bits");
        offset += rasterBitsSize;

        for (int j = 0; j < savedImage.ExtensionBlockCount; ++j) {
            const ExtensionBlock& extension = savedImage.ExtensionBlocks[j];
            size_t extensionBlockSize       = extension.ByteCount;
            settings.AddZone(offset, extensionBlockSize, extensionBlockColor, "Extension Block");

            offset += extensionBlockSize;
        }
    }

    settings.AddZone(offset, 1, trailerColor, "Trailer");

    gif->selectionZoneInterface = win->GetSelectionZoneInterfaceFromViewerCreation(settings);
}

PLUGIN_EXPORT bool PopulateWindow(Reference<GView::View::WindowInterface> win)
{
    auto gif = win->GetObject()->GetContentType<GIF::GIFFile>();
    gif->Update();

    // CreateContainerView(win, gif);
    CreateBufferView(win, gif);
    CreateImageView(win, gif);

    win->AddPanel(Pointer<TabPage>(new GIF::Panels::Information(gif)), true);
    win->AddPanel(Pointer<TabPage>(new GIF::Panels::Images(gif, win)), true);

    return true;
}
PLUGIN_EXPORT void UpdateSettings(IniSection sect)
{
    sect["Pattern"]     = "magic:47 49 46 38";
    sect["Priority"]    = 1;
    sect["Description"] = "Graphics Interchange Format image file (*.gif)";
}
}

int main()
{
    return 0;
}