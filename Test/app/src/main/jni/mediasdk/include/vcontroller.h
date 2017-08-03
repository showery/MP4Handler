/*******************************************************************************
 *        Module: paomiantv
 *          File: 
 * Functionality: video controller
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

#ifndef _PAOMIANTV_VCONTROLLER_H_
#define _PAOMIANTV_VCONTROLLER_H_

#include "typedef.h"
#include "autolock.h"
#include "version.h"
#include "autolog.h"
#include "stdlib.h"
#include "transparam.h"
#include "thread.h"
#include "storyboard.h"
#include "controller.h"

namespace paomiantv {

    class CVController : public CController {
    private:
        CVController();

        virtual ~CVController();

        CVController(const CVController &);

        CVController &operator=(const CVController &);

        static CVController *m_pInstance;

        class Garbo {
        public:
            ~Garbo() {
                if (CVController::m_pInstance) {
                    delete CVController::m_pInstance;
                }
            }
        };

        static Garbo garbo;

        void handle(CStoryboard *pStoryboard);

        //输入输出都是解码后的图像数据(YUV or RGBA)
        s32 transform(u8 *pbyIn, u8 *pbyOut, void *ptVTransParam);

    public:
        static CVController *getInstance();
    };
}

#endif /* _PAOMIANTV_VCONTROLLER_H_ */
