/*******************************************************************************
 *        Module: mediasdk
 *          File: 
 * Functionality: audio controller
 *       Related: 
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-24  v1.0        huangxuefeng  created
 ******************************************************************************/
#ifndef _PAOMIANTV_ACONTROLLER_H_
#define _PAOMIANTV_ACONTROLLER_H_

#include "typedef.h"
#include "autolock.h"
#include "autolog.h"
#include "stdlib.h"
#include "transparam.h"
#include "thread.h"
#include "storyboard.h"
#include "controller.h"

namespace paomiantv {

    class CAController : public CController {
    private:
        CAController();

        virtual ~CAController();

        CAController(const CAController &);

        CAController &operator=(const CAController &);

        static CAController *m_pInstance;

        class Garbo {
        public:
            ~Garbo() {
                if (CAController::m_pInstance) {
                    delete CAController::m_pInstance;
                }
            }
        };

        static Garbo garbo;

        void handle(CStoryboard *pStoryboard);
        //输入输出都是解码后的声音数据(PCM)
        BOOL32 transform(u8 *pbyIn, u8 *pbyOut, void *ptATransParam);

    public:
        static CAController *getInstance();
    };
}

#endif /* _PAOMIANTV_ACONTROLLER_H_ */
