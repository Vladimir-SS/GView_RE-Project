#include "jpg.hpp"

using namespace GView::Type::JPG;
using namespace AppCUI::Controls;

Panels::Information::Information(Reference<GView::Type::JPG::JPGFile> _jpg) : TabPage("&Information")
{
    jpg     = _jpg;
    general = Factory::ListView::Create(this, "x:0,y:0,w:100%,h:10", { "n:Field,w:12", "n:Value,w:100" }, ListViewFlags::None);

    issues = Factory::ListView::Create(this, "x:0,y:21,w:100%,h:10", { "n:Info,w:200" }, ListViewFlags::HideColumns);

    this->Update();
}

void Panels::Information::UpdateGeneralInformation()
{
    LocalString<256> tempStr;
    NumericFormatter n;

    general->DeleteAllItems();
    general->AddItem("File");
    // size
    general->AddItem({ "Size", tempStr.Format("%s bytes", n.ToString(jpg->obj->GetData().GetSize(), { NumericFormatFlags::None, 10, 3, ',' }).data()) });
    // Size
    const auto width  = Endian::BigToNative(jpg->sof0MarkerSegment.width);
    const auto height = Endian::BigToNative(jpg->sof0MarkerSegment.height);
    general->AddItem({ "Size", tempStr.Format("%u x %u", width, height) });

    // extra info
    general->AddItem({ "Density Units", tempStr.Format("%u", jpg->app0MarkerSegment.densityUnits) });

    const auto xDensity = Endian::BigToNative(jpg->app0MarkerSegment.xDensity);
    const auto yDensity = Endian::BigToNative(jpg->app0MarkerSegment.yDensity);
    general->AddItem({ "X Density", tempStr.Format("%u", xDensity) });
    general->AddItem({ "Y Density", tempStr.Format("%u", yDensity) });

    general->AddItem({ "X Thumbnail", tempStr.Format("%u", jpg->app0MarkerSegment.xThumbnail) });
    general->AddItem({ "Y Thumbnail", tempStr.Format("%u", jpg->app0MarkerSegment.yThumbnail) });
}

void Panels::Information::UpdateIssues()
{
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
