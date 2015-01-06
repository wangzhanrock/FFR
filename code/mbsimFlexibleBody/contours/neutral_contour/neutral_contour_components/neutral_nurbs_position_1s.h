/*
 * NeutralPosition1sNurbs.h
 *
 *  Created on: 24.10.2013
 *      Author: zwang
 */

#ifndef _NEUTRAL_NURBS_POSITION_1S_H_
#define _NEUTRAL_NURBS_POSITION_1S_H_

#include "neutral_nurbs.h"

namespace MBSimFlexibleBody {
  
  class NeutralNurbsPosition1s : public MBSimFlexibleBody::NeutralNurbs {
    public:
      NeutralNurbsPosition1s(Element* parent_, std::vector<ContourPointData>& transContourPoints_, double nodeOffset_, double uMin_, double uMax_, int degU_, bool openStructure_);
      virtual ~NeutralNurbsPosition1s();
      virtual void update(ContourPointData &cp);
      virtual void updatePositionNormal(ContourPointData &cp);
      virtual void updatePositionFirstTangent(ContourPointData &cp);
      virtual void updatePositionSecondTangent(ContourPointData &cp);
    protected:
      virtual void buildNodelist();
  };

} /* namespace MBSimFlexibleBody */
#endif
