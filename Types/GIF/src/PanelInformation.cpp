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

    // File info
    general->AddItem("File");
    general->AddItem({ "Size", tempStr.Format("%s bytes", n.ToString(gif->obj->GetData().GetSize(), { NumericFormatFlags::None, 10, 3, ',' }).data()) });

    // GIF Header
    general->AddItem("Header");
    general->AddItem({ "Signature", std::string(gif->header.signature, 3) });
    general->AddItem({ "Version", std::string(gif->header.version, 3) });

    // // Logical Screen Descriptor
    // general->AddItem("Logical Screen Descriptor");
    // general->AddItem({ "Canvas Size", tempStr.Format("%u x %u", gif->screenDescriptor.canvasWidth, gif->screenDescriptor.canvasHeight) });

    // // Global Color Table Info
    // const bool hasGlobalColorTable = gif->screenDescriptor.packedFields & 0x80;
    // general->AddItem({ "Global Color Table", hasGlobalColorTable ? "Yes" : "No" });

    // if (hasGlobalColorTable) {
    //     const uint8 globalColorTableSize = 1 << ((gif->screenDescriptor.packedFields & 0x07) + 1);
    //     general->AddItem({ "Global Color Table Size", tempStr.Format("%u", globalColorTableSize) });
    // }

    // general->AddItem({ "Background Color Index", tempStr.Format("%u", gif->screenDescriptor.backgroundColorIndex) });

    // // Images
    // general->AddItem("Images");
    // const size_t imageCount = gif->imageDescriptors.size();
    // general->AddItem({ "Image Count", tempStr.Format("%zu", imageCount) });

    // for (size_t i = 0; i < imageCount; i++) {
    //     const auto& image = gif->imageDescriptors[i];
    //     general->AddItem({ tempStr.Format("Image %zu Position", i + 1), tempStr.Format("(%u, %u)", image.leftPosition, image.topPosition) });
    //     general->AddItem({ tempStr.Format("Image %zu Size", i + 1), tempStr.Format("%u x %u", image.width, image.height) });
    // }

    // // Graphics Control Extensions
    // if (!gif->controlExtensions.empty()) {
    //     general->AddItem("Graphics Control Extensions");
    //     for (size_t i = 0; i < gif->controlExtensions.size(); i++) {
    //         const auto& gce = gif->controlExtensions[i];
    //         general->AddItem({ tempStr.Format("Extension %zu Delay Time", i + 1), tempStr.Format("%u ms", gce.delayTime * 10) });
    //         general->AddItem({ tempStr.Format("Extension %zu Transparent Color Index", i + 1), tempStr.Format("%u", gce.transparentColorIndex) });
    //     }
    // }
}

void Panels::Information::UpdateIssues()
{
    issues->DeleteAllItems();

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