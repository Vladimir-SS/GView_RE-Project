#pragma once

#include "GView.hpp"

namespace GView::Type::MachOFB
{
// https://opensource.apple.com/source/xnu/xnu-344/EXTERNAL_HEADERS/mach-o/fat.h
// https://github.com/grumbach/nm_otool
constexpr uint32_t FAT_MAGIC    = 0xcafebabe; /* the fat magic number */
constexpr uint32_t FAT_CIGAM    = 0xbebafeca; /* NXSwapLong(FAT_MAGIC) */
constexpr uint32_t FAT_MAGIC_64 = 0xcafebabf; /* the 64-bit fat magic number */
constexpr uint32_t FAT_CIGAM_64 = 0xbfbafeca; /* NXSwapLong(FAT_MAGIC_64) */

struct fat_header
{
    unsigned long magic;     /* FAT_MAGIC or FAT_MAGIC_64 */
    unsigned long nfat_arch; /* number of structs that follow */
};

// https://opensource.apple.com/source/xnu/xnu-4570.41.2/osfmk/mach/machine.h.auto.html

enum class CPU_STATE : uint32_t
{
    USER   = 0,
    SYSTEM = 1,
    IDLE   = 2,
    NICE   = 3,
    MAX    = 4
};

enum class CPU_ARCH : uint32_t
{
    MASK  = 0xff000000, /* mask for architecture bits */
    ABI64 = 0x01000000  /* 64 bit ABI */
};

// https://opensource.apple.com/source/xnu/xnu-4570.41.2/osfmk/mach/machine.h.auto.html
// https://github.com/opensource-apple/cctools/blob/fdb4825f303fd5c0751be524babd32958181b3ed/include/mach/machine.h

enum class CPU_TYPE : uint32_t
{
    ANY  = static_cast<uint32_t>(-1),
    VAX  = 1,
    ROMP = 2,
    /* skip	((cpu_type_t) 3)	*/
    NS32032     = 4,
    NS32332     = 5,
    MC680x0     = 6,
    X86         = 7,
    I386        = X86, /* compatibility */
    X86_64      = (X86 | static_cast<uint32_t>(CPU_ARCH::ABI64)),
    MIPS        =  8,
    NS32532     =  9,
    MC98000     = 10,
    HPPA        = 11,
    ARM         = 12,
    ARM64       = (ARM | static_cast<uint32_t>(CPU_ARCH::ABI64)),
    MC88000     = 13,
    SPARC       = 14,
    I860        = 15, // big-endian
    I860_LITTLE = 16, // little-endian
    RS6000      = 17,
    POWERPC     = 18,
    POWERPC64   = (POWERPC | static_cast<uint32_t>(CPU_ARCH::ABI64)),
    VEO         = 255
};

#define GET_VARIABLE_NAME(x) (#x)
#define GET_PAIR(x) {x, (GET_VARIABLE_NAME(x))}

static const std::map<CPU_TYPE, std::string_view> CpuTypeNames
{
    GET_PAIR(CPU_TYPE::ANY),
    GET_PAIR(CPU_TYPE::VAX),
    GET_PAIR(CPU_TYPE::ROMP),
    // skipped
    GET_PAIR(CPU_TYPE::NS32032),
    GET_PAIR(CPU_TYPE::NS32332),
    GET_PAIR(CPU_TYPE::MC680x0),
    GET_PAIR(CPU_TYPE::X86),
    GET_PAIR(CPU_TYPE::I386),
    GET_PAIR(CPU_TYPE::X86_64),
    GET_PAIR(CPU_TYPE::MIPS),
    GET_PAIR(CPU_TYPE::NS32532),
    GET_PAIR(CPU_TYPE::MC98000),
    GET_PAIR(CPU_TYPE::HPPA),
    GET_PAIR(CPU_TYPE::ARM),
    GET_PAIR(CPU_TYPE::ARM64),
    GET_PAIR(CPU_TYPE::MC88000),
    GET_PAIR(CPU_TYPE::SPARC),
    GET_PAIR(CPU_TYPE::I860),
    GET_PAIR(CPU_TYPE::I860_LITTLE),
    GET_PAIR(CPU_TYPE::RS6000),
    GET_PAIR(CPU_TYPE::POWERPC),
    GET_PAIR(CPU_TYPE::POWERPC64),
    GET_PAIR(CPU_TYPE::VEO)    
};

enum class CPU_SUBTYPE_COMPATIBILITY : uint32_t
{
    MASK  = 0xff000000, /* mask for feature flags */
    LIB64 = 0x80000000  /* 64 bit libraries */
};

enum class CPU_SUBTYPE : uint32_t
{
    MULTIPLE      = static_cast<uint32_t>(-1),
    LITTLE_ENDIAN = 0,
    BIG_ENDIAN    = 1,
};

enum class CPU_SUBTYPE_VAX : uint32_t
{
    ALL     = 0,
    VAX780  = 1,
    VAX785  = 2,
    VAX750  = 3,
    VAX730  = 4,
    UVAXI   = 5,
    UVAXII  = 6,
    VAX8200 = 7,
    VAX8500 = 8,
    VAX8600 = 9,
    VAX8650 = 10,
    VAX8800 = 11,
    UVAXIII = 12,
};

enum class CPU_SUBTYPE_RT
{
    ALL  = 0,
    PC   = 1,
    APC  = 2,
    _135 = 3
};

enum class CPU_SUBTYPE_MC680 : uint32_t
{
    x0_ALL   = 1,
    _30      = 1, /* compat */
    _40      = 2,
    _30_ONLY = 3,
};

#define CPU_SUBTYPE_INTEL(f, m) ((f) + ((m) << 4))

enum class CPU_SUBTYPE_INTEL : uint32_t
{
    I386_ALL       = CPU_SUBTYPE_INTEL(3, 0),
    _386           = CPU_SUBTYPE_INTEL(3, 0),
    _486           = CPU_SUBTYPE_INTEL(4, 0),
    _486SX         = CPU_SUBTYPE_INTEL(4, 8), // 8 << 4 = 128
    _586           = CPU_SUBTYPE_INTEL(5, 0),
    PENT           = CPU_SUBTYPE_INTEL(5, 0),
    PENTPRO        = CPU_SUBTYPE_INTEL(6, 1),
    PENTII_M3      = CPU_SUBTYPE_INTEL(6, 3),
    PENTII_M5      = CPU_SUBTYPE_INTEL(6, 5),
    CELERON        = CPU_SUBTYPE_INTEL(7, 6),
    CELERON_MOBILE = CPU_SUBTYPE_INTEL(7, 7),
    PENTIUM_3      = CPU_SUBTYPE_INTEL(8, 0),
    PENTIUM_3_M    = CPU_SUBTYPE_INTEL(8, 1),
    PENTIUM_3_XEON = CPU_SUBTYPE_INTEL(8, 2),
    PENTIUM_M      = CPU_SUBTYPE_INTEL(9, 0),
    PENTIUM_4      = CPU_SUBTYPE_INTEL(10, 0),
    PENTIUM_4_M    = CPU_SUBTYPE_INTEL(10, 1),
    ITANIUM        = CPU_SUBTYPE_INTEL(11, 0),
    ITANIUM_2      = CPU_SUBTYPE_INTEL(11, 1),
    XEON           = CPU_SUBTYPE_INTEL(12, 0),
    XEON_MP        = CPU_SUBTYPE_INTEL(12, 1)
};

#define CPU_SUBTYPE_INTEL_FAMILY(x) ((x) &15)
constexpr uint32_t CPU_SUBTYPE_INTEL_FAMILY_MAX = 15;

#define CPU_SUBTYPE_INTEL_MODEL(x) ((x) >> 4)
constexpr uint32_t CPU_SUBTYPE_INTEL_MODEL_ALL = 0;

enum class CPU_SUBTYPE_X86 : uint32_t
{
    ALL     = 3,
    _64_ALL = 3,
    ARCH1   = 4,
    _64_H   = 8, /* Haswell feature subset */
};

enum class CPU_THREADTYPE : uint32_t
{
    NONE      = 0,
    INTEL_HTT = 1
};

enum class CPU_SUBTYPE_MIPS : uint32_t
{
    ALL    = 0,
    R2300  = 1,
    R2600  = 2,
    R2800  = 3,
    R2000a = 4, /* pmax */
    R2000  = 5,
    R3000a = 6, /* 3max */
    R3000  = 7,
};

enum class CPU_SUBTYPE_MC98000 : uint32_t
{
    MC98000_ALL = 0,
    MC98601     = 1
};

enum class CPU_SUBTYPE_HPPA : uint32_t
{
    ALL     = 0,
    _7100   = 0, /* compat */
    _7100LC = 1
};

enum class CPU_SUBTYPE_MC88000 : uint32_t
{
    ALL     = 0,
    MC88100 = 1,
    MC88110 = 2
};

enum class CPU_SUBTYPE_SPARC : uint32_t
{
    ALL = 0
};

enum class CPU_SUBTYPE_I860 : uint32_t
{
    ALL  = 0,
    _860 = 1
};

enum class CPU_SUBTYPE_PowerPC : uint32_t
{
    ALL    = 0,
    _601   = 1,
    _602   = 2,
    _603   = 3,
    _603e  = 4,
    _603ev = 5,
    _604   = 6,
    _604e  = 7,
    _620   = 8,
    _750   = 9,
    _7400  = 10,
    _7450  = 11,
    _970   = 100
};

enum class CPU_SUBTYPE_ARM : uint32_t
{
    ALL    = 0,
    V4T    = 5,
    V6     = 6,
    V5TEJ  = 7,
    XSCALE = 8,
    V7     = 9,
    V7F    = 10, /* Cortex A9 */
    V7S    = 11, /* Swift */
    V7K    = 12,
    V8     = 13,
    V6M    = 14, /* Not meant to be run under xnu */
    V7M    = 15, /* Not meant to be run under xnu */
    V7EM   = 16  /* Not meant to be run under xnu */
};

enum class CPU_SUBTYPE_ARM64 : uint32_t
{
    ALL = 0,
    V8  = 1
};

/* https://github.com/opensource-apple/cctools/blob/fdb4825f303fd5c0751be524babd32958181b3ed/include/mach/machine.h
 * VEO subtypes
 * Note: the CPU_SUBTYPE_VEO_ALL will likely change over time to be defined as
 * one of the specific subtypes.
 */
enum class CPU_SUBTYPE_VEO : uint32_t
{
    _1  = 1,
    _2  = 2,
    _3  = 3,
    _4  = 4,
    ALL = 1
};

enum class CPU_Family : uint32_t
{
    UNKNOWN           = 0,
    POWERPC_G3        = 0xcee41549,
    POWERPC_G4        = 0x77c184ae,
    POWERPC_G5        = 0xed76d8aa,
    INTEL_6_13        = 0xaa33392b,
    INTEL_PENRYN      = 0x78ea4fbc,
    INTEL_NEHALEM     = 0x6b5a4cd2,
    INTEL_WESTMERE    = 0x573b5eec,
    INTEL_SANDYBRIDGE = 0x5490b78c,
    INTEL_IVYBRIDGE   = 0x1f65e835,
    INTEL_HASWELL     = 0x10b282dc,
    INTEL_BROADWELL   = 0x582ed09c,
    INTEL_SKYLAKE     = 0x37fc219f,
    INTEL_KABYLAKE    = 0x0f817246,
    ARM_9             = 0xe73283ae,
    ARM_11            = 0x8ff620d8,
    ARM_XSCALE        = 0x53b005f5,
    ARM_12            = 0xbd1b0ae9,
    ARM_13            = 0x0cc90e64,
    ARM_14            = 0x96077ef1,
    ARM_15            = 0xa8511bca,
    ARM_SWIFT         = 0x1e2d6381,
    ARM_CYCLONE       = 0x37a09642,
    ARM_TYPHOON       = 0x2c91a47e,
    ARM_TWISTER       = 0x92fb37c8,
    ARM_HURRICANE     = 0x67ceee93,
    INTEL_6_23        = INTEL_PENRYN,  /* synonim is deprecated */
    INTEL_6_26        = INTEL_NEHALEM, /* synonim is deprecated */
};

// https://opensource.apple.com/source/xnu/xnu-344/EXTERNAL_HEADERS/mach-o/fat.h
// https://olszanowski.blog/posts/macho-reader-parsing-headers/

struct fat_arch
{
    CPU_TYPE cputype;       /* cpu specifier (int) */
    CPU_SUBTYPE cpusubtype; /* machine specifier (int) */
    uint32_t offset;        /* file offset to this object file */
    uint32_t size;          /* size of this object file */
    uint32_t align;         /* alignment as a power of 2 */
};

// https://opensource.apple.com/source/cctools/cctools-895/include/mach-o/fat.h.auto.html
struct fat_arch64
{
    CPU_TYPE cputype;       /* cpu specifier (int) */
    CPU_SUBTYPE cpusubtype; /* machine specifier (int) */
    uint64_t offset;        /* file offset to this object file */
    uint64_t size;          /* size of this object file */
    uint32_t align;         /* alignment as a power of 2 */
    uint64_t reserved;      /* reserved */
};

/*
 * https://unix.superglobalmegacorp.com/xnu/newsrc/EXTERNAL_HEADERS/architecture/byte_order.h.html
 * Identify the byte order of the current host.
 */

enum class ByteOrder
{
    UnknownByteOrder,
    LittleEndian,
    BigEndian
};

struct ArchInfo
{
    std::string name;
    CPU_TYPE cputype;
    uint32_t cpusubtype;
    ByteOrder byteorder;
    std::string description;
};

static const ArchInfo ArchInfoTable[] =
{
    { "hppa",    CPU_TYPE::HPPA,      static_cast<uint32_t>(CPU_SUBTYPE_HPPA::ALL),     ByteOrder::BigEndian,        "HP-PA"                    },
    { "i386",    CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_X86::ALL),      ByteOrder::LittleEndian,     "Intel 80x86"              },
    { "x86_64",  CPU_TYPE::X86_64,    static_cast<uint32_t>(CPU_SUBTYPE_X86::_64_ALL),  ByteOrder::LittleEndian,     "Intel x86-64"             },
    { "x86_64h", CPU_TYPE::X86_64,    static_cast<uint32_t>(CPU_SUBTYPE_X86::_64_H),    ByteOrder::LittleEndian,     "Intel x86-64h Haswell"    },
    { "i860",    CPU_TYPE::I860,      static_cast<uint32_t>(CPU_SUBTYPE_I860::ALL),     ByteOrder::BigEndian,        "Intel 860"                },
    { "m68k",    CPU_TYPE::MC680x0,   static_cast<uint32_t>(CPU_SUBTYPE_MC680::x0_ALL), ByteOrder::BigEndian,        "Motorola 68K"             },
    { "m88k",    CPU_TYPE::MC88000,   static_cast<uint32_t>(CPU_SUBTYPE_MC88000::ALL),  ByteOrder::BigEndian,        "Motorola 88K"             },
    { "ppc",     CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::ALL),  ByteOrder::BigEndian,        "PowerPC"                  },
    { "ppc64",   CPU_TYPE::POWERPC64, static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::ALL),  ByteOrder::BigEndian,        "PowerPC 64-bit"           },
    { "sparc",   CPU_TYPE::SPARC,     static_cast<uint32_t>(CPU_SUBTYPE_SPARC::ALL),    ByteOrder::BigEndian,        "SPARC"                    },
    { "arm",     CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::ALL),      ByteOrder::LittleEndian,     "ARM"                      },
    { "arm64",   CPU_TYPE::ARM64,     static_cast<uint32_t>(CPU_SUBTYPE_ARM64::ALL),    ByteOrder::LittleEndian,     "ARM64"                    },
    { "any",     CPU_TYPE::ANY,       static_cast<uint32_t>(CPU_SUBTYPE::MULTIPLE),     ByteOrder::UnknownByteOrder, "Architecture Independent" },
    { "veo",     CPU_TYPE::VEO,       static_cast<uint32_t>(CPU_SUBTYPE_VEO::ALL),      ByteOrder::BigEndian,        "veo"                      },
    /* specific architecture implementations */
    { "hppa7100LC", CPU_TYPE::HPPA,      static_cast<uint32_t>(CPU_SUBTYPE_HPPA::_7100LC),    ByteOrder::BigEndian,    "HP-PA 7100LC"             },
    { "m68030",     CPU_TYPE::MC680x0,   static_cast<uint32_t>(CPU_SUBTYPE_MC680::_30_ONLY),  ByteOrder::BigEndian,    "Motorola 68030"           },
    { "m68040",     CPU_TYPE::MC680x0,   static_cast<uint32_t>(CPU_SUBTYPE_MC680::_40),       ByteOrder::BigEndian,    "Motorola 68040"           },
    { "i486",       CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::_486),      ByteOrder::LittleEndian, "Intel 80486"              },
    { "i486SX",     CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::_486SX),    ByteOrder::LittleEndian, "Intel 80486SX"            },
    { "pentium",    CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::PENT),      ByteOrder::LittleEndian, "Intel Pentium"            }, /* same as 586 */
    { "i586",       CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::_586),      ByteOrder::LittleEndian, "Intel 80586"              },
    { "pentpro",    CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::PENTPRO),   ByteOrder::LittleEndian, "Intel Pentium Pro"        }, /* same as 686 */
    { "i686",       CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::PENTPRO),   ByteOrder::LittleEndian, "Intel Pentium Pro"        },
    { "pentIIm3",   CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::PENTII_M3), ByteOrder::LittleEndian, "Intel Pentium II Model 3" },
    { "pentIIm5",   CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::PENTII_M5), ByteOrder::LittleEndian, "Intel Pentium II Model 5" },
    { "pentium4",   CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_INTEL::PENTIUM_4), ByteOrder::LittleEndian, "Intel Pentium 4"          },
    { "x86_64h",    CPU_TYPE::I386,      static_cast<uint32_t>(CPU_SUBTYPE_X86::_64_H),       ByteOrder::LittleEndian, "Intel x86-64h Haswell"    },
    { "ppc601",     CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_601),    ByteOrder::BigEndian,    "PowerPC 601"              },
    { "ppc603",     CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_603),    ByteOrder::BigEndian,    "PowerPC 603"              },
    { "ppc603e",    CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_603e),   ByteOrder::BigEndian,    "PowerPC 603e"             },
    { "ppc603ev",   CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_603ev),  ByteOrder::BigEndian,    "PowerPC 603ev"            },
    { "ppc604",     CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_604),    ByteOrder::BigEndian,    "PowerPC 604"              },
    { "ppc604e",    CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_604e),   ByteOrder::BigEndian,    "PowerPC 604e"             },
    { "ppc750",     CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_750),    ByteOrder::BigEndian,    "PowerPC 750"              },
    { "ppc7400",    CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_7400),   ByteOrder::BigEndian,    "PowerPC 7400"             },
    { "ppc7450",    CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_7450),   ByteOrder::BigEndian,    "PowerPC 7450"             },
    { "ppc970",     CPU_TYPE::POWERPC,   static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_970),    ByteOrder::BigEndian,    "PowerPC 970"              },
    { "ppc970-64",  CPU_TYPE::POWERPC64, static_cast<uint32_t>(CPU_SUBTYPE_PowerPC::_970),    ByteOrder::BigEndian,    "PowerPC 970 64-bit"       },
    { "armv4t",     CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V4T),         ByteOrder::LittleEndian, "arm v4t"                  },
    { "armv5",      CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V5TEJ),       ByteOrder::LittleEndian, "arm v5"                   },
    { "xscale",     CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::XSCALE),      ByteOrder::LittleEndian, "arm xscale"               },
    { "armv6",      CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V6),          ByteOrder::LittleEndian, "arm v6"                   },
    { "armv6m",     CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V6M),         ByteOrder::LittleEndian, "arm v6m"                  },
    { "armv7",      CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V7),          ByteOrder::LittleEndian, "arm v7"                   },
    { "armv7f",     CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V7F),         ByteOrder::LittleEndian, "arm v7f"                  },
    { "armv7s",     CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V7S),         ByteOrder::LittleEndian, "arm v7s"                  },
    { "armv7k",     CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V7K),         ByteOrder::LittleEndian, "arm v7k"                  },
    { "armv7m",     CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V7M),         ByteOrder::LittleEndian, "arm v7m"                  },
    { "armv7em",    CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V7EM),        ByteOrder::LittleEndian, "arm v7em"                 },
    { "armv8",      CPU_TYPE::ARM,       static_cast<uint32_t>(CPU_SUBTYPE_ARM::V8),          ByteOrder::LittleEndian, "arm v8"                   },
    { "arm64",      CPU_TYPE::ARM64,     static_cast<uint32_t>(CPU_SUBTYPE_ARM64::V8),        ByteOrder::LittleEndian, "arm64 v8"                 },
    { "little",     CPU_TYPE::ANY,       static_cast<uint32_t>(CPU_SUBTYPE::LITTLE_ENDIAN),   ByteOrder::LittleEndian, "Little Endian"            },
    { "big",        CPU_TYPE::ANY,       static_cast<uint32_t>(CPU_SUBTYPE::BIG_ENDIAN),      ByteOrder::BigEndian,    "Big Endian"               },
    { "veo1",       CPU_TYPE::VEO,       static_cast<uint32_t>(CPU_SUBTYPE_VEO::_1),          ByteOrder::BigEndian,    "veo 1"                    },
    { "veo2",       CPU_TYPE::VEO,       static_cast<uint32_t>(CPU_SUBTYPE_VEO::_2),          ByteOrder::BigEndian,    "veo 2"                    }
};

static const ArchInfo GetArchInfoFromCPUTypeAndSubtype(CPU_TYPE cputype, uint32_t cpusubtype)
{
    for (const auto& arch : ArchInfoTable)
    {
        if (arch.cputype == cputype && (cpusubtype == static_cast<uint32_t>(CPU_SUBTYPE::MULTIPLE) ||
                                        ((arch.cpusubtype & ~static_cast<uint32>(CPU_SUBTYPE_COMPATIBILITY::MASK)) ==
                                         (cpusubtype & ~static_cast<uint32>(CPU_SUBTYPE_COMPATIBILITY::MASK)))))
        {
            return arch;
        }
    }

    ArchInfo ai;
    for (const auto& arch : ArchInfoTable)
    {
        if (arch.cputype == cputype)
        {
            ai = arch;
            break;
        }
    }

    ai.cpusubtype = cpusubtype;

    if (cputype == CPU_TYPE::I386)
    {
        const auto family = std::to_string(CPU_SUBTYPE_INTEL_FAMILY(cpusubtype & ~(static_cast<uint32>(CPU_SUBTYPE_COMPATIBILITY::MASK))));
        const auto model  = std::to_string(CPU_SUBTYPE_INTEL_MODEL(cpusubtype & ~(static_cast<uint32>(CPU_SUBTYPE_COMPATIBILITY::MASK))));

        ai.description = "Intel family " + family + " model " + model;
    }
    else if (cputype == CPU_TYPE::POWERPC)
    {
        ai.description = "PowerPC cpusubtype " + std::to_string(cpusubtype);
    }

    return ai;
}

namespace Panels
{
    enum class IDs : uint8_t
    {
        Information = 0,
        Objects     = 1
    };
};

class MachOFBFile : public TypeInterface, public GView::View::BufferViewer::OffsetTranslateInterface
{
  public:
    const struct Colors
    {
        const ColorPair header{ Color::Olive, Color::Transparent };
        const ColorPair arch{ Color::Magenta, Color::Transparent };
        const ColorPair objectName{ Color::DarkRed, Color::Transparent };
        const ColorPair object{ Color::Silver, Color::Transparent };
    } colors;

  public:
    Reference<GView::Utils::FileCache> file;
    ArchInfo ai;
    fat_header header;
    std::vector<std::variant<fat_arch, fat_arch64>> archs;
    std::vector<ArchInfo> archsInfo;
    bool shouldSwapEndianess;
    bool is64;

    uint64_t panelsMask;

  public:
    // OffsetTranslateInterface
    uint64_t TranslateToFileOffset(uint64_t value, uint32 fromTranslationIndex) override;
    uint64_t TranslateFromFileOffset(uint64_t value, uint32 toTranslationIndex) override;

    // TypeInterface
    std::string_view GetTypeName() override
    {
        return "Mach-O Fat Binary";
    }

  public:
    MachOFBFile(Reference<GView::Utils::FileCache> file);
    virtual ~MachOFBFile(){};

    bool Update();

    bool HasPanel(Panels::IDs id);
};

namespace Panels
{
    class Information : public AppCUI::Controls::TabPage
    {
        Reference<MachOFBFile> fat;
        Reference<AppCUI::Controls::ListView> general;

        void UpdateGeneralInformation();
        void RecomputePanelsPositions();

      public:
        Information(Reference<MachOFBFile> fat);

        void Update();
        virtual void OnAfterResize(int newWidth, int newHeight) override
        {
            RecomputePanelsPositions();
        }
    };

    class Objects : public AppCUI::Controls::TabPage
    {
        Reference<MachOFBFile> fat;
        Reference<GView::View::WindowInterface> win;
        Reference<AppCUI::Controls::ListView> list;
        int Base;

        std::string_view GetValue(NumericFormatter& n, uint64_t value);
        void GoToSelectedSection();
        void SelectCurrentSection();

      public:
        Objects(Reference<MachOFBFile> fat, Reference<GView::View::WindowInterface> win);

        void Update();
        bool OnUpdateCommandBar(AppCUI::Application::CommandBar& commandBar) override;
        bool OnEvent(Reference<Control>, Event evnt, int controlID) override;
    };
} // namespace Panels
} // namespace GView::Type::MachOFB
