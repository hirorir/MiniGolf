#include "Tile.h"

Tile::Tile(int id, int p, int ecount, vector<float> verts, vector<int> nbors, char *vtx_path, char *frg_path) : Object3D(tile_id, vtx_path, frg_path)
{
	num_position_indices = p;
	num_edge_indices = ecount;
	vertices = verts;
	neighbors = nbors;

	normal = calculate_normal();
	edges = calculate_edges();

	init_gl_tile();
	init_gl_border();
}

void Tile::draw(Camera *camera, Light *light)
{
	shader->use();

	glBindVertexArray(tile_vao);

	material = Material(vec3(0.5f, 0.4f, 0.3f), vec3(0.4f, 0.8f, 0.2f), vec3(0.8f), 100.0f);

	Shader::set_uniforms_camera(shader, camera, mat4(1.0f));
	Shader::set_uniforms_light(shader, camera, light);
	Shader::set_uniforms_material(shader, material);

	glDrawArrays(GL_POLYGON, 0, num_position_indices / 3);

	glBindVertexArray(0);

	if (edges.size() > 0) {
		glBindVertexArray(border_vao);

		glLineWidth(8.0f);

		material = Material(vec3(1.0f, 0.1f, 0.1f), vec3(0.9f, 0.1f, 0.1f), vec3(0.0f), 100.0f);

		Shader::set_uniforms_material(shader, material);

		glDrawArrays(GL_LINES, 0, edges.size());

		glBindVertexArray(0);
	}
}

vector<float> Tile::calculate_edges()
{
	int vertex_index = 0;
	vector<float> edges;

	for (int e = 0; e < num_edge_indices; ++e) {
		if (neighbors[e] == 0) {
			if (vertex_index == num_position_indices - 3) {
				int i;

				for (i = vertex_index; i < num_position_indices; ++i) {
					edges.push_back(vertices[i]);
				}

				for (i = 0; i < 3; ++i) {
					edges.push_back(vertices[i]);
				}
			}
			else {
				for (int i = vertex_index; i < vertex_index + 6; ++i) {
					edges.push_back(vertices[i]);
				}
			}
		}

		vertex_index += 3;
	}

	return edges;
}

vec3 Tile::calculate_normal()
{
	vec3 a, b;

	if (num_position_indices >= 9) {
		vec3 v1(vertices[0], vertices[1], vertices[2]);

		vec3 v2(vertices[3], vertices[4], vertices[5]);

		vec3 v3(vertices[6], vertices[7], vertices[8]);

		a = v1 - v2;
		b = v1 - v3;
	}
	else {
		cout << "error - not enough vertices in this Tile to calculate the normals." << endl;
	}

	return normalize(cross(a, b));
}

void Tile::init_gl_tile()
{
	glGenVertexArrays(1, &tile_vao);
	glBindVertexArray(tile_vao);

	unsigned int handle[2];
	glGenBuffers(2, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, num_position_indices * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	float n[3] = { normal.x, normal.y, normal.z };
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), n, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Tile::init_gl_border()
{
	if (edges.size() > 0) {
		glGenVertexArrays(1, &border_vao);
		glBindVertexArray(border_vao);

		unsigned int handle[1];
		glGenBuffers(1, handle);

		glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
		glBufferData(GL_ARRAY_BUFFER, edges.size() * sizeof(float), &edges[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)));
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
	}
}

void Tile::print()
{
	cout << "TILE (ID: " << tile_id << "): ";
	cout << "# total position indices: " << num_position_indices << ", ";
	cout << "# total edge indices: " << num_edge_indices << endl;
	cout << "Vertex indices: (   ";
	for (int i = 0; i < num_position_indices; ++i) {
			cout << vertices[i] << "   ";
	}
	cout << ")" << endl;
	cout << "Edge indices (Size = " << edges.size() << "): (   ";
	for (vector<float>::size_type i = 0; i < edges.size(); ++i) {
		cout << edges[i] << "   ";
	}
	cout << ")" << endl;
	cout << "Neighbor IDs: ";
	for (int i = 0; i < num_edge_indices; ++i) {
		if (i == num_edge_indices - 1) {
			cout << neighbors[i];
		}
		else {
			cout << neighbors[i] << ", ";
		}
	}
	cout << endl << endl;
}

int Tile::get_tile_id()
{
	return tile_id;
}

int Tile::num_of_pos_indices()
{
	return num_position_indices;
}

int Tile::num_of_edge_indices()
{
	return num_edge_indices;
}

vector<float> Tile::get_verticies()
{
	return vertices;
}

vector<int> Tile::get_neighbors()
{
	return neighbors;
}

vec3 Tile::get_normal()
{
	return normal;
}