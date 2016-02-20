#include <font/FTFontChar.h>
#include <font/FontBatchRenderer.h>

#include <cassert>

#include <ftglyph.h>

FTFontChar::FTFontChar(char charCode, int width, int height, int xOffset, int yOffset, int xAdvance, FT_GlyphRec_* glyph)
    : charCode(charCode), x(0), y(0), width(width), height(height), xOffset(xOffset), yOffset(yOffset), xAdvance(xAdvance), glyph(glyph)
{
}

FTFontChar::~FTFontChar()
{
}

void FTFontChar::render(int x, int y) const
{
    if (isEmpty())
    {
        return;
    }

    x += xOffset;

    float vertices[VERTICES_PER_QUAD * (COMP_VERT_POS)];

    vertices[0]  = (float) x;
    vertices[1]  = (float) (y + yOffset);

    vertices[2]  = (float) x;
    vertices[3]  = (float) y - (height - yOffset);

    vertices[4]  = (float) (x + width);
    vertices[5]  = (float) y - (height - yOffset);

    vertices[6]  = (float) (x + width);
    vertices[7] = (float) (y + yOffset);

    getRenderer().addQuad(vertices, texCoords);
}

void FTFontChar::drawToBitmap(unsigned char* data, int texWidth, int texHeight)
{
    if (isEmpty())
    {
        return;
    }

    initTexCoords(texWidth, texHeight);

    // Convert The Glyph To A Bitmap.
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph) glyph;

    // This Reference Will Make Accessing The Bitmap Easier.
    FT_Bitmap& bitmap = bitmap_glyph->bitmap;

    assert(bitmap.width == width);
    assert(bitmap.rows == height);

    int x, y = 0;
    int index;

    for (y = 0; y < bitmap.rows; y++)
    {
        for (x = 0; x < bitmap.width; x++)
        {
            index = (this->y + y) * texWidth + this->x + x;
            data[index] = bitmap.buffer[y * bitmap.width + x];
        }
    }
}

void FTFontChar::initTexCoords(int texWidth, int texHeight)
{
    float x1 = (float) this->x / (float) texWidth;
    float y1 = (float) this->y / (float) texHeight;
    float x2 = (float)(this->x + this->width) / (float) texWidth;
    float y2 = (float)(this->y + this->height) / (float) texHeight;

    texCoords[0] = x1;
    texCoords[1] = y1;

    texCoords[2] = x1;
    texCoords[3] = y2;

    texCoords[4] = x2;
    texCoords[5] = y2;

    texCoords[6] = x2;
    texCoords[7] = y1;
}

void FTFontChar::releaseGlyph()
{
    if (glyph)
    {
        FT_Done_Glyph(glyph);
    }
}

void FTFontChar::setXY(int x, int y)
{
    this->x = x;
    this->y = y;
}

int FTFontChar::getXAdvance()
{
    return xAdvance;
}

int FTFontChar::getNumPixels() const
{
    return width * height;
}

bool FTFontChar::isEmpty() const
{
    return width == 0 || height == 0;
}

int FTFontChar::getWidth()
{
    return width;
}

int FTFontChar::getHeight()
{
    return height;
}

FontBatchRenderer& FTFontChar::getRenderer() const
{
    return FontBatchRenderer::getRenderer();
}