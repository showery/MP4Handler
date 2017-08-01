#ifndef _PAOMIANTV_THREAD_H_
#define _PAOMIANTV_THREAD_H_

#include <pthread.h>
#include <sys/prctl.h>
#include "autolog.h"

namespace paomiantv
{

typedef void *(*ThreadTask) (void *);

class CThread
{
  public:
    CThread(ThreadTask task, void *data)
        : m_data(data),
          m_task(task)
    {
    }

    ~CThread()
    {
    }

    static void SetName(const s8* pchName){
#ifndef __ANDROID__
        prctl(PR_SET_NAME, pchName);
#else
        prctl(PR_SET_NAME, (unsigned long)pchName, 0, 0, 0);
#endif
    }

    BOOL32 start()
    {
        BOOL32 ret = TRUE;
        do
        {
            int nErr;
            pthread_attr_t attr;
            nErr = pthread_attr_init(&attr);
            if (nErr)
            {
                LOGE("start failed, jni_init thread attribute failed!");
                ret = FALSE;
                break;
            }

            nErr = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
            if (nErr)
            {
                LOGE("start failed, set thread detach state failed!");
                ret = FALSE;
                break;
            }

            nErr = pthread_create(&m_thread, &attr, (void *(*)(void *))m_task, m_data);
            if (nErr)
            {
                LOGE("start failed, create thread failed!");
                ret = FALSE;
                break;
            }

            pthread_attr_destroy(&attr);
        } while (0);
        return ret;
    }

    void* join()
    {
        void *retv = NULL;
        int nErr = pthread_join(m_thread, (void **)&retv);
        if(nErr)
        {
            printf("thread is not started");
        }
        return retv;
    }

  private:
    pthread_t m_thread;
    ThreadTask m_task;
    void *m_data;
};

} // namespace paomiantv

#endif // _PAOMIANTV_THREAD_H_