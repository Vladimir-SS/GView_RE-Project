#include "gif.hpp"

using namespace GView::Type::GIF;
using namespace AppCUI::Controls;

Panels::Information::Information(Reference<GView::Type::GIF::GIFFile> _gif) : TabPage("&Information")
{
    gif     = _gif;
    general = Factory::ListView::Create(this, "x:0,y:0,w:100%,h:10", { "n:Field,w:20", "n:Value,w:80" }, ListViewFlags::None);

    issues = Factory::ListView::Create(this, "x:0,y:21,w:100%,h:10", { "n:Info,w:200" }, ListViewFlags::HideColumns);

    this->Update();
}

void Panels::Information::UpdateGeneralInformation()
{
    LocalString<256> tempStr;
    NumericFormatter n;

    general->DeleteAllItems();

    for (const auto& issue : gif->GetIssues()) {
        general->AddItem(issue);
    } // to be removed later

    // File info
    general->AddItem({ "File", gif->obj->GetPath() });
    general->AddItem({ "Size", tempStr.Format("%s bytes", n.ToString(gif->obj->GetData().GetSize(), { NumericFormatFlags::None, 10, 3, ',' }).data()) });

    // GIF Header
    general->AddItem("Header");
    general->AddItem({ "Signature", std::string(gif->header.signature, 3) });
    general->AddItem({ "Version", std::string(gif->header.version, 3) });

    // GIF Information
    general->AddItem("GIF Information");

    general->AddItem({ "Canvas Size", tempStr.Format("%dx%d", gif->gifFile->SWidth, gif->gifFile->SHeight) });

    general->AddItem({ "Background Color Index", std::to_string(gif->gifFile->SBackGroundColor) });

    general->AddItem({ "Color Resolution", std::to_string(gif->gifFile->SColorResolution) });

    if (gif->gifFile->SColorMap) {
        general->AddItem({ "Global Color Map", std::to_string(gif->gifFile->SColorMap->ColorCount) + " colors" });
    } else {
        general->AddItem({ "Global Color Map", "None" });
    }

    general->AddItem({ "Image Count", std::to_string(gif->gifFile->ImageCount) });

    if (gif->gifFile->ImageCount > 0) {
        general->AddItem("Images");
        for (int i = 0; i < gif->gifFile->ImageCount; ++i) {
            const SavedImage& savedImage = gif->gifFile->SavedImages[i];

            general->AddItem(tempStr.Format("Image %d", i + 1));
            general->AddItem({ "Width", std::to_string(savedImage.ImageDesc.Width) });
            general->AddItem({ "Height", std::to_string(savedImage.ImageDesc.Height) });

            if (savedImage.ImageDesc.ColorMap) {
                general->AddItem({ "Local Color Map", std::to_string(savedImage.ImageDesc.ColorMap->ColorCount) + " colors" });
            } else {
                general->AddItem({ "Local Color Map", "None" });
            }
        }
    }
}

void Panels::Information::UpdateIssues()
{
    issues->DeleteAllItems();

    for (const auto& issue : gif->GetIssues()) {
        issues->AddItem(issue);
    }

    if (gif->header.signature[0] != 'G' || gif->header.signature[1] != 'I' || gif->header.signature[2] != 'F') {
        issues->AddItem("Invalid GIF signature.");
    }

    if (gif->header.version[0] != '8' || (gif->header.version[1] != '7' && gif->header.version[1] != '9') || gif->header.version[2] != 'a') {
        issues->AddItem("Unsupported GIF version.");
    }
}

void Panels::Information::RecomputePanelsPositions()
{
    int py = 0;
    int w  = this->GetWidth();
    int h  = this->GetHeight();

    if ((!general.IsValid()) || (!issues.IsValid())) {
        return;
    }

    issues->SetVisible(false);
    this->general->Resize(w, h);
}

void Panels::Information::Update()
{
    UpdateGeneralInformation();
    UpdateIssues();
    RecomputePanelsPositions();
}