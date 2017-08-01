/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: mp4 parser
 *       Related: 
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 PAOMIANTV, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_MP4PARSE_H_
#define _PAOMIANTV_MP4PARSE_H_

#include "typedef.h"
#include "autolock.h"
#include "version.h"
#include "autolog.h"
#include "jnimodule.h"
#include "stdlib.h"
#include "jnimoduleengine.h"
#include "jnimodulestoryboard.h"
#include "jnimoduleclip.h"

namespace paomiantv {

    class CMP4Parser {
    public:
        CMP4Parser() {};

        CMP4Parser(const CMP4Parser &);

        CMP4Parser &operator=(const CMP4Parser &);

    public:

        BOOL32 loadMp4(s8 *pchPath);
    };
}

#endif /* _PAOMIANTV_MP4PARSE_H_ */
