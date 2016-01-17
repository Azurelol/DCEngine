#pragma once

namespace DCEngine
{
	class ParticleSystem
	{
	public:
		struct Particle
		{
			Particle(Vec2 velocity, Vec2 position, float lifetime);
			void Update(double dt);
			Vec2 mVelocity;
			Vec2 mPosition;
			float mLifeTime;
		};
		ParticleSystem();
		void AddParticle(float lifetime, Vec2 position, Vec2 velocity);
		unsigned GetParticleCount(void);
		void UpdateParticles(double dt);
	private:
		std::vector<Particle> mParticleList;
	};
}