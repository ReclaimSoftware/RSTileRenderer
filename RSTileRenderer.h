#ifndef __RSTileRenderer_header__
#define __RSTileRenderer_header__

#include <CoreGraphics/CoreGraphics.h>

typedef enum {
    RSTileRendererGraphicsSpaceQuadrantOne
} RSTileRendererGraphicsSpace;

void RSTileRendererDrawTile(
                CGContextRef c,
                CGRect bounds,
                uint8_t *data,
                uint32_t dataSize,
                uint32_t zoom,
                RSTileRendererGraphicsSpace graphicsSpace);

#endif
