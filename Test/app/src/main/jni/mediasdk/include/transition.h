#ifndef _PAOMIANTV_TRANSITION_H_
#define _PAOMIANTV_TRANSITION_H_

#include "typedef.h"

namespace paomiantv
{

class CTransition
{
public:
  CTransition();
  ~CTransition();

private:
  void parse();

public:
  void init(s8* pchSrc, s64 sllStart, s64 sllDuration);

  void uninit();
  void setSrc(s8 *pchSrc);
  void setStart(s64 sllStart);
  void setDuration(s64 sllDuration);

  inline s8 *getSrc();
  inline s64 getStart();
  inline s64 getDuration();

private:
  s8 *m_pchSrcPath;
  s64 m_sllStart;
  s64 m_sllDuration;
};
inline s8 *CTransition::getSrc()
{
  return m_pchSrcPath;
}

inline s64 CTransition::getStart()
{
  return m_sllStart;
}

inline s64 CTransition::getDuration()
{
  return m_sllDuration;
}

} // namespace paomiantv

#endif // _PAOMIANTV_TRANSITION_H_