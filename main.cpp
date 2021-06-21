#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

int main()
{

    sf::IpAddress ip = sf::IpAddress::getLocalAddress();
    sf::TcpSocket socket;
    sf::Packet packet;
    sf::Uint16 posX, posY;

    std::cout << "Enter (s) for server, or (c) for client" << std::endl;

    char status;
    std::cin >> status;

    sf::RenderWindow window(sf::VideoMode(1000,1000), "BlockGame");

    switch (status){
        case 's': {

        sf::TcpListener listener;
        listener.listen(2000);
        listener.accept(socket);

        window.setFramerateLimit(60);
            sf::RectangleShape redguy;
            redguy.setFillColor(sf::Color::Red);
            redguy.setSize(sf::Vector2f(20,20));
            redguy.setPosition(200,200);
            
            bool isMoving;
            float speed = 5;

            while (window.isOpen())
            {
                window.clear(sf::Color::Black);

                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }

                packet.clear();

                socket.receive(packet);

                packet >> posX >> posY;

                redguy.setPosition(posX, posY);

                window.draw(redguy);

                window.display();
            }



        }
        
        case 'c': {

            socket.connect(ip, 2000);

            window.setFramerateLimit(60);
            sf::RectangleShape redguy;
            redguy.setFillColor(sf::Color::Red);
            redguy.setSize(sf::Vector2f(20,20));
            redguy.setPosition(200,200);
            int dx, dy;
            bool isMoving;
            float speed = 5;

            while (window.isOpen())
            {
                window.clear(sf::Color::Black);

                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }

                isMoving = false; dx = 0; dy = 0;
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){dy = 1; isMoving = true;}
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){dy = -1; isMoving = true;}
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){dx = 1; isMoving = true;}
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){dx = -1; isMoving = true;}

                posX = redguy.getPosition().x + (speed*dx);

                posY = redguy.getPosition().y - (speed*dy);

                if (isMoving){
                    redguy.setPosition(posX, posY);
                }

                packet << posX << posY;

                socket.send(packet);

                packet.clear();

                window.draw(redguy);

                window.display();
            }

        }
    }

    return 0;
}