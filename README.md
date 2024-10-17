# 3D-Rendering-Software-SFML
PJ Kim / April 26th, 2024<br>
C++, SFML, Linear Algebra, Computer Graphics<br><br>

<h1>Introduction</h1>
The goal of this project was to create a software that could render 3D math functions and linear transformations. Users can enter a math function, and it will graph with coloring based on the x, y, and z axes. Users can also apply linear transformations such as shear, rotate, and scale. I chose this project to learn more about linear algebra, computer graphics, and C++.<br><br>

<h1>How I built it</h1>
I programmed the software in C++ using the SFML library and developed the entire program in Visual Studio. SFML is a 2D graphics library, so I had to convert 3D coordinates to 2D coordinates to render the 3D math functions.<br><br>

The first task was to create a 3D camera. Using linear algebra, I created a dynamic camera that could rotate and move around the 3D space using the mouse.<br><br>

The next task was to create infinite render generation. In order to do this, I implemented a 2D vector that stores the data of all the chunks. A chunk contains the points of a 3D math function within a certain domain and range. Theoretically, this software could render an infinite number of chunks.<br><br>

Finally, I implemented the math rendering capability by allowing users to enter a math function. I also added a linear transformation slider to apply linear transformations to the math function.<br><br>

<h1>Conclusion</h1>
I'm happy with the results of this project. I learned a lot about linear algebra, computer graphics, and C++. I also improved my problem solving skills and gained experience in computer visualizations.<br><br>

<h1>What's Next?</h1>
There are a lot of optimizations still left for this project. One big improvement would be to create a more pleasing user interface. This would allow users to understand the goal of the software more easily.
