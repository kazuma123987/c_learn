#include "particle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

Particle::Particle()
    : position(0.0f), velocity(0.0f), life(0.0f), size(0.0f), type(PARTYPE_CIRCLE), color(1.0f)
{
}

Particle::Particle(const glm::vec2 &position, const glm::vec2 &velocity, float life, float size, PARTYPE parType, glm::vec4 solidColor)
    : position(position), velocity(velocity), initialLife(life), life(life), initialSize(size), size(size), type(parType), color(solidColor)
{
}

Particle::~Particle()
{
}

void Particle::update(float deltaTime)
{
    if (this->life > 0.0f)
    {
        this->position += this->velocity * deltaTime;
        this->life -= deltaTime;
        if (this->initialLife > 0)
        {
            this->color.a -= deltaTime / this->initialLife;
            this->size -= (deltaTime / this->initialLife) * this->initialSize;
        }
    }
}

bool Particle::isAlive() const
{
    return this->life > 0.0f;
}

void Particle::draw(Renderer &renderer)
{
#if 0
    static GLuint VAO = 0, VBO = 0;
    static int numSegments = 144;
    static std::vector<glm::vec2> vertices;

    if (VAO == 0)
    {
        vertices.resize(numSegments + 2);
        // 在显存中创建 VAO 和 VBO 的空间
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }

    // 生成圆形顶点数据
    vertices[0] = glm::vec2(0.0f, 0.0f); // 圆心
    for (int i = 0; i <= numSegments + 1; ++i)
    {
        float angle = i * 2.0f * PI / numSegments;
        float dx = cosf(angle);
        float dy = sinf(angle);
        vertices[i + 1] = glm::vec2(dx, dy);
    }

    // 把顶点数据送入显存
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);
    // VAO的显存空间中设置顶点数据属性
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // 绘制圆形粒子
    glPushMatrix();
    glTranslatef(position.x, position.y, 0.0f);
    glScalef(size, size, 1.0f);
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
    glPopMatrix();
    glBindVertexArray(0);
#endif
    renderer.DrawCircle(this->position, this->size, color);
}

glm::vec2 Particle::getPosition() const
{
    return this->position;
}

glm::vec2 Particle::getVelocity() const
{
    return this->velocity;
}

glm::vec4 Particle::getColor() const
{
    return this->color;
}

float Particle::getLife() const
{
    return this->life;
}

float Particle::getSize() const
{
    return this->size;
}

void Particle::setAlpha(float alpha)
{
    this->color.a = alpha;
}

void Particle::setVelocity(glm::vec2 velocity)
{
    this->velocity = velocity;
}
