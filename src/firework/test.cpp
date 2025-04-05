#include <vector>
#include <list>
#include <ctime>
#include <chrono>
#include <conio.h>
#include <memory>
#include "particle_emitter.h"
#include "resource_manager.h"
#include "windows.h"

const int frameRate = 120;
int winWidth = 1920;
int winHeight = 1080;

static inline float getRandomAngle(void)
{
    return (rand() % 1001) / 500.0f * PI;
}

// 随机值[1-percent,1]
static inline float _getRandom(float percent)
{
    return (((rand() % 1001) / 1000.0f) * percent + 1 - percent);
}

int main()
{
    // 初始化随机数种子
    std::srand(static_cast<unsigned int>(std::time(0)));

    // 创建渲染器
    auto renderer = std::make_unique<Renderer>();
    if (!renderer->initGraph(winWidth, winHeight))
    {
        return -1;
    }

    // 初始化粒子发射器
    std::list<ParticleEmitter> emitters;
    for (size_t i = 0; i < 60; i++)
    {
        ParticleEmitter emitter;
        emitter.setPosition(glm::vec2(winWidth / 2, winHeight / 2)); // 发射器位置
        float angle = getRandomAngle();
        emitter.setVelocity(glm::vec2(cosf(angle), sinf(angle)) * 300.0f * _getRandom(0.4f)); // 发射器速度
        emitter.setParPerSecond(600);                                                         // 发射器生成粒子速度
        emitter.setParInitSize(2.0f);                                                         // 粒子初始尺寸
        emitter.setParInitSpeed(20.0f);                                                       // 粒子初始速度大小
        emitter.setParInitLife(1.2f);                                                         // 粒子生命
        emitter.setSpeedRandom(0.5f);                                                         // 速度随机
        emitter.setLifeRandom(0.2f);                                                          // 生命周期随机
        emitter.setAlphaRandom(0.1f);                                                         // 不透明度随机
        emitter.setSizeRandom(0.1f);                                                          // 大小随机
        emitter.setAirResistance(40.0f);                                                      // 空气阻力
        emitter.setEmitMode(EmitMode_Continous);
        emitter.setEmitterLife(2.0f);
        emitter.setEmitterGravity(80.0f);
        // glm::vec3 color(1.0f, 1.0f, 0.5f);
        glm::vec4 startColor = glm::vec4(1.0f, 1.0f, 0.5f, 1.0f);
        glm::vec4 endColor = glm::vec4(1.0f, 0.9f, 0.2f, 1.0f);
        emitter.setLinearColor(startColor, endColor);
        emitters.push_back(emitter);
    }
    renderer->clear(); // 清屏
    // 初始化时间
    auto lastTime = std::chrono::system_clock::now();
    auto frameTime = std::chrono::system_clock::now();

    // 主循环
    while (!renderer->isRun())
    {
        // 更新时间
        float deltaTime = std::chrono::duration<float>(std::chrono::system_clock::now() - lastTime).count();
        lastTime = std::chrono::system_clock::now();

        // 更新粒子发射器
        for (auto it = emitters.begin(); it != emitters.end();)
        {
            it->update(deltaTime);
            // if (!it->isAlive())
            // {
            //     it = emitters.erase(it);
            // }
            // else
            // {
            it->draw(*renderer);
            it++;
            // }
        }

        // 刷新屏幕
        if (std::chrono::duration<float>(std::chrono::system_clock::now() - frameTime).count() >= 1.0f / frameRate)
        {
            renderer->flush(); // 交换缓冲区
            renderer->clear(); // 清屏
            frameTime = std::chrono::system_clock::now();
        }

        // 处理事件
        renderer->update();
    }
    ResourceManager::Clear();
    renderer->closeGraph();
    return 0;
}