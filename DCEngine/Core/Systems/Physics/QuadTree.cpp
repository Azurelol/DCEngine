#include "QuadTree.h"
#include "../../Components/Transform.h"
#include "../../Precompiled/Precompiled.h"
#include "../../Objects/Entities/GameObject.h"


namespace DCEngine
{
	static bool BBox2BBox(QuadTree::BBox box1, QuadTree::BBox box2)
	{
		if (box1.max.x < box2.min.x) return false;
		if (box1.min.x > box2.max.x) return false;
		if (box1.max.y < box2.min.y) return false;
		if (box1.min.y > box2.max.y) return false;
		else return true;
	}
	QuadTree::BBox::BBox(Vec2 min, Vec2 max) : min(min), max(max)
	{

	}

	QuadTree::QuadTree(const std::vector<Components::Collider*>& collisionList, Vec2 min, Vec2 max, unsigned bucketSize)
	{
		bucketList.push_back(Quad(BBox(min, max)));
		for(auto collider: collisionList)
			bucketList.back().mBucket.push_back(collider);
		for (unsigned i = 0; i < bucketList.size(); ++i)
		{
			//if the bucket is too big
			if (bucketList[i].mBucket.size() < bucketSize)
				continue;
			BBox BoundingBox = bucketList[i].getBoudningBox();
			//if the bucket is not too small to split
			if((BoundingBox.max.x - BoundingBox.min.x > 1) && (BoundingBox.max.y - BoundingBox.min.y > 1))
			{
				std::vector<Quad> quads;
				//split
				bucketList[i].Split(quads);
				//erase the bucket we just split
				bucketList.erase(std::remove(bucketList.begin(), bucketList.end(), bucketList[i]),
					bucketList.end());
				//put in the new buckets
				for (auto quad : quads)
					bucketList.push_back(quad);
				i = -1;
			}
		}
	}

	static bool operator==(Systems::DetectionPairing& p1, Systems::DetectionPairing& p2)
	{
		if (p1.obj1 == p2.obj1 && p1.obj2 == p2.obj2)
			return true;
		else if (p1.obj1 == p2.obj2 && p1.obj2 == p2.obj1)
			return true;
		else return false;
	}
	std::vector<Systems::DetectionPairing> QuadTree::CreatePairs(void)
	{
		std::vector<Systems::DetectionPairing> pairs;
		//for each quad in the tree
		for (auto bucket : bucketList)
		{
			//that can be made into a pair
			if (bucket.mBucket.size() > 1)
			{
				//for each object in the bucket
				for (unsigned i = 0; i < bucket.mBucket.size(); ++i)
				{
					auto collider = bucket.mBucket[i];
					auto rigidbody = collider->Owner()->getComponent<Components::RigidBody>();
					//if it moves
					if (rigidbody)
					{
						//pair it with every other object in the bucket
						for (unsigned j = 0; j < bucket.mBucket.size(); ++j)
						{
							//excpet itself
							if (bucket.mBucket[j] != collider)
							{
								Systems::DetectionPairing pair;
								pair.obj1 = dynamic_cast<GameObject*>(bucket.mBucket[i]->Owner());
								pair.obj2 = dynamic_cast<GameObject*>(bucket.mBucket[j]->Owner());
								//filter uninitialized because i don't know what it is or what it does
								bool addpair = true;
								for (auto p : pairs)
								{
									if (p == pair)
									{
										addpair = false;
										break;
									}
								}
								if(addpair)
									pairs.push_back(pair);
							}
						}
					}
				}
			}
		}
		return pairs;
	}

	QuadTree::Quad::Quad(BBox box) : mBBox(box)
	{

	}
	QuadTree::BBox QuadTree::Quad::getBoudningBox(void)
	{
		return mBBox;
	}
	bool QuadTree::Quad::operator==(const Quad & rhs)
	{
		if (this == &rhs)
			return true;
		else return false;
	}
	QuadTree::Quad & QuadTree::Quad::operator=(const Quad &rhs)
	{
		mBucket = rhs.mBucket;
		mBBox = rhs.mBBox;
		return *this;
	}
	void QuadTree::Quad::Split(std::vector<Quad>& output)
	{
		Vec2 O(mBBox.min.x + (mBBox.max.x - mBBox.min.x) / 2, mBBox.min.y + (mBBox.max.y - mBBox.min.y) / 2);
		Quad q1(BBox(O, mBBox.max)),
			q2(BBox(Vec2(mBBox.min.x, O.y), Vec2(O.x, mBBox.max.y))),
			q3(BBox(mBBox.min, O)),
			q4(BBox(Vec2(O.x, mBBox.min.y), Vec2(mBBox.max.x, O.y)));
		
		for (auto collider : mBucket)
		{
			BBox colliderBBox(Vec2(0,0), Vec2(0,0));
			Components::Transform* transform = collider->Owner()->getComponent<Components::Transform>();
			Components::BoxCollider* box = collider->Owner()->getComponent<Components::BoxCollider>();
			if (box)
			{
				colliderBBox = CreateBoundingBox(box);
			}
			else
			{
				Components::CircleCollider* circle = collider->Owner()->getComponent<Components::CircleCollider>();
				if (circle)
				{
					colliderBBox = CreateBoundingBox(circle);
				}
			}

			if (BBox2BBox(colliderBBox, q1.mBBox))
				q1.mBucket.push_back(collider);
			if (BBox2BBox(colliderBBox, q2.mBBox))
				q2.mBucket.push_back(collider);
			if (BBox2BBox(colliderBBox, q3.mBBox))
				q3.mBucket.push_back(collider);
			if (BBox2BBox(colliderBBox, q4.mBBox))
				q4.mBucket.push_back(collider);
		}
		if(q1.mBucket.size() > 1)
			output.push_back(q1);
		if (q2.mBucket.size() > 1)
			output.push_back(q2);
		if (q3.mBucket.size() > 1)
			output.push_back(q3);
		if (q4.mBucket.size() > 1)
			output.push_back(q4);
	}

	QuadTree::BBox QuadTree::Quad::CreateBoundingBox(Components::BoxCollider* box)
	{
		Components::Transform* transform = box->Owner()->getComponent<Components::Transform>();
		std::vector<Vec2> verts;
		Vec2 topL, topR, botL, botR;
		Vec3 scale = box->getColliderScale();
		float rot = (3.14159265359f / 180.0f) * transform->Rotation.z;
		Vec3 translation = transform->Translation;

		topL.x = translation.x + box->getOffset().x + (scale.y * -sin(rot)) + (-scale.x * cos(rot));
		topL.y = translation.y + box->getOffset().y + (scale.y * cos(rot)) + (-scale.x * sin(rot));

		verts.push_back(topL);

		topR.x = translation.x + box->getOffset().x + (scale.y * -sin(rot)) + (scale.x * cos(rot));
		topR.y = translation.y + box->getOffset().y + (scale.y * cos(rot)) + (scale.x * sin(rot));

		verts.push_back(topR);

		botL.x = translation.x + box->getOffset().x + (-scale.y * -sin(rot)) + (-scale.x * cos(rot));
		botL.y = translation.y + box->getOffset().y + (-scale.y * cos(rot)) + (-scale.x * sin(rot));

		verts.push_back(botL);
		
		botR.x = translation.x + box->getOffset().x + (-scale.y * -sin(rot)) + (scale.x * cos(rot));
		botR.y = translation.y + box->getOffset().y + (-scale.y * cos(rot)) + (scale.x * sin(rot));

		verts.push_back(botR);

		BBox bbox(Vec2(1000, 1000), Vec2(-1000, -1000));
		for (auto vert : verts)
		{
			if (vert.x < bbox.min.x)
				bbox.min.x = vert.x;
			if (vert.x > bbox.max.x)
				bbox.max.x = vert.x;
			if (vert.y < bbox.min.y)
				bbox.min.y = vert.y;
			if (vert.y > bbox.max.y)
				bbox.max.y = vert.y;
		}
		return bbox;
	}

	QuadTree::BBox QuadTree::Quad::CreateBoundingBox(Components::CircleCollider* circle)
	{
		Components::Transform* transform = circle->Owner()->getComponent<Components::Transform>();
		Vec3 translation = transform->Translation;
		Vec3 scale = transform->Scale * circle->getRadius();
		Vec3 offset = circle->getOffset();

		return BBox(Vec2(translation.x - scale.x + offset.x, translation.y - scale.y + offset.y),
			Vec2(translation.x + scale.x + offset.x, translation.y + scale.y + offset.y));
	}
}