#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <stdlib.h> 
#include <string>
#include "Render.h"
# define M_PI           3.14159265358979323846

using Eigen::Matrix;
using Eigen::Vector3d;
using namespace std;

class Game
{
private:
	//WINDOW
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::View view;

	//PROGRAM
	Render* chunks;

	//COORDINATES & BASIS
	Matrix<double, 3, 3> basis;
	sf::Vector2f direction;
	sf::Vector3f position;

	//TEXT
	sf::Font font;
	bool showVariables;
	bool showControls;
	bool showEquation;
	sf::Text printVariables;
	sf::Text printControls;
	sf::Text printEquation;

	//VARIABLES
	int cooldown;
	int dist;
	double zoom;
	float movementSpeed;
	float sensitivity;
	sf::Vector2i mousePosWindow;
	sf::Vector2i lastMousePos;
	bool isLeftMousePressed;
	bool isSpacePressed;

	//INITIATION FUNCTION
	void initRender();
	void initVariables();
	void initWindow();
	void initFont();
	void initText();
public:
	//VARIABLES
	sf::Vector2f windowSize;

	//CONSTRUCTORS & DECONSTRUCTORS
	Game();
	virtual ~Game();

	//ACCESSORS
	const bool getWindowIsOpen() const;
	sf::Vector2f getMousePosWindow();
	void pollEvents();

	//UPDATE FUNCTIONS
	void update();
	void updateMousePosWindow();
	void updateInput();
	void updateBasis();
	void updateText();
	void updateRender();

	//RENDER FUNCTIONS
	void renderText();
	void render();
};