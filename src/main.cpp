#include <vec3.hpp>
#include <vector>
#include <object.hpp>
#include <sphere.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>

// TODO : replace auto computation of normal by getnormal when needed

using namespace raytracer;

int MAXDEPTH = 10;
int WIDTH = 800;
int HEIGHT = 600;
int SAMPLES = 1;
double EPSILON = 1.0e-07;

void initModel(std::vector<Object*>* listObjects)
{
    Sphere* s = new Sphere(Vec3(0, 0, 1000), 300.0, Vec3(255, 0, 0), 0, 0.5);
    Sphere* s2 = new Sphere(Vec3(0, 5, 30), 15.0, Vec3(0, 255, 0), 1 , 0);
    Sphere* s4 = new Sphere(Vec3(-50, 5, 30), 15.0, Vec3(0, 255, 255), 0 , 1);
    Sphere* s3 = new Sphere(Vec3(0, -100400, 0), 100000, Vec3(0, 255, 0), 0, 1);
    listObjects->push_back(s);
    listObjects->push_back(s2);
    listObjects->push_back(s3);
    listObjects->push_back(s4);
}

Vec3 getColor(std::vector<Object*> objects, Ray r, std::vector<Vec3> lights)
{
    Vec3 result = Vec3();
    double minDist = -1;
    Object* minObj = NULL;
    Vec3 minInterPoint;
    Vec3 minNormal;

    // Finding closest intersection
    for (auto it = objects.begin(); it != objects.end(); ++it)
    {
        Vec3 normal = Vec3();
        double dist = (*it)->intersect(r, normal);

        Vec3 intersectionPoint = r.origin + (r.direction*dist) + normal*EPSILON;
        if (dist != 0)
        {
            if ((dist < minDist) or (minDist == -1))
            {
                minDist = dist;
                minObj = (*it);
                minInterPoint = intersectionPoint;
                minNormal = normal;
            }
        }
    }

    //Now computing color
    if (minObj != NULL)
    {
        for (auto lightIt = lights.begin(); lightIt != lights.end(); ++lightIt)
        {
            Ray shadowRay = Ray(minInterPoint, (*lightIt)-minInterPoint);
            bool inShadow = false;
            for(auto objIt = objects.begin(); objIt != objects.end(); ++objIt)
            {
                Vec3 tmp = Vec3();
                double inter = (*objIt)->intersect(shadowRay, tmp);
                if (inter  > 0)
                {
                    if (inter < ((*lightIt)-minInterPoint).length())
                    {
                        inShadow = true;
                    }
                }
            }
            if((minObj->diffuse>0) and (!inShadow))
            {
                double dot = minNormal.dot(shadowRay.direction);
                if (dot > 0)
                {
                    double diff = dot * minObj->diffuse;
                    result += minObj->color * diff; //TODO * light.color
                }
            }
        }

        // Get reflection
        double refl = minObj->reflection;
        if (refl > 0.0)
        {
            if (r.depth < MAXDEPTH)
            {
                Vec3 reflDir = r.direction - minNormal * (r.direction.dot(minNormal) *2);
                Ray reflectedRay = Ray(minInterPoint, reflDir, r.depth+1);
                Vec3 reflColor = getColor(objects, reflectedRay, lights);
                result += reflColor * refl;

            }
        }
    }
    return result;
}

int main()
{

    sf::Texture texture;
    texture.create(WIDTH, HEIGHT);
    sf::Uint8* pixels = new sf::Uint8[WIDTH * HEIGHT * 4];
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Ray Tracer");

    std::vector<Object*> listObjects;
    initModel(&listObjects);

    //lights, temp
    std::vector<Vec3> lights;
    lights.push_back(Vec3(-800, -600, 10));
    lights.push_back(Vec3(0, -50, 0));


    for (int x = 0; x < HEIGHT; ++x)
    {
        //std::cout << x << std::endl;
        for (int y = 0; y < WIDTH; ++y)
        {
            Vec3 origin = Vec3(0, 0, -10000);
            Ray r = Ray(origin, Vec3(x-(HEIGHT/2), y-(WIDTH/2), -origin.z));
            for (int i = 0; i < SAMPLES; ++i)
            {
                finalColor = getColor(listObjects, r , lights);
            }
            pixels[y*4 + x*WIDTH*4] = std::min(finalColor.x, 255.0); //TODO less naive clamp
            pixels[y*4 + x*WIDTH*4 +1] = std::min(finalColor.y, 255.0);
            pixels[y*4 + x*WIDTH*4 +2] = std::min(finalColor.z, 255.0);
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
        window.display();
    }

    //Cleanup
    delete pixels;
    for (auto it = listObjects.begin(); it != listObjects.end(); ++it)
    {
        delete(*it);
    }
}
