#include "ParticleEmitter.h"
#include "Particle.h"
#include "Vector2D.h"
#include "backbuffer.h"

ParticleEmitter::ParticleEmitter()
{
}

ParticleEmitter::~ParticleEmitter()
{
}

void
ParticleEmitter::addParticle(Particle* particle)
{
	particleContainer.push_back(particle);
}

void
ParticleEmitter::Process()
{


	for (int i = 0; i < particleContainer.size(); i++)
	{
		Particle* particle = particleContainer.at(i);
		particle->Process();

		if (particle->isDead()){
			delete particle;
			particleContainer.erase(particleContainer.begin() + i);
		}

	}

}

void
ParticleEmitter::Draw(BackBuffer& backBuffer)
{
	for each(Particle* particle in particleContainer)
	{
		particle->Draw(backBuffer);
	}
}