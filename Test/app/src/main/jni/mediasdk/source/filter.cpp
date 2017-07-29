#include <stdlib.h>
#include "autolog.h"
#include "autofile.h"
#include "autolock.h"

namespace paomiantv
{

CFilter::CFilter()
{
    USE_LOG;
    m_pchSrcPath = (s8 *)malloc(MAX_LEN_FILE_PATH);
    memset(m_pchSrcPath, 0, MAX_LEN_FILE_PATH);
    m_pLock = new ILock;
}

CFilter::~CFilter()
{
    USE_LOG;
    delete m_pLock;
    free(m_pchSrcPath);
}
BOOL32 CFilter::init()
{
}

BOOL32 CFilter::uninit()
{
}

void CFilter::setSrc(s8 *pchSrc)
{
    BEGIN_AUTOLOCK(m_pLock);
    strncpy(m_pchSrcPath, pchSrc, sizeof(m_pchSrcPath));
    parse();
    END_AUTOLOCK;
}

void CFilter::setStart(s64 ullStart)
{
    BEGIN_AUTOLOCK(m_pLock);
    m_ullStart = ullStart;
    END_AUTOLOCK;
}

void CFilter::setDuration(s64 ullDuration)
{
    BEGIN_AUTOLOCK(m_pLock);
    m_ullDuration = ullDuration;
    END_AUTOLOCK;
}
void CFilter::parse()
{
}

} // namespace paomiantv
