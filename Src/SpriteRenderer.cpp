#include"SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const char* textureFilePath,
    const char* vertexShaderFilePath, 
    const char* fragShderFilePath,
    glm::vec3 pos,
    glm::vec3 sizeInWorldSpace, 
    Camera2D* camera,
    glm::vec2 frameSize, 
    b2World* world)
{
    // initialize 4 vertices
    GLfloat m_vertices[] = {
        // vertex Data      // texCords
        -0.5f, -0.5f, 0.0f, //0.0,0.0,
        0.5f, -0.5f, 0.0f,  //1.0,0.0,
        0.5f, 0.5f, 0.0f,   //1.0,1.0,
        -0.5f,0.5f,0.0f,    //0.0,1.0
    };
    // indicies of the triangle
    GLuint m_indices[] = {
        0,1,2,
        2,3,0
    };
    // create shader program
    m_shaderProgram = new ShaderProgram(vertexShaderFilePath, 
        fragShderFilePath);
    m_sizeInWorldSpace = sizeInWorldSpace;
    m_position = pos;
    m_camera2d = camera;
    // move sprite to a predefine position
    Move(m_position,0);
    //create texture
    m_texture = new Texture2D(textureFilePath);
    // create anim step for calculationg uvs
    m_animStepX = (GLfloat)frameSize.x / m_texture->Width();
    m_animStepY = (GLfloat)frameSize.y / m_texture->Height();
    // initial uv
    GLfloat initTexcords[] = {
        0.0,0.0,
        m_animStepX,0.0,
        m_animStepX,m_animStepY,
        0.0,m_animStepY
    };

    #pragma region draw_geometry

    // for drawing geometry
    // initialize vao
    m_vao = new VAO;
    m_vao->Bind();
    m_vbo_vert = new VBO(m_vertices, sizeof(m_vertices), GL_STATIC_DRAW);
    m_vbo_uv = new VBO(initTexcords, sizeof(initTexcords), GL_DYNAMIC_DRAW);
    //m_vbo_uv = new VBO(texcords, sizeof(texcords), GL_DYNAMIC_DRAW);
    // vertex attribute configuration 
    //layout,numOfValues,type, num Of Bytes each vetex,offset in bytes
    m_vao->LinkAttrib(*m_vbo_vert, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);
    m_vao->LinkAttrib(*m_vbo_uv, 1, 2, GL_FLOAT, 2 * sizeof(GLfloat), (void*)0);

    //m_vao->LinkAttrib(*m_vbo, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    m_ebo = new EBO(m_indices, sizeof(m_indices));

    m_ebo->Bind();
    m_vbo_vert->Bind();
    m_vbo_uv->Bind();
    m_vao->UnBind();
    m_vbo_vert->UnBind();
    m_vbo_uv->UnBind();
    m_ebo->UnBind();
    #pragma endregion

}

void SpriteRenderer::animate()
{
    // calculating the base of the each frame
    float baseX = (float)( static_cast<int>(m_currentAnimIndex)) * m_animStepX;
    float baseY = m_currentAnim.x* m_animStepY;
    // calculating the uvs of each frame
    GLfloat uvData[8];
    uvData[0] = baseX;
    uvData[1] = baseY;
    uvData[2] = baseX + m_animStepX;
    uvData[3] = baseY;
    uvData[4] = baseX + m_animStepX;
    uvData[5] = baseY + m_animStepY;
    uvData[6] = baseX;
    uvData[7] = baseY + m_animStepY;

    m_vbo_uv->PassData(uvData,sizeof(uvData));
    m_currentAnimIndex = m_currentAnimIndex + (1/ static_cast<GLfloat>(m_currentAnim.z))*m_freezeAnimMul;
    if (m_currentAnimIndex >= m_currentAnim.y && m_currentAnimIndex < 0.0f) {
        m_currentAnimIndex = 0;
    }
}

void SpriteRenderer::Scale(glm::vec3 scale)
{
    m_sizeInWorldSpace = scale;
    glm::mat4 modelMat = glm::mat4(1);
    modelMat = glm::translate(modelMat, m_position);
    modelMat = glm::scale(modelMat, scale);
    m_shaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
        1, GL_FALSE,
        glm::value_ptr(modelMat));
}

void SpriteRenderer::Rotate(float angleDegree)
{
    glm::mat4 modelMat = glm::mat4(1);
    modelMat = glm::translate(modelMat, m_position);
    modelMat = glm::rotate(modelMat,
        glm::radians(angleDegree), glm::vec3(0, 0, 1));    
    m_shaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
        1, GL_FALSE,
        glm::value_ptr(modelMat));
}

void SpriteRenderer::SetCurrentAnim(glm::uvec3 animData)
{
    m_currentAnim = animData;
}


void SpriteRenderer::Draw()
{
    m_shaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
        "CameraMat"),
        1, GL_FALSE,
        glm::value_ptr(m_camera2d->ReturnCameraMat()));

    m_texture->Bind();
    m_vao->Bind();
    animate();
    glDrawElements(GL_TRIANGLES, m_elementCount, GL_UNSIGNED_INT, 0);
}


SpriteRenderer::~SpriteRenderer()
{
    m_vao->Delete();
    m_vbo_vert->Delete();
    m_vbo_uv->Delete();
    m_ebo->Delete();
    m_texture->Delete();
    m_shaderProgram->Delete();

    delete m_vao;
    delete m_vbo_vert;
    delete m_vbo_uv;
    delete m_ebo;
    delete m_texture;
    delete m_shaderProgram;
}

void SpriteRenderer::CleanUp()
{
    m_vao->Delete();
    m_vbo_vert->Delete();
    m_vbo_uv->Delete();

    m_ebo->Delete();
    m_texture->Delete();
    m_shaderProgram->Delete();
    delete m_vao;
    delete m_vbo_vert;
    delete m_vbo_uv;

    delete m_ebo;
    delete m_texture;
    delete m_shaderProgram;
}

void SpriteRenderer::Move(glm::vec3 pos, float angleDegree)
{
    m_position = pos;
    glm::mat4 modelMat = glm::mat4(1);
    modelMat = glm::translate(modelMat, m_position);
    if (angleDegree != 0.0f) {
        modelMat = glm::rotate(modelMat,
            glm::radians(angleDegree), glm::vec3(0, 0, 1));
    }
    modelMat = glm::scale(modelMat, m_sizeInWorldSpace);
    m_shaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
        1, GL_FALSE,
        glm::value_ptr(modelMat));
}


