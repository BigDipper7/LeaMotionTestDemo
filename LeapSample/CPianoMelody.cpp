#include "stdafx.h"
#include "CPianoMelody.h"
#include "DisplayGL.h"
#include "LeapMath.h"
#include <windows.h>
#include <iostream>
using namespace std;
CPianoMelody::CPianoMelody()
{
	m_bFingersInited	= false;
	m_currentFinger		= -1;
	m_keyTapOccured		= false;
	m_currentPianoArea	= -1;
}
bool	CPianoMelody::UpdateData(const Frame &frame)
{
	FingerList fingerList = frame.fingers();
	return true;
}
//get current clicking event from leap
bool	CPianoMelody::OccurGesture(const KeyTapGesture &keyTap)
{
	Pointable pt = keyTap.pointable();
	Vector v= pt.tipVelocity();
	Vector pos = pt.tipPosition();
	int xPos = pos.x;
	if(xPos <= -210)
	{
		xPos = -210;
	}
	else if(xPos >= 210)
	{
		xPos = 210;
	}
	int musicNum = (pos.x + 210)/30;//get the key num

	SetKeyPress(musicNum);
	
	return false;
}
void CPianoMelody::PlayBackgroundMusic(char fileName[])//play BGM
{
	m_musicPlayer.Play(fileName);
}
void CPianoMelody::StopPlayingBackgroundMusic()
{
	m_musicPlayer.Stop();
}
bool CPianoMelody::LoadMusic(char fileName[])
{
	return m_musicPlayer.LoadMusic(fileName);
}
bool CPianoMelody::Play(int musicNum)//process the original data 
{
	if((musicNum >= MAX_KEY_NUM) || (musicNum < 0))
	{
		return false;
	}
	m_musicPlayer.Play(musicNum);
	return true;
}