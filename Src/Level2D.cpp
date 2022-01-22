#include"Level2D.h"

Level2D::Level2D(glm::vec3 pos,
    glm::vec3 sizeInWorldSpace,
    Camera2D* camera,
    glm::vec2 frameSize,
    b2World* world)
{
    std::vector<GLfloat> vertices;
    std::vector<b2Vec2> collisionEdges;

    // create shader
    m_shaderProgram = new ShaderProgram("default.vert", "default.frag");
    // create texture
    m_texture = new Texture2D("Assets/2D/env.png");
    // uv step for different object
    m_uvStepX = (GLfloat)frameSize.x / m_texture->Width();
    m_uvStepY = (GLfloat)frameSize.y / m_texture->Height();
    m_sizeInWorldSpace = sizeInWorldSpace;
    m_position = pos;
    m_camera2d = camera;
    Move(m_position, 0);
    calculateLevelVertices("Level.txt", vertices, collisionEdges);
    calculateNavPath("path.txt");

    m_howmanyVertices = vertices.size();

    m_vao = new VAO;
    m_vao->Bind();
    m_vbo_vert = new VBO(vertices.data(), vertices.size()*sizeof(GLfloat), GL_STATIC_DRAW);
    // vertex attribute configuration 
    //layout,numOfValues,type, num Of Bytes each vetex,offset in bytes
    m_vao->LinkAttrib(*m_vbo_vert, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void*)0);
    m_vao->LinkAttrib(*m_vbo_vert, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));


    m_vbo_vert->Bind();
    m_vao->UnBind();
    m_vbo_vert->UnBind();

    // physics
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position.Set(pos.x,pos.y);

    m_body = world->CreateBody(&groundBodyDef);

    // create ground collider for the level
    for (int i = 0; i < collisionEdges.size() - 1; i=i+2) {
        
        b2EdgeShape edge;
        edge.SetTwoSided(collisionEdges[i], collisionEdges[i+1]);
        m_body->CreateFixture(&edge, 0.0f);
    }

}

void Level2D::Draw()
{
    m_shaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
        "CameraMat"),
        1, GL_FALSE,
        glm::value_ptr(m_camera2d->ReturnCameraMat()));

    m_texture->Bind();
    m_vao->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_howmanyVertices);
}

void Level2D::CleanUp()
{
    m_vao->Delete();
    m_vbo_vert->Delete();

    m_texture->Delete();
    m_shaderProgram->Delete();

    delete m_vao;
    delete m_vbo_vert;

    delete m_texture;
    delete m_shaderProgram;
}

void Level2D::Move(glm::vec3 pos, float angleDegree)
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
Level2D::~Level2D()
{
}

void Level2D::readLevelData(std::vector<std::string> &leveldata,const char* filePath)
{
    std::ifstream file;
    file.open(filePath);
    if (file.fail())
    {
        std::cout << "can not open the file" << std::endl;
    }
    std::string data;
    while (std::getline(file, data))
    {
        leveldata.push_back(data);
    }
    std::reverse(leveldata.begin(), leveldata.end());
    //std::cout << leveldata[4] << std::endl;
    file.close();
}

void Level2D::calculateLevelVertices(const char* filePath, std::vector<GLfloat> &vertices, std::vector<b2Vec2>& collisionEdges)
{
    // read level data from txt file
    std::vector<std::string> LevelData;
    readLevelData(LevelData, filePath);

    // inserting level geometry
    for (int i = 0; i < LevelData.size(); i++) {
        for (int j = 0; j < LevelData[0].length(); j++) {
            switch (LevelData[i][j])
            {
            case 'L':
                insertVertices(vertices, i, j, 0, 2 * m_uvStepY);
                insertCollisionGeometry(collisionEdges, i, j,true);
                break;
            case 'M':
                insertVertices(vertices, i, j, m_uvStepX, 2 * m_uvStepY);
                break;
            case 'R':
                insertVertices(vertices, i, j, 2 * m_uvStepX, 2 * m_uvStepY);
                insertCollisionGeometry(collisionEdges, i, j,false);
                break;
            case 'l':
                insertVertices(vertices, i, j, 3 * m_uvStepX, 2 * m_uvStepY);
                break;
            case 'm':
                insertVertices(vertices, i, j, 4 * m_uvStepX, 2 * m_uvStepY);
                break;
            case 'r':
                insertVertices(vertices, i, j, 5 * m_uvStepX, 2 * m_uvStepY);
                break;
            case 'E':
                insertVertices(vertices, i, j, 6 * m_uvStepX, 2 * m_uvStepY);
                break;
            case 'B':
                insertVertices(vertices, i, j, 7 * m_uvStepX, 2 * m_uvStepY);
                break;
            case 'W':
                insertVertices(vertices, i, j, 8 * m_uvStepX, 2 * m_uvStepY);
                break;
            case 'F':
                insertVertices(vertices, i, j, 9 * m_uvStepX, 2 * m_uvStepY);
                insertCollisionGeometry(collisionEdges, i, j,true);
                break;
            case 'D':
                insertVertices(vertices, i, j, 10 * m_uvStepX, 2 * m_uvStepY);
                break;
            case 'H':
                insertVertices(vertices, i, j, 11 * m_uvStepX, 2 * m_uvStepY);
                insertCollisionGeometry(collisionEdges, i, j,false);

                break;
            case 'C':
                insertVertices(vertices, i, j, 3 * m_uvStepX, 1 * m_uvStepY);
                break;
            case 'X':
                insertVertices(vertices, i, j, 5 * m_uvStepX, 1 * m_uvStepY);
                break;
            case 'S':
                insertVertices(vertices, i, j, 2 * m_uvStepX, 1 * m_uvStepY);
                break;
            case 'K':
                insertVertices(vertices, i, j, 0 * m_uvStepX, 0 * m_uvStepY);
                break;
            case '@':
                m_playerPos = glm::vec3(j* m_sizeInWorldSpace.x, i* m_sizeInWorldSpace.y, 0);
                break;
            case '*':
                m_enemyPos.emplace_back(glm::vec3(j * m_sizeInWorldSpace.x, i * m_sizeInWorldSpace.y, 0));
                break;
            default:
                break;
            }
        }
    }
}

void Level2D::insertVertices(std::vector<GLfloat>& vertices,int i,int j, GLfloat uvBaseX, GLfloat uvBaseY)
{
    // first vertex
    vertices.emplace_back(-0.5 + j);
    vertices.emplace_back(-0.5 + i);
    vertices.emplace_back(0);
    // uv
    vertices.emplace_back(uvBaseX);
    vertices.emplace_back(uvBaseY);
    // second vertex
    vertices.emplace_back(0.5 + j);
    vertices.emplace_back(-0.5 + i);
    vertices.emplace_back(0);
    // uv
    vertices.emplace_back(uvBaseX+m_uvStepX);
    vertices.emplace_back(uvBaseY);
    // third vertex
    vertices.emplace_back(0.5 + j);
    vertices.emplace_back(0.5 + i);
    vertices.emplace_back(0);
    // uv
    vertices.emplace_back(uvBaseX + m_uvStepX);
    vertices.emplace_back(uvBaseY + m_uvStepY);
    // fourth vertex
    vertices.emplace_back(0.5 + j);
    vertices.emplace_back(0.5 + i);
    vertices.emplace_back(0);
    // uv
    vertices.emplace_back(uvBaseX + m_uvStepX);
    vertices.emplace_back(uvBaseY + m_uvStepY);
    // fifth vertex
    vertices.emplace_back(-0.5 + j);
    vertices.emplace_back(0.5 + i);
    vertices.emplace_back(0);
    // uv
    vertices.emplace_back(uvBaseX);
    vertices.emplace_back(uvBaseY + m_uvStepY);
    // sixth vertex
    vertices.emplace_back(-0.5 + j);
    vertices.emplace_back(-0.5 + i);
    vertices.emplace_back(0);
    // uv
    vertices.emplace_back(uvBaseX);
    vertices.emplace_back(uvBaseY);
}

void Level2D::insertCollisionGeometry(std::vector<b2Vec2>& collisionEdges, int i, int j,bool firstVert)
{
    collisionEdges.emplace_back(b2Vec2(j * m_sizeInWorldSpace.x + (firstVert? -m_sizeInWorldSpace.x / 2.0 : m_sizeInWorldSpace.x / 2.0),
        i * m_sizeInWorldSpace.y + m_sizeInWorldSpace.y / 2.0));
}

void Level2D::calculateNavPath(const char* filePath)
{
    readLevelData(m_pathData, filePath);
}
bool Level2D::IsInWalkableTile(glm::vec3 pos)
{
    int x = (int)pos.x / m_sizeInWorldSpace.x;
    int y = (int)pos.y / m_sizeInWorldSpace.y;

    x = x % m_pathData[0].size();
    y = y % m_pathData.size();

    if (m_pathData[y][x] == 'p') {
        return true;
    }
    else {
        return false;
    }
}

glm::vec3 Level2D::FindNeighbourTile(glm::vec3 pos,GLfloat F,glm::vec3 targetPos)
{
    int x = (int)pos.x / m_sizeInWorldSpace.x;
    int y = (int)pos.y / m_sizeInWorldSpace.y;

    //x = x % m_pathData[0].size();
    //y = y % m_pathData.size();

    glm::vec3 neighbourPos = pos;
    GLfloat minDistance=898696675675;
    //std::cout <<"x: " << x << std::endl;
    //std::cout <<"y: " << y << std::endl;
    if (x < 0 || y < 0) {
        return glm::vec3();
    }


    if (m_pathData[y][x-1] == 'p') {
        glm::vec3 pos = glm::vec3( (x - 1) * m_sizeInWorldSpace.x, y * m_sizeInWorldSpace.y, 0);

        float dis = F+1+glm::distance(pos,targetPos);
        if (minDistance > dis) {
            minDistance = dis;
            neighbourPos = pos;
        }
        //std::cout << "yo" << std::endl;

    }
    if (m_pathData[y][x+1] =='p') {
        glm::vec3 pos = glm::vec3( (x + 1) * m_sizeInWorldSpace.x, y * m_sizeInWorldSpace.y, 0);
        float dis = F + 1+glm::distance(pos, targetPos);
        if (minDistance > dis) {
            minDistance = dis;
            neighbourPos = pos;
        }
        //std::cout << "yo" << std::endl;

    }
    if (m_pathData[y-1][x-1]=='p') {
       glm::vec3 pos=  glm::vec3( (x - 1) * m_sizeInWorldSpace.x, (y - 1) * m_sizeInWorldSpace.y, 0);
       float dis = F + 1 + glm::distance(pos, targetPos);
       if (minDistance > dis) {
           minDistance = dis;
           neighbourPos = pos;
       }

    }
    if (m_pathData[y-1][x] =='p') {
       glm::vec3 pos = glm::vec3( (x ) * m_sizeInWorldSpace.x, (y - 1) * m_sizeInWorldSpace.y, 0);
       float dis = F + 1 + glm::distance(pos, targetPos);
       if (minDistance > dis) {
           minDistance = dis;
           neighbourPos = pos;
       }

    }
    if (m_pathData[y-1][x+1]=='p') {
        glm::vec3 pos = glm::vec3( (x+1)*m_sizeInWorldSpace.x, (y - 1) * m_sizeInWorldSpace.y, 0);
        float dis = F + 1 + glm::distance(pos, targetPos);
        if (minDistance > dis) {
            minDistance = dis;
            neighbourPos = pos;
        }

    }
    if (m_pathData[y+1][x-1] == 'p') {
        glm::vec3 pos = glm::vec3( (x - 1) * m_sizeInWorldSpace.x, (y + 1) * m_sizeInWorldSpace.y, 0);
        float dis = F + 1 + glm::distance(pos, targetPos);
        if (minDistance > dis) {
            minDistance = dis;
            neighbourPos = pos;
        }

    }
    if (m_pathData[y+1][x] == 'p') {
        glm::vec3 pos =glm::vec3( (x ) * m_sizeInWorldSpace.x, (y + 1) * m_sizeInWorldSpace.y, 0);
        float dis = F + 1 + glm::distance(pos, targetPos);
        if (minDistance > dis) {
            minDistance = dis;
            neighbourPos = pos;
        }

    }
    if (m_pathData[y+1][x+1]=='p') {
        glm::vec3 pos = glm::vec3( (x+1)*m_sizeInWorldSpace.x, (y + 1) * m_sizeInWorldSpace.y, 0);
        float dis = F + 1+glm::distance(pos, targetPos);
        if (minDistance > dis) {
            minDistance = dis;
            neighbourPos = pos;
        }

    }

    //std::cout << minDistance << std::endl;
    return neighbourPos;
}