#include "Player.h"

Player::Player() : xposi(0.0f), yposi(0.0f), speed(0.0f) {};
Player::Player(ShaderProgram* program, SheetSprite sprite, SheetSprite bulletSprite, float xposi, float yposi, float speed)
: program(program), keys(SDL_GetKeyboardState(NULL)), sprite(sprite), bulletSprite(bulletSprite), xposi(xposi), yposi(yposi), speed(speed),
bullet(Bullet(program, bulletSprite)), alive(true)
{};

void Player::Update() {
	if (alive){
		program->setModelMatrix(modelMatrix);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		modelMatrix.identity();
		modelMatrix.Translate(xposi, yposi, 0.0f);

		if (keys[SDL_SCANCODE_RIGHT] && xposi + sprite.width / 2 < 1.15f) { xposi += elapsed * speed; } // move right
		else if (keys[SDL_SCANCODE_LEFT] && xposi - sprite.width / 2 > -1.15f){ xposi -= elapsed * speed; } // move left

		if (keys[SDL_SCANCODE_SPACE]) {
			if (!bullet.alive){
				bullet.xposi = xposi;
				bullet.yposi = yposi + this->sprite.height * 2.0f;
				bullet.direction = 1.0f;
				bullet.alive = true;
			}

		}
		sprite.Draw();
		bullet.Update(elapsed);
	}
}