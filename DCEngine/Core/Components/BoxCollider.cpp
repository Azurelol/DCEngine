#include "BoxCollider.h"
#include "PhysicsSpace.h"
#include "EngineReference.h"
#include "DebugDrawReference.h"

namespace DCEngine {

  /**************************************************************************/
  /*!
  @brief Provides the definition of this class to Zilch.
  @note This can only go in the translational unit (.cpp)
  */
  /**************************************************************************/
  #if(DCE_USE_ZILCH_INTERNAL_BINDING)
  ZilchDefineType(BoxCollider, "BoxCollider", DCEngineCore, builder, type) {
    // Constructor / Destructor
    ZilchBindConstructor(builder, type, BoxCollider, "owner", Entity&);
    ZilchBindDestructor(builder, type, BoxCollider);
    // Properties
    ZilchBindProperty(builder, type, &BoxCollider::getSize, &BoxCollider::setSize, "Size");
    ZilchBindProperty(builder, type, &BoxCollider::getOffset, &BoxCollider::setOffset, "Offset");
    ZilchBindProperty(builder, type, &BoxCollider::getGhost, &BoxCollider::setGhost, "Ghost");
    ZilchBindProperty(builder, type, &BoxCollider::getSendsEvents, &BoxCollider::setSendsEvents, "SendsEvents");
    ZilchBindProperty(builder, type, &BoxCollider::getIsDrawingCollider, &BoxCollider::setIsDrawingCollider, "IsDrawingCollider");
  }
  #endif


	void BoxCollider::Initialize()
	{
		auto owner = dynamic_cast<GameObject*>(Owner());
		// Store a reference to the Transform Component
		TransformComponent = owner->getComponent<Transform>();
		// Subscribe this physics component to the physics space
		SpaceRef->getComponent<PhysicsSpace>()->AddCollider(owner);

		// If this component needs to draw the collider with debug draw,
		// subscribe to LogicUpdate events so it can draw
		if (IsDrawingCollider)
			Connect(SpaceRef, Events::LogicUpdate, BoxCollider::OnLogicUpdateEvent);

		// Make sure the collider scales with the transform's scale

	}

	void BoxCollider::DrawCollider()
	{
		auto debugScale = Vec2(getColliderScale().x,
			getColliderScale().x);

		SpaceRef->getComponent<GraphicsSpace>()->DrawRectangle(TransformComponent->Translation + Offset,
			debugScale.x, //TransformComponent->Scale.x * Size.x,
			debugScale.y, //TransformComponent->Scale.y * Size.y, 
			Vec4(1, 0, 0, 1)); // Red
	}
  
	/**************************************************************************/
	/*!
	\brief  CollisionEvents
	*/
	/**************************************************************************/
	void BoxCollider::OnCollisionStartedEvent(Events::CollisionStarted * event)
	{
		// Move the GameObject??
	}

	void BoxCollider::OnCollisionEndedEvent(Events::CollisionEnded * event)
	{

	}

	/**************************************************************************/
	/*!
	@brief  The true scale of the collider is the size set times the scale
	of the component.
	@note   So if an object's transform scale was set at 1 while the collider's
	size was set at 2, the collider would be twice the size of the object.
	*/
	/**************************************************************************/
	Vec3 BoxCollider::getColliderScale()
	{
		return (Size * TransformComponent->Scale);
	}

	void BoxCollider::OnLogicUpdateEvent(Events::LogicUpdate* event)
	{
		DrawCollider();
	}

  /**************************************************************************/
  /**************************************************************************!
                                  PROPERTIES

  /**************************************************************************/
	Vec3 BoxCollider::getSize(void)
	{
		return this->Size;
	}

  void BoxCollider::setSize(Vec3 size)
  {
    Size = size;
  }

	Vec3 BoxCollider::getOffset(void)
	{
		return this->Offset;
	}

  void BoxCollider::setOffset(Vec3 offset)
  {
    this->Offset = offset;
  }

	bool BoxCollider::getGhost(void)
	{
		return this->Ghost;
	}

  void BoxCollider::setGhost(bool ghost)
  {
    this->Ghost = ghost;
  }

	bool BoxCollider::getSendsEvents(void)
	{
		return this->SendsEvents;
	}

  void BoxCollider::setSendsEvents(bool sends)
  {
    this->SendsEvents = sends;
  }

  bool BoxCollider::getIsDrawingCollider(void)
  {
    return this->IsDrawingCollider;
  }

  void BoxCollider::setIsDrawingCollider(bool isDrawing)
  {
    this->IsDrawingCollider = isDrawing;
  }

}