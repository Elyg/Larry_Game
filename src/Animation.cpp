#include "Animation.h"

Animation::Animation()
{

}

Animation::~Animation()
{

}

void Animation::loadAnimationData(animType _type)
{
  // load our xml files for various animation types/objects
  if(_type == animType::CHAR_TYPE)
    {
      m_characterAnimData.reset(new ngl::NCCAPointBake("models/sphereCharAnim.xml"));
    }
  else if(_type == animType::COIN_TYPE)
    {
      m_coinAnimData.reset(new ngl::NCCAPointBake("models/coinAnim.xml"));
    }

}


void Animation::setAnimationTo(animCycle _cycle)
{
  // set start,end frames depending on chosen animation cycle
  if (_cycle == animCycle::WALK)
    {
      m_charAnimStartFrame = 1;
      m_charAnimEndFrame = m_charAnimStartFrame + 40;

    }

  else if(_cycle == animCycle::JUMP)
    {
      m_charAnimStartFrame = 42;
      m_charAnimEndFrame = m_charAnimStartFrame + 51 ;

    }

  else if(_cycle == animCycle::REST)
    {
      m_charAnimStartFrame = 95;
      m_charAnimEndFrame = m_charAnimStartFrame + 99 ;

    }

  else if(_cycle == animCycle::REST2)
    {
      m_charAnimStartFrame = 196;
      m_charAnimEndFrame = m_charAnimStartFrame + 163 ;

    }
  else if(_cycle == animCycle::COIN_REST)
    {
      m_coinAnimStartFrame = 1;
      m_coinAnimEndFrame = m_coinAnimStartFrame + 58;
    }




}

void Animation::attachAnimToMesh(std::unique_ptr<ngl::Obj> const& _mesh,animType _type)
{
  // bind our animation data to the corresponding mesh
  if(_type == animType::CHAR_TYPE)
    {

      m_characterAnimData->setFrame(0);
      m_characterAnimData->attachMesh(_mesh.get());
    }
  else if(_type == animType::COIN_TYPE)
    {
      m_coinAnimData->setFrame(0);
      m_coinAnimData->attachMesh(_mesh.get());
    }
}





void Animation::setAnimationLoopOn(bool _state, animType _type)
{
  if(_type == animType::CHAR_TYPE)
    {
      // if animation is active, continously loop
      if (_state == 1)
        {

          if(m_charCurrentAnimFrame > m_charAnimEndFrame)
            {
              // if current frame is greater than the end frame, reset it back to start to loop again
              m_charCurrentAnimFrame = m_charAnimStartFrame;
            }
          else if(m_charCurrentAnimFrame < m_charAnimEndFrame )
            {
              // if current frame is less than end frame, set our mesh to the current frame
              m_characterAnimData->setMeshToFrame(m_charCurrentAnimFrame);
            }
          else
            m_charCurrentAnimFrame = m_charAnimStartFrame;

        }
      // if animtion state is set to inactive, complete current loop and stop
      else if(_state == 0)
        {

          if(m_charCurrentAnimFrame < m_charAnimEndFrame)
            {
              m_characterAnimData->setMeshToFrame(m_charCurrentAnimFrame);
            }
          else if(m_charCurrentAnimFrame >= m_charAnimEndFrame)
            m_charCurrentAnimFrame = m_charAnimStartFrame;

        }
    }

  else if(_type == animType::COIN_TYPE)
    {
      if (_state == 1)
        {

          if(m_coinCurrentAnimFrame > m_coinAnimEndFrame)
            {
              m_coinCurrentAnimFrame = m_coinAnimStartFrame;
            }
          else if(m_coinCurrentAnimFrame < m_coinAnimEndFrame )
            {

              m_coinAnimData->setMeshToFrame(m_coinCurrentAnimFrame);
            }
          else
            m_coinCurrentAnimFrame = m_coinAnimStartFrame;

        }
      else if(_state == 0)
        {

          if(m_coinCurrentAnimFrame < m_coinAnimEndFrame)
            {
              m_coinAnimData->setMeshToFrame(m_coinCurrentAnimFrame);
            }
          else if(m_coinCurrentAnimFrame >= m_coinAnimEndFrame)
            m_coinCurrentAnimFrame = m_coinAnimStartFrame;

        }
    }

}


void Animation::initStartFrame()
{
  m_charCurrentAnimFrame = m_charAnimStartFrame;
}

void Animation::setAnimUpdate(int _update, animType _type)
{
  // set our characteranimation state to active/inactive
  if(_type == animType::CHAR_TYPE)
    {
      m_charAnimUpdate = _update;
    }
  // set our coin animation state to active/inactive
  else if(_type == animType::COIN_TYPE)
    {
      m_coinAnimUpdate = _update;
    }
}

bool Animation::getAnimUpdate(animType _type)
{
  if(_type == animType::CHAR_TYPE)
    {
      return m_charAnimUpdate;
    }
  else if(_type == animType::COIN_TYPE)
    {
      return m_coinAnimUpdate;
    }
}

void Animation::incrCurrentFrame(animType _type)
{
  if(_type == animType::CHAR_TYPE)
    {
      m_charCurrentAnimFrame++;
    }
  else if(_type == animType::COIN_TYPE)
    {
      m_coinCurrentAnimFrame++;
    }
}
