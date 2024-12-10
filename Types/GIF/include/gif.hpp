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

#pragma pack(pop)

        class GIFFile : public TypeInterface, public View::ImageViewer::LoadImageInterface
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

            bool UpdateKeys(KeyboardControlsInterface* interface) override
            {
                return true;
            }

            const std::vector<std::string>& GetIssues() const
            {
                return issues;
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