// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _cartridgechip_h_
#define _cartridgechip_h_

#include "types.h"
#include "cartridge.h"

typedef struct cartridgeChip *CartridgeChip;

CartridgeChip cartridgeChip_Create();

void cartridgeChip_InsertCartridge(CartridgeChip self, Cartridge cartridge);

colorData *cartridgeChip_GetColors(CartridgeChip self, int *colorsLen);

TextureData *cartridgeChip_GetSprites(CartridgeChip self,
    int spriteWidth, int spriteHeight, GetColorRef getColorRef, int *spritesLen);

const char *cartridgeChip_GetScript(CartridgeChip self, int *scriptLen);

#endif
