/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: audio process
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
#ifndef _PAOMIANTV_ACONTROLLER_H_
#define _PAOMIANTV_ACONTROLLER_H_

#include "typedef.h"
#include "autolock.h"
#include "autolog.h"
#include "stdlib.h"
#include "transparam.h"

namespace paomiantv
{

class CAController
{
  private:
    CAController(){};
    CAController(const CAController &);

    CAController &operator=(const CAController &);
    static CAController *m_pInstance;

    class Garbo
    {
      public:
        ~Garbo()
        {
            if (CAController::m_pInstance)
            {
                delete CAController::m_pInstance;
            }
        }
    };
    static Garbo garbo;

  public:
    static CAController *getInstance();
    BOOL32 process(u8 *pbyIn, u8 *pbyOut, TATransParam *ptATransParam); //输入输出都是解码后的声音数据(PCM)
};

CAController::Garbo CAController::garbo; // 一定要初始化，不然程序结束时不会析构garbo

CAController *CAController::m_pInstance = NULL;

CAController *CAController::getInstance()
{
    if (m_pInstance == NULL)
        m_pInstance = new CAController();
    return m_pInstance;
}
}

#endif /* _PAOMIANTV_LOADMODULE_H_ */
