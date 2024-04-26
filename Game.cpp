#include "Game.h"

//CONSTRUCTOR & DECONSTRUCTOR
Game::Game()
{
	initVariables();
	initWindow();
	initRender();
	initFont();
	initText();
}
Game::~Game()
{
	delete window;
	delete chunks;
}

//INITIATION FUNCTIONS
void Game::initVariables()
{
	showVariables = true;
	showControls = true;
	showEquation = true;
	window = nullptr;
	direction = { M_PI / 2, (3 * M_PI) / 2 };
	sensitivity = 2 * 3.23209259e-3;
	movementSpeed = 10;
	zoom = 1;
	dist = 100;
	cooldown = 0;
}
void Game::initWindow()
{
	videoMode.height = 1500;
	videoMode.width = 2500;
	window = new sf::RenderWindow(videoMode, "3d render");
	view.setSize(sf::Vector2f(videoMode.width, videoMode.height));
	view.setCenter(sf::Vector2f(0.f, 0.f));
	window->setView(view);
	window->setFramerateLimit(144);
}
void Game::initRender()
{
	this->chunks = new Render();
}
void Game::initFont()
{
	if (!font.loadFromFile("Arial.ttf"))
	{
		// error...
	}
}
void Game::initText()
{
	printVariables.setFont(font);
	printVariables.setCharacterSize(24);
	printVariables.setFillColor(sf::Color::White);

	printControls.setFont(font);
	printControls.setCharacterSize(24);
	printControls.setFillColor(sf::Color::White);

	printEquation.setFont(font);
	printEquation.setCharacterSize(48);
	printEquation.setFillColor(sf::Color::White);
}

//ACCESSORS
const bool Game::getWindowIsOpen() const
{
	return window->isOpen();
}
sf::Vector2f Game::getMousePosWindow()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);

	// convert it to world coordinates
	return window->mapPixelToCoords(pixelPos);
}
void Game::pollEvents()
{
	while (window->pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape)
				window->close();
			break;
		case sf::Event::MouseButtonPressed:
			if (ev.mouseButton.button == sf::Mouse::Left)
			{
				// Start tracking mouse position when left mouse button is pressed
				isLeftMousePressed = true;
				lastMousePos = sf::Mouse::getPosition(*window);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (ev.mouseButton.button == sf::Mouse::Left)
			{
				// Stop tracking mouse position when left mouse button is released
				isLeftMousePressed = false;
			}
			break;
		}
	}

}


//UPDATE FUNCTIONS
void Game::update()
{
	updateInput();
	pollEvents();
	updateBasis();
	updateText();
	updateRender();
	updateMousePosWindow();
}
void Game::updateInput()
{
	++cooldown;

	//change camera view
	if (isLeftMousePressed)
	{
		sf::Vector2i currentMousePos = sf::Mouse::getPosition(*window);
		sf::Vector2f offset = window->mapPixelToCoords(currentMousePos) - window->mapPixelToCoords(lastMousePos);
		direction.x += offset.y * sensitivity;
		direction.y -= offset.x * sensitivity;
		lastMousePos = currentMousePos;

		if (direction.x > 2 * M_PI)
		{
			direction.x -= 2 * M_PI;
		}
		if (direction.x < 0)
		{
			direction.x += 2 * M_PI;
		}
		if (direction.y > 2 * M_PI)
		{
			direction.y -= 2 * M_PI;
		}
		if (direction.y < 0)
		{
			direction.y += 2 * M_PI;
		}
	}

	//change camera zoom
	if (ev.type == sf::Event::MouseWheelScrolled)
	{
		if (ev.mouseWheelScroll.delta > 0)
		{
			chunks->zoom(1.01);
			zoom *= 1.01;
		}
		else
		{
			chunks->zoom(.99);
			zoom *= .99;
		}
	}
	
	//settings
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && cooldown > 25)
	{
		if (showVariables)
		{
			showVariables = false;
		}
		else
		{
			showVariables = true;
		}
		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && cooldown > 25)
	{
		if (showControls)
		{
			showControls = false;
		}
		else
		{
			showControls = true;
		}
		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && cooldown > 25)
	{
		showVariables = true;
		showControls = true;
		showEquation = true;
		direction = { M_PI / 2, (3 * M_PI) / 2 };
		zoom = 1;
		dist = 100;
		position = { 0, 0, 0 };
		chunks->clear();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && cooldown > 25)
	{
		chunks->displayData();
		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && cooldown > 25)
	{
		if (chunks->getNumRenderChunks() == 9)
		{
			chunks->setNumRenderChunks(1);
		}
		else if (chunks->getNumRenderChunks() == 1)
		{
			chunks->setNumRenderChunks(5);
		}
		else if (chunks->getNumRenderChunks() == 5)
		{
			chunks->setNumRenderChunks(9);
		}
		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
	{
		if (chunks->getEquationNum() == 0 || chunks->getEquationNum() == 4 || chunks->getEquationNum() == 7)
		{
			chunks->setCoefficient(chunks->getCoefficient() + 1);
		}
		else if (chunks->getEquationNum() == 5)
		{
			chunks->setCoefficient(chunks->getCoefficient() + .0001);
		}
		else
		{
			chunks->setCoefficient(chunks->getCoefficient() + .01);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
	{
		if (chunks->getEquationNum() == 0 || chunks->getEquationNum() == 4 || chunks->getEquationNum() == 7)
		{
			chunks->setCoefficient(chunks->getCoefficient() - 1);
		}
		else if (chunks->getEquationNum() == 5)
		{
			chunks->setCoefficient(chunks->getCoefficient() - .0001);
		}
		else
		{
			chunks->setCoefficient(chunks->getCoefficient() - .01);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
	{
		dist -= 1;
		chunks->setDist(dist);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
	{
		dist += 1;
		chunks->setDist(dist);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8) && cooldown > 50)
	{
		if (chunks->getXRotation())
		{
			chunks->setXRotation(false);
		}
		else
		{
			chunks->setXRotation(true);
		}

		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9) && cooldown > 50)
	{
		if (chunks->getYRotation())
		{
			chunks->setYRotation(false);
		}
		else
		{
			chunks->setYRotation(true);
		}

		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0) && cooldown > 50)
	{
		if (chunks->getZRotation())
		{
			chunks->setZRotation(false);
		}
		else
		{
			chunks->setZRotation(true);
		}

		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		chunks->setXRotation(false);
		chunks->setYRotation(false);
		chunks->setZRotation(false);
		chunks->setRotationSpeed(0);
		chunks->setRotation({ 0 ,0 ,0 });
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
	{
		chunks->setRotationSpeed(chunks->getRotationSpeed() + .001);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
	{
		chunks->setRotationSpeed(chunks->getRotationSpeed() - .001);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U) && cooldown > 50)
	{
		if (chunks->getColor())
		{
			chunks->setColor(false);
		}
		else
		{
			chunks->setColor(true);
		}

		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I) && cooldown > 50)
	{
		if (chunks->getCursor())
		{
			chunks->setCursor(false);
		}
		else
		{
			chunks->setCursor(true);
		}

		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) && cooldown > 50)
	{
		chunks->setCoefficient(0);
		if (chunks->getEquationNum() == 8)
		{
			chunks->setEquationNum(0);
		}
		else
		{
			chunks->setEquationNum(chunks->getEquationNum() + 1);
		}
		cooldown = 0;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && cooldown > 25)
	{
		if (showEquation)
		{
			showEquation = false;
		}
		else
		{
			showEquation = true;
		}
		cooldown = 0;
	}

	//movement
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		chunks->move(sf::Vector3f(0, -movementSpeed * zoom, 0));
		position.y -= movementSpeed * zoom;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		chunks->move(sf::Vector3f(0, movementSpeed * zoom, 0));
		position.y += movementSpeed * zoom;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		chunks->move(sf::Vector3f(movementSpeed * zoom, 0, 0));
		position.x += movementSpeed * zoom;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		chunks->move(sf::Vector3f(-movementSpeed * zoom, 0, 0));
		position.x -= movementSpeed * zoom;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		chunks->move(sf::Vector3f(0, 0, movementSpeed * zoom));
		position.z += movementSpeed * zoom;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		chunks->move(sf::Vector3f(0, 0, -movementSpeed * zoom));
		position.z -= movementSpeed * zoom;
	}
}
void Game::updateMousePosWindow()
{
	mousePosWindow = sf::Mouse::getPosition(*window);
}
void Game::updateBasis()
{
	Vector3d i, j, k;

	// Check if spherical coordinates are {0, 0}
	if (direction.x == 0 && direction.y == 0) {
		// Set basis vectors to standard Cartesian basis
		basis.col(0) = Vector3d(1, 0, 0);
		basis.col(1) = Vector3d(0, 1, 0);
		basis.col(2) = Vector3d(0, 0, 1);
	}
	else {
		// Calculate k vector
		float sin_theta = sin(direction.x);
		float cos_theta = cos(direction.x);
		float sin_phi = sin(direction.y);
		float cos_phi = cos(direction.y);
		k = { sin_theta * cos_phi, sin_theta * sin_phi, cos_theta };

		// Calculate i vector
		i = { sin_theta * cos(direction.y + M_PI / 2), sin_theta * sin(direction.y + M_PI / 2), 0 };
		i.normalize();

		// Calculate j vector
		j = -i.cross(k);

		// Update basis matrix
		basis.col(0) = i;
		basis.col(1) = j;
		basis.col(2) = k;
	}
}
void Game::updateText()
{
	string tempColor;
	if (chunks->getColor())
	{
		tempColor = "On";
	}
	else
	{
		tempColor = "Off";
	}
	string tempCursor;
	if (chunks->getCursor())
	{
		tempCursor = "On";
	}
	else
	{
		tempCursor = "Off";
	}
	string output =
		"Variables: (Q to toggle hide/show)\nPosition: " + to_string((int)position.x) + "," + to_string((int)position.y) + "," + to_string((int)position.z) + "     Rotation: " + to_string((int)chunks->getRotation().x) + "," + to_string((int)chunks->getRotation().y) + "," + to_string((int)chunks->getRotation().z) +
		"\n" + "Direction: " + to_string((int)round(direction.x * 57.2958)) + "," + to_string((int)round(direction.y * 57.2958)) + "     Rotation Speed: " + to_string(((int)(100 * chunks->getRotationSpeed()))) +
		"\n" + "Zoom: " + to_string(zoom) + "     Color: " + tempColor + 
		"\n" + "Render: " + to_string(chunks->getNumRenderChunks()) + "     Cursor: " + tempCursor + 
		"\n" + "Intensity: " + to_string(chunks->getCoefficient()) +
		"\n" + "Number of Points: " + to_string(10000/dist);
	if (chunks->getXRotation())
	{
		output.append("\nX Rotation: On");
	}
	else
	{
		output.append("\nX Rotation: Off");
	}
	if (chunks->getYRotation())
	{
		output.append("\nY Rotation: On");
	}
	else
	{
		output.append("\nY Rotation: Off");
	}
	if (chunks->getZRotation())
	{
		output.append("\nZ Rotation: On");
	}
	else
	{
		output.append("\nZ Rotation: Off");
	}
	printVariables.setString(output);
	printVariables.setPosition(-1240, -740);

	output = "Controls: (E to toggle hide/show)\nWSAD, Space, & Shift to move\nDrag Mouse for direction\nScroll Mouse to zoom\n1 = Reset Render     R = Reset Rotation\n2 = Display Data in Terminal     T = Increase Rotation\n3 = Render More/Less Chunks     Y = Decrease Rotation\n4 = Increase Intensity     U = Toggle Color\n5 = Decrease Intensity     I = Toggle Cursor\n6 = Increase Number of Points     O = Change Equation\n7 = Decrease Number of Points     P = Toggle Equation\n8 = Toggle X Rotation\n9 = Toggle Y Rotation\n0 = Toggle Z Rotation";
	printControls.setString(output);
	printControls.setPosition(-1240, 350);

	output = "z = " + chunks->getEquation();
	printEquation.setString(output);
	sf::FloatRect textRect = printEquation.getLocalBounds();
	printEquation.setOrigin(textRect.width / 2, textRect.height / 2);
	printEquation.setPosition(0, -700);
}
void Game::updateRender()
{
	chunks->setBasis(basis);
	chunks->update();
}

//RENDER FUNCTIONS
void Game::render()
{
	window->clear();
	chunks->render(*window);
	renderText();
	window->display();
}
void Game::renderText()
{
	if (showVariables)
	{
		window->draw(printVariables);
	}
	if (showControls)
	{
		window->draw(printControls);
	}
	if (showEquation)
	{
		window->draw(printEquation);
	}
}