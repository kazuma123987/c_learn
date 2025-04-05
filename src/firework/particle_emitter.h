#ifndef __PARTICLE_EMITTER_H
#define __PARTICLE_EMITTER_H

#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "particle.h"

enum EmitMode
{
    EmitMode_Continous,
    EmitMode_Explose
};

class ParticleEmitter
{
public:
    ParticleEmitter();
    const std::list<Particle> &getParticles() const;
    bool isAlive() const;
    void update(float deltaTime);
    void draw(Renderer &renderer);
    void setPosition(glm::vec2 pos);
    void setVelocity(glm::vec2 vec);
    void setParInitSpeed(float speed);
    void setParInitSize(float size);
    void setParInitLife(float life);
    void setParPerSecond(int count);
    void setEmitterLife(float life);
    void setEmitterGravity(float gravity);
    void setEmitMode(EmitMode mode);
    void setSpeedRandom(float speedRandom);
    void setSizeRandom(float sizeRandom);
    void setAlphaRandom(float alphaRandom);
    void setLifeRandom(float lifeRandom);
    void setAirResistance(float force);
    void setLinearColor(glm::vec4 start, glm::vec4 end);

private:
    void emitParticle();   // 发射粒子
    glm::vec2 position;    // 发射器位置
    glm::vec2 velocity;    // 发射器速度
    float emitterLife;     // 发射器生命
    float emitterInitLife; // 发射器初始生命
    bool _isActive;        // 发射器是否存活
    EmitMode emitmode;     // 发射器模式(连续/爆炸)
    float emitterGravity;  // 重力
    float airResistance;   // 空气阻力

    float particleInitialSpeed;       // 发射时粒子的初始速度
    float particleInitialSize;        // 发射时粒子初始大小
    float particleInitialLife;        // 发射时粒子初始生命
    float speedRandom;                // 速度随机
    float sizeRandom;                 // 大小随机
    float alphaRandom;                // alpha随机
    float lifeRandom;                 // 生命周期随机
    int particlesPerSecond;           // 每秒发射粒子数
    glm::vec4 startColor, deltaColor; // 起始颜色、改变颜色
    std::list<Particle> particles;    // 所有粒子
};

#endif