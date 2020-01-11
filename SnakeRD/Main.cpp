#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <time.h>

using namespace std;

int N = 30, M = 20;
int size = 16;
int w = 16 * N;
int h = 16 * M;

int dir, num = 4;

struct Snake
{
	int x, y;
} s[100];

struct Fruit
{
	int x, y;
} f;

void Tick()
{
	for (int i = num; i > 0; --i)
	{
		s[i].x = s[i - 1].x; s[i].y = s[i - 1].y;
	}

	if (dir == 0) s[0].y += 1;
	if (dir == 1) s[0].x -= 1;
	if (dir == 2) s[0].x += 1;
	if (dir == 3) s[0].y -= 1;

	if ((s[0].x == f.x) && (s[0].y == f.y))
	{
		num++; f.x = rand() % N; f.y = rand() % M;
	}

	if (s[0].x > N) s[0].x = 0;		if (s[0].x < 0) s[0].x = N;
	if (s[0].y > M) s[0].y = 0;		if (s[0].y < 0) s[0].y = M;

	for (int i = 1; i < num; i++)
	{
		if (s[0].x == s[i].x && s[0].y == s[i].y) num = i;
	}

}

int main()
{
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();	// local ip address
	sf::TcpSocket socket;								// Program's inteface for provision exchange
	sf::Packet packet;									// For implementation packet's transferring of data
	char type;
	char mode = ' ';								// Mod: s - server, c - clients
	int x = 0;
	int y = 0;
	srand(time(0));
	char buffer[2000];
	size_t received;
	string text = "connect to";

	// --- Connecting --- //
	cout << ip << endl;
	cout << "Enter type of connecting: c - client or s - server" << endl;
	cin >> type;
	if (type == 's')
	{
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);					// Will be contain a new connection
		text += " server";
		mode = 's';
	}
	else if (type == 'c')
	{
		cout << "Enter the new IP: ";
		cin >> ip;
		socket.connect(ip, 2000);
		text += " client";
		mode = 'r';
	}
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	cout << buffer << endl;

	// --- Draw --- //
	sf::RenderWindow window(sf::VideoMode(w, h), "SnakeTest");

	sf::Texture t1, t2, t3;
	t1.loadFromFile("C:\\Users\\eldorado\\source\\repos\\SnakeRD\\SnakeRD\\images\\white.png");
	t2.loadFromFile("C:\\Users\\eldorado\\source\\repos\\SnakeRD\\SnakeRD\\images\\red.png");
	t3.loadFromFile("C:\\Users\\eldorado\\source\\repos\\SnakeRD\\SnakeRD\\images\\green.png");

	// --- Elements --- //
	sf::Sprite sprite1(t1);
	sf::Sprite sprite2(t2);
	sf::Sprite sprite3(t3);

	sf::Clock clock;
	float timer = 0, delay = 0.1;

	f.x = 10;
	f.y = 10;

	
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;


		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		if (mode == 's')
		{
			socket.receive(packet);					// The command that excepted data in packet from client's part
			if (packet >> x >> y)					// Pull value from packet in variable x and y (the action is already happening)
			{
				cout << x << ":" << y << endl;		// If success then input coordinates
			}
		}

		if (mode == 'r')
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dir = 1;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dir = 2;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) dir = 3;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) dir = 0;

			packet << x << y;
			socket.send(packet);
			packet.clear();

		}
		
		// --- Draw --- //

		if (timer > delay) { timer = 0; Tick(); }
		window.clear();

		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
			{
				sprite1.setPosition(i * 16, j * 16);
				window.draw(sprite1);
			}

		for (int i = 0; i < num; i++)
		{
			sprite2.setPosition(s[i].x * 16, s[i].y * 16);
			window.draw(sprite2);
		}

		sprite3.setPosition(f.x * 16, f.y * 16);
		window.draw(sprite3);

		window.display();
		}
	
	
	
	
	system("pause");
	return 0;
}