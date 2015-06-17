#include <ctime>

#include <vector>
#include <iostream>
#include <algorithm>
#include <random>
#include <cmath>

#include <constants.hpp>
#include <vec3.hpp>
#include <object.hpp>
#include <parser.hpp>
// TODO : kdtree
using namespace raytracer;


Ray getReflectionRay(Vec3 intersectionPoint, Vec3 normal, Ray incidentRay)
{
    Vec3 reflDir = incidentRay.direction - normal * (incidentRay.direction.dot(normal) *2);
    return Ray(intersectionPoint, reflDir, incidentRay.depth+1, incidentRay.refr);
}

Ray getRefractionRay(Vec3 interPoint, Vec3 normal, double objectRefraction, Ray incidentRay)
{
    interPoint -= normal * EPSILON;

    double R = incidentRay.refr/objectRefraction;
    double nextRefr = objectRefraction;

    if (incidentRay.refr != 1) //coming from, air
    {
        R = objectRefraction;
        nextRefr = 1;
    }

    Vec3 refrDir;

    // Trying to detect total internal reflection
    if ((incidentRay.refr > 1)or ((incidentRay.refr == 1)and (objectRefraction < 1)))
    {
        double critAngle = asin(1/R);
        double incidentAngle = acos( (normal * (-1)).dot(incidentRay.direction) / (normal.length() * incidentRay.direction.length()) );

        if (incidentAngle > critAngle)
        {
            return getReflectionRay(interPoint, normal, incidentRay);
        }
    }

    double C = (normal * (-1)).dot(incidentRay.direction);
    refrDir = incidentRay.direction * R + normal * (C * R - sqrt(1 - R*R * (1 - C*C)));
    interPoint += refrDir * EPSILON;
    return Ray(interPoint, refrDir, incidentRay.depth, nextRefr);
}


Vec3 getColor(std::vector<Object*> objects, Ray r, std::vector<Object*> lights, Vec3 prevEmittance, int emission, int& contributiveRay, std::default_random_engine &rng, std::normal_distribution<double> &gauss)
{
    //Stop and return black if maxDepth
    if (r.depth >= MAXDEPTH)
    {
        //std::cout << "depth reached" << std::endl;
        //contributiveRay--;
        return Vec3(0,0,0);
    }

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
            if ((dist < minDist)or (minDist == -1))
            {
                minDist = dist;
                minObj = (*it);
                minInterPoint = intersectionPoint;
                minNormal = normal;
            }
        }
    }

    //return (minNormal.norm() + Vec3(1, 1, 1)) /2;

    //Now computing color
    if (minObj != NULL)
    {
        //Stop and return black if not enough energy
        Vec3 emittance = prevEmittance * minObj->color;
        if((emittance.x < 0.001)and (emittance.y < 0.001) and (emittance.z < 0.001))
        {
            //std::cout << "not enough energy" << std::endl;
            //contributiveRay--;
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

            Ray randomRay = Ray(minInterPoint, Vec3(gauss(rng), gauss(rng), gauss(rng)), r.depth+1, r.refr);
            if (randomRay.direction.dot(minNormal) < 0)
            {
                randomRay.direction *= -1;
            }
            double dot = minNormal.dot(randomRay.direction);
            double diff = minObj->diffuse * dot;
            result += minObj->color * (getColor(objects, randomRay, lights, emittance, 1, contributiveRay, rng, gauss)) * diff;

            /*//Spec (Phong)
            if (minObj->spec > 0)
            {
                Vec3 reflDir = randomRay.direction - minNormal * (randomRay.direction.dot(minNormal) *2);
                dot = r.direction.dot(reflDir);
                if (dot > 0)
                {
                    double spec = pow(dot, 50) * minObj->spec;
                    result += minObj->color * (getColor(objects, randomRay, lights, emittance, 1, contributiveRay, rng, gauss)) * spec;
                }
            }*/
        }


        // Get reflection
        double refl = minObj->reflection;
        if (refl > 0.0)
        {
            Ray reflectedRay = getReflectionRay(minInterPoint, minNormal, r);
            Vec3 reflColor = getColor(objects, reflectedRay, lights, emittance, 1, contributiveRay, rng, gauss);
            result += reflColor * refl;
        }

        // Get refraction
        double objRefr = minObj->refr;
        if (objRefr > 0.0)
        {
            Ray refrRay = getRefractionRay(minInterPoint, minNormal, objRefr, r);
            Vec3 refrColor = getColor(objects, refrRay, lights, emittance, 1, contributiveRay, rng, gauss);
            result += refrColor;
        }
    }
    else
    {
        result = Vec3(0,0,0.0);
    }

    return result;
    //return (minNormal.norm() + Vec3(1, 1, 1)) /2;
}

int main(int argc, char* argv[])
{

    std::vector<Object*> listObjects;
    std::vector<Object*> lights;

    int size;
    double samples;

    //Camera parameters
    Vec3 cameraPos;
    Vec3 cameraDir;
    Vec3 cameraUp;
    double fieldOfView;

    loadFromScene(&listObjects,
        &lights, argv[1],
        size,
        samples,
        cameraPos,
        cameraDir,
        cameraUp,
        fieldOfView);

    unsigned char pixels[size * size * 3];

    Vec3 cameraRight = cameraDir.cross(cameraUp);

    // Making sure camera Dir and Up that were defined are 90°
    cameraUp = cameraRight.cross(cameraDir);
    for (int y = 0; y < size; ++y)
    {
        //std::cout << x << std::endl;
        #pragma omp parallel for schedule(dynamic, 1)
        for (int x = 0; x < size; ++x)
        {
            std::default_random_engine rng(x+y*size);
            std::normal_distribution<double> gauss(0,1);

            int contributiveRay = samples * samples;
            Vec3 finalColor = Vec3();
            for (int dx = 0; dx < samples; ++dx)
            {
                for (int dy = 0; dy < samples; ++dy)
                {
                    double dist = size / fieldOfView;
                    double curX = double(x) + dx/samples - size/2;
                    double curY = double(y) + dy/samples - size/2;
                    //std::random_device rd;

                    Vec3 imagePoint = cameraUp * curY + cameraRight * curX + cameraDir * dist + cameraPos;

                    Ray r = Ray(cameraPos, imagePoint-cameraPos);
                    finalColor += getColor(listObjects, r, lights, Vec3(1, 1, 1), 1, contributiveRay, rng, gauss);
                }
            }
            if (contributiveRay != 0)
            {
                Vec3 old = finalColor;
                finalColor /= contributiveRay;
                /*if ((finalColor.y >0.8) and (finalColor.z >0.8))
                {
                    //std::cout << "old " << old.x << " " << old.y << " " << old.z << std::endl;
                    //std::cout << contributiveRay << std::endl;
                    //std::cout <<"final " << finalColor.x << " " << finalColor.y << " " << finalColor.z << std::endl;
                }*/
            }

            pixels[x*3 + (size - y - 1)*size*3] = std::min(finalColor.x*255.0, 255.0); //TODO less naive clamp
            pixels[x*3 + (size - y -1) *size*3 +1] = std::min(finalColor.y*255.0, 255.0);
            pixels[x*3 + (size - y - 1)*size*3 +2] = std::min(finalColor.z*255.0, 255.0);
        }
        if (y%10 == 0)
        {
            std::cout << y << std::endl;
        }
    }

    // Write to file
    FILE *f = fopen("image.ppm", "w");
    fprintf(f, "P3\n%d %d\n%d\n", size, size, 255);
    for (int i = 0; i < size*size*3; i+=3)
    {
        fprintf(f, "%d %d %d ", pixels[i], pixels[i+1], pixels[i+2]);
    }

    std::cout << "done" << std::endl;

    for (auto it = listObjects.begin(); it != listObjects.end(); ++it)
    {
        delete(*it);
    }
}
