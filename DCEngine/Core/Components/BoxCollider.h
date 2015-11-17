/******************************************************************************/
/*!
@file   BoxCollider.h
@author Blaine Reiner, Christian Sagel
@par    email: c.sagel\@digipen.edu
@date   9/23/2015
@brief  The BoxCollider component gives a physical representation of a bounding
box in world space so objects can experience collisions with one
another. This component interacts directly with the physics system.
*/
/******************************************************************************/
#pragma once
#include "ComponentReference.h"

namespace DCEngine {

	class Transform;
	class Physics;
	class BoxCollider : public Component {
	public:
		friend class Physics;

    #if (DCE_USE_ZILCH_INTERNAL_BINDING)
    ZilchDeclareDerivedType(BoxCollider, Component);
    #endif

    /* Variables */
		Vec3 Size = Vec3(1, 1, 1);
		Vec3 Offset = Vec3(0, 0, 0);
		Boolean Ghost = false;
		Boolean SendsEvents = true;
		Boolean IsDrawingCollider = false;
    
    /* Properties */
		Vec3 getSize(void);
    void setSize(Vec3);
		Vec3 getOffset(void);
    void setOffset(Vec3);
		bool getGhost(void);
    void setGhost(bool);
		bool getSendsEvents(void);
    void setSendsEvents(bool);
    bool getIsDrawingCollider(void);
    void setIsDrawingCollider(bool);

    // @todo For now it's a string, it will be later a tagged property so there's a dropdown menu in the editor
    String getCollisionGroup() const;
    void setCollisionGroup(String);

		// These should be private!
    BoxCollider(Entity& owner);
    ~BoxCollider();
		void Initialize();
    Vec3 getColliderScale();

	private:

    Transform* TransformComponent;
    String CollisionGroup;

    /* Events */
    void OnLogicUpdateEvent(Events::LogicUpdate* event);
    void OnCollisionStartedEvent(Events::CollisionStarted* event);
    void OnCollisionEndedEvent(Events::CollisionEnded* event);
    /* Methods */
    void DrawCollider();

		/* Variables */


	};

}
