#pragma once
#include "Forge.h"

struct ParticleProps {
	glm::vec2 Position;
	glm::vec2 Velocity, VelocityVariation;
	glm::vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float Lifetime;
};

class ParticleSystem {
public:
	ParticleSystem();
	~ParticleSystem() {};

	void OnUpdate(Forge::Timestep time);
	void OnRender(Forge::OrthographicCamera& camera);
	void Emit(const ParticleProps& particleProps );

private:
	struct Particle {
		glm::vec2 Position;
		float Rotation;
		glm::vec2 Velocity;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd;
		float Lifetime = 1.0f, LifetimeRemaining = 0.0f;
		bool IsActive = false;
	};

	std::vector<Particle>m_ParticlePool;
	uint32_t m_ParticleIndex = 999; 

};
