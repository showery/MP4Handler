/*******************************************************************************
 *        Module: paomiantv
 *          File: jnimodulemanager.cpp
 * Functionality: manage jni objects.
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
#include "jnimodulemanager.h"

namespace paomiantv {

    CJNIModuleManager::CJNIModuleManager() {

    }

    CJNIModuleManager::~CJNIModuleManager() {

    }

    CJNIModuleManager::Garbo CJNIModuleManager::garbo; // 一定要初始化，不然程序结束时不会析构garbo

    CJNIModuleManager *CJNIModuleManager::m_pInstance = NULL;

    CJNIModuleManager *CJNIModuleManager::getInstance() {
        if (m_pInstance == NULL)
            m_pInstance = new CJNIModuleManager();
        return m_pInstance;
    }

    void CJNIModuleManager::add(void *p) {
        m_Registry.Add(p);
    }

    void CJNIModuleManager::remove(void *p) {
        m_Registry.Remove(p);
    }

    bool CJNIModuleManager::contains(void *p) {
        return m_Registry.Contains(p);
    }
}
