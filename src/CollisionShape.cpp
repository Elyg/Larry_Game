#include "CollisionShape.h"
#include <ngl/Obj.h>
#include <ngl/NGLStream.h>

CollisionShape *CollisionShape::instance()
{
  // in C++ 11 we use this to avoid having to do a double check lock pattern
  static CollisionShape s_instance;
  return &s_instance;
}

void CollisionShape::addMesh(const std::string & _name, const std::string &_objFilePath)
{
  //read the obj file
  ngl::Obj mesh(_objFilePath);
  //create a dynamic rigidbody
  btConvexHullShape*  shape =  new btConvexHullShape();
  auto points=mesh.getVertexList();
  for(auto p : points)
    {
      shape->addPoint(btVector3(p.m_x,p.m_y,p.m_z));
    }
  m_shapes[_name].reset(shape);
}
void CollisionShape::addLevelMesh(const std::string & _name, const std::string &_objFilePath)
{
  //load in the obj file
  ngl::Obj mesh(_objFilePath);
  //creates a triangle mesh
  btTriangleMesh *mTriMesh = new btTriangleMesh();
  //get face list and iterate through to load in triangles to the triangle mesh
  auto faceList=mesh.getFaceList();
  for(auto face: faceList)
    {
      //convert to btVector3
      btVector3 vertex_1,vertex_2,vertex_3;
      ngl::Vec3 v_1 = mesh.getVertexAtIndex(face.m_vert[0]);
      ngl::Vec3 v_2 = mesh.getVertexAtIndex(face.m_vert[1]);
      ngl::Vec3 v_3 = mesh.getVertexAtIndex(face.m_vert[2]);

      vertex_1.setX(v_1.m_x);
      vertex_1.setY(v_1.m_y);
      vertex_1.setZ(v_1.m_z);

      vertex_2.setX(v_2.m_x);
      vertex_2.setY(v_2.m_y);
      vertex_2.setZ(v_2.m_z);

      vertex_3.setX(v_3.m_x);
      vertex_3.setY(v_3.m_y);
      vertex_3.setZ(v_3.m_z);
      //load to triangle mesh
      mTriMesh->addTriangle(vertex_1,vertex_2,vertex_3);
    }
  //load to the triangle mesh to a triangle
  btCollisionShape *mTriMeshShape = new btBvhTriangleMeshShape(mTriMesh,true);
  m_shapes[_name].reset(mTriMeshShape);
}
void CollisionShape::addSphere(const std::string & _name, ngl::Real _rad)
{
  btCollisionShape* shape = new btSphereShape(btScalar(_rad/2.0));
  m_shapes[_name].reset(shape);
}

void CollisionShape::addBox(const std::string & _name, ngl::Real _w, ngl::Real _h, ngl::Real _d)
{
  btCollisionShape* shape = new btBoxShape(btVector3(_w/2.0,_h/2.0,_d/2.0));
  m_shapes[_name].reset(shape);
}

void CollisionShape::addCapsule(const std::string & _name, ngl::Real _rad, ngl::Real _height)
{
  btCollisionShape* shape = new  btCapsuleShape(_rad,_height);
  m_shapes[_name].reset(shape);
}
void CollisionShape::addCylinder(const std::string & _name, ngl::Real _rad, ngl::Real _height)
{
  btCollisionShape* shape = new  btCylinderShapeZ(btVector3(_rad,_height/2.0,_rad));
  m_shapes[_name].reset(shape);
}

void CollisionShape::addCone(const std::string & _name, ngl::Real _rad, ngl::Real _height)
{
  btCollisionShape* shape = new  btConeShapeZ(_rad,_height);
  m_shapes[_name].reset(shape);
}
btCollisionShape* CollisionShape::getShape(const std::string &_name)
{
  btCollisionShape *shape=nullptr;
  auto shapeIt=m_shapes.find(_name);
  // make sure we have a valid shader
  if(shapeIt!=m_shapes.end())
    {
      shape=shapeIt->second.get();
    }
  return shape;
}
void CollisionShape::printShapeMap()
{
  //print shapes contained
  std::cout << "Shapes contained:"<<std::endl;
  for ( auto it = m_shapes.begin(); it != m_shapes.end(); ++it )
    {
      std::cout << " " << it->first;
      std::cout << std::endl;}
}
