#pragma once
//#include <deque>
#include "Leap.h"
#include "MusicPlayer.h"
using namespace std;
using namespace Leap;
//manage main events
class CPianoMelody
{
public:
	CPianoMelody();
	bool			UpdateData(const Frame &frame);//update current finger statement
	bool			OccurGesture(const KeyTapGesture &keyTap);//get current clicking statement from leap
	void			PlayBackgroundMusic(char fileName[]);//play BGM
	void			StopPlayingBackgroundMusic();
	bool			LoadMusic(char fileName[]);//loading msc
	bool			Play(int musicNum);//play piano key sounds
	//bool			OccurGesture(SwipeGesture &swip);
	
private:
	bool			m_bFingersInited;//to confirm fingers are updated
	int				m_currentFinger;
	int				m_currentPianoArea;
	bool			m_keyTapOccured;
	bool			m_swipOccured;
	FingerList		m_fullFingerList;
	CMusicPlayer	m_musicPlayer;
};