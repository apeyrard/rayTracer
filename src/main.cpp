#include <vec3.hpp>
#include <vector>
#include <object.hpp>
#include <sphere.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace raytracer;

int main()
{

    sf::Texture texture;
    int WIDTH = 800;
    int HEIGHT = 600;
    int SAMPLES = 1;
    texture.create(WIDTH, HEIGHT);
    sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracer");

    std::vector<Object*> listObjects;
    Sphere s = Sphere(Vec3(0, 0, 6), 10.0, Vec3(255, 0, 0));
    Sphere s2 = Sphere(Vec3(0, 10, 16), 15.0, Vec3(0, 255, 0));
    listObjects.push_back(&s);
    listObjects.push_back(&s2);

    //lights, temp
    std::vector<Vec3> lights;
    lights.push_back(Vec3(-800, -600, 10));
    lights.push_back(Vec3(0, 30, 16));


    for (int x = 0; x < HEIGHT; ++x)
    {
        //std::cout << x << std::endl;
        for (int y = 0; y < WIDTH; ++y)
        {
            Vec3 finalColor = Vec3();
            Vec3 origin = Vec3(0, 0, -50);
            Ray r = Ray(origin, Vec3(x-(HEIGHT/2), y-(WIDTH/2), -origin.z));
            for (int i = 0; i < SAMPLES; ++i)
            {
                double minDist = -1;
                for (auto it = listObjects.begin(); it != listObjects.end(); ++it)
                {
                    Vec3 normal = Vec3();
                    double dist = (*it)->intersect(r, normal);

                    Vec3 intersectionPoint = r.origin + (r.direction*dist);

                    if (dist != 0)
                    {
                        if ((dist < minDist) or (minDist == -1))
                        {
                            minDist = dist;
                            //iterating over lights
                            for (auto lightIt = lights.begin(); lightIt != lights.end(); ++lightIt)
                            {
                                finalColor = (*it)->color;

                            }
                        }
                    }
                }
            }
            pixels[y*4 + x*WIDTH*4] = finalColor.x;
            pixels[y*4 + x*WIDTH*4 +1] = finalColor.y;
            pixels[y*4 + x*WIDTH*4 +2] = finalColor.z;
            pixels[y*4 + x*WIDTH*4 +3] = 255;
        }
        texture.update(pixels);

        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            window.close();
        }

        window.clear(sf::Color::Black);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
    }

    std::cout << "done" << std::endl;
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        sf::Sprite sprite;
        sprite.setTexture(texture);
        window.draw(sprite);

        // draw everything here...
        // window.draw(...);

        // end the current frame
        window.display();
    }
}
