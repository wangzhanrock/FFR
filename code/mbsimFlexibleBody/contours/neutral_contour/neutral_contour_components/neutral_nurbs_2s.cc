/*
 * neutral_nurbs_2s.cc
 *
 *  Created on: 03.12.2013
 *      Author: zwang
 */

#include <config.h>

#include "neutral_nurbs_2s.h"

namespace MBSimFlexibleBody {


  NeutralNurbs2s::NeutralNurbs2s(Element* parent_, fmatvec::GeneralMatrix<ContourPointData>& ContourPoints_, double nodeOffset_, int degU_, int degV_, bool openStructure_):
      surface(), parent(parent_), contourPoints(ContourPoints_), nodeOffset(nodeOffset_), numOfNodesU(ContourPoints_.rows()), numOfNodesV(ContourPoints_.cols()), Nodelist(ContourPoints_.rows(), ContourPoints_.cols()), degU(degU_), degV(degV_), openStructure(openStructure_){

  }

  NeutralNurbs2s:: ~NeutralNurbs2s(){

  }

  void NeutralNurbs2s::computeCurve(bool update){
    buildNodelist();

//    if (update)
//      surface.update(Nodelist);
//    else {
//      if (openStructure) {
//        surface.globalInterp(Nodelist, degV, degU, true);
//      }
//      else {
//        surface.globalInterpClosedU(Nodelist, degV, degU, true);
//      }
//    }
    if (openStructure) {
      surface.globalInterp(Nodelist, degU, degV);
    }else{
      surface.globalInterpClosedU(Nodelist, degU, degV);
    }
  }
}
