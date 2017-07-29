#include <stdlib.h>
#include "autolog.h"
#include "autofile.h"
#include "autolock.h"

namespace paomiantv
{

CTransition::CTransition()
{
    USE_LOG;
    m_pchSrcPath = (s8 *)malloc(MAX_LEN_FILE_PATH);
    memset(m_pchSrcPath, 0, MAX_LEN_FILE_PATH);
    m_pLock = new ILock;
}

CTransition::~CTransition()
{
    USE_LOG;
    delete m_pLock;
    free(m_pchSrcPath);
}
BOOL32 CTransition::init()
{
}

BOOL32 CTransition::uninit()
{
}

void CTransition::setSrc(s8 *pchSrc)
{
    BEGIN_AUTOLOCK(m_pLock);
    strncpy(m_pchSrcPath, pchSrc, sizeof(m_pchSrcPath));
    parse();
    END_AUTOLOCK;
}

void CTransition::setStart(s64 ullStart)
{
    BEGIN_AUTOLOCK(m_pLock);
    m_ullStart = ullStart;
    END_AUTOLOCK;
}

void CTransition::setDuration(s64 ullDuration)
{
    BEGIN_AUTOLOCK(m_pLock);
    m_ullDuration = ullDuration;
    END_AUTOLOCK;
}

void CTransition::parse()
{
}

} // namespace paomiantv
