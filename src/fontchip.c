// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "chip.h"
#include "sprite.h"
#include "spritechip.h"
#include "texturedata.h"
#include "fontchip.h"

#define MAX_FONTS 4

typedef struct font {
    char name[64];
    spriteId map[96];
} *Font;

typedef struct fontChip {
    chip base;
    struct font fonts[MAX_FONTS];
    SpriteChip spriteChip;
} fontChip;
//
static void fontChip_Destroy(FontChip self);
static void fontChip_Init(FontChip self, GetChip getChip);
static Font fontChip_FindFont(FontChip self, const char *fontName);

FontChip fontChip_Create()
{
    FontChip self = NULL;

    self = (FontChip)calloc(1, sizeof(fontChip));
    if (self == NULL)
        return self;

    strncpy(self->base.name, nameof(FontChip), sizeof(self->base.name) - 1);
    self->base.destroy = fontChip_Destroy;
    self->base.init = fontChip_Init;

    return self;
}

static void fontChip_Destroy(FontChip self)
{
    assert(self);
    memset(self, 0, sizeof(struct fontChip));
    free(self);
}

static void fontChip_Init(FontChip self, GetChip getChip)
{
    assert(self);
    self->spriteChip = (SpriteChip)func_Invoke(getChip, nameof(SpriteChip));
}

void fontChip_AddFont(FontChip self, const char *name, int mapLen, spriteId *map)
{
    assert(self);
    Font slot = NULL;
    for (int i = 0; i < MAX_FONTS; i++)
        if (self->fonts[i].name[0] == '\0')
        {
            slot = &self->fonts[i];
            break;
        }
    slot = slot == NULL ? &self->fonts[0] : slot;
    strncpy(slot->name, name, sizeof(slot->name) - 1);
    memset(slot->map, 0, sizeof(slot->map));
    memcpy(slot->map, map, min(mapLen * sizeof(int), sizeof(slot->map)));
}

void fontChip_ConvertTextToSprites(FontChip self, const char *text, const char *fontName, spriteId *spriteIds)
{
    assert(self);
    assert(text);
    assert(spriteIds);

    Font font = fontChip_FindFont(self, fontName);
    if (font == NULL)
        return;

    for (int i = 0; i < strlen(text); i++)
        spriteIds[i] = font->map[clamp(text[i] - ' ', 0, 96)];
}

TextureData fontChip_ConvertTextToTexture(FontChip self, const char *text, const char *fontName, int letterSpacing)
{
    assert(self);
    assert(self->spriteChip);

    Font font = fontChip_FindFont(self, fontName);
    if (font == NULL)
        return NULL;

    int lines = 0;
    int longestLine =  0;
    char *line = strtok(text, "\n");
    while (line != NULL)
    {
        lines++;
        int len = strlen(line);
        if (len > longestLine)
            longestLine = len;

        line = strtok(NULL, "\n");
    }

    int cWidth = spriteChip_GetSpriteWidth(self->spriteChip);
    int textureWidth = (cWidth + letterSpacing) * longestLine; // should be len of longest line

    int cHeight = spriteChip_GetSpriteHeight(self->spriteChip);
    int textureHeight = cHeight * lines;

    // TODO: this can fail
    TextureData outputTexture = textureData_Create(textureWidth, textureHeight);

    int y = 0;
    line = strtok(text, "\n");
    while (line != NULL)
    {
        for (int x = 0; x < (int)strlen(line); x++)
        {
            int spriteIdx = font->map[clamp(line[x] - ' ', 0, 96)];
            Sprite sprite = spriteChip_GetSprite(self->spriteChip, spriteIdx);
            sprite_CopyToTextureAtPos(sprite, outputTexture, x * (cWidth + letterSpacing), y);
        }

        y += textureHeight;
        line = strtok(NULL, "\n");
    }

    return outputTexture;
}

static Font fontChip_FindFont(FontChip self, const char *fontName)
{
    assert(self);
    for (int i = 0; i < MAX_FONTS; i++)
        if (strcmp(self->fonts[i].name, fontName) == 0)
            return &self->fonts[i];
    return NULL;
}
