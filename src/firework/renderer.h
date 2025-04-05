#ifndef __RENDERER_H
#define __RENDERER_H
#include "shader.h"
#include "texture.h"
#include <list>
#include <vector>
class Renderer
{
public:
    Renderer();
    ~Renderer();
    bool initGraph(int width,int height);
    void closeGraph(void);
    bool isRun(void);
    void flush(void);
    void clear(void);
    void update(void);
    void DrawSprite(Texture &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), 
    float rotate = 0.0f, glm::vec4 spriteColor = glm::vec4(1.0f),float darker=1.0f);
    void DrawCircle(glm::vec2 position, float radius , glm::vec4 color = glm::vec4(1.0f));
    void DrawCircleInstance(std::vector<glm::vec2> &offsets, std::vector<float> &scales, std::vector<glm::vec4> &colors);

private:
    Shader spriteShader,solidShader;
    GLuint quadVAO,quadVBO;
    GLuint circleVAO,circleVBO;
    GLuint offsetVBO, scaleVBO, colorVBO; // 实例化VBO
    glm::vec2 *s_vertices = NULL; // 圆形顶点数据
    GLFWwindow *window = NULL;
    void initRender();
};


#endif