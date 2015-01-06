/* Copyright (C) 2004-2009 MBSim Development Team
 * This library is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU Lesser General Public 
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version. 
 *  
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
 * Lesser General Public License for more details. 
 *  
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * Contact: rzander@users.berlios.de
 */

#ifndef _CONTOUR_PDATA_H_
#define _CONTOUR_PDATA_H_

#include "fmatvec/fmatvec.h"
#include "mbsim/frame.h"
#include <vector>

namespace MBSim {

  class Point;

  enum ContourParameterType { TRANSNODE, ROTNODE, CONTINUUM, EXTINTERPOL, FFRORIGIN };

  /**
   * \brief struct for data-management for single point on a contour to describe contact kinematics
   * \author Roland Zander
   * \date 2009-03-19 some comments (Thorsten Schindler)
   * \date 2009-04-02 Wn / Wt / WrOC deleted (Thorsten Schindler)
   * \date 2009-04-05 added specific constructors for arguments double and Vec (Schindler / Zander)
   * \date 2012-03-14 added ContourParameterType for staggered grid and modified constructor for argument int (Cebulla)
   */
  class ContourPointData {
    public:
      /**
       * \brief constructor
       */
      ContourPointData() : type(CONTINUUM), ID(0) {}
      ContourPointData(const double       &alpha_) : type(CONTINUUM), ID(0), alpha(1,fmatvec::INIT,alpha_) {}
      ContourPointData(const fmatvec::VecV &alpha_) : type(CONTINUUM), ID(0), alpha(alpha_) {}
      ContourPointData(const int  &id_, const ContourParameterType type_ = TRANSNODE) : type(type_), ID(id_) {}

      /**
       * \brief destructor
       */
      virtual ~ContourPointData() {}

      /* GETTER / SETTER */
      ContourParameterType& getContourParameterType() { return type; }
      const ContourParameterType& getContourParameterType() const { return type; }
      int& getNodeNumber() { return ID; }
      const int& getNodeNumber() const { return ID; }
      fmatvec::VecV& getLagrangeParameterPosition() { return alpha; }
      const fmatvec::VecV& getLagrangeParameterPosition() const { return alpha; }
      fmatvec::VecV& getLagrangeParameterVelocity() { return alphap; }
      const fmatvec::VecV& getLagrangeParameterVelocity() const { return alphap; }
      fmatvec::VecV& getInterpolationWeights() { return iWeights; }
      const fmatvec::VecV& getInterpolationWeights() const { return iWeights; }
      Frame& getFrameOfReference() { return cosy; }
      const Frame& getFrameOfReference() const { return cosy; }
      /***************************************************/

    private:
      /** 
       * \brief type of data representation: node, continuum, interpolation (extinterpol) 
       */
      ContourParameterType type;

      /** 
       * \brief ID of node or other discret interface within body -> FiniteElements
       */
      int ID;

      /**
       * \brief contour parameter(s)
       */
      fmatvec::VecV alpha;

      /**
       * \brief contour parameter(s) velocities
       */
      fmatvec::VecV alphap;

      /** 
       * \brief interpolation weights
       */
      fmatvec::VecV iWeights;

      /**
       * \brief list of nodes used in interpolation
       *
       * the (body specific) ID can be accessed using ->iPoint[NNumber]->getID();
       */
      std::vector<Point*> iPoints;

      /**
       * \brief accompanying frame
       */
      Frame cosy;
  };

}

#endif /* _CONTOUR_PDATA_H_ */

