#pragma once

#include "../../Systems/Physics/Physics.h"
#include "../../Components/CircleCollider.h"

namespace DCEngine
{
	class QuadTree
	{
	public:
		struct BBox
		{
			BBox(Vec2, Vec2);
			Vec2 min, max;
		};
		QuadTree(const std::vector<Components::Collider*>&, Vec2, Vec2, unsigned);
		std::vector<Systems::DetectionPairing> CreatePairs(void);
	private:
		class Quad
		{
		public:
			Quad(BBox);
			void Split(std::vector<Quad>&);
			BBox getBoudningBox(void);
			bool operator==(const Quad&);
			Quad& operator=(const Quad&);

			std::vector<Components::Collider*> mBucket;
		private:
			BBox CreateBoundingBox(Components::BoxCollider*);
			BBox CreateBoundingBox(Components::CircleCollider*);

			//std::vector<Quad>& mBucketList;
			BBox mBBox;
		};
		std::vector<Quad> bucketList;
	};
}