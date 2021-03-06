#pragma once
#ifndef _GAMESCREENLEVEL1_H
#include "GameScreen.h"
#include "Commons.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "CharacterKoopa.h"
#include "LevelMap.h"
#include <vector>

class Texture2D;
class PowBlock;

class GameScreenLevel1 : GameScreen
{
private:
	Texture2D* m_background_texture;
	CharacterMario* mario;
	CharacterLuigi* luigi;
	CharacterKoopa* koopa;
	PowBlock* m_pow_block;
	bool SetUpLevel();

	//screen shake
	bool m_screenshake;
	float m_shake_time;
	float m_wobble;
	float m_background_yPos;
	void DoScreenShake();

	//Levelmap
	void SetLevelMap();
	LevelMap* m_level_map;

	//Enemies
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	std::vector<CharacterKoopa*> m_enemies;
	float m_countdown;
public:
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1();

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;

	void UpdatePOWBlock();
};

#endif // !_GAMESCREENLEVEL1_H