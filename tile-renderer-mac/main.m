#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <ApplicationServices/ApplicationServices.h>
#import <RSTileRenderer.h>


CGContextRef createContext(CGRect bounds);
void exportAndRelease(CGContextRef c, CGRect bounds, CFStringRef format);


int main(int argc, const char * argv[]) {
    @autoreleasepool {
        if (
                    (argc != 5) ||
                    (strcmp(argv[1], "--zoom") != 0) ||
                    (strcmp(argv[3], "--format") != 0) ||
                    (! (
                            (strcmp(argv[4], "png") == 0) ||
                            (strcmp(argv[4], "bmp") == 0)))) {
            fprintf(stderr, "Usage: tile-renderer-mac --zoom 17 --format png|bmp\n");
            exit(1);
        }
        uint32_t zoom = (uint32_t)atol(argv[2]);
        CFStringRef format;
        if (strcmp(argv[4], "png") == 0) {
            format = kUTTypePNG;
        } else {
            format = kUTTypeBMP;
        }
        NSData *tiledata = [[NSFileHandle fileHandleWithStandardInput] readDataToEndOfFile];
        uint8_t *data = (uint8_t *)tiledata.bytes;
        uint32_t dataSize = (uint32_t)tiledata.length;
        CGRect bounds = CGRectMake(0, 0, 512, 512);
        CGContextRef c = createContext(bounds);
        RSTileRendererDrawTile(c, bounds, data, dataSize, zoom, RSTileRendererGraphicsSpaceQuadrantOne);
        exportAndRelease(c, bounds, format);
    }
    return 0;
}


CGContextRef createContext(CGRect bounds) {
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef c = CGBitmapContextCreate(nil, bounds.size.width, bounds.size.height, 8, 0, colorSpace, (CGBitmapInfo)kCGImageAlphaPremultipliedLast);
    if (c == nil) {
        [NSException raise:@"CG"
                    format:@"error creating context"];
    }
    CGContextSetFillColorSpace(c, colorSpace);
    CGContextSetStrokeColorSpace(c, colorSpace);
    CGColorSpaceRelease(colorSpace);
    return c;
}


void exportAndRelease(CGContextRef c, CGRect bounds, CFStringRef format) {
    CGImageRef img = CGBitmapContextCreateImage(c);
    CGContextDrawImage(c, bounds, img);
    CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, 0);
    CGImageDestinationRef destination = CGImageDestinationCreateWithData(data, format, 1, nil);
    CGImageDestinationAddImage(destination, img, nil);
    CGImageDestinationFinalize(destination);
    CFIndex size = CFDataGetLength(data);
    const uint8_t *bytes = CFDataGetBytePtr(data);
    if (fwrite(bytes, size, 1, stdout) != 1) {
        [NSException raise:@"fwrite"
                    format:@"fwrite"];
    }
    CFRelease(destination);
    CFRelease(data);
    CGContextRelease(c);
    CGImageRelease(img);
}
