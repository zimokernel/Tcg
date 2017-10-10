//
//  FileWrapper.h
//  libgles
//
//  Created by ttc on 10/10/2017.
//  Copyright © 2017 TY. All rights reserved.
//

/**
 Given a fileName, convert into a path that can be used to open from
 the mainBundle

 @param fileName Name of file to convert to mainBundle path
 @return Path that can be used to fopen() from the mainBundle
 */
const char *GetBundleFileName(const char *fileName);

