#pragma once
#include <string>
using namespace std;
class CMusicPlayer
{
public:
	CMusicPlayer(void);
	~CMusicPlayer();
	bool	LoadMusic(char fileName[]);
	void	Play(int musicName);//play key sound
	void    Play(char fileName[]);//play BGM
	void    Stop();
	//~CMusicPlayer(void);
private:
	int	    m_nMusicNum;
	string  m_sID;
};