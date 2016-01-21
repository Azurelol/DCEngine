#include "ParticleSystem.h"

namespace DCEngine
{
	ParticleSystem::Particle::Particle(Vec2 velocity, Vec2 position, float lifetime)
		: mVelocity(velocity), mPosition(position), mLifeTime(lifetime)
	{

	}

	void ParticleSystem::Particle::Update(double dt)
	{
		mPosition.x += mVelocity.x * dt;
		mPosition.y += mVelocity.y * dt;
		mLifeTime -= dt;
	}

	ParticleSystem::ParticleSystem()
		:mEmissionRate(0), mTimer(0)
	{

	}

	void ParticleSystem::SetEmissionRate(float emissionRate)
	{
		mEmissionRate = emissionRate;
	}
	void ParticleSystem::AddParticle(float lifetime, Vec2 position, Vec2 velocity)
	{
		mParticleList.push_back(Particle(position, velocity, lifetime));
	}
	unsigned ParticleSystem::GetParticleCount(void)
	{
		return mParticleList.size();
	}
	void ParticleSystem::UpdateParticles(double dt)
	{
		if (mEmissionRate > 0)
		{
			if (mTimer == 0)
			{
				//AddParticle()
			}
			else
			{

			}
		}
		for (auto&& particle : mParticleList)
		{
			particle.Update(dt);
		}
		std::remove_if(mParticleList.begin(), mParticleList.end(),
			[](const Particle& p)
		{
			return p.mLifeTime <= 0;
		});
	}

}