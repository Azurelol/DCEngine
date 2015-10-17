/*****************************************************************************/
/*!
@file   Orientation.h
@author Blaine Reiner, Christian Sagel
@par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
@date   10/13/2015
@brief  The orientation component is an utility component that helps make the
        manipulation of the transform of an object easier. It can be particularly
        useful when the object has its own sense of forward. By using the
        orientation component, the object can then be easily manipulated
        relatively to the object's forward instead of being restricted to the 
        world axis.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

  class Orientation : public Component {
  public:
    
    Real AbsoluteAngle; //!< The angle of the object about the up vector.
    //Quaternion Rotation; //!< The rotation that this object defines. Useful to align the object's
                    // bases with the world bases (using the inverse rotation)
    Real3 LocalForward;
    Real3 LocalUp;
    Real3 LocalRight;
    Real3 WorldForward = Real3(0, 1, 0);
    Real3 WorldUp = Real3(0, 1, 0);
    Real3 WorldRight = Real3(0, 0, 1);
    //Quaternion WorldRotation;

    /* Look At */
    void LookAtPoint(Real3 point);
    void LookAtPointWithUp(Real3 point);
    void LookAtDirection(Real3 direction);
    void LookAtDirectionWithUp(Real3 direction);
    void LookAtUp(void);

    /* Get LookAt */
    Real3 GetLookAtPointRotation(void);
    Real3 GetLookAtDirectionRotation(void);
    Real3 GetLookAtPointWithUpRotation(void);
    Real3 GetLookAtDirectionWithUpRotation(void);

    float GetVectorAngle(Real3 vector);

    /* Initialize */
    Orientation(Entity& owner) : Component(std::string("Orientation"), owner) {}
    void Initialize();
    virtual void Serialize(Json::Value& root);
    virtual void Deserialize(Json::Value& root);

  private:
  };
}