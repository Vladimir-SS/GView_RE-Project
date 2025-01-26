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

    if (memcmp(header->version, "87a", 3) != 0 &&
        memcmp(header->version, "89a", 3) != 0) {
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
    win->AddPanel(Pointer<TabPage>(new GIF::Panels::Images(gif)), true);

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