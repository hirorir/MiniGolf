#ifndef BALL_H
#define BALL_H

#include "Object3D.h"
#include "PhysicsObject.h"
#include "Tile.h"

#define PI 3.141592653589793

class Ball : public Object3D, public PhysicsObject
{
public:
	Ball();

	Ball(int tile_id, vec3 pos, char *vtx_path, char *frg_path);

	virtual void draw(Camera *camera, Light *light);

	float get_radius();

	virtual void run_simulation(); // Run physics simulation;

	void set_current_tile(Tile *tile);

	bool collide_with_edge(double time_elapsed);

private:
	Tile *t;

	GLuint nVerts, elements;
	float radius, slices, stacks;

	void init_gl();
	void generate_verts(float * verts, float * norms, float * tex, unsigned int * el);
};

#endif