#pragma once
class MainMenuAndGameOver
{
private:
	int started = 3;

public:
	void Menu();
	int GetStarted();
	void SetStared(int startedValue);
	void GameOver();
};

