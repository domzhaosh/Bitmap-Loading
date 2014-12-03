#pragma once

#include "resource.h"

typedef struct bmpSystemFile
{
    public:
        IShellItem * selectedFile;
        Bitmap * bitmap;
        INT bitmapFlipCase;
		INT currentRotationValue;
        BOOL noFileSelected;
} BMPSYSTEMFILE_S;
