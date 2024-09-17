#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem()
{
	m_ParticlePool.resize(1000);
} 

void ParticleSystem::OnUpdate(Forge::Timestep time)
{
	for (Particle& particle : m_ParticlePool) {
		if (!particle.IsActive) {
			continue;
		}

		if (particle.LifetimeRemaining <= 0) {
			particle.IsActive = false;
			continue;
		}

		particle.LifetimeRemaining -= time;
		particle.Position += particle.Velocity * (float)time;
		particle.Rotation += 0.001f * time; 
	}
}

void ParticleSystem::OnRender(Forge::OrthographicCamera& camera)
{

	Forge::Renderer2D::BeginScene(camera);

	for (Particle& particle : m_ParticlePool) {
		if (!particle.IsActive) {
			continue;
		}

		if (particle.LifetimeRemaining <= 0) {
			particle.IsActive = false;
			continue;
		}

		float life = particle.LifetimeRemaining / particle.Lifetime;
		glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
		color.a = color.a * life;

		float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

		glm::vec3 position = { particle.Position,0.2f };

		Forge::Renderer2D::DrawRotatedQuad(position, glm::vec2(size), particle.Rotation, color);
	}

	Forge::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
	Particle& particle = m_ParticlePool[m_ParticleIndex];
	particle.IsActive = true;
	particle.Rotation = Forge::Random::Float() * 90;
	particle.Position = particleProps.Position;

	particle.Velocity = particleProps.Velocity;
	particle.Velocity.x += particleProps.VelocityVariation.x * (Forge::Random::Float() - 0.5f);
	particle.Velocity.x += particleProps.VelocityVariation.y * (Forge::Random::Float() - 0.5f);

	particle.Lifetime = particleProps.Lifetime;
	particle.LifetimeRemaining = particleProps.Lifetime;

	particle.ColorBegin = particleProps.ColorBegin;
	particle.ColorEnd = particleProps.ColorEnd;

	particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Forge::Random::Float() -0.5f);
	particle.SizeEnd = particleProps.SizeEnd;

	m_ParticleIndex = --m_ParticleIndex % m_ParticlePool.size();

}
