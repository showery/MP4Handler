/*******************************************************************************
 *        Module: interface
 *          File: jniloader.cpp
 * Functionality: define jni loader.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-08-01  v1.0        huangxuefeng  created
 ******************************************************************************/

#ifndef _PAOMIANTV_JNILOADER_H
#define _PAOMIANTV_JNILOADER_H

#include <stdlib.h>

#include "residentreg.h"
#include "typedef.h"
#include "autolock.h"
#include "version.h"
#include "autolog.h"

#include "jnimoduleengine.h"
#include "jnimodulestoryboard.h"
#include "jnimoduleclip.h"
#include "jnimodulerenderer.h"

namespace paomiantv {

    class CJNILoader {
    private:
        CJNILoader();

        CJNILoader(const CJNILoader &);

        CJNILoader &operator=(const CJNILoader &);

        ~CJNILoader();

        static CJNILoader *m_pInstance;

        class Garbo {
        public:
            ~Garbo() {
                if (CJNILoader::m_pInstance) {
                    delete CJNILoader::m_pInstance;
                }
            }
        };

        void loadModule(JNIEnv *env, TJavaClazzParam *param);

        static Garbo garbo;

    public:
        static CJNILoader *getInstance();

        s32 loadModules(JNIEnv *env);
    };


}

#endif //_PAOMIANTV_JNILOADER_H
