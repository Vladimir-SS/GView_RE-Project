#include "gif.hpp"

using namespace GView::Type::GIF;
using namespace AppCUI::Controls;

Panels::Images::Images(Reference<GView::Type::GIF::GIFFile> _gif, Reference<GView::View::WindowInterface> _win) : TabPage("&Images")
{
    gif        = _gif;
    win        = _win;
    imagesList = Factory::ListView::Create(this, "x:0,y:0,w:100%,h:10", { "n:Field,w:20", "n:Value,w:80" }, ListViewFlags::None);

    this->UpdateImages();
}

void Panels::Images::UpdateImages()
{
    imagesList->DeleteAllItems();

    if (!gif->gifFile)
        return;

    LocalString<256> tempStr;
    for (int i = 0; i < gif->gifFile->ImageCount; ++i) {
        const SavedImage& savedImage = gif->gifFile->SavedImages[i];

        auto item = imagesList->AddItem(tempStr.Format("Image %d", i));
        item.SetData(i);
        imagesList->AddItem({ "Width", std::to_string(savedImage.ImageDesc.Width) });
        imagesList->AddItem({ "Height", std::to_string(savedImage.ImageDesc.Height) });

        if (savedImage.ImageDesc.ColorMap) {
            imagesList->AddItem({ "Local Color Map", std::to_string(savedImage.ImageDesc.ColorMap->ColorCount) + " colors" });
        } else {
            imagesList->AddItem({ "Local Color Map", "None" });
        }
    }
}

bool Panels::Images::OnEvent(Reference<Control> ctrl, Event evnt, int controlID)
{
    if (TabPage::OnEvent(ctrl, evnt, controlID))
        return true;

    if (evnt == Event::ListViewItemPressed) {
        auto selectedItem = imagesList->GetCurrentItem();
        auto imageIndex   = selectedItem.GetData(-1);
        win->GetCurrentView()->Select(imageIndex, 0);
        return true;
    }

    return false;
}

void Panels::Images::RecomputePanelsPositions()
{
    int py = 0;
    int w  = this->GetWidth();
    int h  = this->GetHeight();

    if ((!imagesList.IsValid())) {
        return;
    }

    this->imagesList->Resize(w, h);
}