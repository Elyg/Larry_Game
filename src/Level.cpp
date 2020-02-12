#include "Level.h"

Level::Level(PhysicsWorld *_physics)
{
  m_physics = _physics;
  m_text.reset(new Text("font/arial.ttf",15));

  //initialises variables
  m_formOne = "sphere";
  m_formTwo = "larry";
  m_initialBallPos = ngl::Vec3(0,6,0);
  m_check1 = false;
  m_check2 = false;
  m_check3 = false;
  m_check4 = false;
  m_check5 = false;

  CollisionShape *shapes=CollisionShape::instance();

  //adds all the level meshes
  shapes->addLevelMesh("level_1","models/level_1.obj");
  shapes->addLevelMesh("level_2_part_1","models/level_2_part_1.obj");
  shapes->addLevelMesh("level_2_part_2","models/level_2_part_2.obj");
  shapes->addLevelMesh("level_2_part_3a","models/level_2_part_3a.obj");
  shapes->addLevelMesh("level_2_part_3b","models/level_2_part_3b.obj");
  shapes->addLevelMesh("level_3","models/level_3.obj");
  shapes->addLevelMesh("level_4_part_1","models/level_4_part_1.obj");
  shapes->addLevelMesh("level_4_part_2","models/level_4_part_2.obj");
  shapes->addLevelMesh("level_5","models/level_5.obj");
  shapes->addLevelMesh("level_5Stick", "models/level_5_sticky.obj");
  shapes->addLevelMesh("finish", "models/finish_line.obj");

}

void Level::createLevel(std::string _levelName, ngl::Vec3 _pos)
{
  m_physics->addLevelMesh(_levelName, _pos);
}

ngl::Vec3 Level::getInitialPos()
{
  return m_initialBallPos;
}

void Level::checkpoints(bool _shapeshift)
{

  //find which form the character is and gets the position
  if(_shapeshift==true)
    {
      m_charPos = m_physics->getPosition(m_formOne);
    }

  if(_shapeshift==false)
    {
      m_charPos = m_physics->getPosition(m_formTwo);
    }

  //if character hits checkpoint 1 area
  if(m_charPos.m_x<=-104 && m_charPos.m_x>=-120 && m_charPos.m_z<=8 && m_charPos.m_z>=-8)
    {
      //Writes CHECKPOINT
      m_text->setColour(0.0,1.0,0.0);
      m_text->renderText(500,250,"CHECKPOINT");
      //std::cout<<"\n \n Pos: ";
      //std::cout<<m_charPos<<std::endl;
      //std::cout<<"\n \n Initial Pos: ";
      //std::cout<<m_initialBallPos<<std::endl;
      if (m_check1 == false)
        {
          //Changes start position
          m_initialBallPos.m_x = -110;
          m_initialBallPos.m_y = 5;
          m_initialBallPos.m_z = 1.5;
          m_check1=true;
        }
    }

  //if character hits checkpoint 2 area
  if(m_charPos.m_x<=-138 && m_charPos.m_x>=-149 && m_charPos.m_z<=-112 && m_charPos.m_z>=-128)
    {
      //Writes CHECKPOINT
      m_text->setColour(0.0,1.0,0.0);
      m_text->renderText(500,250,"CHECKPOINT");

      if (m_check2 == false)
        {
          //Changes start position
          m_initialBallPos.m_x = -145;
          m_initialBallPos.m_y = -19;
          m_initialBallPos.m_z = -120;
          m_check2=true;
        }

    }

  //if character hits checkpoint 3 area
  if(m_charPos.m_x<=-186 && m_charPos.m_x>=-206 && m_charPos.m_z<=-204 && m_charPos.m_z>=-216)
    {
      //Writes CHECKPOINT
      m_text->setColour(0.0,1.0,0.0);
      m_text->renderText(500,250,"CHECKPOINT");

      if (m_check3 == false)
        {
          //Changes start position
          m_initialBallPos.m_x = -196;
          m_initialBallPos.m_y = -43;
          m_initialBallPos.m_z = -210;
          m_check3=true;
        }

    }
  //if character hits checkpoint 4 area
  if(m_charPos.m_x<=-117 && m_charPos.m_x>=-142 && m_charPos.m_z<=-378 && m_charPos.m_z>=-397)
    {
      //Writes CHECKPOINT
      m_text->setColour(0.0,1.0,0.0);
      m_text->renderText(500,250,"CHECKPOINT");

      if (m_check4 == false)
        {
          //Changes start position
          m_initialBallPos.m_x = -130;
          m_initialBallPos.m_y = -150;
          m_initialBallPos.m_z = -389;
          m_check4=true;
        }

    }

  //if character hits checkpoint 5 area
  if(m_charPos.m_x<=-117 && m_charPos.m_x>=-142 && m_charPos.m_z<=-610 && m_charPos.m_z>=-629)
    {
      //Writes CHECKPOINT
      m_text->setColour(0.0,1.0,0.0);
      m_text->renderText(500,250,"CHECKPOINT");

      if (m_check5 == false)
        {
          //Changes start position
          m_initialBallPos.m_x = -130;
          m_initialBallPos.m_y = -150;
          m_initialBallPos.m_z = -619;
          m_check5=true;
        }

    }


}
