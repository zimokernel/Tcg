//
//  FileWrapper.m
//  libgles
//
//  Created by ttc on 10/10/2017.
//  Copyright © 2017 TY. All rights reserved.
//

#import "FileWrapper.h"
#import <Foundation/Foundation.h>

const char *
GetBundleFileName(const char *fileName)
{
    NSString* fileNameNS = [NSString stringWithUTF8String:fileName];
    NSString* baseName = [fileNameNS stringByDeletingPathExtension];
    NSString* extension = [fileNameNS pathExtension];
    NSString *path = [[NSBundle mainBundle] pathForResource: baseName ofType: extension ];
    fileName = [path cStringUsingEncoding:1];
    
    return fileName;
}

