#ifndef _PAOMIANTV_FILTER_H_
#define _PAOMIANTV_FILTER_H_

#include "typedef.h"

namespace paomiantv
{

class CFilter
{
public:
  CFilter();
  ~CFilter();

private:
  void parse();

public:
  BOOL32 init();

  BOOL32 uninit();
  void setSrc(s8 *pchSrc);
  void setStart(s64 ullStart);
  void setDuration(s64 ullDuration);

  inline s8 *getSrc();
  inline s64 getStart();
  inline s64 getDuration();

private:
  s8 *m_pchSrcPath;
  s64 m_ullStart;
  s64 m_ullDuration;
  ILock *m_pLock;
};
inline s8 *CFilter::getSrc()
{
  return m_pchSrcPath;
}

inline s64 CFilter::getStart()
{
  return m_ullStart;
}

inline s64 CFilter::getDuration()
{
  return m_ullDuration;
}

} // namespace paomiantv

#endif // _PAOMIANTV_MP4CUTTER_H_