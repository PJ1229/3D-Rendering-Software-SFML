#pragma once
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
# define M_PI           3.14159265358979323846
using Eigen::Matrix;
using Eigen::Vector3d;

using namespace std;

class Equation
{
private:
	//variables
	double zoom;
	bool color;
	string equation;
	sf::Vector3f size;
	sf::Vector3f position;

	//projection
	Matrix<double, 3, 3> basis;
	std::vector<sf::Vector3f> points;
	sf::VertexArray projection;

	//functions
	void initPoints(sf::Vector3f pos, sf::Vector3f size, double intensity, int dist);
	float function(float x, float y);
	void rotate(sf::Vector3f degrees);
	void project();
public:
	Equation(sf::Vector3f position, double zoom, sf::Vector3f size, Matrix<double, 3, 3> basis, double coefficient, int dist, sf::Vector3f degrees, string equation, bool color);
	virtual ~Equation();

	sf::Vector3f getPosition();
	void render(sf::RenderTarget& target);
};

