#include "Material.h"

Material::Material()
{

}

Material::Material(vec3 a, vec3 d, vec3 s, float shine)
{
	ambient = a;
	diffuse = d;
	specular = s;
	shininess = shine;
}

vec3 Material::get_ambient()
{
	return ambient;
}

vec3 Material::get_diffuse()
{
	return diffuse;
}

vec3 Material::get_specular()
{
	return specular;
}

float Material::get_shininess()
{
	return shininess;
}