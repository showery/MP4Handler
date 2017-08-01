/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: define jni objects manager
 *       Related: 
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-20  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_JNIMODULEMANAGER_H_
#define _PAOMIANTV_JNIMODULEMANAGER_H_

#include <stdlib.h>

#include "residentreg.h"
#include "typedef.h"
#include "autolock.h"
#include "version.h"
#include "autolog.h"

namespace paomiantv {

    class CJNIModuleManager {
    private:
        CJNIModuleManager();

        CJNIModuleManager(const CJNIModuleManager &);

        CJNIModuleManager &operator=(const CJNIModuleManager &);

        virtual ~CJNIModuleManager();

        static CJNIModuleManager *m_pInstance;
        CResidentReg m_Registry;

        class Garbo {
        public:
            ~Garbo() {
                if (CJNIModuleManager::m_pInstance) {
                    delete CJNIModuleManager::m_pInstance;
                }
            }
        };

        static Garbo garbo;

    public:
        static CJNIModuleManager *getInstance();

        void add(void *p);
        void remove(void *p);

        bool contains(void *p);
    };
}

#endif /* _PAOMIANTV_JNIMODULEMANAGER_H_ */
