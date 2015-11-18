#include "Enemy.h"

Enemy::Enemy() : xposi(0.0f), yposi(0.0f), speed(0.0f) {};

Enemy::Enemy(ShaderProgram* program, SheetSprite sprite, SheetSprite bulletSprite, float xposi, float yposi)
: program(program), keys(SDL_GetKeyboardState(NULL)), sprite(sprite), bulletSprite(bulletSprite), xposi(xposi), yposi(yposi), speed(0.5),
bullet(Bullet(program, bulletSprite)), alive(true) {};

void Enemy::Update(const float &pxposi, const SheetSprite &psprite, Bullet &pbullet, bool &palive, int &ealive) {
	if (alive == true){
		program->setModelMatrix(modelMatrix);
		//Define ticks again so it can be used here.
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - eTicks;
		eTicks = ticks;

		modelMatrix.identity();
		modelMatrix.Translate(xposi, yposi, 0.0f);

		//Enemies shooting 
		int trigger = rand() % 5000000 +1; // change value to change how often you get shot at. I like 5000 for normal play

		if (!bullet.alive && trigger == 1 && palive){
			bullet.xposi = xposi;
			bullet.yposi = yposi - (this->sprite.height * 2.0f);
			bullet.direction = -1.0f;
			bullet.alive = true;
		}


		//What happens when bullets hit their targets


		//enemy shooting a bullet Box Case
		if (-1.0f + psprite.height > bullet.yposi - bullet.sprite.height && //-0.5 is not good enough
			-1.0f - psprite.height < bullet.yposi + bullet.sprite.height && //-1.5 is too big
			pxposi + psprite.width > bullet.xposi + bullet.sprite.width &&
			pxposi - psprite.width < bullet.xposi - bullet.sprite.width
			){


			//if bullet impacts player, player = dead
			bullet.alive = false;
			palive = false;
		}

		//player shooting a bullet Box Case
		if (yposi - sprite.height < pbullet.yposi + pbullet.sprite.height &&
			yposi + sprite.height > pbullet.yposi - pbullet.sprite.height &&
			xposi + sprite.width > pbullet.xposi + pbullet.sprite.width &&
			xposi - sprite.width < pbullet.xposi - pbullet.sprite.width
			){

			//if bullet impacts enemy, set the enemy and bullet to dead
			//ALSO set ealive to ealive--  NOT EALIVE = EALIVE-1. 
			//EALIVE-1 DOES NOT END THE GAME PROPERLY
			alive = false; 
			pbullet.alive = false;
			ealive--;
		}



		sprite.Draw();
		bullet.Update(elapsed);
	}
}