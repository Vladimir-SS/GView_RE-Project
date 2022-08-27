#include "LexicalViewer.hpp"

namespace GView::View::LexicalViewer
{
#define INSTANCE reinterpret_cast<Instance*>(this->data)
#define CREATE_TOKENREF(err)                                                                                                               \
    if (this->data == nullptr)                                                                                                             \
        return (err);                                                                                                                      \
    if ((size_t) this->index >= INSTANCE->tokens.size())                                                                                   \
        return (err);                                                                                                                      \
    auto& tok = INSTANCE->tokens[this->index];

#define CREATE_BLOCKREF(err)                                                                                                               \
    if (this->data == nullptr)                                                                                                             \
        return (err);                                                                                                                      \
    if ((size_t) this->index >= INSTANCE->blocks.size())                                                                                   \
        return (err);                                                                                                                      \
    auto& block = INSTANCE->blocks[this->index];

// TOKEN methods
uint32 Token::GetTypeID(uint32 error) const
{
    CREATE_TOKENREF(error);
    return tok.type;
}
TokenAlignament Token::GetAlignament() const
{
    CREATE_TOKENREF(TokenAlignament::None);
    return tok.align;
}
bool Token::SetAlignament(TokenAlignament align)
{
    CREATE_TOKENREF(false);
    tok.align = align;
    return true;
}
bool Token::UpdateAlignament(TokenAlignament flagsToAdd, TokenAlignament flagsToRemove)
{
    CREATE_TOKENREF(false);
    tok.align |= flagsToAdd;
    tok.align = static_cast<TokenAlignament>(static_cast<uint16>(tok.align) & (~(static_cast<uint16>(flagsToRemove))));
    return true;
}
bool Token::SetTokenColor(TokenColor col)
{
    CREATE_TOKENREF(false);
    tok.color = col;
    return true;
}
u16string_view Token::GetText() const
{
    CREATE_TOKENREF(u16string_view{});
    return { INSTANCE->GetUnicodeText() + tok.start, (size_t) (tok.end - tok.start) };
}
Block Token::GetBlock() const
{
    CREATE_TOKENREF(Block());
    if (tok.blockID < INSTANCE->blocks.size())
        return Block(this->data, tok.blockID);
    return Block();
}
bool Token::SetBlock(Block block)
{
    return SetBlock(block.GetIndex());
}
bool Token::SetBlock(uint32 blockIndex)
{
    CREATE_TOKENREF(false);
    if (tok.IsBlockStarter())
        return false; // already has a block
    if (blockIndex >= INSTANCE->blocks.size())
        return false; // invalid block index
    const auto& block = INSTANCE->blocks[blockIndex];
    // token index can not be inside pointed block
    if (block.hasEndMarker)
    {
        if ((this->index >= block.tokenStart) && (this->index <= block.tokenEnd))
            return false;
    }
    else
    {
        if ((this->index >= block.tokenStart) && (this->index < block.tokenEnd))
            return false;
    }
    // all good --> link it
    tok.blockID = blockIndex;
    return true;
}
Token Token::Next() const
{
    if (this->data == nullptr)
        return Token();
    if ((size_t) (this->index + 1) >= INSTANCE->tokens.size())
        return Token();
    return Token(this->data, this->index + 1);
}
Token Token::Precedent() const
{
    if ((this->data == nullptr) || (this->index == 0))
        return Token();
    if ((size_t) (this->index - 1) >= INSTANCE->tokens.size())
        return Token();
    return Token(this->data, this->index - 1);
}
bool Token::DisableSimilartyHighlight()
{
    CREATE_TOKENREF(false);
    tok.SetDisableSimilartyHighlightFlag();
    return true;
}
bool Token::SetText(const ConstString& text)
{
    CREATE_TOKENREF(false);
    return tok.value.Set(text);
}
bool Token::Delete()
{
    CREATE_TOKENREF(false);
    tok.SetShouldDeleteFlag();
    return true;
}
// Token Object
void TokenObject::UpdateSizes(const char16* text)
{
    const char16* p = text + start;
    const char16* e = text + end;
    if (this->value.Len() > 0)
    {
        p = this->value.GetString();
        e = this->value.GetString() + this->value.Len();
    }
    auto nrLines = 1U;
    auto w       = 0U;
    auto maxW    = 0U;
    while (p < e)
    {
        if (((*p) == '\n') || ((*p) == '\r'))
        {
            nrLines++;
            maxW   = std::max<>(maxW, w);
            w      = 0;
            auto c = *p;
            p++;
            if ((p < e) && (((*p) == '\n') || ((*p) == '\r')) && ((*p) != c))
                p++; // auto detect \n\r or \r\n
        }
        else
        {
            p++;
            w++;
        }
    }
    this->height = nrLines;
    this->width  = std::max<>(maxW, w);
}
// Block method
Token Block::GetStartToken() const
{
    CREATE_BLOCKREF(Token());
    return Token(this->data, block.tokenStart);
}
Token Block::GetEndToken() const
{
    CREATE_BLOCKREF(Token());
    return Token(this->data, block.tokenEnd);
}
bool Block::SetFoldMessage(std::string_view txt)
{
    CREATE_BLOCKREF(false);
    block.foldMessage = txt;
    return true;
}
// TOKENLIST methods

uint32 TokensList::Len() const
{
    return (uint32) (INSTANCE->tokens.size());
}
Token TokensList::operator[](uint32 index) const
{
    if ((size_t) index >= INSTANCE->tokens.size())
        return Token();
    return Token(this->data, index);
}
Token TokensList::GetLastToken() const
{
    uint32 count = (uint32) INSTANCE->tokens.size();
    if (count > 0)
        return Token(this->data, count - 1);
    else
        return Token();
}
Token TokensList::Add(uint32 typeID, uint32 start, uint32 end, TokenColor color)
{
    return Add(typeID, start, end, color, TokenDataType::None, TokenAlignament::None, false);
}
Token TokensList::Add(uint32 typeID, uint32 start, uint32 end, TokenColor color, TokenDataType dataType)
{
    return Add(typeID, start, end, color, dataType, TokenAlignament::None, false);
}
Token TokensList::Add(uint32 typeID, uint32 start, uint32 end, TokenColor color, TokenAlignament align)
{
    return Add(typeID, start, end, color, TokenDataType::None, align, false);
}
Token TokensList::Add(uint32 typeID, uint32 start, uint32 end, TokenColor color, TokenDataType dataType, TokenAlignament align)
{
    return Add(typeID, start, end, color, dataType, align, false);
}
Token TokensList::Add(
      uint32 typeID, uint32 start, uint32 end, TokenColor color, TokenDataType dataType, TokenAlignament align, bool disableSimilartySearch)
{
    uint32 itemsCount = static_cast<uint32>(INSTANCE->tokens.size());
    uint32 len        = INSTANCE->GetUnicodeTextLen();
    if ((start >= end) || (start >= len) || (end > (len + 1)))
    {
        LOG_ERROR("Invalid token offset: start=%du, end=%u, length=%u", start, end, len);
        return Token();
    }
    if (itemsCount > 0)
    {
        auto& lastToken = INSTANCE->tokens[itemsCount - 1];
        if (start < lastToken.end)
        {
            LOG_ERROR("All tokens must be provided in order (current token starts at %u, but last token ends at %u)", start, lastToken.end);
            return Token();
        }
    }
    auto& cToken     = INSTANCE->tokens.emplace_back();
    cToken.type      = typeID;
    cToken.start     = start;
    cToken.end       = end;
    cToken.height    = 1;
    cToken.maxWidth  = 0;
    cToken.maxHeight = 0;
    cToken.color     = color;
    cToken.width     = (uint8) (std::min(end - start, (uint32) 0xFE));
    cToken.blockID   = BlockObject::INVALID_ID;
    cToken.status    = TokenStatus::Visible;
    cToken.align     = align;
    cToken.dataType  = dataType;

    if (disableSimilartySearch)
        cToken.SetDisableSimilartyHighlightFlag();

    this->lastTokenID = typeID;

    return Token(this->data, itemsCount);
}
Token TokensList::AddErrorToken(uint32 start, uint32 end, ConstString error)
{
    auto tok = Add(0, start, end, TokenColor::Error);
    if (tok.IsValid())
    {
        // add error
    }
    return tok;
}

// block list
Block BlocksList::Add(uint32 start, uint32 end, BlockAlignament align, bool hasBlockEndMarker)
{
    uint32 itemsCount = static_cast<uint32>(INSTANCE->tokens.size());
    CHECK(start < itemsCount, Block(), "Invalid token index (start=%u), should be less than %u", start, itemsCount);
    CHECK(end < itemsCount, Block(), "Invalid token index (end=%u), should be less than %u", end, itemsCount);
    CHECK(start < end, Block(), "Start token index(%u) should be smaller than end token index(%u)", start, end);

    // create a block
    auto& block               = INSTANCE->blocks.emplace_back();
    uint32 blockID            = (uint32) (INSTANCE->blocks.size() - 1);
    block.tokenStart          = start;
    block.tokenEnd            = end;
    block.align               = align;
    block.hasEndMarker        = hasBlockEndMarker;
    block.leftHighlightMargin = 0;

    // set token flags
    INSTANCE->tokens[start].SetBlockStartFlag();
    INSTANCE->tokens[start].blockID = blockID;

    if (hasBlockEndMarker)
        INSTANCE->tokens[end].blockID = blockID;

    return Block(this->data, blockID);
}
uint32 BlocksList::Len() const
{
    return static_cast<uint32>(INSTANCE->blocks.size());
}
Block BlocksList::operator[](uint32 index) const
{
    if (index < INSTANCE->blocks.size())
        return Block(this->data, index);
    return Block();
}
}; // namespace GView::View::LexicalViewer