#include <RSTileRenderer.h>
#include <RSVarint.h>
#include <RSTiledata.h>


void RSTileRendererDrawTile(
                 CGContextRef c,
                 CGRect bounds,
                 uint8_t *data,
                 uint32_t dataSize,
                 uint32_t zoom,
                 RSTileRendererGraphicsSpace graphicsSpace)
{
    CGFloat width = (CGFloat)bounds.size.width;
    CGFloat height = (CGFloat)bounds.size.height;
    
    const CGFloat backgroundColor[4] = {0.5, 0.5, 0.5, 1};
    const CGFloat fillColors[8][4] = {
        {0, 0, 0, 0},
        {(210 / 255.0), (173 / 255.0), (104 / 255.0), 1}, // Building = 1,
        {(66 / 255.0), (66 / 255.0), (255 / 255.0), 1}, // HighwayMotorway = 2,
        {(255 / 255.0), (66 / 255.0), (66 / 255.0), 1}, // HighwayPrimary = 3,
        {(255 / 255.0), (155 / 255.0), (66 / 255.0), 1}, // HighwaySecondary = 4,
        {(255 / 255.0), (255 / 255.0), (66 / 255.0), 1}, // HighwayTertiary = 5,
        {(255 / 255.0), (255 / 255.0), (255 / 255.0), 1}, // HighwayResidential = 6,
        {(255 / 255.0), (255 / 255.0), (255 / 255.0), 1} // HighwayService = 7,
    };
    const CGFloat strokeColors[8][4] = {
        {0, 0, 0, 0},
        {(140 / 255.0), (93 / 255.0), (33 / 255.0), 1}, // Building = 1,
        {(44 / 255.0), (44 / 255.0), (200 / 255.0), 1}, // HighwayMotorway = 2,
        {(200 / 255.0), (44 / 255.0), (44 / 255.0), 1}, // HighwayPrimary = 3,
        {(200 / 255.0), (100 / 255.0), (44 / 255.0), 1}, // HighwaySecondary = 4,
        {(200 / 255.0), (200 / 255.0), (44 / 255.0), 1}, // HighwayTertiary = 5,
        {(200 / 255.0), (200 / 255.0), (200 / 255.0), 1}, // HighwayResidential = 6,
        {(200 / 255.0), (200 / 255.0), (200 / 255.0), 1} // HighwayService = 7,
    };
    
    CGContextSetFillColor(c, backgroundColor);
    CGContextFillRect(c, bounds);
    
    uint32_t pos = 0;
    
    uint32_t resolutionBits = 8;
    double intToFloat = width / (1 << resolutionBits);
    
    int32_t ix, iy;
    double x, y, firstX, firstY;
    
    while (pos < dataSize) {
        ix = 0;
        iy = 0;
        RSTiledataThingType thingType = (RSTiledataThingType)RSVarintRead(data, &pos);
        uint32_t numNodes = RSVarintRead(data, &pos);
        for (uint32_t i = 0; i < numNodes; i++) {
            ix += RSVarintSignedRead(data, &pos);
            iy += RSVarintSignedRead(data, &pos);
            x = intToFloat * ix;
            y = intToFloat * iy;
            if (graphicsSpace == RSTileRendererGraphicsSpaceQuadrantOne) {
                y = height - y;
            }
            if (i == 0) {
                firstX = x;
                firstY = y;
                CGContextMoveToPoint(c, x, y);
            } else {
                CGContextAddLineToPoint(c, x, y);
            }
        }
        CGContextSetFillColor(c, fillColors[thingType]);
        CGContextSetStrokeColor(c, strokeColors[thingType]);
        switch (thingType) {
            case RSTiledataBuilding:
                CGContextSetLineWidth(c, 4);
                break;
            case RSTiledataHighwayMotorway:
            case RSTiledataHighwayPrimary:
            case RSTiledataHighwaySecondary:
            case RSTiledataHighwayTertiary:
            case RSTiledataHighwayResidential:
            case RSTiledataHighwayService:
                CGContextSetLineWidth(c, 12);
                break;
        }
        if (RSTiledataThingTypeIsClosedWay(thingType)) {
            CGContextAddLineToPoint(c, firstX, firstY);
            CGContextDrawPath(c, kCGPathFillStroke);
        } else {
            CGContextStrokePath(c);
        }
    }
}
