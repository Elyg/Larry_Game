#include "HighScore.h"
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QDebug>
#include <QtCore/QTextStream>

HighScore::HighScore()
{
  //initialise the highscore variables
  m_currentScore = 0;
  m_highScore = 0;
  m_highScoreString = "";
}
void HighScore::highScoreStart()
{
  //start timer
  m_scoreTimer.start();
}
void HighScore::highScorePause()
{
  //pause timer
  m_scoreTimer.pause();
}
void HighScore::setCurrentScore()
{
  //store the current time
  m_currentScore = m_scoreTimer.getTicks();
}
void HighScore::setHighScore()
{
  //set current score and check if it is less than highscore
  setCurrentScore();
  if(m_highScore > m_currentScore || m_highScore == 0)
    {
      //store and write it to the txt file
      m_highScore = m_currentScore;
      highScoreWrite();
    }
}
void HighScore::highScoreRead()
{
  //opens the file
  QFile inFile("Preferences/highScore.txt");
  if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      std::cerr<<"Error Reading file"<<std::endl;
      std::exit(1);
    }
  //read all variables as string lines
  QTextStream in(&inFile);
  m_highScoreString = in.readLine();
  //close file
  inFile.close();
  m_highScore = m_highScoreString.toInt();
}
void HighScore::highScoreWrite()
{
  QString outputFilename = "Preferences/highScore.txt";
  QFile outputFile(outputFilename);
  outputFile.open(QIODevice::WriteOnly);

  /* Check it opened OK */
  if(!outputFile.isOpen())
    {
      return;
    }
  /* Point a QTextStream object at the file */
  QTextStream outStream(&outputFile);
  /* Write the line to the file */
  outStream <<QString::number(m_highScore);
  /* Close the file */
  outputFile.close();
}
