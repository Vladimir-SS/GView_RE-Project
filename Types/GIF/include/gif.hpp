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
            uint16 width;
            uint16 height;
            uint8 packedFields;
            uint8 backgroundColorIndex;
            uint8 pixelAspectRatio;
        };

        struct ImageDescriptor {
            uint16 leftPosition;
            uint16 topPosition;
            uint16 width;
            uint16 height;
            bool hasLocalColorTable;
            bool isInterlaced;
        };

#pragma pack(pop)

        class GIFFile : public TypeInterface, public View::ImageViewer::LoadImageInterface
        {
          public:
            GifFileType* gifFile; // Giflib's file type structure
            Header header{};
            LogicalScreenDescriptor logicalScreenDescriptor{};
            std::vector<ImageDescriptor> imageDescriptors;

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

            uint32 GetSelectionZonesCount() override
            {
                return static_cast<uint32>(imageDescriptors.size());
            }

            TypeInterface::SelectionZone GetSelectionZone(uint32 index) override
            {
                if (index >= imageDescriptors.size())
                    return TypeInterface::SelectionZone{ 0, 0 };

                const auto& descriptor = imageDescriptors[index];
                return TypeInterface::SelectionZone{ static_cast<uint32>(descriptor.leftPosition), static_cast<uint32>(descriptor.topPosition) };
            }

            bool UpdateKeys(KeyboardControlsInterface* interface) override
            {
                return true;
            }

            const LogicalScreenDescriptor& GetLogicalScreenDescriptor() const
            {
                return logicalScreenDescriptor;
            }

            const std::vector<ImageDescriptor>& GetImageDescriptors() const
            {
                return imageDescriptors;
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
        }; // namespace Panels
    } // namespace GIF
} // namespace Type
} // namespace GView