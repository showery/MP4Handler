//
// Created by John.Huang on 2017/8/1.
//
#include "jnimodulemanager.h"

namespace paomiantv {

    CJNIModuleManger::CJNIModuleManger() {

    }

    CJNIModuleManger::~CJNIModuleManger() {

    }

    CJNIModuleManger::Garbo CJNIModuleManger::garbo; // 一定要初始化，不然程序结束时不会析构garbo

    CJNIModuleManger *CJNIModuleManger::m_pInstance = NULL;

    CJNIModuleManger *CJNIModuleManger::getInstance() {
        if (m_pInstance == NULL)
            m_pInstance = new CJNIModuleManger();
        return m_pInstance;
    }

    void CJNIModuleManger::add(void *p) {
        m_Registry.Add(p);
    }

    void CJNIModuleManger::remove(void *p) {
        m_Registry.Remove(p);
    }

    bool CJNIModuleManger::contains(void *p) {
        return m_Registry.Contains(p);
    }
}
