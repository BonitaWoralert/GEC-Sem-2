#include "Character.h"
#include "constants.h"

Character::Character(SDL_Renderer* renderer, std::string imagePath, Vector2D start_position,
	LevelMap* map)
{
	m_collision_radius = 15.0f;
	m_alive = true;
	m_current_level_map = map;
	m_moving_left = false;
	m_moving_right = false;
	m_facing_direction = FACING_RIGHT;
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Character.cpp: Failed to load m_texture";
	}

	//animation
	xPos = 0; //idle anim coords
	yPos = 0;

}
Character::~Character()
{
	m_renderer = nullptr;
}

float Character::GetCollisionRadius()
{
	return m_collision_radius;
}

void Character::SetAlive(bool isAlive)
{
	m_alive = isAlive;
}

void Character::Render()
{
	SDL_Rect src_rect = { xPos, yPos, m_single_sprite_w, m_single_sprite_h };
	SDL_Rect dest_rect = {
(int)(m_position.x), (int)(m_position.y),
			m_single_sprite_w, m_single_sprite_h
};

	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(src_rect, dest_rect, SDL_FLIP_NONE);
	}
	if (m_facing_direction == FACING_LEFT)
	{
		m_texture->Render(src_rect, dest_rect, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Update(float deltaTime, SDL_Event e)
{
	Uint32 seconds = SDL_GetTicks() /150;
	int frame = seconds % 3;

	//idle
	if (!(m_jumping || m_moving_left || m_moving_right))
	{
		xPos = 0;
		yPos = 0;
	}

	//jumping
	if (m_jumping)
	{
		//change anim
		yPos = m_single_sprite_h;
		//adjust position
		m_position.y -= m_jump_force * deltaTime;
		//reduce jump force
		m_jump_force -= JUMP_FORCE_DECREMENT * deltaTime;
		//is jump force 0?
		if (m_jump_force <= 0.0f)
		{
			yPos = 0; //back to idle
			m_jumping = false;
		}
	}
	
	//moving left/right
	if (m_moving_left)
	{
		xPos = 0; //prevent jump and walk from attempting to play at same time - makes character disappear
		MoveLeft(deltaTime);
		if (!m_jumping)
		{
			yPos = m_single_sprite_h*3; //go to bottom row of sprite sheet where the walk animations are
			xPos = m_single_sprite_w * frame; // co ords are based on frame 
		}
	}
	else if (m_moving_right)
	{
		xPos = 0;
		MoveRight(deltaTime);
		if (!m_jumping) //prevent jump and walk from attempting to play at same time - makes character disappear
		{
			yPos = m_single_sprite_h*3; //go to bottom row of sprite sheet where the walk animations are
			xPos = m_single_sprite_w * frame; //co ords are based on frame
		}
	}

	//Collision position variables
	int centralX_position = (int)(m_position.x + (m_single_sprite_w * 0.5)) / TILE_WIDTH;
	int foot_position = (int)(m_position.y + (m_single_sprite_h)) / TILE_HEIGHT;

	//add gravity
	if (m_current_level_map->GetTileAt(foot_position, centralX_position) == 0)
	{
		AddGravity(deltaTime);
	}
	else
	{
		//collided with ground
		m_can_jump = true;
	}
}

void Character::SetPosition(Vector2D new_position)
{
	std::cout << "set pos";
	m_position = new_position;
}
Vector2D Character::GetPosition()
{
	return m_position;
}

void Character::MoveRight(float deltaTime)
{
	m_position.x += deltaTime * MOVEMENT_SPEED;
	m_facing_direction = FACING_RIGHT;
}

void Character::MoveLeft(float deltaTime)
{
	m_position.x -= deltaTime * MOVEMENT_SPEED;
	m_facing_direction = FACING_LEFT;
}

void Character::AddGravity(float deltaTime)
{	
	if (m_position.y + m_single_sprite_h <= SCREEN_HEIGHT)
	{
		m_position.y += GRAVITY_STRENGTH * deltaTime;
	}
	else
	{
		m_can_jump = true;
		m_position.y = SCREEN_HEIGHT - m_single_sprite_h;
	}
}

void Character::Jump()
{
	if (!m_jumping)
	{
		m_jump_force = INITIAL_JUMP_FORCE;
		m_jumping = true;
		m_can_jump = false;
	}
}