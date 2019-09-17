#include "config.hpp"
#include "parametric_shapes.hpp"
#include "core/Bonobo.h"
#include "core/FPSCamera.h"
#include "core/helpers.hpp"
#include "core/Misc.h"
#include "core/node.hpp"
#include "core/ShaderProgramManager.hpp"
//#include "EDAF80/CelestialBody.hpp"
#include "glm/trigonometric.hpp"
#include "glm/gtc/constants.hpp"

#include <imgui.h>
#include <external/imgui_impl_glfw_gl3.h>

#include <stack>

#include <cstdlib>
#include <EDAF80/autoRend.hpp>


int main()
{
	//
	// Set up the framework
	//
	Bonobo framework;

	//
	// Set up the camera
	//
	InputHandler input_handler;
	FPSCameraf camera(0.5f * glm::half_pi<float>(),
	                  static_cast<float>(config::resolution_x) / static_cast<float>(config::resolution_y),
	                  0.01f, 1000.0f);
	camera.mWorld.SetTranslate(glm::vec3(0.0f, 3.0f, 7.0f));
	camera.mWorld.LookAt(glm::vec3(0.0f,0.0f,0.0f));
	camera.mMouseSensitivity = 0.003f;
	camera.mMovementSpeed = 0.25f * 12.0f;

	//
	// Create the window
	//
	WindowManager& window_manager = framework.GetWindowManager();
	WindowManager::WindowDatum window_datum{ input_handler, camera, config::resolution_x, config::resolution_y, 0, 0, 0, 0};
	GLFWwindow* window = window_manager.CreateGLFWWindow("EDAF80: Assignment 1", window_datum, config::msaa_rate);
	if (window == nullptr) {
		LogError("Failed to get a window: exiting.");

		return EXIT_FAILURE;
	}

	//
	// Load the sphere geometry
	//
	std::vector<bonobo::mesh_data> const objects = bonobo::loadObjects("sphere.obj");
	if (objects.empty()) {
		LogError("Failed to load the sphere geometry: exiting.");

		return EXIT_FAILURE;
	}
	bonobo::mesh_data const& sphere = objects.front();
	bonobo::mesh_data const& ring = parametric_shapes::createCircleRing(10,256,1.2f,2.0f);


	//
	// Create the shader program
	//
	ShaderProgramManager program_manager;
	GLuint celestial_body_shader = 0u;
	program_manager.CreateAndRegisterProgram("Celestial Body",
	                                         { { ShaderType::vertex, "EDAF80/default.vert" },
	                                           { ShaderType::fragment, "EDAF80/default.frag" } },
	                                         celestial_body_shader);
	if (celestial_body_shader == 0u) {
		LogError("Failed to generate the “Celestial Body” shader program: exiting.");

		Log::View::Destroy();
		Log::Destroy();

		return EXIT_FAILURE;
	}
	GLuint celestial_ring_shader = 0u;
	program_manager.CreateAndRegisterProgram("Celestial Ring",
	                                         { { ShaderType::vertex, "EDAF80/celestial_ring.vert" },
	                                           { ShaderType::fragment, "EDAF80/celestial_ring.frag" } },
	                                         celestial_ring_shader);
	if (celestial_ring_shader == 0u) {
		LogError("Failed to generate the “Celestial Ring” shader program: exiting.");

		Log::View::Destroy();
		Log::Destroy();

		return EXIT_FAILURE;
	}


	//
	// Set up the planet related attributes
	//
	//sun
	//mercury
	//venus
	//earth + moon
	//mars
	//jupiter
	//saturn
	//uranus
	//neptune
	//pluto

	GLuint const sun_texture = bonobo::loadTexture2D("sunmap.png");
	float const sun_spin_speed = glm::two_pi<float>()/6.0f; // Full rotation in 6 seconds

	GLuint const mercury_texture = bonobo::loadTexture2D("mercurymap.png");
	float const mercury_scale = 0.02f;
	float const mercury_spin_speed = glm::two_pi<float>() / 180.0f; // Full rotation in 180 seconds
	float const mercury_spin_inclination = glm::radians(2.0f);
	float const mercury_orbit_speed = glm::two_pi<float>() / 4.0f;
	float const mercury_orbit_inclination = glm::radians(3.4f);
	float const mercury_orbit_radius = 2.0f;

	GLuint const venus_texture = bonobo::loadTexture2D("venusmap.png");
	float const venus_scale = 0.05f;
	float const venus_spin_speed = glm::two_pi<float>() / 600.0f; // Full rotation in 600 seconds
	float const venus_spin_inclination = glm::radians(177.0f);
	float const venus_orbit_speed = glm::two_pi<float>() / 12.0f;
	float const venus_orbit_inclination = glm::radians(3.9f);
	float const venus_orbit_radius = 3.0f;

	GLuint const earth_texture = bonobo::loadTexture2D("earth_diffuse.png");
	float const earth_scale = 0.05f;
	float const earth_spin_speed = glm::two_pi<float>() / 3.0f; // Full rotation in twenty secondsfloat const venus_orbit_speed = glm::two_pi<float>() / 12.0f;
	float const earth_spin_inclination = glm::radians(23.0f);
	float const earth_orbit_speed = glm::two_pi<float>() / 20.0f;
	float const earth_orbit_inclination = glm::radians(7.2f);
	float const earth_orbit_radius = 4.0f;

	GLuint const moon_texture = bonobo::loadTexture2D("moonmap.png");
	float const moon_scale = 0.01f;
	float const moon_spin_speed = glm::two_pi<float>() / 90.0f; // Full rotation in ten seconds
	float const moon_spin_inclination = glm::radians(6.7f);
	float const moon_orbit_speed = glm::two_pi<float>() / 1.3f;
	float const moon_orbit_inclination = glm::radians(-66.0f);
	float const moon_orbit_radius = 0.2f;

	GLuint const mars_texture = bonobo::loadTexture2D("marsmap.png");
	float const mars_scale = 0.03f;
	float const mars_spin_speed = glm::two_pi<float>() / 3.0f; // Full rotation in ten seconds
	float const mars_spin_inclination = glm::radians(25.0f);
	float const mars_orbit_speed = glm::two_pi<float>() / 36.0f;
	float const mars_orbit_inclination = glm::radians(5.7f);
	float const mars_orbit_radius = 5.0f;

	GLuint const jupiter_texture = bonobo::loadTexture2D("jupitermap.png");
	float const jupiter_scale = 0.5f;
	float const jupiter_spin_speed = glm::two_pi<float>(); // Full rotation in ten seconds
	float const jupiter_spin_inclination = glm::radians(3.1f);
	float const jupiter_orbit_speed = glm::two_pi<float>() / 220.0f;
	float const jupiter_orbit_inclination = glm::radians(6.1f);
	float const jupiter_orbit_radius = 13.0f;

	GLuint const saturn_texture = bonobo::loadTexture2D("saturnmap.png");
	GLuint const saturn_ring_color = bonobo::loadTexture2D("saturnringcolor.png");
	GLuint const saturn_ring_texture = bonobo::loadTexture2D("saturnringpattern.png");
	float const saturn_scale = 0.4f;
	float const saturn_spin_speed = glm::two_pi<float>()/1.2f; // Full rotation in ten seconds
	float const saturn_spin_inclination = glm::radians(27.0f);
	float const saturn_orbit_speed = glm::two_pi<float>() / 400.0f;
	float const saturn_orbit_inclination = glm::radians(5.5f);
	float const saturn_orbit_radius = 16.0f;
	float const saturn_inner_ring_radius = 0.45f;
	float const saturn_outer_ring_radius = 0.9f;
	float const saturn_ring_resolution_1 = 10.0f;
	float const saturn_ring_resolution_2 = 256.0f;
	glm::vec2 saturn_ring_scale = glm::vec2(1.0f,1.25f);

	GLuint const uranus_texture = bonobo::loadTexture2D("uranusmap.png");
	GLuint const uranus_ring_color = bonobo::loadTexture2D("uranusringcolor.png");
	GLuint const uranus_ring_texture = bonobo::loadTexture2D("uranusringpattern.png");
	float const uranus_scale = 0.2f;
	float const uranus_spin_speed = glm::two_pi<float>() / 2.0f; // Full rotation in ten seconds
	float const uranus_spin_inclination = glm::radians(98.0f);
	float const uranus_orbit_speed = glm::two_pi<float>() / 1680.0f;
	float const uranus_orbit_inclination = glm::radians(6.5f);
	float const uranus_orbit_radius = 18.0f;
	float const uranus_inner_ring_radius = 0.4f;
	float const uranus_outer_ring_radius = 0.5f;
	float const uranus_ring_resolution_1 = 10.0f;
	float const uranus_ring_resolution_2 = 256.0f;
	glm::vec2 uranus_ring_scale = glm::vec2(1.0f, 1.25f);

	GLuint const neptune_texture = bonobo::loadTexture2D("neptunemap.png");
	float const neptune_scale = 0.2f;
	float const neptune_spin_speed = glm::two_pi<float>() / 2.0f; // Full rotation in ten seconds
	float const neptune_spin_inclination = glm::radians(28.0f);
	float const neptune_orbit_speed = glm::two_pi<float>() / 3200.0f;
	float const neptune_orbit_inclination = glm::radians(6.4f);
	float const neptune_orbit_radius = 19.0f;

	GLuint const pluto_texture = bonobo::loadTexture2D("plutomap.png");
	float const pluto_scale = 0.1f;
	float const pluto_spin_speed = glm::two_pi<float>() / 19.0f; // Full rotation in ten seconds
	float const pluto_spin_inclination = glm::radians(123.0f);
	float const pluto_orbit_speed = glm::two_pi<float>() / 10000.0f;
	float const pluto_orbit_inclination = glm::radians(11.0f);
	float const pluto_orbit_radius = 20.0f;

	//set up celestialbody nodes for all planets

	CelestialBody sun_node(sphere, &celestial_body_shader, sun_texture);
	sun_node.SetScale(glm::vec3(1, 1, 1));
	sun_node.SetSpinning(glm::radians(0.0f), sun_spin_speed, glm::radians(45.0f));

	CelestialBody mercury_node(sphere, &celestial_body_shader, mercury_texture);
	mercury_node.SetScale(glm::vec3(mercury_scale, mercury_scale, mercury_scale));
	mercury_node.SetSpinning(mercury_spin_inclination, mercury_spin_speed, glm::radians(45.0f));
	mercury_node.set_orbit(mercury_orbit_inclination, mercury_orbit_speed, mercury_orbit_radius, glm::radians(0.0f));

	CelestialBody venus_node(sphere, &celestial_body_shader,venus_texture);
	venus_node.SetScale(glm::vec3(venus_scale, venus_scale, venus_scale));
	venus_node.SetSpinning(venus_spin_inclination, venus_spin_speed, glm::radians(45.0f));
	venus_node.set_orbit(venus_orbit_inclination, venus_orbit_speed, venus_orbit_radius, glm::radians(15.0f));
	
	CelestialBody earth_node(sphere, &celestial_body_shader, earth_texture);
	earth_node.SetScale(glm::vec3(earth_scale, earth_scale,earth_scale));
	earth_node.SetSpinning(earth_spin_inclination, earth_spin_speed, glm::radians(45.0f));
	earth_node.set_orbit(earth_orbit_inclination, earth_orbit_speed, earth_orbit_radius, glm::radians(30.0f));
	
	CelestialBody moon_node(sphere, &celestial_body_shader, moon_texture);
	moon_node.SetScale(glm::vec3(moon_scale, moon_scale, moon_scale));
	moon_node.SetSpinning(moon_spin_inclination, moon_spin_speed, glm::radians(90.0f));
	moon_node.set_orbit(moon_orbit_inclination, moon_orbit_speed, moon_orbit_radius, glm::radians(45.0f));
	
	CelestialBody mars_node(sphere, &celestial_body_shader, mars_texture);
	mars_node.SetScale(glm::vec3(mars_scale, mars_scale, mars_scale));
	mars_node.SetSpinning(mars_spin_inclination, mars_spin_speed, glm::radians(45.0f));
	mars_node.set_orbit(mars_orbit_inclination, mars_orbit_speed, mars_orbit_radius, glm::radians(60.0f));

	CelestialBody jupiter_node(sphere, &celestial_body_shader, jupiter_texture);
	jupiter_node.SetScale(glm::vec3(jupiter_scale, jupiter_scale, jupiter_scale));
	jupiter_node.SetSpinning(jupiter_spin_inclination, jupiter_spin_speed, glm::radians(45.0f));
	jupiter_node.set_orbit(jupiter_orbit_inclination, jupiter_orbit_speed, jupiter_orbit_radius, glm::radians(75.0f));

	CelestialBody saturn_node(sphere, &celestial_body_shader, saturn_texture);
	saturn_node.SetScale(glm::vec3(saturn_scale, saturn_scale, saturn_scale));
	saturn_node.SetSpinning(saturn_spin_inclination, saturn_spin_speed, glm::radians(45.0f));
	saturn_node.set_orbit(saturn_orbit_inclination, saturn_orbit_speed,saturn_orbit_radius, glm::radians(90.0f));
	saturn_node.add_rings(ring, glm::vec2(1, 1), &celestial_ring_shader, saturn_ring_color, saturn_ring_texture);
	
	CelestialBody uranus_node(sphere, &celestial_body_shader, uranus_texture);
	uranus_node.SetScale(glm::vec3(uranus_scale, uranus_scale, uranus_scale));
	uranus_node.SetSpinning(uranus_spin_inclination, uranus_spin_speed, glm::radians(45.0f));
	uranus_node.set_orbit(uranus_orbit_inclination, uranus_orbit_speed, uranus_orbit_radius, glm::radians(105.0f));
	uranus_node.add_rings(ring, glm::vec2(1, 1), &celestial_ring_shader, uranus_ring_color, uranus_ring_texture);

	CelestialBody neptune_node(sphere, &celestial_body_shader, neptune_texture);
	neptune_node.SetScale(glm::vec3(neptune_scale, neptune_scale, neptune_scale));
	neptune_node.SetSpinning(neptune_spin_inclination,neptune_spin_speed, glm::radians(45.0f));
	neptune_node.set_orbit(neptune_orbit_inclination, neptune_orbit_speed, neptune_orbit_radius, glm::radians(120.0f));

	CelestialBody pluto_node(sphere, &celestial_body_shader, pluto_texture);
	pluto_node.SetScale(glm::vec3(pluto_scale, pluto_scale, pluto_scale));
	pluto_node.SetSpinning(pluto_spin_inclination,pluto_spin_speed, glm::radians(45.0f));
	pluto_node.set_orbit(pluto_orbit_inclination, pluto_orbit_speed, pluto_orbit_radius, glm::radians(135.0f));

	Node solar_system_node;
	//solar_system_node.add_child(&sun_node);
	sun_node.add_child(&mercury_node);
	sun_node.add_child(&venus_node);
	sun_node.add_child(&earth_node);
	sun_node.add_child(&mars_node);
	sun_node.add_child(&jupiter_node);
	sun_node.add_child(&saturn_node);
	sun_node.add_child(&uranus_node);
	sun_node.add_child(&neptune_node);
	sun_node.add_child(&pluto_node);
	earth_node.add_child(&moon_node);



	//
	// TODO: Create nodes for the remaining of the solar system
	//


	// Retrieve the actual framebuffer size: for HiDPI monitors, you might
	// end up with a framebuffer larger than what you actually asked for.
	// For example, if you ask for a 1920x1080 framebuffer, you might get a
	// 3840x2160 one instead.
	int framebuffer_width, framebuffer_height;
	glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);

	glViewport(0, 0, framebuffer_width, framebuffer_height);
	glClearDepthf(1.0f);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);


	size_t fpsSamples = 0;
	double lastTime = GetTimeSeconds();
	double fpsNextTick = lastTime + 1.0;


	bool show_logs = true;
	bool show_gui = true;

	while (!glfwWindowShouldClose(window)) {
		//
		// Compute timings information
		//
		double const nowTime = GetTimeSeconds();
		double const delta_time = nowTime - lastTime;
		lastTime = nowTime;
		if (nowTime > fpsNextTick) {
			fpsNextTick += 1.0;
			fpsSamples = 0;
		}
		++fpsSamples;


		//
		// Process inputs
		//
		glfwPollEvents();

		ImGuiIO const& io = ImGui::GetIO();
		input_handler.SetUICapture(io.WantCaptureMouse, io.WantCaptureKeyboard);
		input_handler.Advance();
		camera.Update(delta_time, input_handler);

		if (input_handler.GetKeycodeState(GLFW_KEY_F3) & JUST_RELEASED)
			show_logs = !show_logs;
		if (input_handler.GetKeycodeState(GLFW_KEY_F2) & JUST_RELEASED)
			show_gui = !show_gui;


		//
		// Start a new frame for Dear ImGui
		//
		ImGui_ImplGlfwGL3_NewFrame();


		//
		// Clear the screen
		//
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


		//
		// Update the transforms
		//
		//sun_transform_reference.RotateY(sun_spin_speed * delta_time);


		//
		// Traverse the scene graph and render all nodes
		//
		std::stack<CelestialBody*> node_stack({ &sun_node });
		std::stack<CelestialBody*> test_stack;
		autoRend(&earth_node, test_stack*);
		//std::cout << node_stack.size();


		std::stack<glm::mat4> matrix_stack({ glm::mat4(1.0f) });
		// TODO: Replace this explicit rendering of the Sun with a
		// traversal of the scene graph and rendering of all its nodes.

		sun_node.render((float)delta_time, camera.GetWorldToClipMatrix());
		mercury_node.render((float)delta_time, camera.GetWorldToClipMatrix(), sun_node.getTransform());
		venus_node.render((float)delta_time, camera.GetWorldToClipMatrix(), sun_node.getTransform());
		earth_node.render((float) delta_time, camera.GetWorldToClipMatrix(),sun_node.getTransform());
		moon_node.render((float)delta_time, camera.GetWorldToClipMatrix(),earth_node.getTransform());
		mars_node.render((float)delta_time, camera.GetWorldToClipMatrix(), sun_node.getTransform());
		jupiter_node.render((float)delta_time, camera.GetWorldToClipMatrix(), sun_node.getTransform());
		saturn_node.render((float)delta_time, camera.GetWorldToClipMatrix(), sun_node.getTransform());
		uranus_node.render((float)delta_time, camera.GetWorldToClipMatrix(), sun_node.getTransform());
		neptune_node.render((float)delta_time, camera.GetWorldToClipMatrix(), sun_node.getTransform());
		pluto_node.render((float)delta_time, camera.GetWorldToClipMatrix(), sun_node.getTransform());
		//
		// Display Dear ImGui windows
		//
		if (show_logs)
			Log::View::Render();
		if (show_gui)
			ImGui::Render();


		//
		// Queue the computed frame for display on screen
		//
		glfwSwapBuffers(window);
	}

	glDeleteTextures(1, &sun_texture);

	return EXIT_SUCCESS;
}
