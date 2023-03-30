#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

bool Graphics::Initialize(int width, int height)
{
	// Used for the linux OS
#if !defined(__APPLE__) && !defined(MACOSX)
  // cout << glewGetString(GLEW_VERSION) << endl;
	glewExperimental = GL_TRUE;

	auto status = glewInit();

	// This is here to grab the error that comes from glew init.
	// This error is an GL_INVALID_ENUM that has no effects on the performance
	glGetError();

	//Check for error
	if (status != GLEW_OK)
	{
		std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
		return false;
	}
#endif



	// Init Camera
	m_camera = new Camera();
	if (!m_camera->Initialize(width, height))
	{
		printf("Camera Failed to Initialize\n");
		return false;
	}

	// Set up the shaders
	m_shader = new Shader();
	if (!m_shader->Initialize())
	{
		printf("Shader Failed to Initialize\n");
		return false;
	}

	// Add the vertex shader
	if (!m_shader->AddShader(GL_VERTEX_SHADER))
	{
		printf("Vertex Shader failed to Initialize\n");
		return false;
	}

	// Add the fragment shader
	if (!m_shader->AddShader(GL_FRAGMENT_SHADER))
	{
		printf("Fragment Shader failed to Initialize\n");
		return false;
	}

	// Connect the program
	if (!m_shader->Finalize())
	{
		printf("Program to Finalize\n");
		return false;
	}

	// Populate location bindings of the shader uniform/attribs
	if (!collectShPrLocs()) {
		printf("Some shader attribs not located!\n");
	}

	//skybox
	m_skysphere = new Sphere(64, "assets\\eso0932a.jpg");

	// Starship
	m_mesh2 = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\SpaceShip-1.obj", "assets\\SpaceShip-1.png");
	m_comet = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\asteroid2.obj", "assets\\asteroid2.jpg");

	// The Sun
	m_sun = new Sphere(64, "assets\\2k_sun.jpg");

	m_mercury = new Sphere(48, "assets\\mercury.jpg");

	m_venus = new Sphere(48, "assets\\venus.jpg");

	m_earth = new Sphere(48, "assets\\2k_earth_daymap.jpg");

	m_mars = new Sphere(48, "assets\\mars.jpg");

	m_jupiter = new Sphere(48, "assets\\jupiter.jpg");

	m_saturn = new Sphere(48, "assets\\Saturn.jpg");
	m_satring = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\saturn_ring.obj", "assets\\Saturn_diff.jpg");

	m_uranus = new Sphere(48, "assets\\uranus.jpg");

	m_neptune = new Sphere(48, "assets\\neptune.jpg");


	for (int i = 0; i < 20; i++) {
		m_asteroid[i] = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\asteroid2.obj", "assets\\asteroid2.jpg");
	}
	for (int i = 0; i < 20; i++) {
		m_ast[i] = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\asteroid2.obj", "assets\\asteroid2.jpg");
	}
	for (int i = 0; i < 20; i++) {
		m_as[i] = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\asteroid2.obj", "assets\\asteroid2.jpg");
	}
	for (int i = 0; i < 20; i++) {
		m_a[i] = new Mesh(glm::vec3(2.0f, 3.0f, -5.0f), "assets\\asteroid2.obj", "assets\\asteroid2.jpg");
	}
	for (int i = 0; i < 20; i++) {
		m_moons[i] = new Sphere(48, "assets\\2k_moon.jpg");
	}

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void Graphics::HierarchicalUpdate2(double dt) {
	std::vector<float> speed, dist, rotSpeed, scale;
	glm::vec3 rotVector;
	glm::mat4 localTransform;
	//position of the sun
	modelStack.push(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)));  // sun's coordinate
	localTransform = modelStack.top();		// The sun origin

	m_skysphere->Update(localTransform * glm::scale(glm::vec3(9000., 9000., 9000.)));//Skysphere render

	localTransform *= glm::rotate(glm::mat4(1.0f), (float)dt / 4, glm::vec3(0.f, 1.f, 0.f));
	localTransform *= glm::scale(glm::vec3(10, 10, 10));
	if (m_sun != NULL)
		m_sun->Update(localTransform);

	planetView[0] = localTransform * glm::inverse(glm::rotate(glm::mat4(1.0f), (float)dt / 4, glm::vec3(0.f, 1.f, 0.f))) * glm::lookAt(glm::vec3(-20, 0, -20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));/////////////////////////////////////////////////SUN OBSERVATION MODE///////////////////////////////////////////////////////////



	// position of MERCURY
	speed = { -.75, -.75, -.75 };
	dist = { 12.25, 0, 12.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 0.3, 0.3, 0.3 };
	scale = { .8,.8,.8 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
	//modelStack.push(localTransform);			// store planet-sun coordinate//USED WITH MOONS
	planetView[9] = glm::inverse(localTransform);/////////////////////////////////////////////////MERCURY OBSERVATION MODE///////////////////////////////////////////////////////////
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_mercury != NULL)
		m_mercury->Update(localTransform);

	//modelStack.pop(); //USED WITH MOONS






	// position of the VENUS
	speed = { -0.7, -0.7, -0.7 };
	dist = { 20., 0, 20. };
	rotVector = { 0., -1.,0. };
	rotSpeed = { 0.1, 0.1, 0.1 };
	scale = { 1.3,1.3,1.3 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
	//modelStack.push(localTransform);			// store planet-sun coordinate
	planetView[2] = glm::inverse(localTransform);/////////////////////////////////////////////////VENUS OBSERVATION MODE///////////////////////////////////////////////////////////
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_venus != NULL)
		m_venus->Update(localTransform);

	//modelStack.pop();





	//position of EARTH
	speed = { -0.65, -0.65, -0.65 };
	dist = { 30., 0, 30. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 0.8, 0.8, 0.8 };
	scale = { 1.5,1.5,1.5 };
	localTransform = modelStack.top();			// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
	modelStack.push(localTransform);			// store planet-sun coordinate
	planetView[3] = glm::inverse(localTransform);/////////////////////////////////////////////////EARTH OBSERVATION MODE///////////////////////////////////////////////////////////
	localTransform *= glm::rotate(glm::mat4(1.f), glm::radians(23.f), glm::vec3(0., 0., 1.));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_earth != NULL)
		m_earth->Update(localTransform);


	//EARTHS MOON//////////////////////////////////////
	speed = { 1, 1, 1 };
	dist = { 4.25, .55, 4.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .4f, .4f, .4f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[1] != NULL)
		m_moons[1]->Update(localTransform);

	modelStack.pop();
	//EARTHS MOON////////////////////////////////////////

	//POSITION OF MARS
	speed = { -0.4, -0.4, -0.4 };
	dist = { 40., 0, 40. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { .9, .9, .9 };
	scale = { 0.73,0.73,0.73 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
	modelStack.push(localTransform);			// store planet-sun coordinate
	planetView[4] = glm::inverse(localTransform);/////////////////////////////////////////////////MARS OBSERVATION MODE///////////////////////////////////////////////////////////
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_mars != NULL)
		m_mars->Update(localTransform);

	//MARS MOON//////////////////////////////////////
	speed = { 1.5, 1.5, 1.5 };
	dist = { 4.25, .55, 4.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[2] != NULL)
		m_moons[2]->Update(localTransform);

	speed = { 1, 1, 1 };
	dist = { 2.25, 0.0, 2.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[3] != NULL)
		m_moons[3]->Update(localTransform);
	modelStack.pop();
	//MARS MOON////////////////////////////////////////




	//POSIOTION OF JUPITER
	speed = { -0.2, -0.2, -0.2 };
	dist = { 72., 0, 72. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1.5, 1.5, 1.5 };
	scale = { 6, 6, 6 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
	modelStack.push(localTransform);			// store planet-sun coordinate
	planetView[5] = glm::inverse(localTransform);/////////////////////////////////////////////////JUPITER OBSERVATION MODE///////////////////////////////////////////////////////////
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_jupiter != NULL)
		m_jupiter->Update(localTransform);

	//JUPITER MOONS//////////////////////////////////////
	speed = { 1.5, 1.5, 1.5 };
	dist = { 7.25, -.55, 7.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[4] != NULL)
		m_moons[4]->Update(localTransform);

	speed = { 1, 1, 1 };
	dist = { 7.25, 0.0, 7.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[5] != NULL)
		m_moons[5]->Update(localTransform);


	speed = { -.6, -.6, -.6 };
	dist = { 9.25, 0.55, 8.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[6] != NULL)
		m_moons[6]->Update(localTransform);

	speed = { -1, -1, -1 };
	dist = { 9.25, 0.7, 9.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[7] != NULL)
		m_moons[7]->Update(localTransform);
	modelStack.pop();
	//JUPITER MOON////////////////////////////////////////




	//POSITION OF SATURN
	speed = { -0.1, -0.1, -0.1 };
	dist = { 90., 0, 90. };
	rotVector = { 0.,-1.,0. };
	rotSpeed = {1.3, 1.3, 1.3 };
	scale = { 5.,5.,5. };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
	modelStack.push(localTransform);			// store planet-sun coordinate
	planetView[6] = glm::inverse(localTransform);/////////////////////////////////////////////////SATURN OBSERVATION MODE///////////////////////////////////////////////////////////
	if (m_satring != NULL) m_satring->Update(localTransform * glm::rotate(glm::mat4(1.f), glm::radians(100.f), glm::vec3(.5, 0., .1)) * glm::scale(glm::vec3(.02, .02, .02)));
	localTransform *= glm::rotate(glm::mat4(1.f), glm::radians(200.f), glm::vec3(1., 0., .5));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_saturn != NULL)
		m_saturn->Update(localTransform);

	//SATURN MOONS//////////////////////////////////////
	speed = { 1.5, 1.5, 1.5 };
	dist = { 7.25, -.55, 7.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[8] != NULL)
		m_moons[8]->Update(localTransform);

	speed = {1, 1, 1 };
	dist = { 7.25, 0.0, 7.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[9] != NULL)
		m_moons[9]->Update(localTransform);

	modelStack.pop();
	//SATURN MOON////////////////////////////////////////



	//POSITION OF URANUS
	speed = { -0.05, -0.05, -0.05 };
	dist = { 108., 0, 108. };
	rotVector = { 0., 1., 0. };
	rotSpeed = { 1., 1., 1. };
	scale = { 4,4,4 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
	modelStack.push(localTransform);			// store planet-sun coordinate
	planetView[7] = glm::inverse(localTransform);/////////////////////////////////////////////////URANUS OBSERVATION MODE///////////////////////////////////////////////////////////
	localTransform *= glm::rotate(glm::mat4(1.f), glm::radians(90.f), glm::vec3(0., 0., 1.));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_uranus != NULL)
		m_uranus->Update(localTransform);

	//URANUS MOONS//////////////////////////////////////
	speed = { 1.5, 1.5, 1.5 };
	rotVector = { 0.,1.,1. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)dt) * 6.2, cos((float)dt) * 6.2));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[10] != NULL)
		m_moons[10]->Update(localTransform);

	speed = { 1, 1, 1 };
	rotVector = { 0.,1.,1. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, sin((float)dt) * 6.0, cos((float)dt) * 7.0));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[11] != NULL)
		m_moons[11]->Update(localTransform);
	modelStack.pop();
	//URANUS MOON////////////////////////////////////////




	//position of neptune
	speed = { -0.025, -0.025, -0.025 };
	dist = { 126., 0, 126. };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1.2, 1.2, 1.2 };
	scale = {3.8,3.8,3.8 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
	modelStack.push(localTransform);			// store planet-sun coordinate
	planetView[8] = glm::inverse(localTransform);/////////////////////////////////////////////////NEPTUNE OBSERVATION MODE///////////////////////////////////////////////////////////
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

	if (m_neptune != NULL)
		m_neptune->Update(localTransform);


	//NEPTUNE MOONS//////////////////////////////////////
	speed = { 1.5, 1.5, 1.5 };
	dist = { 7.25, -.55, 7.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[12] != NULL)
		m_moons[12]->Update(localTransform);

	speed = { 1, 1, 1 };
	dist = { 7.25, 0.0, 7.25 };
	rotVector = { 0.,1.,0. };
	rotSpeed = { 1., 1., 1. };
	scale = { .2f, .2f, .2f };
	localTransform = modelStack.top();
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2]));
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_moons[13] != NULL)
		m_moons[13]->Update(localTransform);
	modelStack.pop();
	//NEPTUNE MOON////////////////////////////////////////




	speed = { -.8, -.8, -.8 };
	dist = { 120.25, 0, 120.25 };
	rotVector = { 0.,(float)dt,0. };
	rotSpeed = { 0.3, 0.3, 0.3 };
	scale = { 3,3,3 };
	localTransform = modelStack.top();				// start with sun's coordinate
	localTransform *= glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0] * 5.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 3.0f)); //ELLIPSES
	localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
	if (m_comet != NULL)
		m_comet->Update(localTransform);

	for (int i = 0; i < 20; i++) {
		float svec[20];
		svec[i] = glm::linearRand(0.f, 1.f);

		speed = { 1 * svec[i], 1 * svec[i], 1 * svec[i] };
		dist = { 45., 0, 45. };
		rotVector = { 0.,1.,0. };
		rotSpeed = { .9, .9, .9 };
		scale = { 0.5,0.5,0.5 };
		localTransform = modelStack.top();				// start with sun's coordinate
		localTransform *= glm::translate(glm::mat4(1.f),
			glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
		localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
		localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

		if (m_asteroid[i] != NULL)
			m_asteroid[i]->Update(localTransform);
	}

	for (int i = 0; i < 20; i++) {
		float svec[20];
		svec[i] = glm::linearRand(0.f, 1.f);

		speed = { 1 * svec[i], 1 * svec[i], 1 * svec[i] };
		dist = { 45.5, 0, 45.5 };
		rotVector = { 0.,1.,0. };
		rotSpeed = { .9, .9, .9 };
		scale = { 0.5,0.5,0.5 };
		localTransform = modelStack.top();				// start with sun's coordinate
		localTransform *= glm::translate(glm::mat4(1.f),
			glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
		localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
		localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

		if (m_as[i] != NULL)
			m_as[i]->Update(localTransform);
	}


	for (int i = 0; i < 20; i++) {
		float svec[20];
		svec[i] = glm::linearRand(0.f, 1.f);

		speed = { 1 * svec[i], 1 * svec[i], 1 * svec[i] };
		dist = { 142., 0, 142. };
		rotVector = { 0.,1.,0. };
		rotSpeed = { .9, .9, .9 };
		scale = { 0.5,0.5,0.5 };
		localTransform = modelStack.top();				// start with sun's coordinate
		localTransform *= glm::translate(glm::mat4(1.f),
			glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
		localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
		localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

		if (m_ast[i] != NULL)
			m_ast[i]->Update(localTransform);
	}

	for (int i = 0; i < 20; i++) {
		float svec[20];
		svec[i] = glm::linearRand(0.f, 1.f);

		speed = { 1 * svec[i], 1 * svec[i], 1 * svec[i] };
		dist = { 142.5, 0, 142.5 };
		rotVector = { 0.,1.,0. };
		rotSpeed = { .9, .9, .9 };
		scale = { 0.5,0.5,0.5 };
		localTransform = modelStack.top();				// start with sun's coordinate
		localTransform *= glm::translate(glm::mat4(1.f),
			glm::vec3(cos(speed[0] * dt) * dist[0] * 2.0f, sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2] * 1.3f)); //ELLIPSES
		localTransform *= glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
		localTransform *= glm::scale(glm::vec3(scale[0], scale[1], scale[2]));

		if (m_a[i] != NULL)
			m_a[i]->Update(localTransform);
	}


	modelStack.pop(); 	// back to the nothing coordinate
	glm::mat4 mtx_trans = glm::mat4(1.0f);
	mtx_trans = glm::inverse(m_camera->GetView());
	mtx_trans *= glm::translate(glm::mat4(1.f), glm::vec3(0.0, -.55, -2.5));
	mtx_trans *= glm::rotate(glm::mat4(1.f), .15f, glm::vec3(.1, 0.0f, 0.f));
	mtx_trans *= glm::scale(glm::vec3(.02, .02, -.02));
	//SPACESHIP DISSAPEARS WHEN MODE SWITCH
	if (m_mesh2 != NULL && switchMode == false) m_mesh2->Update(mtx_trans);
}


void Graphics::ComputeTransforms(double dt, std::vector<float> speed, std::vector<float> dist,
	std::vector<float> rotSpeed, glm::vec3 rotVector, std::vector<float> scale, glm::mat4& tmat, glm::mat4& rmat, glm::mat4& smat) {
	tmat = glm::translate(glm::mat4(1.f),
		glm::vec3(cos(speed[0] * dt) * dist[0], sin(speed[1] * dt) * dist[1], sin(speed[2] * dt) * dist[2])
	);
	rmat = glm::rotate(glm::mat4(1.f), rotSpeed[0] * (float)dt, rotVector);
	smat = glm::scale(glm::vec3(scale[0], scale[1], scale[2]));
}

void Graphics::Render()
{
	//clear the screen
	glClearColor(0.5, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Start the correct program
	m_shader->Enable();

	// Send in the projection and view to the shader (stay the same while camera intrinsic(perspective) and extrinsic (view) parameters are the same
	glUniformMatrix4fv(m_projectionMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjection()));
	glUniformMatrix4fv(m_viewMatrix, 1, GL_FALSE, glm::value_ptr(m_camera->GetView()));

	// Render the objects
	/*if (m_cube != NULL){
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_cube->GetModel()));
		m_cube->Render(m_positionAttrib,m_colorAttrib);
	}*/



	if (m_mesh2 != NULL) {
		glUniform1i(m_hasTexture, false);
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mesh2->GetModel()));
		if (m_mesh2->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mesh2->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);

		}
	}

	//skysphere galaxy
	if (m_skysphere != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_skysphere->GetModel()));
		if (m_skysphere->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_skysphere->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_skysphere->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_sun != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_sun->GetModel()));
		if (m_sun->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_sun->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_sun->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//RENDER MERCURY
	if (m_mercury != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mercury->GetModel()));
		if (m_mercury->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mercury->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mercury->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	if (m_earth != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_earth->GetModel()));
		if (m_earth->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_earth->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_earth->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}



	// Render VENUS
	if (m_venus != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_venus->GetModel()));
		if (m_venus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_venus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_venus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}


	//RENDER MARS
	if (m_mars != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_mars->GetModel()));
		if (m_mars->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_mars->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_mars->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}



	//RENDER JUPITER
	if (m_jupiter != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_jupiter->GetModel()));
		if (m_jupiter->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_jupiter->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_jupiter->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}



	//RENDER SATURN
	if (m_saturn != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_saturn->GetModel()));
		if (m_saturn->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_saturn->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_saturn->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}
	if (m_satring != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_satring->GetModel()));
		if (m_satring->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_satring->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_satring->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}



	//RENDER URANUS
	if (m_uranus != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_uranus->GetModel()));
		if (m_uranus->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_uranus->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_uranus->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}



	//RENDER NEPTUNE
	if (m_neptune != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_neptune->GetModel()));
		if (m_neptune->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_neptune->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_neptune->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}




	//RENDER COMET
	if (m_comet != NULL) {
		glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_comet->GetModel()));
		if (m_comet->hasTex) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_comet->getTextureID());
			GLuint sampler = m_shader->GetUniformLocation("sp");
			if (sampler == INVALID_UNIFORM_LOCATION)
			{
				printf("Sampler Not found not found\n");
			}
			glUniform1i(sampler, 0);
			m_comet->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
		}
	}

	//ASTEROIDS
	for (int i = 0; i < 20; i++) {
		if (m_asteroid[i] != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_asteroid[i]->GetModel()));
			if (m_asteroid[i]->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_asteroid[i]->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_asteroid[i]->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}
	}
	for (int i = 0; i < 20; i++) {
		if (m_ast[i] != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_ast[i]->GetModel()));
			if (m_ast[i]->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_ast[i]->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_ast[i]->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}
	}
	for (int i = 0; i < 20; i++) {
		if (m_as[i] != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_as[i]->GetModel()));
			if (m_as[i]->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_as[i]->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_as[i]->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}
	}
	for (int i = 0; i < 20; i++) {
		if (m_a[i] != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_a[i]->GetModel()));
			if (m_a[i]->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_a[i]->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_a[i]->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}
	}
	//MOONS CODE
	for (int i = 1; i < 20; i++) {
		if (m_moons[i] != NULL) {
			glUniformMatrix4fv(m_modelMatrix, 1, GL_FALSE, glm::value_ptr(m_moons[i]->GetModel()));
			if (m_moons[i]->hasTex) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_moons[i]->getTextureID());
				GLuint sampler = m_shader->GetUniformLocation("sp");
				if (sampler == INVALID_UNIFORM_LOCATION)
				{
					printf("Sampler Not found not found\n");
				}
				glUniform1i(sampler, 0);
				m_moons[i]->Render(m_positionAttrib, m_colorAttrib, m_tcAttrib, m_hasTexture);
			}
		}
	}

	// Get any errors from OpenGL
	auto error = glGetError();
	if (error != GL_NO_ERROR)
	{
		string val = ErrorString(error);
		std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
	}
}


bool Graphics::collectShPrLocs() {
	bool anyProblem = true;
	// Locate the projection matrix in the shader
	m_projectionMatrix = m_shader->GetUniformLocation("projectionMatrix");
	if (m_projectionMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_projectionMatrix not found\n");
		anyProblem = false;
	}

	// Locate the view matrix in the shader
	m_viewMatrix = m_shader->GetUniformLocation("viewMatrix");
	if (m_viewMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_viewMatrix not found\n");
		anyProblem = false;
	}

	// Locate the model matrix in the shader
	m_modelMatrix = m_shader->GetUniformLocation("modelMatrix");
	if (m_modelMatrix == INVALID_UNIFORM_LOCATION)
	{
		printf("m_modelMatrix not found\n");
		anyProblem = false;
	}

	// Locate the position vertex attribute
	m_positionAttrib = m_shader->GetAttribLocation("v_position");
	if (m_positionAttrib == -1)
	{
		printf("v_position attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_colorAttrib = m_shader->GetAttribLocation("v_color");
	if (m_colorAttrib == -1)
	{
		printf("v_color attribute not found\n");
		anyProblem = false;
	}

	// Locate the color vertex attribute
	m_tcAttrib = m_shader->GetAttribLocation("v_tc");
	if (m_tcAttrib == -1)
	{
		printf("v_texcoord attribute not found\n");
		anyProblem = false;
	}

	m_hasTexture = m_shader->GetUniformLocation("hasTexture");
	if (m_hasTexture == INVALID_UNIFORM_LOCATION) {
		printf("hasTexture uniform not found\n");
		anyProblem = false;
	}

	return anyProblem;
}

std::string Graphics::ErrorString(GLenum error)
{
	if (error == GL_INVALID_ENUM)
	{
		return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
	}

	else if (error == GL_INVALID_VALUE)
	{
		return "GL_INVALID_VALUE: A numeric argument is out of range.";
	}

	else if (error == GL_INVALID_OPERATION)
	{
		return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
	}

	else if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
	{
		return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
	}

	else if (error == GL_OUT_OF_MEMORY)
	{
		return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
	}
	else
	{
		return "None";
	}
}

