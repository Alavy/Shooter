#include"BezierObject.h"

BezierObect::BezierObect(glm::vec3 pos, 
    glm::vec3 sizeInWorldSpace, 
    Camera* camera,
    btDiscreteDynamicsWorld* world)
{
    std::vector<GLfloat> vertices;
    m_shaderProgram = new ShaderProgram("sprite.vert", "sprite.frag");
    m_texture = m_texture = new Texture2D("Assets/2D/texture/floor.png");

    m_camera = camera;
    m_sizeInWorldSpace = sizeInWorldSpace;
    m_position = pos;
    Move(m_position, 90, glm::vec3(0,0,1));

    calculateVertices(vertices);
    m_howmanyVertices = vertices.size();
    m_vao = new VAO;
    m_vao->Bind();
    m_vbo = new VBO(vertices.data(), vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    // pos
    m_vao->LinkAttrib(*m_vbo, 0, 3, GL_FLOAT, 5 * sizeof(GLfloat), (void*)0);
    // texcoords
    m_vao->LinkAttrib(*m_vbo, 1, 2, GL_FLOAT, 5 * sizeof(GLfloat), (void*) (3*sizeof(GLfloat)));



    m_vbo->Bind();
    m_vao->UnBind();
    m_vbo->UnBind();

    // physics
    m_collisionShape = new btBoxShape(btVector3(0.25,0.25,0.25));

    btTransform startTransform;
    startTransform.setIdentity();
    m_collisionShape->calculateLocalInertia(m_Mass, m_LocalInertia);
    startTransform.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));

    m_motionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(m_Mass, m_motionState,
        m_collisionShape, m_LocalInertia);

    m_rigidBoby = new btRigidBody(rbInfo);
    //m_rigidBoby->setFriction(btScalar(10.5f));
    //m_rigidBoby->setAngularFactor(btVector3(0, 1, 0));
    //m_rigidBoby->setDamping(.95, 0.9);
    world->addRigidBody(m_rigidBoby);

}

void BezierObect::Move(glm::vec3 pos, float angleDegree, glm::vec3 rotAxis)
{
    m_position = pos;
    glm::mat4 modelMat = glm::mat4(1);
    modelMat = glm::translate(modelMat, m_position);
    if (angleDegree != 0.0f) {
        m_RotAxis = rotAxis;
        m_angle = angleDegree;

    }
    modelMat = glm::rotate(modelMat,
        glm::radians(m_angle), m_RotAxis);

    modelMat = glm::scale(modelMat, m_sizeInWorldSpace);
    m_shaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID, "ModelMat"),
        1, GL_FALSE,
        glm::value_ptr(modelMat));
}

void BezierObect::Update(float elapsedTime, float deltaTime)
{
    btTransform trans;
    if (m_rigidBoby && m_rigidBoby->getMotionState())
    {
        m_rigidBoby->getMotionState()->getWorldTransform(trans);
    }
    glm::vec3 pos = glm::vec3(
        trans.getOrigin().getX(),
        trans.getOrigin().getY()-0.5,
        trans.getOrigin().getZ());
    m_position = pos;
    Move(pos, 0,glm::vec3(0,1,0));
}

void BezierObect::Draw(float elapsedTime, float deltaTime)
{
    m_shaderProgram->Use();

    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram->ProgramID,
        "CameraMat"),
        1, GL_FALSE,
        glm::value_ptr(m_camera->ReturnCameraMat()));

    m_texture->Bind();
    m_vao->Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_howmanyVertices);
    m_vao->UnBind();

}

void BezierObect::CleanUp()
{
    m_vao->Delete();
    m_texture->Delete();
    m_shaderProgram->Delete();

    delete m_vao;
    delete m_texture;
    delete m_shaderProgram;
}

void BezierObect::calculateVertices(std::vector<GLfloat>& vertices)
{
    GLfloat ctrlpoints[][3] =
    {
        { 0.0, 0.0, 0.0}, 
        { -0.3, 0.5, 0.0},
        { 0.1, 1.7, 0.0},
        { 0.5, 1.7, 0.0},
        {1.0, 1.5, 0.0}, 
        {1.4, 1.4, 0.0},
        {1.8, 1.4, 0.0},
        {2.2, 1.4, 0.0},
        {2.6, 1.5, 0.0}, 
        {3.0, 1.5, 0.0},
        {3.4, 1.4, 0.0},
        {3.8, 1.4, 0.0},
        {4.2, 1.6, 0.0},
        {4.6, 1.0, 0.0},
        {5.0, 1.4, 0.0},
        {5.4, 1.0, 0.0},
        {5.8, 0.5, 0.0},
        {6.2, 0.5, 0.0},
        {6.6, 0.3, 0.0},
        {7.2, 0.2, 0.0},
        {6.8, 0.52, 0.0}
    };
    int i, j;
    float x, y, z, r;				
    float x1, y1, z1, r1;
    float theta;

    const float startx = 0, endx = ctrlpoints[L][0];

    const float dx = (endx - startx) / nt;	
    const float dtheta = 2 * PI / ntheta;

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];
    BezierCurve(t, xy, ctrlpoints);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x, p1y, p1z, p2x, p2y, p2z;
    for (i = 0; i < nt; ++i)  
    {
        theta = 0;
        t += dt;
        BezierCurve(t, xy, ctrlpoints);
        x1 = xy[0];
        r1 = xy[1];

        for (j = 0; j <= ntheta; ++j)
        {
            theta += dtheta;
            double cosa = cos(theta);
            double sina = sin(theta);
            y = r * cosa;
            y1 = r1 * cosa;	
            z = r * sina;
            z1 = r1 * sina;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(abs(y));
            vertices.push_back(abs(z));

            p1x = x;
            p1y = y;
            p1z = z;
            p2x = x1;
            p2y = y1;
            p2z = z1;
            vertices.push_back(x1);
            vertices.push_back(y1);
            vertices.push_back(z1);
            vertices.push_back(abs(y1));
            vertices.push_back(abs(z1));

        }

        x = x1;
        r = r1;
    }
}

long long BezierObect::nCr(int n, int r)
{
    if (r > n / 2) r = n - r;
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

void BezierObect::BezierCurve(double t, float xy[2], float ctrlpoints[][3])
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i <= L; i++)
    {
        int ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i][0];
        y += coef * ctrlpoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}
