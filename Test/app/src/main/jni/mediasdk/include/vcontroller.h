/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: video process
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
#ifndef _PAOMIANTV_VCONTROLLER_H_
#define _PAOMIANTV_VCONTROLLER_H_

#include "typedef.h"
#include "autolock.h"
#include "version.h"
#include "autolog.h"
#include "stdlib.h"

namespace paomiantv
{

class CVController
{
  private:
    CVController(){};
    CVController(const CVController &);

    CVController &operator=(const CVController &);
    static CVController *m_pInstance;

    class Garbo
    {
      public:
        ~Garbo()
        {
            if (CVController::m_pInstance)
            {
                delete CVController::m_pInstance;
            }
        }
    };
    static Garbo garbo;

  public:
    static CVController *getInstance();
    s32 process(u8 *pbyIn, u8 *pbyOut, TVTransParam *ptVTransParam); //输入输出都是解码后的图像数据(YUV or RGBA)
};

CVController::Garbo CVController::garbo; // 一定要初始化，不然程序结束时不会析构garbo

CVController *CVController::m_pInstance = NULL;

CVController *CVController::getInstance()
{
    if (m_pInstance == NULL)
        m_pInstance = new CVController();
    return m_pInstance;
}
}

#endif /* _PAOMIANTV_VCONTROLLER_H_ */
