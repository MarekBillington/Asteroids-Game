#ifndef __PARTICLEEMITTER_H__
#define __PARTICLEEMITTER_H__

#include <vector>

class Particle;
class Vector2D;
class BackBuffer;


class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void addParticle(Particle* particle);
	void Process();
	void Draw(BackBuffer& backBuffer);

private:

	std::vector<Particle*> particleContainer;

};

#endif // __PARTICLEEMITTER_H__

