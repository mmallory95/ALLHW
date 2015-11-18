#include "Bullet.h"

Bullet::Bullet() {};

Bullet::Bullet(ShaderProgram* program, SheetSprite sprite) : program(program), sprite(sprite), xposi(), yposi(), speed(2.0),
direction(0.0f), alive(false) {};

void Bullet::Update(float elapsed) {
	program->setModelMatrix(modelMatrix);
	modelMatrix.identity();
	modelMatrix.Translate(xposi, yposi, 0.0f);

	//while the bullet is in bounds
	if (alive == true) {
		yposi += (elapsed * (speed * direction));
		sprite.Draw();
	}

	//destroy the bullet if it goes out of bounds
	if (direction == 1 && yposi > 1.0f){
		alive = false;
	}
		
	else if (direction == -1 && yposi < -1.0f){
		alive = false;
	}
}