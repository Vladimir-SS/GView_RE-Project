#pragma once

#include "Internal.hpp"
#include <array>

#include <chrono>
#include <thread>

namespace GView
{
namespace View
{
    namespace ImageViewer
    {
        using namespace AppCUI;

        namespace Commands
        {
            using namespace AppCUI::Input;
            constexpr int32 CMD_ID_ZOOMIN     = 0xBF00;
            constexpr int32 CMD_ID_ZOOMOUT    = 0xBF01;
            constexpr int32 CMD_ID_NEXT_IMAGE = 0xBF02;
            constexpr int32 CMD_ID_PREV_IMAGE = 0xBF03;
            constexpr int32 CMD_ID_OPEN_PLAY  = 0xBF04;
            constexpr int32 CMD_ID_PLAY_IN_MAIN_WINDOW = 0xBF05;

            static KeyboardControl ZoomIn    = { Key::F3, "ZoomIn", "Zoom in the picture", CMD_ID_ZOOMIN };
            static KeyboardControl ZoomOut   = { Key::F2, "ZoomOut", "Zoom out the picture", CMD_ID_ZOOMOUT };
            static KeyboardControl NextImage = { Key::PageUp, "NextImage", "Go to the next image", CMD_ID_NEXT_IMAGE };
            static KeyboardControl PrevImage = { Key::PageDown, "PrevImage", "Go to the previous image", CMD_ID_PREV_IMAGE };
            static KeyboardControl OpenPlayWindow = { Key::O, "Open Play Window", "Open window to play gif animation", CMD_ID_OPEN_PLAY };
            static KeyboardControl PlayInMainWindow = { Key::Space, "Play", "Play in Main Window", CMD_ID_PLAY_IN_MAIN_WINDOW };

            static std::array ImageViewCommands = { &ZoomIn, &ZoomOut, &NextImage, &PrevImage, &OpenPlayWindow, &PlayInMainWindow };
        }

        struct ImageInfo
        {
            uint64 start, end;
        };
        struct SettingsData
        {
            String name;
            vector<ImageInfo> imgList;            
            Reference<LoadImageInterface> loadImageCallback;
            Reference<LoadGifImageInterface> loadGifImageCallback=nullptr;
            SettingsData();
        };

        struct Config
        {
            bool Loaded;

            static void Update(IniSection sect);
            void Initialize();
        };

        class Instance : public View::ViewControl
        {
          protected:
            Image img;
            Pointer<SettingsData> settings;
            Reference<AppCUI::Controls::ImageView> imgView;
            Reference<GView::Object> obj;
            uint32 currentImageIndex;
            ImageScaleMethod scale;

            static Config config;

            void LoadImage();
            void LoadImageWith(long long& delayTimeMilliseconds);
            void RedrawImage();
            ImageScaleMethod NextPreviousScale(bool next);
          public:
            inline static bool startedMainWindowAnimation = false;
            inline static bool gifPlayerWindowIsOpen      = false;

            Instance(Reference<GView::Object> obj, Settings* settings);

            virtual bool OnUpdateCommandBar(AppCUI::Application::CommandBar& commandBar) override;
            virtual bool OnKeyEvent(AppCUI::Input::Key keyCode, char16 characterCode) override;
            virtual bool OnEvent(Reference<Control>, Event eventType, int ID) override;

            virtual bool GoTo(uint64 offset) override;
            virtual bool Select(uint64 offset, uint64 size) override;
            virtual bool ShowGoToDialog() override;
            virtual bool ShowFindDialog() override;
            virtual bool ShowCopyDialog() override;

            virtual void PaintCursorInformation(AppCUI::Graphics::Renderer& renderer, uint32 width, uint32 height) override;


            // property interface
            bool GetPropertyValue(uint32 id, PropertyValue& value) override;
            bool SetPropertyValue(uint32 id, const PropertyValue& value, String& error) override;
            void SetCustomPropertyValue(uint32 propertyID) override;
            bool IsPropertyValueReadOnly(uint32 propertyID) override;
            const vector<Property> GetPropertiesList() override;
            bool UpdateKeys(KeyboardControlsInterface* interface) override;

            friend class VideoScreen;
        };

        class VideoScreen : public AppCUI::Controls::UserControl
        {
          private:
            bool continueAnimation;
            size_t currentFrame;
            Reference<Instance> instanceRef;

            uint32 initialImageIndex;
            long long delayTimeMilliseconds;
          public:
            VideoScreen(std::string_view layout, Instance* instancePtr);
            virtual void Paint(Graphics::Renderer& renderer) override;
            bool OnFrameUpdate() override;

            void SetContinueAnimation(const bool _continueAnimation)
            {
                continueAnimation = _continueAnimation;
            }
        };
        class GIF_Player_Window : public AppCUI::Controls::Window
        {
          private:
            Reference<VideoScreen> myVideo;

          public:
            GIF_Player_Window(Instance* instancePtr);
            virtual bool OnEvent(Reference<Control>, Event eventType, int controlID);
        };

        class GoToDialog : public Window
        {
            Reference<RadioBox> rbImageIndex;
            Reference<ComboBox> cbImageList;
            Reference<RadioBox> rbFileOffset;
            Reference<TextField> txFileOffset;
            uint64 maxSize;
            uint64 resultedPos;
            bool gotoImageIndex;

            void UpdateEnableStatus();
            void Validate();


        public:
            GoToDialog(Reference<SettingsData> settings, uint32 currentImageIndex, uint64 size);

            virtual bool OnEvent(Reference<Control>, Event eventType, int ID) override;
            bool ShouldGoToImage() const
            {
                return gotoImageIndex;
            }
            uint32 GetSelectedImageIndex() const
            {
                return static_cast<uint32>(resultedPos);                
            }
            uint64 GetFileOffset() const
            {
                return resultedPos;
            }
        };
    } // namespace ImageViewer
} // namespace View

}; // namespace GView