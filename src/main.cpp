#include <vec3.hpp>
#include <vector>
#include <object.hpp>
#include <sphere.hpp>
#include <plane.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>

// TODO : replace auto computation of normal by getnormal when needed

using namespace raytracer;

int MAXDEPTH = 10;
int WIDTH = 1500;
int HEIGHT = 700;
double SAMPLES = 4.0; // 16X supersampling
double EPSILON = 1.0e-07;

void initModel(std::vector<Object*>* listObjects)
{
    Sphere* s = new Sphere(Vec3(0, 0, 0), 5, Vec3(1, 0, 0 ), 0, 0.8);
    Sphere* sx = new Sphere(Vec3(10, 0, 0), 5, Vec3(0, 1, 0 ), 0, 0.8);
    Sphere* sy = new Sphere(Vec3(0, 10, 0), 5, Vec3(0, 0, 1), 0, 0.8);
    Sphere* sz = new Sphere(Vec3(0, 0, 10), 5, Vec3(0, 1, 1), 0, 0.8);

    Sphere* s2 = new Sphere(Vec3(50, 50, 1), 50, Vec3(0, 1, 1), 0.5, 0.2, 0.5);
    Spere* s3 = new Sphere(Vec3(50, -50, 1), 50, Vec3(0, 1, 1), 0, 0.5, 0.5);

    Plane* sol = new Plane(Vec3(0, 1, 0).norm(), -200, Vec3(1, 1, 1), 0, 0.8);
    Plane* plafond = new Plane(Vec3(0, 1, 0), 200, Vec3(1, 1, 0.2), 0, 0.8);
    Plane* gauche = new Plane(Vec3(1, 0, 0).norm(), 200, Vec3(0.8, 0.1, 1), 0, 0.8);
    Plane* droite = new Plane(Vec3(1, 0, 0).norm(), -200, Vec3(0.2, 1, 1), 0, 0.8);
    Plane* derriere = new Plane(Vec3(0, 0, 1).norm(), 200, Vec3(0.2, 1, 1), 0.8, 0.2);
    Plane* fond = new Plane(Vec3(0, 0, 1).norm(), -150, Vec3(0.5, 1.0, 0.2), 0, 0.8);
    listObjects->push_back(s);
    listObjects->push_back(sx);
    listObjects->push_back(sy);
    listObjects->push_back(sz);

    listObjects->push_back(s2);
    listObjects->push_back(s3);

    listObjects->push_back(sol);
    listObjects->push_back(plafond);
    listObjects->push_back(gauche);
    listObjects->push_back(droite);
    listObjects->push_back(derriere);
    listObjects->push_back(fond);
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
            // add spec lighting
            if((minObj->spec > 0) and (!inShadow))
            {
                Vec3 reflDir = shadowRay.direction - minNormal * (shadowRay.direction.dot(minNormal) *2);
                double dot = r.direction.dot(reflDir);
                if (dot > 0)
                {
                    double spec = pow(dot, 20) * minObj->spec;
                    result += minObj->color*spec;
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
    //lights.push_back(Vec3(-800, -600, 10));
    lights.push_back(Vec3(-100, 0, 1));
    lights.push_back(Vec3(0, 0, -100));


    for (int y = 0; y < HEIGHT; ++y)
    {
        //std::cout << x << std::endl;
        for (int x = 0; x < WIDTH; ++x)
        {
            Vec3 finalColor = Vec3();
            Vec3 origin = Vec3(0, 0, -150);
            for (double dx = double(x); dx < double(x)+1; dx += 1.0/SAMPLES)
            {
            for (double dy = double(y); dy < double(y)+1; dy += 1.0/SAMPLES)
            {
                Ray r = Ray(origin, Vec3(dx-(WIDTH/2), dy-(HEIGHT/2), -origin.z));
                finalColor += getColor(listObjects, r , lights)/(SAMPLES*SAMPLES);
            }
            }
            pixels[x*4 + y*WIDTH*4] = std::min(finalColor.x*255.0, 255.0); //TODO less naive clamp
            pixels[x*4 + y*WIDTH*4 +1] = std::min(finalColor.y*255.0, 255.0);
            pixels[x*4 + y*WIDTH*4 +2] = std::min(finalColor.z*255.0, 255.0);
            pixels[x*4 + y*WIDTH*4 +3] = 255;
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
