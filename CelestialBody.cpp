

#include "CelestialBody.hpp"
#include "core/helpers.hpp"



CelestialBody::CelestialBody(bonobo::mesh_data const& shape, GLuint
	const* program, GLuint diffuse_texture_id)
{
	_node.set_geometry(shape);
	_node.set_program(program, [](GLuint) {});
	_node.add_texture("diffuse_texture", diffuse_texture_id, GL_TEXTURE_2D);

	scaleFactor = glm::vec3(1, 1, 1);
	ringScaleFactor = glm::vec3(1, 1, 1);
	spinningAxis = glm::vec3(0,1,0);
	spinningAngle = 0.0f;
	spinningSpeed = 0.0f;
	spinning_inclination = 0.0f;
	orbit_inclination = 0.0f;
	orbit_radius = 0.0f;
	orbit_speed = 0.0f;
	orbitAngle = 0.0f;

	//std::vector<CelestialBody*> children;

	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scaleFactor);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), spinningAngle, spinningAxis);
	glm::mat4 tiltMatrix = glm::rotate(glm::mat4(1.0f), spinning_inclination, glm::vec3(0, 0, 1));

	glm::mat4 orbitTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(orbit_radius, 0, 0));
	glm::mat4 orbitRotationMatrix = glm::rotate(glm::mat4(1.0f), orbitAngle, spinningAxis);
	glm::mat4 tiltOrbitMatrix = glm::rotate(glm::mat4(1.0f), orbit_inclination, glm::vec3(0, 0, 1));

	
	coreMatrix = tiltMatrix * rotationMatrix * scalingMatrix;
	orbitMatrix = tiltOrbitMatrix * orbitRotationMatrix * orbitTranslation;
	TSR = orbitMatrix;

}

void CelestialBody::render(float ellapsed_time, glm::mat4
	const& view_projection, glm::mat4 const& parent_transform)
{
	spinningAngle += spinningSpeed * ellapsed_time;
	orbitAngle += orbit_speed * ellapsed_time;

	glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), scaleFactor);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), spinningAngle, spinningAxis);
	glm::mat4 tiltMatrix = glm::rotate(glm::mat4(1.0f), spinning_inclination, glm::vec3(0, 0, 1));

	glm::mat4 orbitRotationMatrix = glm::rotate(glm::mat4(1.0f), orbitAngle, spinningAxis);
	glm::mat4 orbitTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(orbit_radius, 0, 0));
	glm::mat4 tiltOrbitMatrix = glm::rotate(glm::mat4(1.0f), orbit_inclination, glm::vec3(0, 0, 1));

	glm::mat4 initialTiltRingMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1, 0, 0));
	glm::mat4 ringScalingMatrix = glm::scale(glm::mat4(1.0f), ringScaleFactor);

	glm::mat4 coreMatrix = tiltMatrix *rotationMatrix * scalingMatrix;
	glm::mat4 orbitMatrix = tiltOrbitMatrix * orbitRotationMatrix *  orbitTranslation;
	glm::mat4 ringMatrix = ringScalingMatrix;

	TSR = orbitMatrix * parent_transform;
	_node.render(view_projection, parent_transform*orbitMatrix*coreMatrix);
	_rings_node.render(view_projection, parent_transform*orbitMatrix*ringScalingMatrix* coreMatrix*initialTiltRingMatrix);
}

void CelestialBody::SetScale(glm::vec3 const& scale)
{
	scaleFactor = scale;
}

void CelestialBody::SetSpinning(float spinning_inclination, float spinningSpeed,
	float initialspinningAngle)
{
	this->spinning_inclination = spinning_inclination;
	this->spinningSpeed = spinningSpeed;
	spinningAngle = initialspinningAngle;
}

void CelestialBody::set_orbit(float orbit_inclination, float orbit_speed,
	float orbit_radius, float initial_orbit_angle)
{
	this->orbit_inclination = orbit_inclination;
	this->orbit_speed = orbit_speed;
	this->orbit_radius = orbit_radius;
	this->orbitAngle = initial_orbit_angle;
}

void CelestialBody::add_rings(bonobo::mesh_data const& shape,
	glm::vec2 const& scaling, GLuint const* program, GLuint diffuse_texture_id,
	GLuint opacity_texture_id){
	_rings_node.set_geometry(shape);
	_rings_node.set_program(program, [](GLuint) {});
	_rings_node.add_texture("diffuse_texture", diffuse_texture_id, GL_TEXTURE_2D);
	_rings_node.add_texture("opacity_texture_id", opacity_texture_id, GL_TEXTURE_2D);

	ringScaleFactor = glm::vec3(scaling,1);
}


void CelestialBody::add_child(CelestialBody* child)
{

	children.push_back(child);

}

std::vector<CelestialBody*> const& CelestialBody::get_children()
{
	return children;
}

glm::mat4 CelestialBody::getTransform()
{
	return TSR;
}

Node const* CelestialBody::get_node()
{
	return &_node;
}

Node const* CelestialBody::get_rings_node()
{
	return &_rings_node;
}
