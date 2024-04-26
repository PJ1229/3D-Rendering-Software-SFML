#pragma once
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include <random>
#include <set>
#include <functional>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <cmath>
#include "Equation.h"
# define M_PI           3.14159265358979323846

using Eigen::Matrix;
using Eigen::Vector3d;

using namespace std;

class Render
{
private:
	//EQUATION
	int dist;
	double coefficient;
	double rotationSpeed;
	bool color;
	bool cursor;
	bool xRotation;
	bool yRotation;
	bool zRotation;
	int equationNum;
	string equations[100];
	sf::Vector3f rotation;
	Equation* equation[9];
	void initEquations();
	sf::Vector3f distFromChunkCenter(sf::Vector3f chunk);


	//OBJECTS
	Matrix<double, 3, 3> basis;	//3d space basis matrix
	sf::Vector3f basisPoints[3][2]; //i hat, j hat, k hat points
	sf::VertexArray basisProjection[3]; //i hat, j hat, k hat projection

	//ZOOM & POSITION
	double intensity; //zoom coefficient
	sf::Vector3f position; //real position in 3d space
	sf::Vector2f prevPos; //previous position relative to chunks (every 1000 points)
	sf::Vector2f currentPos; //position relative to chunks (every 1000 points)
	sf::Vector2f offset; //offset of current position to the stored data

	//DATA STORAGE
	int numRenderChunks;
	std::vector<sf::Vector3f> chunks; //every position of every chunk
	std::vector<std::vector<sf::Vector3f>> sortedChunks; //all the chunks sorted based on x and y positions
	std::vector<sf::Vector2f> posTable; //position list
	bool posTableContains(sf::Vector2f pos); //checks if position is in the position list

	//MOVEMENT OF CHUNKS
	void left();
	void down();
	void right();
	void up();

	//SORTING FUNCTIONS
	void sortChunks();
	sf::Vector2f  findOffset();
	float findMinY();
	float findMaxY();

	//INITIATION FUNCTIONS
	void initVariables(); //initates variables
	void initChunk(); //initializes the starting chunks
	void initPosTable(); //initializes starting chunk positions
	void initBasisPoints(); //initializes basis 
	void initBasisProjection(int index, sf::Color color);

	//UPDATE FUNCTIONS
	void updateChunk();
	void updateRenderChunk();
	void updateBasisVectors();
public:
	//CONSTRUCTOR & DECONSTRUCTOR
	Render();
	virtual ~Render();
	
	//ZOOM & MOVEMENT
	void zoom(double intensity);
	void move(sf::Vector3f movement);

	//SETTER FUNCTIONS
	void setXRotation(bool x);
	void setYRotation(bool y);
	void setZRotation(bool z);
	void setRotationSpeed(double r);
	void setRotation(sf::Vector3f rotation);
	void setBasis(Matrix<double, 3, 3> basis);
	void setNumRenderChunks(int num);
	void setCoefficient(double coefficient);
	void setDist(int dist);
	void setColor(bool color);
	void setCursor(bool cursor);
	void setEquationNum(int eq);

	//GETTER FUNCTIONS
	int getNumRenderChunks();
	sf::Vector3f getRotation();
	double getRotationSpeed();
	double getCoefficient();
	bool getColor();
	bool getCursor();
	bool getXRotation();
	bool getYRotation();
	bool getZRotation();
	string getEquation();
	int getEquationNum();

	//RENDER & UPDATE
	void update();
	void render(sf::RenderTarget& target);

	//DATA MANAGEMENT
	void clear();
	void displayData();
};