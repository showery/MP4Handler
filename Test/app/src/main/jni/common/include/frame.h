//
// Created by John.Huang on 2017/8/9.
//

#ifndef TEST_FRAME_H
#define TEST_FRAME_H

#include "unistd.h"
#include "typedef.h"
namespace paomiantv{
    typedef struct tagFrame {
        u8 *data;
        u32 size;
        u64 startTm;
        u64 duration;
        u64 renderOffset;
        BOOL isSync;
        BOOL32 isLast;
        BOOL32 isPPS;
        BOOL32 isSPS;
    public:
        tagFrame() { memset(this, 0, sizeof(struct tagFrame)); }
    } TFrame, *PTFrame;
}

#endif //TEST_FRAME_H
