// Microsoft Public License (MS-PL) - Copyright (C) Shawn Rakowski
// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#ifndef _displaychip_h_
#define _displaychip_h_

typedef struct displayChip *DisplayChip;

DisplayChip displayChip_Create(int width, int height);

int displayChip_GetPixelCount(DisplayChip self);

int displayChip_GetPixelAt(DisplayChip self, int idx);

#endif
