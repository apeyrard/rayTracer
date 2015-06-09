#include <ctime>

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

#include <SFML/Graphics.hpp>

#include <constants.hpp>
#include <vec3.hpp>
#include <object.hpp>
#include <sphere.hpp>
#include <plane.hpp>
#include <box.hpp>
// TODO : replace auto computation of normal by getnormal when needed
// TODO : monte carlo
// TODO : kdtree
using namespace raytracer;


void initModel(std::vector<Object*>* listObjects, std::vector<Object*>* lights)
{
    Sphere* c = new Sphere(Vec3(0, 0, 0), 20, Vec3(1, 1, 1), 0, 1, 0);
    Sphere* x = new Sphere(Vec3(20, 0, 0), 20, Vec3(1, 0, 0), 0, 1, 0);
    Sphere* y = new Sphere(Vec3(0, 20, 0), 20, Vec3(0, 1, 0), 0, 1, 0);
    Sphere* z = new Sphere(Vec3(0, 0, 20), 20, Vec3(0, 0, 1), 0, 1, 0);

    Sphere* s2 = new Sphere(Vec3(50, 50, 20), 50, Vec3(0, 1, 1), 0.8, 0.2, 0);
    Sphere* s3 = new Sphere(Vec3(150, -150, -50), 50, Vec3(0, 1, 1), 0, 0, 0, 1.5);

    Sphere* lum = new Sphere(Vec3(0, 200, -200), 50, Vec3(1, 1, 1), 0, 0, 0, 0, 1);

    Box* b = new Box(Vec3(300, -300, -300), Vec3(), Vec3(50, 50, 50), Vec3(0.35, 0.65, 0.80), 0.8 , 0.20, 0, 0, 0.0);
    Box* b2 = new Box(Vec3(-300, -300, -300), Vec3(), Vec3(50, 50, 50), Vec3(0.35, 0.65, 0.80), 0 , 0, 0, 1.5, 0.0);

    //Sphere* globe = new Sphere(Vec3(0, 0, 0), 1000000, Vec3(0, 1, 1), 0, 0, 0, 1.5);

    //Box* boite = new Box(Vec3(0, 0, 0), Vec3(), Vec3(3000, 3000, 3000), Vec3(0.35, 0.65, 0.80), 0 , 0, 0, 1.33, 0.0);

    Box* sol = new Box(Vec3(0, -500, 0), Vec3(), Vec3(2000, 5, 2000), Vec3(1, 1, 1), 0, 0.6, 0.2, 0.0, 0.0);
    Box* plafond = new Box(Vec3(0, 500, 0), Vec3(), Vec3(2000, 5, 2000), Vec3(1, 1, 0), 0, 0.6, 0.2, 0.0, 0.0);
    Box* gauche = new Box(Vec3(-500, 0, 0), Vec3(), Vec3(5, 500, 500), Vec3(1, 0, 1), 0, 0.6, 0.2, 0.0, 0.0);
    Box* droite = new Box(Vec3(500, 0, 0), Vec3(), Vec3(20, 500, 525), Vec3(0.5 ,0.5 , 0.5), 0, 0, 0, 1.5, 0.0);
    Box* fond = new Box(Vec3(0, 0, -500), Vec3(), Vec3(500, 500, 5), Vec3(0, 1 , 0.3), 0, 0.6, 0.2, 0.0, 0.0);
    Box* derriere = new Box(Vec3(0, 0, 500), Vec3(), Vec3(500, 500, 5), Vec3(0, 1 , 0.3), 0.8, 0.2, 0, 0.0, 0.0);

    listObjects->push_back(b);
    listObjects->push_back(b2);
    //listObjects->push_back(boite);

    listObjects->push_back(s2);
    listObjects->push_back(s3);

    listObjects->push_back(lum);
    lights->push_back(lum);

    listObjects->push_back(c);
    listObjects->push_back(x);
    listObjects->push_back(y);
    listObjects->push_back(z);

    //listObjects->push_back(globe);

    listObjects->push_back(sol);
    listObjects->push_back(fond);
    listObjects->push_back(plafond);
    listObjects->push_back(gauche);
    listObjects->push_back(droite);
    //listObjects->push_back(derriere);
}

Vec3 getColor(std::vector<Object*> objects, Ray r, std::vector<Object*> lights, Vec3 prevEmittance, int emission)
{
    //Stop and return black if maxDepth
    if (r.depth >= MAXDEPTH)
    {
        return Vec3(0,0,0);
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
        if ((minObj->diffuse > 0) or (minObj->spec > 0))
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

                    // add spec lighting
                    Vec3 reflDir = shadowRay.direction - minNormal * (shadowRay.direction.dot(minNormal) *2);
                    dot = r.direction.dot(reflDir);
                    if (dot > 0)
                    {
                        double spec = pow(dot, 50) * minObj->spec;
                        result += minObj->color * (*lightIt)->color * spec;
                    }
                }
                if (r.depth < MAXDEPTH)
                {
                    std::normal_distribution<double> gauss(0,1);
                    Ray randomRay = Ray(minInterPoint, Vec3(gauss(rng), gauss(rng), gauss(rng)), r.depth+1, r.refr);
                    if (randomRay.direction.dot(minNormal) < 0)
                    {
                        randomRay.direction *= -1;
                    }
                    double dot = minNormal.dot(randomRay.direction);
                    double diff = minObj->diffuse * dot;
                    result += minObj->color * (getColor(objects, randomRay, lights, emittance, 0)) * diff;
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
                Ray reflectedRay = Ray(minInterPoint, reflDir, r.depth+1, r.refr);
                Vec3 reflColor = getColor(objects, reflectedRay, lights, emittance, 1);
                result += reflColor * refl;
            }
        }

        // Get refraction
        double objRefr = minObj->refr;
        if (objRefr > 0.0)
        {
            if (r.depth < MAXDEPTH)
            {
                minInterPoint -= minNormal * EPSILON;
                //std::cout << "hit at " << minInterPoint.x << " " << minInterPoint.y << " " << minInterPoint.z << " norm is "<< minNormal.x << " " << minNormal.y << " " << minNormal.z << " direction is " << r.direction.x << " " << r.direction.y << " " << r.direction.z << std::endl;

                //minNormal *= (-1);

                double R = r.refr/objRefr;
                double nextRefr = objRefr;

                if (r.refr != 1) //coming from, air
                {
                    R = objRefr;
                    nextRefr = 1;
                }

                Vec3 refrDir;
                bool TIP = false;
                // Trying to detect total internal reflection
                if ((r.refr > 1) or ((r.refr == 1) and (objRefr < 1)))
                {
                    double critAngle = asin(1/R);
                    double incidentAngle = acos( (minNormal * (-1)).dot(r.direction) / (minNormal.length() * r.direction.length()) );
                    //std::cout << critAngle << " " << incidentAngle << std::endl;
                    if (incidentAngle > critAngle)
                    {
                        //std::cout << "TIP" << std::endl;
                        refrDir = r.direction - minNormal * (r.direction.dot(minNormal) *2);
                        TIP = true;
                    }
                }

                if(!TIP)
                {
                    double C = (minNormal * (-1)).dot(r.direction);
                    refrDir = r.direction * R + minNormal * (C * R - sqrt(1 - R*R * (1 - C*C)));
                    minInterPoint += refrDir * EPSILON;
                }

                Ray refrRay = Ray(minInterPoint, refrDir, r.depth, nextRefr);
                Vec3 refrColor = getColor(objects, refrRay, lights, emittance, 1);
                //std::cout << refrColor.x << " " << refrColor.y << " " << refrColor.z << std::endl;
                result += refrColor;
            }
        }
    }
    else
    {
        result = Vec3(0,0 ,0.0);
    }

    return result;
    //return (minNormal.norm() + Vec3(1, 1, 1)) /2;
}

int main()
{
    sf::Texture texture;
    texture.create(SIZE, SIZE);
    sf::Uint8* pixels = new sf::Uint8[SIZE * SIZE * 4];
    sf::RenderWindow window(sf::VideoMode(SIZE, SIZE), "Ray Tracer");

    std::vector<Object*> listObjects;
    std::vector<Object*> lights;
    initModel(&listObjects, &lights);

    //Camera stuff
    Vec3 cameraPos = Vec3(1000, 0, 2000);
    Vec3 cameraDir = Vec3(-0.5, 0, -1).norm();
    Vec3 cameraUp = Vec3(0, 1, 0).norm();
    Vec3 cameraRight = cameraDir.cross(cameraUp);
    cameraUp = cameraRight.cross(cameraDir);
    double fieldOfView = 1.0;

    for (int y = 0; y < SIZE; ++y)
    {
        //std::cout << x << std::endl;
        //#pragma omp parallel for schedule(dynamic, 1)
        for (int x = 0; x < SIZE; ++x)
        {
            Vec3 finalColor = Vec3();
            for (int dx = 0; dx < SAMPLES; ++dx)
            {
            for (int dy = 0; dy < SAMPLES; ++dy)
            {
                double dist = SIZE / fieldOfView;
                double curX = double(x) + dx/SAMPLES - SIZE/2;
                double curY = double(y) + dy/SAMPLES - SIZE/2;
                Vec3 imagePoint = cameraUp * curY + cameraRight * curX + cameraDir * dist + cameraPos;

                Ray r = Ray(cameraPos, imagePoint-cameraPos);
                finalColor += getColor(listObjects, r, lights, Vec3(1, 1, 1), 1)/(SAMPLES*SAMPLES);
            }
            }
            pixels[x*4 + (SIZE - y - 1)*SIZE*4] = std::min(finalColor.x*255.0, 255.0); //TODO less naive clamp
            pixels[x*4 + (SIZE - y -1) *SIZE*4 +1] = std::min(finalColor.y*255.0, 255.0);
            pixels[x*4 + (SIZE - y - 1)*SIZE*4 +2] = std::min(finalColor.z*255.0, 255.0);
            pixels[x*4 + (SIZE -y -1)*SIZE*4 +3] = 255;
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
