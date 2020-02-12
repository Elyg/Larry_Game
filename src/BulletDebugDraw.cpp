#include <BulletDebugDraw.h>
#include <iostream>

BulletDebugDraw::BulletDebugDraw() {}
BulletDebugDraw::~BulletDebugDraw() {}

void BulletDebugDraw::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color)
{
  //push back all the points from the bullet engine
  m_vertices.push_back(ngl::Vec3(from.getX(),from.getY(),from.getZ()));
  m_vertices.push_back(ngl::Vec3(color.getX(),color.getY(),color.getZ()));
  m_vertices.push_back(ngl::Vec3(to.getX(),to.getY(),to.getZ()));
  m_vertices.push_back(ngl::Vec3(color.getX(),color.getY(),color.getZ()));
}
void BulletDebugDraw::setDebugMode(int _debugMode)
{
  //set the debug mode
  m_debugModes = _debugMode;
  std::cout<<"Debug Mode"<<std::endl;
}
void BulletDebugDraw::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
{
  std::cout<<"drawcontactpoint called"<<std::endl;
}
void BulletDebugDraw::reportErrorWarning(const char* warningString)
{
  std::cout<<warningString<<std::endl;
}
void BulletDebugDraw::draw3dText(const btVector3& location,const char* textString)
{
  std::cout<<"draw3dText called"<<std::endl;
}
void BulletDebugDraw::drawUpdate()
{
  //Create a VAO
  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  // This will identify our vertex buffer
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*m_vertices.size(), &m_vertices[0], GL_DYNAMIC_DRAW);
  // 1st attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,24,0);
  glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,24,(GLvoid*)12);

  //draw the line
  glLineWidth(4.0f);
  glDrawArrays(GL_LINES, 0, m_vertices.size()*2);
  glLineWidth(1.0f);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  m_vertices.clear();
}

