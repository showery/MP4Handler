//
// Created by John.Huang on 2017/8/1.
//

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
