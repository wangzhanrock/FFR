/*
 * contour_1s_neutral_linear_external_FFR.cc
 *
 *  Created on: 25.10.2013
 *      Author: zwang
 */
#include <config.h>
#include "contour_1s_neutral_linear_external_FFR.h"
#include "mbsimFlexibleBody/utils/cardan.h"
#include "mbsimFlexibleBody/flexible_body/flexible_body_linear_external_ffr.h"
#include "mbsim/utils/rotarymatrices.h"

namespace MBSimFlexibleBody {
  
  Contour1sNeutralLinearExternalFFR::Contour1sNeutralLinearExternalFFR(const std::string &name_, FlexibleBodyLinearExternalFFR* parent_, std::vector<int> transNodes_, double nodeOffset_, double uMin_, double uMax_, int degU_, bool openStructure_):
      Contour1sNeutralFactory(name_), transNodes(transNodes_), nodeOffset(nodeOffset_), numOfTransNodes(transNodes_.size()), uMin(uMin_), uMax(uMax_), degU(degU_), openStructure(openStructure_), NP(NULL), NLP(NULL), NV(NULL), qSize(0) {

    parent_->addContour(this);
  }
  
  Contour1sNeutralLinearExternalFFR::~Contour1sNeutralLinearExternalFFR() {
    delete NP;
    delete NLP;
    delete NV;

    NP = NULL;
    NLP = NULL;
    NV = NULL;
  }

  NeutralNurbsVelocity1s* Contour1sNeutralLinearExternalFFR::createNeutralVelocity() {
    return new NeutralNurbsVelocity1s(parent, transContourPoints, nodeOffset, uMin, uMax, degU, openStructure);
  }

  NeutralNurbsPosition1s* Contour1sNeutralLinearExternalFFR::createNeutralPosition() {
    return new NeutralNurbsPosition1s(parent, transContourPoints, nodeOffset, uMin, uMax, degU, openStructure);
  }

  NeutralNurbsLocalPosition1s* Contour1sNeutralLinearExternalFFR::createNeutralLocalPosition() {
    return new NeutralNurbsLocalPosition1s(parent, transContourPoints, nodeOffset, uMin, uMax, degU, openStructure);
  }

  void Contour1sNeutralLinearExternalFFR::createNeutralModeShape() {
    for (int k = 0; k < qSize - 6; k++) {
      curveModeShape.push_back(NurbsCurve());
    }

    MatVx3 Nodelist(numOfTransNodes, NONINIT);
    for (int k = 0; k < qSize - 6; k++) {
      for (int i = 0; i < numOfTransNodes; i++) {
        Nodelist.set(i, trans((static_cast<FlexibleBodyLinearExternalFFR*>(parent))->getModeShapeVector(transNodes.at(i),k)));
      }
      if (openStructure) {
        curveModeShape.at(k).globalInterp(Nodelist,uMin, uMax, degU, false); // calculate once, as the mode shape is constant.
      }
      else {
        curveModeShape.at(k).globalInterpClosed(Nodelist, uMin, uMax, degU, false); // calculate once, as the mode shape is constant.
      }
    }
  }

  void Contour1sNeutralLinearExternalFFR::init(MBSim::InitStage stage) {

    if (stage == resize) {
      // construct contourPoint for translation nodes
      nodes.reserve(numOfTransNodes);
      transContourPoints.reserve(numOfTransNodes);
//      rotContourPoints.reserve(numOfRotNodes);

//        nodeOffset = (static_cast<FlexibleBodyContinuum<double>*>(parent))->getNodeOffset();  // TODO change to be user set value

    }
    else if (stage == worldFrameContourLocation) {
        R->getOrientation() = (static_cast<FlexibleBodyLinearExternalFFR*>(parent))->getFrameOfReference()->getOrientation();
        R->getPosition() = (static_cast<FlexibleBodyLinearExternalFFR*>(parent))->getFrameOfReference()->getPosition();
    }
    else if (stage == unknownStage) { //TODO: Actually for the calculate Initial values in the contact search it is necessary to call the following functions before (even though they also just compute initial values)

      qSize = (static_cast<FlexibleBodyLinearExternalFFR*>(parent))->getqSize();

      for (int i = 0; i < numOfTransNodes; i++)
        transContourPoints.push_back(ContourPointData(transNodes.at(i)));

      NP = createNeutralPosition();
      NLP = createNeutralLocalPosition();
      NV = createNeutralVelocity();
      createNeutralModeShape();

      NP->computeCurve(false); // the first time call the computeCurveVelocity, the flag should be false
      NLP->computeCurve(false);
      NV->computeCurve(false);

      // TODO: check this!!!
      Vec u(NV->getuVec());
      for (int i = 0; i < u.size() - degU; i++)
        nodes.push_back(u(i));
    }

    Contour::init(stage);
  }

  void Contour1sNeutralLinearExternalFFR::updateKinematicsForFrame(MBSim::ContourPointData &cp, MBSim::FrameFeature ff) {
    if (ff == position || ff == position_cosy || ff == all)
      NP->update(cp);
    if (ff == velocity || ff == velocity_cosy || ff == velocities || ff == velocities_cosy || ff == all)
      NV->update(cp);
    ContourPointData FFR(0, FFRORIGIN);
    if (ff == angularVelocity || ff == velocities || ff == velocities_cosy || ff == all) {
      static_cast<FlexibleBodyLinearExternalFFR*>(parent)->updateKinematicsForFrame(FFR, angularVelocity);
      cp.getFrameOfReference().setAngularVelocity(FFR.getFrameOfReference().getAngularVelocity());
    }
    if (ff == normal || ff == cosy || ff == position_cosy || ff == velocity_cosy || ff == velocities_cosy || ff == all)
      NP->updatePositionNormal(cp);
    if (ff == firstTangent || ff == cosy || ff == position_cosy || ff == velocity_cosy || ff == velocities_cosy || ff == all)
      NP->updatePositionFirstTangent(cp);
    if (ff == secondTangent || ff == cosy || ff == position_cosy || ff == velocity_cosy || ff == velocities_cosy || ff == all)
      NP->updatePositionSecondTangent(cp);

    if (ff == angle){ // only for opmbvBody visualization
      SqrMat3 ALocal(INIT, 0);
      NP->updatePositionNormal(cp);
      NP->updatePositionFirstTangent(cp);
      NP->updatePositionSecondTangent(cp);
      ALocal = cp.getFrameOfReference().getOrientation();
      cp.getFrameOfReference().setAnglesOfOrientation(AIK2Cardan(ALocal));
    }
  }

  void Contour1sNeutralLinearExternalFFR::updateJacobiansForFrame(MBSim::ContourPointData &cp, int j) {

    /******************************************************************  Jacobian of Translation  *******************************************************************************/
    Mat3xV Jacobian_trans(qSize, INIT, 0.);
    // translational DOF
    Jacobian_trans.set(Index(0, 2), Index(0, 2), SqrMat(3, EYE));

    // rotational DOF
    SqrMat3 A = static_cast<FlexibleBodyLinearExternalFFR*>(parent)->getOrientationOfFFR();
    SqrMat3 G_bar = static_cast<FlexibleBodyLinearExternalFFR*>(parent)->getGBarOfFFR();
    NLP->update(cp);
    Vec3 u_bar = cp.getFrameOfReference().getLocalPosition();
    Jacobian_trans.set(Index(0, 2), Index(3, 5), -A * tilde(u_bar) * G_bar);

    // elastic DOF
    Mat3xV modeShapeMatrix(qSize - 6, NONINIT);
    double position = cp.getLagrangeParameterPosition()(0);
    for (int k = 0; k < qSize - 6; k++) {
      Vec3 temp = curveModeShape.at(k).pointAt(position);
      modeShapeMatrix.set(k,temp);
    }

    Jacobian_trans.set(Index(0, 2), Index(6, qSize - 1), A * modeShapeMatrix);

    SqrMat3 wRA = (static_cast<FlexibleBodyLinearExternalFFR*>(parent))->getFrameOfReference()->getOrientation();
    cp.getFrameOfReference().setJacobianOfTranslation(wRA * Jacobian_trans);

    /******************************************************************  Jacobian of Rotation  *******************************************************************************/
    Mat3xV Jacobian_rot(qSize, INIT, 0.);
    Jacobian_rot.set(Index(0, 2), Index(3, 5), A * G_bar);
    cp.getFrameOfReference().setJacobianOfRotation(wRA * Jacobian_rot);

  }
  
  void Contour1sNeutralLinearExternalFFR::updateStateDependentVariables(double t) {
    // TODO: is this function called by the integrator at timestep???
    NP->computeCurve(true);   // the first time call the computeCurveVelocity, the flag should be false
    NLP->computeCurve(true);
    NV->computeCurve(true);

    Contour::updateStateDependentVariables(t);

    // debug

//    cout << "****************Velocity****************" << endl;
//    double num = 26;
//    Mat TestVelocity(num, 3, INIT);
//    Mat TestPosition(num, 3, INIT);
//    Mat TestLocalPosition(num, 3, INIT);
//    //Mat TestDotAngle(num, 3, INIT);
//
//    for (int i = 0; i < num; i++ ) {
//      VecV alpha(3, INIT, 0);
//      alpha(0) = i * (uMax - uMin)/(num -1);
//      ContourPointData cpTest(alpha);
////      cout << "alpha: " << alpha << endl;
//      //cout << "position:" << cpTest.getLagrangeParameterPosition() << endl;
//      NP->update(cpTest);
//      NLP->update(cpTest);
//      NV->update(cpTest);
//
//      Vec p = cpTest.getFrameOfReference().getPosition();
//      Vec lp = cpTest.getFrameOfReference().getLocalPosition();
//      Vec v = cpTest.getFrameOfReference().getVelocity();
//
//      //cout << i << " : " << a << endl << endl;
//      TestPosition(Index(i,i), Index(0,2)) = p.T();
//      TestLocalPosition(Index(i,i), Index(0,2)) = lp.T();
//      TestVelocity(Index(i,i), Index(0,2)) = v.T();
//
//    }
//
//    cout << "positionCurve" << TestPosition << endl << endl;
//    cout << "localPositionCurve" << TestPosition << endl << endl;
//    cout << "velocityCurve" << TestVelocity << endl << endl;

  }
} /* namespace MBSimFlexibleBody */
