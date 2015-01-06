/*
 * NeutralVelocity1sNurbs.h
 *
 *  Created on: 24.10.2013
 *      Author: zwang
 */

#ifndef _NEUTRAL_NURBS_VELOCITY_1S_H_
#define _NEUTRAL_NURBS_VELOCITY_1S_H_

#include "neutral_nurbs.h"

namespace MBSimFlexibleBody {
  
  class NeutralNurbsVelocity1s : public MBSimFlexibleBody::NeutralNurbs {
    public:
      NeutralNurbsVelocity1s(Element* parent_, std::vector<ContourPointData>& transContourPoints_, double nodeOffset, double uMin_, double uMax_, int degU_, bool openStructure_);
      virtual ~NeutralNurbsVelocity1s();
      virtual void update(ContourPointData &cp) ;
    protected:
      virtual void buildNodelist();
  };

} /* namespace MBSimFlexibleBody */
#endif
