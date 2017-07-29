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

namespace paomiantv
{

class CMP4Parser
{
  private:
    CMP4Parser(){};
    CMP4Parser(const CMP4Parser &);

    CMP4Parser &operator=(const CMP4Parser &);
    static CMP4Parser *m_pInstance;

    class Garbo
    {
      public:
        ~Garbo()
        {
            if (CMP4Parser::m_pInstance)
            {
                delete CMP4Parser::m_pInstance;
            }
        }
    };
    static Garbo garbo;

  public:
    static CJNIModuleManger *getInstance();
    BOOL32 loadMp4(s8 *pchPath);
};

CMP4Parser::Garbo CMP4Parser::garbo; // 一定要初始化，不然程序结束时不会析构garbo

CMP4Parser *CMP4Parser::m_pInstance = NULL;

CMP4Parser *CMP4Parser::getInstance()
{
    if (m_pInstance == NULL)
        m_pInstance = new CMP4Parser();
    return m_pInstance;
}
}

#endif /* _PAOMIANTV_MP4PARSE_H_ */
