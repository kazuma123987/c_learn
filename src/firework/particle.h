#ifndef __PARTICLE_H
#define __PARTICLE_H

#include <glm/glm.hpp>
#include "renderer.h"
#define PI 3.14159265359
enum PARTYPE{PARTYPE_CIRCLE,PARTYPE_SPRITE,PARTYPE_CLOUD,PARTYPE_SQUARE};
class Particle
{
public:

    Particle();
    Particle(const glm::vec2& position, const glm::vec2& velocity, float life , float size , PARTYPE parType , glm::vec4 solidColor = glm::vec4(1.0f));
    ~Particle();

    void update(float deltaTime);
    bool isAlive() const;
    void draw(Renderer &renderer);

    glm::vec2 getPosition() const;
    glm::vec2 getVelocity() const;
    glm::vec4 getColor() const;
    float getLife() const;
    float getSize() const;

    void setAlpha(float alpha);
    void setVelocity(glm::vec2 velocity);

private:
    glm::vec2 position;
    glm::vec2 velocity;
    float initialLife;
    float life;
    float initialSize;
    float size;
    PARTYPE type;
    glm::vec4 color;
};

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
}
#endif

#endif