#include <ctime>

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>

#include <SFML/Graphics.hpp>

#include <vec3.hpp>
#include <object.hpp>
#include <sphere.hpp>
#include <plane.hpp>
#include <box.hpp>
// TODO : replace auto computation of normal by getnormal when needed
// TODO : monte carlo

using namespace raytracer;

int MAXDEPTH = 5;
int MAXREFLDEPTH = 5;
int WIDTH = 800;
int HEIGHT = 600;
double SAMPLES = 8.0;
double EPSILON = 1.0e-07;


void initModel(std::vector<Object*>* listObjects, std::vector<Object*>* lights)
{
    Sphere* s = new Sphere(Vec3(0, 0, 0), 5, Vec3(1, 0, 0 ), 0, 0.8);
    Sphere* sx = new Sphere(Vec3(10, 0, 0), 5, Vec3(0, 1, 0 ), 0, 0.8);
    Sphere* sy = new Sphere(Vec3(0, 10, 0), 5, Vec3(0, 0, 1), 0, 0.8);
    Sphere* sz = new Sphere(Vec3(0, 0, 10), 5, Vec3(0, 1, 1), 0, 0.8);

    Sphere* s2 = new Sphere(Vec3(50, 50, 20), 50, Vec3(0, 1, 1), 0.8, 0.2, 0);
    Sphere* s3 = new Sphere(Vec3(50, -150, 1), 50, Vec3(0, 1, 1), 0, 0.5, 0.5);

    Sphere* lum = new Sphere(Vec3(0, -100, 1), 50, Vec3(1, 1, 1), 0, 0, 0, 0, 1);

    Plane* sol = new Plane(Vec3(0, 1, 0).norm(), -200, Vec3(1, 1, 1), 0, 0.8);
    Plane* plafond = new Plane(Vec3(0, 1, 0), 200, Vec3(1, 1, 0.2), 0, 0.8);
    Plane* gauche = new Plane(Vec3(1, 0, 0).norm(), 200, Vec3(0.8, 0.1, 1), 0, 0.8);
    Plane* droite = new Plane(Vec3(1, 0, 0).norm(), -200, Vec3(0.2, 1, 1), 1, 0);
    Plane* derriere = new Plane(Vec3(0, 0, 1).norm(), 2000, Vec3(0.2, 1, 1), 0.8, 0.2);
    Plane* fond = new Plane(Vec3(0, 0, 1).norm(), -150, Vec3(0.5, 1.0, 0.2), 0, 0.8);
    Box* b = new Box(Vec3(140, 140, 90), Vec3(), Vec3(50, 50, 50), Vec3(0.35, 0.65, 0.80), 0.8, 0, 0.0, 0.0, 0.0);

    listObjects->push_back(b);

    listObjects->push_back(s);
    listObjects->push_back(sx);
    listObjects->push_back(sy);
    listObjects->push_back(sz);

    listObjects->push_back(s2);
    listObjects->push_back(s3);

    listObjects->push_back(lum);
    lights->push_back(lum);

    listObjects->push_back(sol);
    listObjects->push_back(plafond);
    listObjects->push_back(gauche);
    listObjects->push_back(droite);
    listObjects->push_back(derriere);
    listObjects->push_back(fond);
}

Vec3 getColor(std::vector<Object*> objects, Ray r, std::vector<Object*> lights, Vec3 prevEmittance, int emission)
{
    //Stop and return black if maxDepth
    if (r.depth >= MAXDEPTH)
    {
        return Vec3();
    }

    std::random_device rd;
    std::default_random_engine rng(rd());
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
        //Stop and return black if not enough energy
        Vec3 emittance = prevEmittance * minObj->color;
        if((emittance.x < 0.001) and (emittance.y < 0.001) and (emittance.z < 0.001))
        {
            return Vec3();
        }

        //If a light is hit, we got lighting
        if (minObj->light > 0)
        {
            result += minObj->color * minObj->light;
            return result*emission;
        }

        //diff
        if (minObj->diffuse > 0)
        {
            //TODO make this faster by getting shadow percentage
            //for each light, check if we are in shadow
            for (auto lightIt = lights.begin(); lightIt != lights.end(); ++lightIt)
            {
                //Get random point on surface of light
                Vec3 randSurfacePoint = (*lightIt)->getRandPoint(rng);
                //Shoot shadow ray to random point on light surface
                Ray shadowRay = Ray(minInterPoint, randSurfacePoint - minInterPoint);
                bool inShadow = false;
                for(auto objIt = objects.begin(); objIt!=objects.end();++objIt)
                {
                    if((*objIt) != (*lightIt))
                    {
                        Vec3 tmp = Vec3();
                        double inter = (*objIt)->intersect(shadowRay, tmp);
                        if ((inter > 0) and (inter < (randSurfacePoint - minInterPoint).length()))
                        {
                            inShadow = true;
                            break;
                        }
                    }
                }
                if(!inShadow)
                {
                    double dot = minNormal.dot(shadowRay.direction);
                    if (dot > 0)
                    {
                        double diff = dot * minObj->diffuse;
                        result += minObj->color * (*lightIt)->color * diff;
                    }

                }
                if (r.depth < MAXDEPTH)
                {
                    std::normal_distribution<double> gauss(0,1);
                    Ray randomRay = Ray(minInterPoint, Vec3(gauss(rng), gauss(rng), gauss(rng)), r.depth+1);
                    if (randomRay.direction.dot(minNormal) < 0)
                    {
                        randomRay.direction *= -1;
                    }
                    double dot = minNormal.dot(randomRay.direction);
                    double diff = minObj->diffuse * dot;
                    result += minObj->color * (getColor(objects, randomRay, lights, emittance, 0)) * diff;
                }
            }
            /*// add spec lighting
            if((minObj->spec > 0) and (!inShadow))
            {
                Vec3 reflDir = shadowRay.direction - minNormal * (shadowRay.direction.dot(minNormal) *2);
                double dot = r.direction.dot(reflDir);
                if (dot > 0)
                {
                    double spec = pow(dot, 128) * minObj->spec;
                    result += minObj->color*spec;
                }
            }*/
        }


        // Get reflection
        double refl = minObj->reflection;
        if (refl > 0.0)
        {
            if (r.depth < MAXDEPTH)
            {
                Vec3 reflDir = r.direction - minNormal * (r.direction.dot(minNormal) *2);
                Ray reflectedRay = Ray(minInterPoint, reflDir, r.depth+1);
                Vec3 reflColor = getColor(objects, reflectedRay, lights, emittance, 1);
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
    std::vector<Object*> lights;
    initModel(&listObjects, &lights);

    for (int y = 0; y < HEIGHT; ++y)
    {
        //std::cout << x << std::endl;
        #pragma omp parallel for schedule(dynamic, 1)
        for (int x = 0; x < WIDTH; ++x)
        {
            Vec3 finalColor = Vec3();
            Vec3 origin = Vec3(0, 0, -1500);
            for (int dx = 0; dx < SAMPLES; ++dx)
            {
            for (int dy = 0; dy < SAMPLES; ++dy)
            {
                Ray r = Ray(origin, Vec3(double(x)+(dx/SAMPLES)-(WIDTH/2), double(y)+(dy/SAMPLES)-(HEIGHT/2), -origin.z));
                finalColor += getColor(listObjects, r, lights, Vec3(1, 1, 1), 1)/(SAMPLES*SAMPLES);
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
