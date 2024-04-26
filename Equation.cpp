#include "Equation.h"
Equation::Equation(sf::Vector3f position, double zoom, sf::Vector3f size, Matrix<double, 3, 3> basis, double coefficient, int dist, sf::Vector3f degrees, string equation, bool color) : position(position), zoom(zoom), size(size), basis(basis), equation(equation), color(color)
{
    projection.setPrimitiveType(sf::LinesStrip);
	initPoints(position, size, coefficient, dist);
    rotate(degrees);
    project();
}
Equation::~Equation()
{

}
float Equation::function(float x, float y)
{
    if (equation == "1")
    {
        return 1;
    }
    else if (equation == "x")
    {
        return x;
    }
    else if (equation == "y")
    {
        return y;
    }
    else if (equation == "x + y")
    {
        return x + y;
    }
    else if (equation == "sin(x) + cos(y)")
    {
        return sin(x * 0.0174533) + cos(y * 0.0174533);
    }
    else if (equation == "x^2 + y^2")
    {
        return pow(x, 2) + pow(y, 2);
    }
    else if (equation == "1-abs(x+y)-abs(y-x)")
    {
        return 1 - abs(x + y) - abs(y - x);
    }
    else if (equation == "sin(x^2+y^2)")
    {
        return sin(pow(x * 0.0174533, 2) + pow(y * 0.0174533, 2));
    }
    else if (equation == "tan(x+y)")
    {
        return tan(x * 0.0174533 + y * 0.0174533);
    }
    return 0;
}
sf::Vector3f Equation::getPosition()
{
    return position;
}


void Equation::rotate(sf::Vector3f degrees)
{
    //roll
    Matrix<double, 3, 3> Rx
    {
        {1, 0, 0},
        {0, cos(degrees.x * 0.0174533), -sin(degrees.x * 0.0174533)},
        {0, sin(degrees.x * 0.0174533), cos(degrees.x * 0.0174533)}
    };

    //pitch
    Matrix<double, 3, 3> Ry
    {
        {cos(degrees.y * 0.0174533), 0, sin(degrees.y * 0.0174533)},
        {0, 1, 0},
        {-sin(degrees.y * 0.0174533), 0, cos(degrees.x * 0.0174533)}
    };

    //yaw
    Matrix<double, 3, 3> Rz
    {
        {cos(degrees.z * 0.0174533), -sin(degrees.z * 0.0174533), 0},
        {sin(degrees.z * 0.0174533), cos(degrees.z * 0.0174533), 0},
        {0, 0, 1}
    };

    //general 3d rotation
    Matrix<double, 3, 3> R;
    R = Rx * Ry * Rx;

    for (int i = 0; i < points.size(); i++)
    {
        Vector3d u = { points[i].x, points[i].y, points[i].z };
        points[i] = sf::Vector3f((R * u)[0], (R * u)[1], (R * u)[2]);
    }
}


void Equation::project()
{
    //establishes scaling transformation matrix
    Matrix<double, 3, 3> A{
        {zoom, 0, 0},
        {0, zoom, 0},
        {0, 0, zoom}
    };
    for (int i = 0; i < points.size(); i++)
    {
        Vector3d u = { points[i].x, points[i].y, points[i].z };
        points[i] = sf::Vector3f((A * u)[0], (A * u)[1], (A * u)[2]);
    }

    //establishes transformation projection matrix onto plane
    Vector3d e1 = basis.col(0);
    Vector3d e2 = basis.col(1);
    Matrix<double, 2, 3> T;
    T.row(0) = e1;
    T.row(1) = e2;

    //multiplies basis vectors by transformation matrices
    for (int i = 0; i < points.size(); i++)
    {
        Vector3d u = { points[i].x, points[i].y, points[i].z };
        projection[i] = sf::Vector2f((T * u)[0], (T * u)[1]);

        sf::Uint8 red = round(points[i].x / 10);
        sf::Uint8 blue = round(points[i].y / 10);
        sf::Uint8 green = round(points[i].z / 10);
        if ((red > 255) && (blue < 255) && (green < 255))
        {
            red -= 155;
            blue -= 155;
            green -= 155;
        }
        if ((red < 100) && (blue < 100) && (green < 100))
        {
            red += 100;
            blue += 100;
            green += 100;
        }
        if (color)
        {
            projection[i].color = sf::Color{ red, blue, green, 255 };
        }
        else
        {
            projection[i].color = sf::Color::White;
        }
    }
}
void Equation::render(sf::RenderTarget& target)
{
    target.draw(projection);
}
void Equation::initPoints(sf::Vector3f pos, sf::Vector3f size, double intensity, int dist)
{

    // Define the dimensions of the rectangular prism
    float width = pos.x + (size.x / 2);  // along x-axis
    float height = pos.y + (size.y / 2); // along y-axis
    
    float x = pos.x - (size.x / 2);
    float y = pos.y - (size.y / 2);
    float z;

    // Initialize direction variables for zigzag motion
    bool moveRight = true;
    bool moveDown = true;

    while (y < height)
    {
        // Iterate over x-coordinates based on the direction
        if (moveRight) {
            while (x < width)
            {
                z = intensity * function(x, y) + pos.z;
                points.push_back({ x, y, z });
                x += dist;
            }
        }
        else {
            while (x > pos.x - (size.x / 2))
            {
                z = intensity * function(x, y) + pos.z;
                points.push_back({ x, y, z });
                x -= dist;
            }
        }

        // Change direction for the next iteration
        moveRight = !moveRight;

        // Move to the next row
        z = intensity * function(x, y) + pos.z;
        points.push_back({ x, y, z });
        y += dist;
        if (y + dist < height)
        {
            z = intensity * function(x, y) + pos.z;
            points.push_back({ x, y, z });
        }
    }

    while (x < width)
    {
        z = intensity * function(x, y) + pos.z;
        points.push_back({ x, y, z });
        // Iterate over x-coordinates based on the direction
        if (moveRight) {
            while (y < height)
            {
                z = intensity * function(x, y) + pos.z;
                points.push_back({ x, y, z });
                y += dist;
            }
        }
        else {
            while (y > pos.y - (size.y / 2))
            {
                z = intensity * function(x, y) + pos.z;
                points.push_back({ x, y, z });
                y -= dist;
            }
        }

        // Change direction for the next iteration
        moveRight = !moveRight;

        // Move to the next row
        z = intensity * function(x, y) + pos.z;
        points.push_back({ x, y, z });
        x += dist;
    }

    projection.resize(points.size());
}