#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>


void *
es_load_png(char *filename, int *width, int *height)
{
    NSString *filePath = [NSString stringWithUTF8String: filename];
    NSString *path = [[NSBundle mainBundle] pathForResource: filePath ofType:@"png"];
    NSData *texData = [[NSData alloc] initWithContentsOfFile:path];
    UIImage *image = [[UIImage alloc] initWithData:texData];
    
    *width =(int)CGImageGetWidth(image.CGImage);
    *height =(int)CGImageGetHeight(image.CGImage);
    
    CGColorSpaceRef c_space = CGColorSpaceCreateDeviceRGB();
    void *img_data = malloc(*height *(*width));
    
    CGContextRef context = CGBitmapContextCreate(img_data, *width, *height, 8, 4 * *width, c_space, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    CGColorSpaceRelease(c_space);
    CGContextClearRect(context, CGRectMake(0, 0, *width, *height));
    CGContextTranslateCTM(context, 0, *height - *height);
    CGContextDrawImage(context, CGRectMake(0, 0, *width, *height), image.CGImage);
    
    CGContextRelease(context);
    return img_data;
}
