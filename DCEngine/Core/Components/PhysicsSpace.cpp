/*****************************************************************************/
/*!
\file   PhysicSpaces.cpp
\author Blaine Reiner, Christian Sagel
\par    email: blaine.reiner@digipen.edu, c.sagel\@digipen.edu
\date   9/23/2015
\brief  The physics space hold the settings for all physical interactions within
a space. It also holds several containers of objects that require
physics integration and resolution, for collisions, velocity, etc...
@copyright Copyright 2015, DigiPen Institute of Technology. All rights reserved.
*/
/******************************************************************************/
#include "PhysicsSpace.h"
#include "../Systems/Physics/Physics.h"
#include "EngineReference.h"
#include "../../Core/Systems/Physics/Physics.h"
#include "../Resources/CollisionTable.h"

namespace DCEngine {
  namespace Components {

    /**************************************************************************/
    /*!
    @brief Provides the definition of this class to Zilch.
    @note This can only go in the translational unit (.cpp)
    */
    /**************************************************************************/
    #if(DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDefineType(PhysicsSpace, "PhysicsSpace", DCEngineCore, builder, type) {
      DCE_BINDING_COMPONENT_DEFINE_CONSTRUCTOR(PhysicsSpace);
      DCE_BINDING_DEFINE_RESOURCE_ATTRIBUTE(CollisionTable);
      DCE_BINDING_DEFINE_PROPERTY(PhysicsSpace, AllowSleep);
      DCE_BINDING_DEFINE_PROPERTY(PhysicsSpace, Mode2D);
      DCE_BINDING_DEFINE_PROPERTY(PhysicsSpace, Deterministic);
      DCE_BINDING_DEFINE_PROPERTY(PhysicsSpace, CollisionTable);
      DCE_BINDING_PROPERTY_SET_ATTRIBUTE(propertyCollisionTable, attributeCollisionTable);


      ZilchBindMethod(builder, type, &PhysicsSpace::CastRay, (CastResult (PhysicsSpace::*)(Ray &)), "CastRay", "ray");
      ZilchBindMethod(builder, type, &PhysicsSpace::CastRay, (CastResult(PhysicsSpace::*)(Ray &, CastFilter &)), "CastRay", "ray, filter");
      /*ZilchBindMethod(builder, type, &PhysicsSpace::CastRay, (CastResultsRange (PhysicsSpace::*)(Ray&, unsigned)), "CastRay", "ray, count");
      ZilchBindMethod(builder, type, &PhysicsSpace::CastRay, (CastResultsRange (PhysicsSpace::*)(Ray&, unsigned, CastFilter&)), "CastRay", "ray, count, filter");

      ZilchBindMethod(builder, type, &PhysicsSpace::CastSegment, (CastResultsRange(PhysicsSpace::*)(Vec3&, Vec3&, unsigned)), "CastSegment", "start, end, count");
      ZilchBindMethod(builder, type, &PhysicsSpace::CastSegment, (CastResultsRange(PhysicsSpace::*)(Vec3&, Vec3&, unsigned, CastFilter&)), "CastSegment", "start, end, count, filter");

      ZilchBindMethod(builder, type, &PhysicsSpace::CastAabb, ZilchNoOverload, "CastAabb", "center, size, count, filter");
      ZilchBindMethod(builder, type, &PhysicsSpace::CastSphere, ZilchNoOverload, "CastSphere", "center, radius, count, filter");
      ZilchBindMethod(builder, type, &PhysicsSpace::CastCollider, ZilchNoOverload, "CastCollider", "offset, testCollider, filter");*/
    }
    #endif


    /**************************************************************************/
    /*!
    \brief PhysicsSpace constructor.
    */
    /**************************************************************************/
    PhysicsSpace::PhysicsSpace(Entity& owner) : Component(std::string("PhysicsSpace"), owner) {
      CollisionTable = CollisionTable::Default();
    }

    /**************************************************************************/
    /*!
    @brief PhysicsSpace destructor.
    */
    /**************************************************************************/
    PhysicsSpace::~PhysicsSpace()
    {
      // Deregister from the Physics System
      Daisy->getSystem<Systems::Physics>()->DeregisterSpace(*this);
    }

    /**************************************************************************/
    /*!
    \brief Initializes the Physics Space. The presence of a physis space component
           signals to the Physics system that the space requires physics resolution.
    */
    /**************************************************************************/
    void PhysicsSpace::Initialize() {
      // Register this space to the physics system
      Daisy->getSystem<Systems::Physics>()->RegisterSpace(*this);
    }

    /**************************************************************************/
    /*!
    \brief Adds a GameObject with a 'RigidBody' component.
    \param A pointer to the 'GameObject'
    */
    /**************************************************************************/
    void PhysicsSpace::AddRigidBody(RigidBody& rigidbody)
    {
      RigidBodiesContainer.push_back(&rigidbody);
      //DCTrace << Owner()->getObjectName()
      //  << "::PhysicsSpace::AddRigidBody - " << rigidbody.Owner()->Name() << "\n";
    }

    /**************************************************************************/
    /*!
    \brief Remvoes a GameObject with a 'RigidBody' component.
    \param A pointer to the 'GameObject'
    */
    /**************************************************************************/
    void PhysicsSpace::RemoveRigidBody(RigidBody& rigidbody)
    {
      //DCTrace << Owner()->getObjectName()
      //  << "::PhysicsSpace::RemoveRigidBody - " << rigidbody.Owner()->Name() << "\n";
      RigidBody* ptr = &rigidbody;
      RigidBodiesContainer.erase(std::remove(RigidBodiesContainer.begin(),
        RigidBodiesContainer.end(), ptr),
        RigidBodiesContainer.end());
    }

    /**************************************************************************/
    /*!
    \brief Adds a GameObject with a 'BoxCollider' component.
    \param A pointer to the 'GameObject'
    */
    /**************************************************************************/
    void PhysicsSpace::AddCollider(Collider* collider)
    {
      CollidersContainer.push_back(collider);
      //DCTrace << Owner()->Name() << "::PhysicsSpace::AddCollider - "
      //  << collider->Owner()->Name() << "\n";
    }

    /**************************************************************************/
    /*!
    \brief Removes a GameObject with a 'RigidBody' component.
    \param A pointer to the 'GameObject'
    */
    /**************************************************************************/
    void PhysicsSpace::RemoveCollider(Collider* collider)
    {
      //DCTrace << Owner()->Name() << "::PhysicsSpace::RemoveCollider - "
      //  << collider->Owner()->Name() << "\n";
      //BoxCollider* ptr = collider;
      CollidersContainer.erase(std::remove(CollidersContainer.begin(),
        CollidersContainer.end(), collider)  ); // ,
        //CollidersContainer.end());
    }

    /**************************************************************************/
    /*!
    @brief Performs a ray cast and returns the first object that is collided with.
    @param ray A reference to the ray.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResult PhysicsSpace::CastRay(Ray & ray)
    {
      return Daisy->getSystem<Systems::Physics>()->CastRay(ray, this);
    }

    /**************************************************************************/
    /*!
    @brief Performs a filtered ray cast and returns the first object that is collided with.
    @param ray A reference to the ray.
    @param filter A filter...
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResult PhysicsSpace::CastRay(Ray & ray, CastFilter & filter)
    {
      return Daisy->getSystem<Systems::Physics>()->CastRay(ray, filter, this);
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the ray cast.            
    @param ray A reference to the ray.
    @param count The number of objects returned is based on the count passed in.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange PhysicsSpace::CastRay(Ray & ray, unsigned count)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the filtered ray cast.
    @param ray A reference to the ray.
    @param count The number of objects returned is based on the count passed in.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange PhysicsSpace::CastRay(Ray & ray, unsigned count, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the segment cast.
    @param start The starting point of the segment.
    @param end The ending point of the segment.
    @param count The number of objects returned is based on the count passed in.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange PhysicsSpace::CastSegment(Vec3 & start, Vec3 & end, unsigned count)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the filtered segment cast.
    @param start The starting point of the segment.
    @param end The ending point of the segment.
    @param count The number of objects returned is based on the count passed in.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange PhysicsSpace::CastSegment(Vec3 & start, Vec3 & end, unsigned count, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the filtered Aabb cast.
    @param center The origin.
    @param size The dimensions of the shape.
    @param count The number of objects returned is based on the count passed in.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange PhysicsSpace::CastAabb(Vec3 & center, Vec3 & size, unsigned count, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the results of the filtered sphere cast.
    @param center The origin.
    @param radius The radius of the sphere.
    @param count The number of objects returned is based on the count passed in.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange PhysicsSpace::CastSphere(Vec3 & center, float radius, unsigned count, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Cast out a collider to see whats it's colliding with.
    @param offset How much to offset the collider from its current position.
    @param testCollider A reference to the collider.
    @param filter The filter object.
    @return An object containing the results from the cast.
    */
    /**************************************************************************/
    CastResultsRange PhysicsSpace::CastCollider(Vec3 & offset, Collider & testCollider, CastFilter & filter)
    {
      return CastResultsRange();
    }

    /**************************************************************************/
    /*!
    @brief Returns the container of active RigidBodies.
    @return A container of GameObjects.
    */
    /**************************************************************************/
    RigidBodyContainer& PhysicsSpace::AllRigidBodies()
    {
      return RigidBodiesContainer;
    }
    /**************************************************************************/
    /*!
    @brief Returns the container of active Colliders.
    @return A container of GameObjects.
    */
    /**************************************************************************/
    ColliderContainer& PhysicsSpace::AllColliders()
    {
      return CollidersContainer;
    }    

  }
}