#include "particle_emitter.h"
#include <cstdlib> // for rand()
#include <chrono>

static inline float _getRandom(float percent);

ParticleEmitter::ParticleEmitter()
{
    this->position = glm::vec2(0.0f);
    this->velocity = glm::vec2(0.0f);
    this->emitterLife = 0.0f;
    this->emitterInitLife = 0.0f;
    this->_isActive = true;
    this->particleInitialSpeed = 0.0f;
    this->particleInitialSize = 0.0f;
    this->particleInitialLife = 0.0f;
    this->particlesPerSecond = 0;
    this->emitterGravity = 0.0f;
    this->emitmode = EmitMode_Continous;
    this->speedRandom = 0.0f;
    this->sizeRandom = 0.0f;
    this->alphaRandom = 0.0f;
    this->lifeRandom = 0.0f;
    this->airResistance = 0.0f;
    this->startColor = glm::vec4(1.0f);
    this->deltaColor = glm::vec4(1.0f);
}

const std::list<Particle> &ParticleEmitter::getParticles() const
{
    return this->particles;
}

bool ParticleEmitter::isAlive() const
{
    return this->_isActive;
}

void ParticleEmitter::update(float deltaTime)
{
    if (this->_isActive)
    {
        this->position += deltaTime * this->velocity;
        this->velocity -= glm::normalize(this->velocity) * deltaTime * this->airResistance;
        this->velocity.y -= deltaTime * this->emitterGravity;
        this->startColor += (deltaTime * this->deltaColor / this->emitterInitLife);
        this->emitterLife -= deltaTime;
        if (this->emitterLife <= 0.0f)
        {
            this->_isActive = false;
        }

        // 发射粒子
        if (this->emitmode == EmitMode_Continous)
        {
            static float emissionAccumulator = 0.0f;
            emissionAccumulator += this->particlesPerSecond * deltaTime;
            while (emissionAccumulator >= 1.0f)
            {
                emitParticle();
                emissionAccumulator -= 1.0f;
            }
        }
        else
        {
            for (int i = 0; i < this->particlesPerSecond; i++)
            {
                emitParticle();
            }
            this->_isActive = false;
        }
    }

    // 更新粒子
    for (auto it = particles.begin(); it != particles.end();)
    {
        // glm::vec2 temp_velocity = it->getVelocity();
        // it->setVelocity(glm::vec2(temp_velocity.x, temp_velocity.y - deltaTime * this->gravity));
        it->update(deltaTime);
        if (!it->isAlive())
        {
            it = particles.erase(it);
        }
        else
        {
            it++; // 没擦除时自增迭代器
        }
    }
}

void ParticleEmitter::draw(Renderer &renderer)
{
    // 更新粒子
#if 0
    for (auto it = particles.begin(); it != particles.end(); ++it)
    {
        if (it->isAlive())
        {
            it->draw(renderer);
        }
    }
#else
    // 提取粒子数据
    static std::vector<glm::vec2> offsets;
    static std::vector<float> scales;
    static std::vector<glm::vec4> colors;

    offsets.resize(0);
    scales.resize(0);
    colors.resize(0);

    for (const auto &particle : particles)
    {
        offsets.push_back(particle.getPosition());
        scales.push_back(particle.getSize());
        colors.push_back(particle.getColor());
    }
    renderer.DrawCircleInstance(offsets, scales, colors);
#endif
}

void ParticleEmitter::setPosition(glm::vec2 pos)
{
    this->position = pos;
}
void ParticleEmitter::setVelocity(glm::vec2 vec)
{
    this->velocity = vec;
}
void ParticleEmitter::setParInitSpeed(float speed)
{
    this->particleInitialSpeed = speed;
}
void ParticleEmitter::setParInitSize(float size)
{
    this->particleInitialSize = size;
}
void ParticleEmitter::setParInitLife(float life)
{
    this->particleInitialLife = life;
}
void ParticleEmitter::setParPerSecond(int count)
{
    this->particlesPerSecond = count;
}

void ParticleEmitter::setEmitterLife(float life)
{
    this->emitterLife = life;
    this->emitterInitLife = life;
}

void ParticleEmitter::setEmitterGravity(float _gravity)
{
    this->emitterGravity = _gravity;
}

void ParticleEmitter::setEmitMode(EmitMode mode)
{
    this->emitmode = mode;
}

void ParticleEmitter::setSpeedRandom(float speedRandom)
{
    this->speedRandom = speedRandom;
}

void ParticleEmitter::setSizeRandom(float sizeRandom)
{
    this->sizeRandom = sizeRandom;
}

void ParticleEmitter::setAlphaRandom(float alphaRandom)
{
    this->alphaRandom = alphaRandom;
}

void ParticleEmitter::setLifeRandom(float lifeRandom)
{
    this->lifeRandom = lifeRandom;
}

void ParticleEmitter::setAirResistance(float force)
{
    this->airResistance = force;
}

void ParticleEmitter::emitParticle()
{
    float angle = (rand() % 201) / 100.0f * PI; // [0 , 2PI]
    glm::vec2 particleVelocity = glm::vec2(cosf(angle), sinf(angle)) * this->particleInitialSpeed * _getRandom(this->speedRandom);
    float particleLife = _getRandom(this->lifeRandom) * this->particleInitialLife;
    float particleSize = _getRandom(this->sizeRandom) * this->particleInitialSize;
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    // float time = (duration.count() % 4001) / 2000.0f * PI;
    glm::vec4 color = glm::vec4(glm::vec3(this->startColor), this->startColor.a * _getRandom(this->alphaRandom));
    // color = glm::pow(color,glm::vec4(0.8f));
    particles.emplace_back(this->position, particleVelocity, particleLife, particleSize, PARTYPE_CIRCLE, color);
}

void ParticleEmitter::setLinearColor(glm::vec4 start, glm::vec4 end)
{
    this->startColor = start;
    this->deltaColor = end - start;
};

// 随机值[1-percent,1]
static inline float _getRandom(float percent)
{
    return (((rand() % 1001) / 1000.0f) * percent + 1 - percent);
}