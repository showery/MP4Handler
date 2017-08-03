/*******************************************************************************
 *        Module: mediasdk
 *          File:
 * Functionality: define filter entity.
 *       Related:
 *        System: android
 *      Language: C++
 *        Author: huangxuefeng
 *       Version: V1.0 Copyright(C) 2017 paomiantv, All rights reserved.
 * -----------------------------------------------------------------------------
 * Revisions:
 * Date        Version     Reviser       Description
 * 2017-07-27  v1.0        huangxuefeng  created
 ******************************************************************************/

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
  void init(s8* pchSrc, s64 sllStart, s64 sllDurantion);

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
inline s8 *CFilter::getSrc()
{
  return m_pchSrcPath;
}

inline s64 CFilter::getStart()
{
  return m_sllStart;
}

inline s64 CFilter::getDuration()
{
  return m_sllDuration;
}

} // namespace paomiantv

#endif // _PAOMIANTV_FILTER_H_