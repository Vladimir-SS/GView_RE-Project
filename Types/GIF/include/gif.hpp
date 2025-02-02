#pragma once

#include "GView.hpp"
#include <gif_lib.h> // Include giflib for GIF processing

namespace GView
{
namespace Type
{
    namespace GIF
    {
#pragma pack(push, 1)

        struct Header {
            char signature[3]; // "GIF"
            char version[3];   // "87a" or "89a"
        };

        struct LogicalScreenDescriptor {
            uint16_t canvasWidth;
            uint16_t canvasHeight;
            uint8_t packedFields;
            uint8_t backgroundColorIndex;
            uint8_t pixelAspectRatio;
        };

#pragma pack(pop)

        class GIFFile : public TypeInterface, public View::ImageViewer::LoadImageInterface, public View::ImageViewer::LoadGifImageInterface
        {
          public:
            GifFileType* gifFile; // Giflib's file type structure
            Header header{};
            std::vector<std::string> issues;

          public:
            GIFFile();
            virtual ~GIFFile()
            {
            }

            bool Update();

            std::string_view GetTypeName() override
            {
                return "GIF";
            }
            void RunCommand(std::string_view) override
            {
            }

            bool LoadImageToObject(Image& img, uint32 index) override;

            long long getGifImageDelayTimeInMilliseconds(const SavedImage& savedImage);
            bool LoadGifImageToObject(Image& img, long long& delayTime, uint32 index) override;

            bool UpdateKeys(KeyboardControlsInterface* interface) override
            {
                return true;
            }

            const std::vector<std::string>& GetIssues() const
            {
                return issues;
            }

          public:
            Reference<GView::Utils::SelectionZoneInterface> selectionZoneInterface;

            uint32 GetSelectionZonesCount() override
            {
                CHECK(selectionZoneInterface.IsValid(), 0, "");
                return selectionZoneInterface->GetSelectionZonesCount();
            }

            TypeInterface::SelectionZone GetSelectionZone(uint32 index) override
            {
                static auto d = TypeInterface::SelectionZone{ 0, 0 };
                CHECK(selectionZoneInterface.IsValid(), d, "");
                CHECK(index < selectionZoneInterface->GetSelectionZonesCount(), d, "");

                return selectionZoneInterface->GetSelectionZone(index);
            }
        };

        namespace Panels
        {
            class Information : public AppCUI::Controls::TabPage
            {
                Reference<GView::Type::GIF::GIFFile> gif;
                Reference<AppCUI::Controls::ListView> general;
                Reference<AppCUI::Controls::ListView> issues;

                void UpdateGeneralInformation();
                void UpdateIssues();
                void RecomputePanelsPositions();

              public:
                Information(Reference<GView::Type::GIF::GIFFile> gif);

                void Update();
                virtual void OnAfterResize(int newWidth, int newHeight) override
                {
                    RecomputePanelsPositions();
                }
            };

            class Images : public AppCUI::Controls::TabPage
            {
                Reference<GView::Type::GIF::GIFFile> gif;
                Reference<AppCUI::Controls::ListView> imagesList;
                Reference<GView::View::WindowInterface> win;

                void UpdateImages();
                void RecomputePanelsPositions();
                bool OnEvent(Reference<Control>, Event evnt, int controlID) override;

              public:
                Images(Reference<GView::Type::GIF::GIFFile> gif, Reference<GView::View::WindowInterface> win);
                virtual void OnAfterResize(int newWidth, int newHeight) override
                {
                    RecomputePanelsPositions();
                }
            };
        }; // namespace Panels
    } // namespace GIF
} // namespace Type
} // namespace GView