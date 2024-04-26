#include "Render.h"

//CONSTRUCTOR & DECONSTRUCTOR
Render::Render() 
{
	initVariables();
	initPosTable();
	initBasisPoints();
	initChunk();
	initEquations();
	sortChunks();
}
Render::~Render()
{

}

//INITIATION FUNCTIONS
void Render::initVariables()
{
	xRotation = false;
	yRotation = false;
	zRotation = false;
	cursor = true;
	equationNum = 0;
	rotationSpeed = 0;
	color = false;
	numRenderChunks = 1;
	intensity = 1;
	coefficient = 0;
	dist = 100;
	offset = { 1, 1 };
	rotation = { 0, 0, 0 };
	currentPos = { 0, 0 };
	position = { 0 ,0 ,0 };
	prevPos = currentPos;
}
void Render::initEquations()
{
	equations[0] = "1";
	equations[1] = "x";
	equations[2] = "y";
	equations[3] = "x + y";
	equations[4] = "sin(x) + cos(y)";
	equations[5] = "x^2 + y^2";
	equations[6] = "1-abs(x+y)-abs(y-x)";
	equations[7] = "sin(x^2+y^2)";
	equations[8] = "tan(x+y)";
}
void Render::initChunk() //initiates the 9 initial chunks
{ 

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1)
			{
				chunks.push_back({ (float)i * 1000 - 1000, (float)j * 1000 - 1000, 0 });
			}
			else
			{
				chunks.push_back({ (float)i * 1000 - 1000, (float)j * 1000 - 1000, 0 });
			}
		}
	}
}
void Render::initPosTable() //initiates the 9 initial chunk positions
{
	posTable.push_back({ -1, -1 });
	posTable.push_back({ -1, 0 });
	posTable.push_back({ -1, 1 });
	posTable.push_back({ 0, -1 });
	posTable.push_back({ 0, 0 });
	posTable.push_back({ 0, 1 });
	posTable.push_back({ 1, -1 });
	posTable.push_back({ 1, 0 });
	posTable.push_back({ 1, 1 });
}
void Render::initBasisPoints() 
{
	basisPoints[0][0] = { 0, 0, 0 }; //i hat
	basisPoints[0][1] = { 100, 0, 0 };
	initBasisProjection(0, sf::Color::Red);
	basisPoints[1][0] = { 0, 0, 0 }; //j hat
	basisPoints[1][1] = { 0, 100, 0 };
	initBasisProjection(1, sf::Color::Green);
	basisPoints[2][0] = { 0, 0, 0 }; //k hat
	basisPoints[2][1] = { 0, 0, -100 };
	initBasisProjection(2, sf::Color::Blue);
}
void Render::initBasisProjection(int index, sf::Color color) //allows initiation of bases
{
	basisProjection[index].setPrimitiveType(sf::Lines);
	basisProjection[index].resize(2);
	basisProjection[index][0].color = color;
	basisProjection[index][1].color = color;
}

//SORTING FUNCTIONS
void Render::sortChunks()
{
	//sorts objects based on x and y
	std::sort(chunks.begin(), chunks.end(), [](sf::Vector3f obj1, sf::Vector3f obj2) {
		if (obj1.x != obj2.x) {
			return obj1.x < obj2.x;
		}
		else {
			return obj1.y < obj2.y;
		}
		});


	//places the objects into sorted objects 2d vector
	sortedChunks.clear();
	int i = 0;
	int j = 0;
	while (i < chunks.size())
	{
		std::vector<sf::Vector3f> row;
		row.push_back(chunks[i]);
		j = i + 1;
		while (j < chunks.size() && chunks[i].x == chunks[j].x)
		{
			row.push_back(chunks[j]);
			j++;
		}
		sortedChunks.push_back(row);
		i = j;
	}
	float minY = findMinY();
	float maxY = findMaxY();
	std::vector<sf::Vector3f> range;
	for (int i = minY / 1000; i <= maxY / 1000; i++)
	{
		range.push_back({ 0, (float)i * 1000, 0 });
	}
	for (int i = 0; i < sortedChunks.size(); i++)
	{
		std::vector<sf::Vector3f> row;
		row.clear();
		int k = 0;
		for (int j = 0; j < range.size(); j++)
		{
			if (k < sortedChunks[i].size() && range[j].y == sortedChunks[i][k].y)
			{
				row.push_back(sortedChunks[i][k]);
				k++;
			}
			else
			{
				row.push_back({ -99999, -99999, -99999 }); //placeholder for empty spots
			}
		}
		sortedChunks[i] = row;
	}

	//finds offset
	offset = findOffset();
}
sf::Vector2f Render::findOffset()
{
	//finds location of current position relative to the 2d sorted objects vector
	for (int i = 0; i < sortedChunks.size(); ++i)
	{
		for (int j = 0; j < sortedChunks[i].size(); ++j)
		{
			if (sortedChunks[i][j].x == -currentPos.x * 1000 && sortedChunks[i][j].y == -currentPos.y * 1000)
			{
				return sf::Vector2f((float)i, (float)j);
			}
		}
	}
	return sf::Vector2f(-1, -1);
}
float Render::findMinY() //returns minimum y of objects
{
	if (chunks.empty()) {
		return 0.0f;
	}
	float minY = chunks[0].y;
	for (int i = 1; i < chunks.size(); ++i) {
		float currentY = chunks[i].y;
		if (currentY < minY) {
			minY = currentY;
		}
	}
	return minY;
}
float Render::findMaxY() //returns maximum y of objects
{
	if (chunks.empty()) {
		return 0.0f;
	}
	float maxY = chunks[0].y;
	for (int i = 1; i < chunks.size(); ++i) {
		float currentY = chunks[i].y;
		if (currentY > maxY) {
			maxY = currentY;
		}
	}
	return maxY;
}

//UPDATE FUNCTIONS
void Render::update()
{
	updateChunk();
	sortChunks();
	updateBasisVectors();
	updateRenderChunk();

	if (xRotation)
	{
		rotation.x += rotationSpeed;
	}
	if (yRotation)
	{
		rotation.y += rotationSpeed;
	}
	if (zRotation)
	{
		rotation.z += rotationSpeed;
	}
}
void Render::updateChunk()
{
	//renders position relative to the chunks
	currentPos.x = round(position.x / 1000);
	currentPos.y = round(position.y / 1000);

	//calls movement function based on the change of the chunk position
	if (currentPos - prevPos == sf::Vector2f{ 1,0 })
	{
		left();
	}
	if (currentPos - prevPos == sf::Vector2f{ 0,1 })
	{
		down();
	}
	if (currentPos - prevPos == sf::Vector2f{ -1,0 })
	{
		right();
	}
	if (currentPos - prevPos == sf::Vector2f{ 0,-1 })
	{
		up();
	}
	if (currentPos - prevPos != sf::Vector2f{ 0,0 })
	{
		prevPos = currentPos;
	}
}
void Render::updateBasisVectors()
{
	//establishes transformation projection matrix onto plane
	Vector3d e1 = basis.col(0);
	Vector3d e2 = basis.col(1);
	Matrix<double, 2, 3> T;
	T.row(0) = e1;
	T.row(1) = e2;

	//multiplies basis vectors by transformation matrices
	for (int i = 0; i < 3; i++)
	{
		Vector3d u = { basisPoints[i][1].x, basisPoints[i][1].y, basisPoints[i][1].z };
		basisProjection[i][1].position = sf::Vector2f((T * u)[0], (T * u)[1]);
	}
}
void Render::updateRenderChunk() //creates cubes for the render of the space
{
	//center render
	equation[0] = new Equation(distFromChunkCenter(sortedChunks[offset.x][offset.y]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum] , color);

	//sides render
	if (numRenderChunks == 5 || numRenderChunks == 9)
	{
		equation[1] = new Equation(distFromChunkCenter(sortedChunks[offset.x - 1][offset.y]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum], color);
		equation[2] = new Equation(distFromChunkCenter(sortedChunks[offset.x + 1][offset.y]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum], color);
		equation[3] = new Equation(distFromChunkCenter(sortedChunks[offset.x][offset.y - 1]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum], color);
		equation[4] = new Equation(distFromChunkCenter(sortedChunks[offset.x][offset.y + 1]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum], color);
	}

	//corners render
	if (numRenderChunks == 9)
	{
		equation[5] = new Equation(distFromChunkCenter(sortedChunks[offset.x - 1][offset.y - 1]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum], color);
		equation[6] = new Equation(distFromChunkCenter(sortedChunks[offset.x + 1][offset.y - 1]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum], color);
		equation[7] = new Equation(distFromChunkCenter(sortedChunks[offset.x - 1][offset.y + 1]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum], color);
		equation[8] = new Equation(distFromChunkCenter(sortedChunks[offset.x + 1][offset.y + 1]), intensity, { 1000, 1000, 1000 }, basis, coefficient, dist, rotation, equations[equationNum], color);
	}
	

}
sf::Vector3f Render::distFromChunkCenter(sf::Vector3f chunk)
{
	return sf::Vector3f(chunk.x + position.x, chunk.y + position.y, chunk.z + position.z);
}

//POSITION TABLE FUNCTIONS
bool Render::posTableContains(sf::Vector2f pos)
{
	if (std::find(posTable.begin(), posTable.end(), pos) != posTable.end()) {
		return true;
	}
	return false;
}

//ZOOM & POSITION FUNCTIONS
void Render::zoom(double intensity) 
{
	this->intensity *= intensity;
	Matrix<double, 3, 3> T{
		{intensity, 0, 0},
		{0, intensity, 0},
		{0, 0, intensity}
	};
	for (int i = 0; i < 3; i++) {
		Vector3d u = { basisPoints[i][1].x, basisPoints[i][1].y, basisPoints[i][1].z };
		basisPoints[i][1] = sf::Vector3f((T * u)[0], (T * u)[1], (T * u)[2]);
	}
}
void Render::move(sf::Vector3f movement) 
{
	//scales movement based on zoom
	position += sf::Vector3f(movement.x * (1 / intensity), movement.y * (1 / intensity), movement.z * (1 / intensity));
}

//MOVEMENT FUNCTIONS
void Render::right()
{

	for (int i = -1; i < 2; i++)
	{
		float x = (-currentPos.x + 1);
		float y = (-currentPos.y + i);
		if (!posTableContains(sf::Vector2f(x, y)))
		{
			posTable.push_back(sf::Vector2f(x, y));
			chunks.push_back({ 1000 * x, 1000 * y, 0});
		}
	}
}
void Render::up()
{

	for (int i = -1; i < 2; i++)
	{
		float x = (-currentPos.x - i);
		float y = (-currentPos.y + 1);
		if (!posTableContains(sf::Vector2f(x, y)))
		{
			posTable.push_back(sf::Vector2f(x, y));
			chunks.push_back({ 1000 * x, 1000 * y, 0});
		}
	}
}
void Render::left()
{

	for (int i = 0; i < 3; i++)
	{
		float x = (-currentPos.x - 1);
		float y = (-currentPos.y - i + 1);
		if (!posTableContains(sf::Vector2f(x, y)))
		{
			posTable.push_back(sf::Vector2f(x, y));
			chunks.push_back({ 1000 * x, 1000 * y, 0});
		}
	}
}
void Render::down()
{

	for (int i = 0; i < 3; i++)
	{
		float x = (-currentPos.x + i - 1);
		float y = (-currentPos.y - 1);
		if (!posTableContains(sf::Vector2f(x, y)))
		{
			posTable.push_back(sf::Vector2f(x, y));
			chunks.push_back({ 1000 * x, 1000 * y, 0});
		}
	}
}

//RENDER FUNCTION
void Render::render(sf::RenderTarget& target) {
	for (int i = 0; i < numRenderChunks; i++)
	{
		equation[i]->render(target);
	}
	for (int i = 0; i < 3; i++) {
		if (cursor)
		{
			target.draw(basisProjection[i]);
		}
	}
}

//DATA MANAGEMENT
void Render::displayData()
{	
	std::cout << "Stored & Sorted Data:" << std::endl;
	for (int i = 0; i < sortedChunks.size(); i++)
	{
		for (int j = 0; j < sortedChunks[i].size(); j++)
		{
			if (sortedChunks[i][j].z != -99999 && i == offset.x && j == offset.y)
			{
				std::cout << "O";
			}
			else if (sortedChunks[i][j].z != -99999)
			{
				std::cout << "#";
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}
void Render::setBasis(Matrix<double, 3, 3> basis)
{
	this->basis = basis;
}
void Render::setCoefficient(double coefficient)
{
	this->coefficient = coefficient;
}
void Render::setDist(int dist)
{
	this->dist = dist;
}
void Render::setRotation(sf::Vector3f rotation)
{
	this->rotation = rotation;
}
void Render::setColor(bool color)
{
	this->color = color;
}
void Render::setXRotation(bool x)
{
	xRotation = x;
}
void Render::setYRotation(bool y)
{
	yRotation = y;
}
void Render::setZRotation(bool z)
{
	zRotation = z;
}
void Render::setRotationSpeed(double r)
{
	rotationSpeed = r;
}
void Render::setCursor(bool cursor)
{
	this->cursor = cursor;
}
void Render::setEquationNum(int eq)
{
	equationNum = eq;
}
bool Render::getColor()
{
	return color;
}
double Render::getRotationSpeed()
{
	return rotationSpeed;
}
sf::Vector3f Render::getRotation()
{
	return rotation;
}
bool Render::getCursor()
{
	return cursor;
}
bool Render::getXRotation()
{
	return xRotation;
}
bool Render::getYRotation()
{
	return yRotation;
}
bool Render::getZRotation()
{
	return zRotation;
}
string Render::getEquation()
{
	return equations[equationNum];
}
int Render::getEquationNum()
{
	return equationNum;
}
double Render::getCoefficient()
{
	return coefficient;
}
void Render::setNumRenderChunks(int num)
{
	numRenderChunks = num;
}
int Render::getNumRenderChunks()
{
	return numRenderChunks;
}
void Render::clear()
{
	posTable.clear();
	chunks.clear();
	sortedChunks.clear();
	initVariables();
	initPosTable();
	initBasisPoints();
	initChunk();
}