#ifndef _CHARACTER_H
#define _CHARACTER_H
#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"
#include "LevelMap.h"

class Texture2D;

class Character
{
protected:
	float m_single_sprite_w, m_single_sprite_h;

	float m_collision_radius;
	bool m_alive;

	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;

	//moving
	bool m_moving_right;
	virtual void MoveRight(float deltaTime);
	bool m_moving_left;
	virtual void MoveLeft(float deltaTime);

	//gravity
	virtual void AddGravity(float deltaTime);

	//jumping
	bool m_jumping;
	bool m_can_jump;
	float m_jump_force;
	virtual void Jump();
public:
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position,
		LevelMap* map);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	void SetPosition(Vector2D new_position);
	Vector2D GetPosition();

	//collisions
	float GetCollisionRadius();
	Rect2D GetCollisionBox() { return Rect2D(m_position.x, m_position.y, m_single_sprite_w, m_single_sprite_h); }

	bool IsJumping() { return m_jumping; }
	void CancelJump() { m_jumping = false; }

	void SetAlive(bool isAlive);
	bool GetAlive() { return m_alive; }
private:
	FACING m_facing_direction;
	LevelMap* m_current_level_map;
	bool m_spritesheet;

	int xPos, yPos;
};


#endif // !_CHARACTER_H