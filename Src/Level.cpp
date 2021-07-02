#include"Level.h"

Level::Level(glm::vec3 pos,
	glm::vec3 sizeInWorldSpace,
	Camera* camera, glm::vec3 worldOffsetStep,
    btDiscreteDynamicsWorld* world)
{
	m_position = pos;
	m_camera = camera;
    m_worldOffsetStep = worldOffsetStep;

    // create shader
    m_grounShaderProgram = new ShaderProgram("normalmesh.vert", "normalmesh.frag");
   m_wallShaderProgram = new ShaderProgram("normalmesh.vert", "normalmesh.frag");
    // create texture
    m_groundTexture = new Texture2D("Assets/2D/texture/dirt.png");
    m_wallTexture = new Texture2D("Assets/2D/texture/concrete.png");

	std::vector<Vertex> vertices;
    std::vector<Vertex> wallVertices;

    std::vector<GLuint> indices = {0,2,1,1,2,3};
    std::vector<GLuint> wallindices = { 0,2,1,1,2,3, 4,0,1,4,1,5, 6,4,5,6,5,7, 2,6,3,3,6,7 };
    m_howmanyIndicesForGround = indices.size();
    m_howmanyIndicesForWall = wallindices.size();
    Move(m_position, 0);
    calculateLevelVertices("Level3d.txt", vertices,wallVertices);
    calculateStaticCollisionObjects(world, m_heightOftheWall);
    
    // for ground
    m_groundVao = new VAO;
    m_groundVao->Bind();
    m_groundVbo_vert = new VBO(vertices.data(), vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
    // pos
    m_groundVao->LinkAttrib(*m_groundVbo_vert, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    // normal
    m_groundVao->LinkAttrib(*m_groundVbo_vert, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // texcoords
    m_groundVao->LinkAttrib(*m_groundVbo_vert, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


    m_groundEbo = new EBO(indices.data(), indices.size() * sizeof(GLuint));

    m_groundEbo->Bind();
    m_groundVbo_vert->Bind();
    m_groundVao->UnBind();
    m_groundVbo_vert->UnBind();
    m_groundEbo->UnBind();

    // for wall
    m_wallVao = new VAO;
    m_wallVao->Bind();
    m_wallVbo_vert = new VBO(wallVertices.data(), wallVertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
    // pos
    m_wallVao->LinkAttrib(*m_wallVbo_vert, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    // normal
    m_wallVao->LinkAttrib(*m_wallVbo_vert, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // texcoords
    m_wallVao->LinkAttrib(*m_wallVbo_vert, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


    m_wallEbo = new EBO(wallindices.data(), wallindices.size() * sizeof(GLuint));

    m_wallEbo->Bind();
    m_wallVbo_vert->Bind();
    m_wallVao->UnBind();
    m_wallVbo_vert->UnBind();
    m_wallEbo->UnBind();
}
void Level::readLevelData(const char* filePath,std::vector<std::string>& leveldata)
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

    file.close();
}

void Level::Move(glm::vec3 pos, float angleDegree)
{
    m_position = pos;
    glm::mat4 modelMat = glm::mat4(1);
    modelMat = glm::translate(modelMat, m_position);
    if (angleDegree != 0.0f) {
        modelMat = glm::rotate(modelMat,
            glm::radians(angleDegree), glm::vec3(0, 0, 1));
    }
    modelMat = glm::scale(modelMat, m_sizeInWorldSpace);
    m_grounShaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_grounShaderProgram->ProgramID, "ModelMat"),
        1, GL_FALSE,
        glm::value_ptr(modelMat));

    m_wallShaderProgram->Use();
    glUniformMatrix4fv(glGetUniformLocation(m_wallShaderProgram->ProgramID, "ModelMat"),
        1, GL_FALSE,
        glm::value_ptr(modelMat));
}

void Level::calculateLevelVertices(const char* filePath,
    std::vector<Vertex>& vertices, 
    std::vector<Vertex>& wallVertices)
{
    // read level data from txt file
    std::vector<std::string> LevelData;
    readLevelData(filePath,LevelData);

    // inserting level geometry
    for (int z = 0; z < LevelData.size(); z++) {
        for (int x = 0; x < LevelData[0].length(); x++) {
            switch (LevelData[z][x])
            {
            case 'A':
                insertVertices(vertices, wallVertices, x, z,x,z);
                break;
            case 'B':
                insertVertices(vertices, wallVertices, x, z, x, z);
                break;
            case 'C':
                insertVertices(vertices, wallVertices, x, z, x, z);
                m_lengthInWorldSpace.x = x;
                m_lengthInWorldSpace.z = z;
                m_lengthInWorldSpace.y = .5f;
                break;
            case 'D':
                insertVertices(vertices, wallVertices, x, z, x, z);
                break;
            case 'O':
                m_meshOjects.push_back( new MeshRenderer("Assets/3D/objects/building/building.obj",
                    glm::vec3(x, 0, z), glm::vec3(.3, .3, .3), m_camera, MeshRenderer::RenderMode::NORMAL));
                break;
            case 'T':
                m_meshOjects.push_back(new MeshRenderer("Assets/3D/objects/tree/tree.obj",
                    glm::vec3(x, 0, z), glm::vec3(.3, .3, .3), m_camera, MeshRenderer::RenderMode::NORMAL));
                break;

            default:
                break;
            }
        }
    }
}
void Level::insertVertices(std::vector<Vertex>& vertices,
    std::vector<Vertex>& wallVertices,
    int x, int z, GLfloat u, GLfloat v)
{
    Vertex vertex;
    vertex.Position = glm::vec3(x * m_worldOffsetStep.x,0 , z * m_worldOffsetStep.y);
    vertex.Normal = glm::vec3(0, 1, 0);
    vertex.TexCoords = glm::vec2(u, v);
    vertices.emplace_back(vertex);
    wallVertices.emplace_back(vertex);
    Vertex vertex1;
    vertex1.Position = glm::vec3(x * m_worldOffsetStep.x, m_heightOftheWall, z * m_worldOffsetStep.y);
    vertex1.Normal = glm::vec3(0, 1, 0);
    vertex1.TexCoords = glm::vec2(u+ m_heightOftheWall, v+ m_heightOftheWall);
    wallVertices.emplace_back(vertex1);
}

void Level::calculateStaticCollisionObjects(btDiscreteDynamicsWorld* world,GLfloat heightOftheWall)
{
    btTransform transform;
    GLfloat wallDepth = 0.5f;

    // ground wall
    transform.setIdentity();
    transform.setOrigin(btVector3(
        m_lengthInWorldSpace.x / 2.0,
        0,
        m_lengthInWorldSpace.z / 2.0));
    m_compundShape = new btCompoundShape();
    m_compundShape->addChildShape(transform, new btBoxShape(btVector3(
        m_lengthInWorldSpace.x / 2.0, 
        wallDepth / 2.0,
        m_lengthInWorldSpace.z / 2.0)));
    // left wall
    transform.setIdentity();
    transform.setOrigin(btVector3(
        m_lengthInWorldSpace.x / 2.0,
        heightOftheWall / 2,
        0));
    m_compundShape->addChildShape(transform, new btBoxShape(btVector3(
        m_lengthInWorldSpace.x / 2.0,
        heightOftheWall / 2.0, 
        wallDepth / 2.0f)));
    // right wall
    transform.setIdentity();
    transform.setOrigin(btVector3(
        m_lengthInWorldSpace.x / 2.0,
        heightOftheWall / 2,
        m_lengthInWorldSpace.z));
    m_compundShape->addChildShape(transform, new btBoxShape(btVector3(
        m_lengthInWorldSpace.x / 2.0,
        heightOftheWall / 2.0,
        wallDepth / 2.0f)));

    // back wall
    transform.setIdentity();
    transform.setOrigin(btVector3(
        0,
        heightOftheWall / 2,
        m_lengthInWorldSpace.z/2.0));
    m_compundShape->addChildShape(transform, new btBoxShape(btVector3(
        wallDepth / 2.0f,
        heightOftheWall / 2.0,
        m_lengthInWorldSpace.z / 2.0)));
    // front wall
    transform.setIdentity();
    transform.setOrigin(btVector3(
        m_lengthInWorldSpace.x,
        heightOftheWall / 2,
        m_lengthInWorldSpace.z / 2.0));
    m_compundShape->addChildShape(transform, new btBoxShape(btVector3(
        wallDepth / 2.0f,
        heightOftheWall / 2.0,
        m_lengthInWorldSpace.z / 2.0)));

    btRigidBody::btRigidBodyConstructionInfo info(m_Mass, NULL, m_compundShape, m_LocalInertia);
    m_rigidBoby = new btRigidBody(info);

    world->addRigidBody(m_rigidBoby);
}
void Level::Update(float elapsedTime, float deltaTime)
{
}

void Level::Draw(float elapsedTime, float deltaTime)
{
    m_grounShaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_grounShaderProgram->ProgramID,
        "CameraMat"),
        1, GL_FALSE,
        glm::value_ptr(m_camera->ReturnCameraMat()));

    m_groundTexture->Bind();
    m_groundVao->Bind();
    glDrawElements(GL_TRIANGLES, m_howmanyIndicesForGround, GL_UNSIGNED_INT, 0);
    m_groundVao->UnBind();

    m_wallShaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_wallShaderProgram->ProgramID,
        "CameraMat"),
        1, GL_FALSE,
        glm::value_ptr(m_camera->ReturnCameraMat()));

    m_wallTexture->Bind();
    m_wallVao->Bind();
    glDrawElements(GL_TRIANGLES, m_howmanyIndicesForWall, GL_UNSIGNED_INT, 0);
    m_wallVao->UnBind();

    for (int i = 0; i < m_meshOjects.size(); i++) {
        m_meshOjects[i]->Draw(elapsedTime,deltaTime);
    }

}

void Level::CleanUp()
{
    m_groundVao->Delete();
    m_groundVbo_vert->Delete();
    m_groundEbo->Delete();

    m_groundTexture->Delete();
    m_grounShaderProgram->Delete();

    for (int i = 0; i < m_meshOjects.size(); i++) {
        delete m_meshOjects[i];
    }

    delete m_groundVao;
    delete m_groundVbo_vert;
    delete m_groundEbo;

    delete m_groundTexture;
    delete m_grounShaderProgram;

    delete m_compundShape;
    delete m_rigidBoby;
}
