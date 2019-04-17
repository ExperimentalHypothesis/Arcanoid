#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

constexpr unsigned int window_width{ 800 }, window_height{ 600 };
constexpr float ball_radius{ 5.f }, ball_velocity{ 5.f };
constexpr float paddle_width{ 100.f }, paddle_height{ 10.f }, paddle_velocity{ 6.f };
constexpr float brick_width{ 60 }, brick_height{ 10 }, brick_on_xaxis{ 12 }, brick_on_yaxis{ 5 };


//making a ball class
struct Ball
{
	sf::CircleShape shape_circle;
	sf::Vector2f velocity{ -ball_velocity, -ball_velocity }; //vector storing velocity and updating position each loop -> illusion o movement means it starts going up direction when launched

	//constructor for the ball
	Ball(float x_axis, float y_axis)
	{
		shape_circle.setPosition(x_axis, y_axis);
		shape_circle.setRadius(ball_radius);
		shape_circle.setFillColor(sf::Color::White);
		shape_circle.setOrigin(ball_radius, ball_radius);
	}
	//methods of the ball to know, where it currently is
	float x() { return shape_circle.getPosition().x; };
	float y() { return shape_circle.getPosition().y; };
	float left() { return x() - shape_circle.getRadius(); };
	float right() { return x() + shape_circle.getRadius(); };
	float top() { return y() - shape_circle.getRadius(); };
	float bottom() { return y() + shape_circle.getRadius(); };

	void update()
	{
		shape_circle.move(velocity);

		if (left() < 0) velocity.x = ball_velocity;
		else if (right() > window_width) velocity.x = -ball_velocity;

		if (top() < 0) velocity.y = ball_velocity;
		else if (bottom() > window_height) velocity.y = -ball_velocity;
	}
};

struct Paddle
{
	sf::RectangleShape shape_rectangle;
	sf::Vector2f velocity{};

	//constructor
	Paddle(float x_axis, float y_axis)
	{
		shape_rectangle.setSize({ paddle_width, paddle_height });
		shape_rectangle.setPosition(x_axis, y_axis);
		shape_rectangle.setFillColor(sf::Color::Blue);
		shape_rectangle.setOrigin(paddle_width / 2, paddle_height / 2);
	}
	//methods for the paddle to know, where it currently is / look to the docs
	float x() { return shape_rectangle.getPosition().x; };
	float y() { return shape_rectangle.getPosition().y; };
	float left() { return x() - shape_rectangle.getSize().x; };
	float right() { return x() + shape_rectangle.getSize().x; };
	float top() { return y() - shape_rectangle.getSize().y; };
	float bottom() { return y() + shape_rectangle.getSize().y; };

	void update()
	{
		shape_rectangle.move(velocity);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && left() > 0)
		{
			velocity.x = -paddle_velocity;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && right() < window_width)
		{
			velocity.x = paddle_velocity;
		}
		else velocity.x = 0;
	}
};

struct Brick
{
	sf::RectangleShape shape;

	//constructor taking two parameters saying, where the bricks will be
	Brick(float x_axis, float y_axis)
	{
		shape.setPosition(x_axis, y_axis);
		shape.setSize({ brick_width, brick_height });
		shape.setOrigin(brick_width / 2, brick_height / 2);
		shape.setFillColor(sf::Color::Blue);
	}

	bool destroyed{ false };
};


//check if paddle and ball are hitting (intersection of two objects)
//generic foo taking two objects by reference and returning true if they are hitting each other
//first_obj might be for example "ball" second_obj might be "paddle", or "brick".. that is why it is generic foo
//template<class T1, class T2> bool 	//if (ball.right() > paddle.left() && ball.left() < paddle.right() && ...... && ball.top() < paddle.bottom())
bool is_hitting(Ball &ball, Paddle &paddle)
{
	return ball.right() > paddle.left() && ball.left() < paddle.right() && ball.bottom() > paddle.top() && ball.left() && ball.top() < paddle.bottom();
}

void test_ball_paddle_hit(Paddle &paddle, Ball &ball)
{
	if (!is_hitting(ball, paddle)) return;
	ball.velocity.y = -ball_velocity;

	if (ball.x() < paddle.x()) ball.velocity.x = -ball_velocity;
	else ball.velocity.x = ball_velocity;
}



int main()
{
	Ball ball{ window_width / 2, window_height / 2 };
	Paddle paddle{ window_width / 2, window_height - 50 }; //-fifty pixells above bottom
	std::vector<Brick> bricks;

	//fill the vector using 2d loop
	for (int i = 0; i < brick_on_xaxis; ++i)
		for (int j = 0; j < brick_on_yaxis; ++j)
			bricks.emplace_back( (i + 1) * (brick_width + 2), (j + 1) * (brick_height + 2)); //emplace_back takes two parametrs, the firs one i X-coordinate, the second one is Y-coordinate

	sf::RenderWindow window{ {window_width, window_height}, "Arkanoid One" };
	window.setFramerateLimit(60);
	while (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		window.clear(sf::Color::Black);

		paddle.update();
		ball.update();

		test_ball_paddle_hit(paddle, ball);

		window.draw(ball.shape_circle);
		window.draw(paddle.shape_rectangle);

		for (auto &brick : bricks) window.draw(brick.shape);

		window.display();

	}

	return 0;
}

