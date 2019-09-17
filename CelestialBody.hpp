
#include "core/helpers.hpp"
#include "core/node.hpp"
#include "glm/vec3.hpp"

class CelestialBody {

	std::vector<CelestialBody*> children;
	Node _node;
	glm::vec3 scaleFactor;

	Node _rings_node;
	glm::vec3 ringScaleFactor;
	glm::vec3 spinningAxis;

	glm::mat4 TSR;
	glm::mat4 orbitMatrix;
	glm::mat4 coreMatrix;

	float spinning_inclination;
	float spinningSpeed;
	float spinningAngle;

	float orbit_inclination;
	float orbit_speed;
	float orbit_radius;
	float orbitAngle;

public:

	CelestialBody(bonobo::mesh_data const& shape, GLuint
		const* program, GLuint diffuse_texture_id);

	void render(float ellapsed_time, glm::mat4
		const& view_projection, glm::mat4 const& parent_transform = glm::mat4(1.0f));

	void SetScale(glm::vec3 const& scale);

	void SetSpinning(float spinning_inclination, float spinningSpeed,
		float initialSpinAngle = 0.0f);

	void set_orbit(float orbit_inclination, float orbit_speed,
		float orbit_radius, float initial_orbit_angle = 0.0f);

	void add_rings(bonobo::mesh_data const& shape,
		glm::vec2 const& scaling, GLuint const* program, GLuint diffuse_texture_id,
		GLuint opacity_texture_id);

	Node const* get_node();

	Node const* get_rings_node();

	void add_child(CelestialBody* child);

	std::vector<CelestialBody*> const& get_children();

	glm::mat4  getTransform();
};
