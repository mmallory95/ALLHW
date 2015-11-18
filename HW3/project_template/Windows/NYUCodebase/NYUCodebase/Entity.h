#include "ShaderProgram.h"
#include "SheetSprite.h"

enum EntityType {ENTITY_PLAYER, ENTITY_ENEMY, ENTITY_COIN};

class Entity {
public:

	Entity();

	void Update(float elapsed);
	void Render(ShaderProgram *program);
	bool collidesWith(Entity *entity);

	SheetSprite sprite;

	float x;
	float y;
	float width;
	float height;
	float velocity_x;
	float velocity_y;
	float acceleration_x;
	float acceleration_y;

	bool isStatic;
	EntityType entityType;

	//contact flags
	//if entity is touching a floor/wall/ceiling, set to true
	//i.e. only allow jumping if collidedBottom = true

	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;



};