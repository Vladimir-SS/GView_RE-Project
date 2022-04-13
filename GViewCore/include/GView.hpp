#pragma once

#include <AppCUI/include/AppCUI.hpp>

using namespace AppCUI::Controls;
using namespace AppCUI::Utils;
using namespace AppCUI::Graphics;
using namespace AppCUI;

#ifdef CORE_EXPORTABLE
#    ifdef BUILD_FOR_WINDOWS
#        define CORE_EXPORT __declspec(dllexport)
#    else
#        define CORE_EXPORT
#    endif
#else
#    define CORE_EXPORT
#endif

#ifdef BUILD_FOR_WINDOWS
#    define PLUGIN_EXPORT __declspec(dllexport)
#else
#    define PLUGIN_EXPORT
#endif

namespace GView
{
struct CORE_EXPORT TypeInterface
{
    virtual std::string_view GetTypeName() = 0;
    virtual ~TypeInterface(){};

    template <typename T>
    Reference<T> To()
    {
        return static_cast<T*>(this);
    }
};
namespace Utils
{
    constexpr uint64 INVALID_OFFSET       = 0xFFFFFFFFFFFFFFFFULL;
    constexpr int INVALID_SELECTION_INDEX = -1;

    class CORE_EXPORT ErrorList
    {
        void* data;

      public:
        ErrorList();
        ~ErrorList();

        void Clear();
        bool AddError(const char* format, ...);
        bool AddWarning(const char* format, ...);
        bool Empty() const;

        uint32 GetErrorsCount() const;
        uint32 GetWarningsCount() const;

        std::string_view GetError(uint32 index) const;
        std::string_view GetWarning(uint32 index) const;

        void PopulateListView(AppCUI::Utils::Reference<AppCUI::Controls::ListView> listView) const;
    };
    class CORE_EXPORT FileCache
    {
        AppCUI::OS::DataObject* fileObj;
        uint64 fileSize, start, end, currentPos;
        uint8* cache;
        uint32 cacheSize;

        bool CopyObject(void* buffer, uint64 offset, uint32 requestedSize);

      public:
        FileCache();
        ~FileCache();

        bool Init(std::unique_ptr<AppCUI::OS::DataObject> file, uint32 cacheSize);
        BufferView Get(uint64 offset, uint32 requestedSize, bool failIfRequestedSizeCanNotBeRead);
        inline BufferView GetEntireFile()
        {
            return fileSize < 0xFFFFFFFF ? Get(0, (uint32) fileSize, true) : BufferView();
        }

        Buffer CopyToBuffer(uint64 offset, uint32 requestedSize, bool failIfRequestedSizeCanNotBeRead = true);
        inline Buffer CopyEntireFile(bool failIfRequestedSizeCanNotBeRead = true)
        {
            return CopyToBuffer(0, (uint32) fileSize, failIfRequestedSizeCanNotBeRead);
        }
        inline uint8 GetFromCache(uint64 offset, uint8 defaultValue = 0) const
        {
            if ((offset >= start) && (offset < end))
                return cache[offset - start];
            return defaultValue;
        }
        inline uint32 GetCacheSize() const
        {
            return cacheSize;
        }

        inline uint64 GetSize() const
        {
            return fileSize;
        }
        inline uint64 GetCurrentPos() const
        {
            return currentPos;
        }
        inline void SetCurrentPos(uint64 value)
        {
            currentPos = value;
        }

        template <typename T>
        inline bool Copy(uint64 offset, T& object)
        {
            return CopyObject(&object, offset, sizeof(T));
        }

        bool WriteTo(Reference<AppCUI::OS::DataObject> output, uint64 offset, uint32 size);
    };

    enum class DemangleKind : uint8
    {
        Auto,
        Microsoft,
        Itanium,
        Rust,
    };
    CORE_EXPORT bool Demangle(const char* input, String& output, DemangleKind format = DemangleKind::Auto);

} // namespace Utils

namespace Hashes
{
    class CORE_EXPORT Adler32
    {
      private:
        uint16 a;
        uint16 b;

        bool init;

      public:
        bool Init();
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint32& hash);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(a) + sizeof(b);
    };

    class CORE_EXPORT CRC16
    {
      private:
        uint32 value;
        bool init;

      public:
        bool Init();
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint16& hash);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(value);
    };

    enum class CRC32Type : uint32
    {
        JAMCRC   = 0xFFFFFFFF,
        JAMCRC_0 = 0x00000000
    };

    class CORE_EXPORT CRC32
    {
      private:
        uint32 value;
        CRC32Type type;

        bool init;

      public:
        bool Init(CRC32Type type);
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint32& hash);
        static std::string_view GetName(CRC32Type type);
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(value);
    };

    enum class CRC64Type : uint64
    {
        WE       = 0xFFFFFFFFFFFFFFFF,
        ECMA_182 = 0x0000000000000000
    };

    class CORE_EXPORT CRC64
    {
      private:
        uint64 value;
        CRC64Type type;

        bool init;

      private:
        bool Final();

      public:
        bool Init(CRC64Type type);
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint64& hash);
        static std::string_view GetName(CRC64Type type);
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(value);
    };

    class CORE_EXPORT MD2
    {
      private:
        uint8 m[16];
        uint8 x[48];
        uint8 c[16];
        uint32 size;

        bool init;

      private:
        bool Final();

      public:
        bool Init();
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint8 hash[16]);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(m) / sizeof(m[0]);
    };

    class CORE_EXPORT MD4
    {
      private:
        uint32 state[4];
        uint32 curlen;
        uint8 buf[64];
        uint64 length;

        bool init;

      private:
        bool Final();

      public:
        bool Init();
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint8 hash[16]);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(state);
    };

    class CORE_EXPORT MD5
    {
      private:
        uint32 state[4];
        uint32 curlen;
        uint8 buf[64];
        uint64 length;

        bool init;

      private:
        bool Final();

      public:
        bool Init();
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint8 hash[16]);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(state);
    };

    class CORE_EXPORT SHA1
    {
      private:
        uint64 length;
        uint32 state[5];
        uint32 curlen;
        uint8 buf[64];

        bool init;

      private:
        bool Final();

      public:
        bool Init();
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint8 hash[20]);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(state);
    };

    class CORE_EXPORT SHA256
    {
      private:
        uint64 length;
        uint32 state[8];
        uint32 curlen;
        uint8 buf[64];

        bool init;

      private:
        bool Final();

      public:
        bool Init();
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint8 hash[32]);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(state);
    };

    class CORE_EXPORT SHA512
    {
      protected:
        uint64 length;
        uint64 state[8];
        uint32 curlen;
        uint8 buf[128];

        bool init;

      private:
        bool Final();

      public:
        bool Init();
        bool Update(const unsigned char* input, uint32 length);
        bool Update(Buffer buffer);
        bool Final(uint8 hash[64]);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = sizeof(state);
    };

    class CORE_EXPORT SHA384 : public SHA512
    {
      public:
        bool Init();
        bool Final(uint8 hash[48]);
        static std::string_view GetName();
        const std::string GetHexValue();

      public:
        inline static const uint32 ResultBytesLength = SHA512::ResultBytesLength - 16;
    };

    enum class OpenSSLHashKind : uint8
    {
        Md4,
        Md5,
        Blake2s256,
        Blake2b512,
        Sha1,
        Sha224,
        Sha256,
        Sha384,
        Sha512,
        Sha512_224,
        Sha512_256,
        Sha3_224,
        Sha3_256,
        Sha3_384,
        Sha3_512,
        Shake128,
        Shake256
    };

    class CORE_EXPORT OpenSSLHash
    {
      public:
        OpenSSLHash(OpenSSLHashKind kind);
        ~OpenSSLHash();

        bool Update(const void* input, uint32 length);
        bool Final();
        std::string GetHexValue();
        const uint8* Get() const;
        uint32 GetSize() const;

      public:
        void* handle;
        uint8 hash[64];
        uint32 size;
    };
} // namespace Hashes

/*
* Object can be:
*   - a file
*   - a folder
*   - a process
*   - a memory buffer
*/
struct CORE_EXPORT Object
{
    Utils::FileCache cache;
    TypeInterface* type;
    AppCUI::Utils::UnicodeStringBuilder name;
    AppCUI::Utils::UnicodeStringBuilder filePath;

    Object() : type(nullptr)
    {
    }
};

namespace View
{
    typedef uint8 MethodID;
    typedef void* ExtractItem;
    struct CORE_EXPORT ViewControl : public AppCUI::Controls::UserControl, public AppCUI::Utils::PropertiesInterface
    {
      protected:
        const AppCUI::Application::Config& Cfg;

      public:
        virtual bool GoTo(uint64 offset)                                                                       = 0;
        virtual bool Select(uint64 offset, uint64 size)                                                        = 0;
        virtual std::string_view GetName()                                                                     = 0;
        virtual void PaintCursorInformation(AppCUI::Graphics::Renderer& renderer, uint32 width, uint32 height) = 0;
        virtual bool ExtractTo(Reference<AppCUI::OS::DataObject> output, ExtractItem item, uint64 size)             = 0;

        int WriteCursorInfo(AppCUI::Graphics::Renderer& renderer, int x, int y, int width, std::string_view key, std::string_view value);

        ViewControl() : UserControl("d:c"), Cfg(this->GetConfig())
        {
        }
    };
    namespace BufferViewer
    {
        struct BufferColor
        {
            uint64_t start;
            uint64_t end;
            ColorPair color;
            constexpr inline void Reset()
            {
                start = end = GView::Utils::INVALID_OFFSET;
            }
            constexpr inline bool IsValue() const
            {
                return start != GView::Utils::INVALID_OFFSET;
            }
            constexpr inline bool Empty() const
            {
                return start == GView::Utils::INVALID_OFFSET;
            }
        };
        struct CORE_EXPORT PositionToColorInterface
        {
            virtual bool GetColorForBuffer(uint64_t offset, BufferView buf, BufferColor& result) = 0;
        };
        struct CORE_EXPORT OffsetTranslateInterface
        {
            virtual uint64_t TranslateToFileOffset(uint64_t value, uint32 fromTranslationIndex) = 0;
            virtual uint64_t TranslateFromFileOffset(uint64_t value, uint32 toTranslationIndex) = 0;
        };

        struct CORE_EXPORT Settings
        {
            void* data;

            Settings();
            void AddZone(uint64 start, uint64 size, ColorPair col, std::string_view name);
            void AddBookmark(uint8 bookmarkID, uint64 fileOffset);
            void SetOffsetTranslationList(std::initializer_list<std::string_view> list, Reference<OffsetTranslateInterface> cbk);
            void SetPositionToColorCallback(Reference<PositionToColorInterface> cbk);
            void SetEntryPointOffset(uint64_t offset);
        };
    }; // namespace BufferViewer

    namespace ImageViewer
    {
        struct CORE_EXPORT LoadImageInterface
        {
            virtual bool LoadImageToObject(Image& img, uint32 index) = 0;
        };
        struct CORE_EXPORT Settings
        {
            void* data;

            Settings();
            void SetLoadImageCallback(Reference<LoadImageInterface> cbk);
            void AddImage(uint64 offset, uint64 size);
        };
    }; // namespace ImageViewer

    namespace ContainerViewer
    {
        struct CORE_EXPORT EnumerateInterface
        {
            virtual bool BeginIteration(std::u16string_view path, AppCUI::Controls::TreeViewItem parent) = 0;
            virtual bool PopulateItem(AppCUI::Controls::TreeViewItem item)                               = 0;
        };
        struct CORE_EXPORT ActionInterface
        {
            virtual void OnContaineItemAction(std::u16string_view path, AppCUI::Controls::TreeViewItem item) = 0;
        };
        struct CORE_EXPORT Settings
        {
            void* data;

            Settings();
            bool SetIcon(string_view imageStringFormat16x16);
            bool SetPathSeparator(char16 separator);
            bool AddProperty(string_view name, string_view value);
            void SetColumns(std::initializer_list<AppCUI::Controls::ColumnBuilder> columns);
            void SetEnumarateCallback(Reference<EnumerateInterface> callback);
            void SetActionCallback(Reference<ActionInterface> callback);
            void SetItems(std::initializer_list<std::initializer_list<ConstString>> items);
        };
    }; // namespace ContainerViewer

    namespace TextViewer
    {
        struct CORE_EXPORT LoadImageInterface
        {
            virtual bool LoadImageToObject(Image& img, uint32 index) = 0;
        };
        struct CORE_EXPORT Settings
        {
            void* data;

            Settings();
            void SetLoadImageCallback(Reference<LoadImageInterface> cbk);
            void AddImage(uint64 offset, uint64 size);
        };
    }; // namespace TextViewer

    namespace GridViewer
    {
        struct CORE_EXPORT Settings
        {
            void* data;

            Settings();

            void SetSeparator(char separator[2]);
        };
    }; // namespace GridViewer

    // namespace ImageViewer
    namespace DissasmViewer // StructureViewer
    {
        using TypeID = uint32;

        enum class DissamblyLanguage : uint32
        {
            Default,
            x86,
            x64,
            JavaByteCode,
            IL,
            Count
        };

        enum class VariableType : uint32
        {
            UInt8,
            UInt16,
            UInt32,
            UInt64,
            Int8,
            Int16,
            Int32,
            Int64,
            AsciiZ,
            Utf16Z,
            Utf32Z
        };

        struct CORE_EXPORT Settings
        {
            void* data;

            void SetDefaultDissasemblyLanguage(DissamblyLanguage lang);
            void ReserverZonesCapacity(uint32 reserved_size);
            void AddDissasemblyZone(uint64 start, uint64 size, DissamblyLanguage lang = DissamblyLanguage::Default);

            void AddMemmoryMapping(uint64 address, std::string_view name);

            /**
             * Add a new data type with its definition. Default data types: UInt8-64,Int8-64, float,double, asciiZ, Unicode16Z,Unicode32Z
             *
             *
             * @param[in] name Name of the new type
             * @param[in] definition Multiple statements in the form DataType variableName followed by semicolon. Example: name="Point",
             * definition="UInt32 x;UInt32 y;"
             * @returns The id of the new data type generated.
             */
            TypeID AddType(std::string_view name, std::string_view definition);

            // structure view
            void AddVariable(uint64 offset, std::string_view name, VariableType type);
            void AddArray(uint64 offset, std::string_view name, VariableType type, uint32 count);
            void AddBiDiminesionalArray(uint64 offset, std::string_view name, VariableType type, uint32 width, uint32 height);

            void AddVariable(uint64 offset, std::string_view name, TypeID type);
            void AddArray(uint64 offset, std::string_view name, TypeID type, uint32 count);
            void AddBiDiminesionalArray(uint64 offset, std::string_view name, TypeID type, uint32 width, uint32 height);

            /*
             * types: uin8-64,int8-64, float,double, char* (asciiZ), Unicode16Z,Unicode32Z
             * auto point_id = AddType("Point","struct Point{ uint32 x;uint32 y };");
             * AddVariable(0x1234, "MyPoint", point_id);
             */

            Settings();
        };
    }; // namespace DissasmViewer
    struct CORE_EXPORT WindowInterface
    {
        virtual Reference<Object> GetObject()                                                        = 0;
        virtual bool AddPanel(Pointer<TabPage> page, bool vertical)                                  = 0;
        virtual bool CreateViewer(const std::string_view& name, BufferViewer::Settings& settings)    = 0;
        virtual bool CreateViewer(const std::string_view& name, ImageViewer::Settings& settings)     = 0;
        virtual bool CreateViewer(const std::string_view& name, GridViewer::Settings& settings)      = 0;
        virtual bool CreateViewer(const std::string_view& name, DissasmViewer::Settings& settings)   = 0;
        virtual bool CreateViewer(const std::string_view& name, TextViewer::Settings& settings)      = 0;
        virtual bool CreateViewer(const std::string_view& name, ContainerViewer::Settings& settings) = 0;
        virtual Reference<ViewControl> GetCurrentView()                                              = 0;
    };
}; // namespace View
namespace App
{
    bool CORE_EXPORT Init();
    void CORE_EXPORT Run();
    bool CORE_EXPORT ResetConfiguration();
    void CORE_EXPORT OpenFile(const char*);
    void CORE_EXPORT OpenItem(View::ExtractItem item, Reference<View::ViewControl> view, uint64 size, string_view name);
    Reference<GView::Object> CORE_EXPORT GetObject(uint32 index);
    uint32 CORE_EXPORT GetObjectsCount();
}; // namespace App
}; // namespace GView
