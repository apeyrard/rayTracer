#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <object.hpp>
#include <sphere.hpp>
#include <box.hpp>
#include <cylinder.hpp>

namespace raytracer
{

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void reset(double& refl, double& diff, double& refr, double& spec, double& light, double& radius, double& length, Vec3& pos, Vec3& color, Vec3& dim, Vec3& dir)
{
    //reset
    refl = 0;
    diff = 0;
    refr = 0;
    spec = 0;
    light = 0;
    radius = 0;
    length = 0;
    pos = Vec3();
    color = Vec3();
    dim = Vec3();
    dir = Vec3();
}

void addPrevObject(std::string & objectInProgress, double& refl, double& diff, double& refr, double& spec, double& light, double& radius, double& length, Vec3& pos, Vec3& color, Vec3& dim, Vec3& dir, std::vector<Object*>* objects, std::vector<Object*>* lights)
{
    Object* obj = NULL;
    if (objectInProgress == "SPHERE")
    {
        obj = new Sphere(pos, radius, color, refl, diff, spec, refr, light);
    }
    else if (objectInProgress == "BOX")
    {
        obj = new Box(pos, Vec3(), dim, color, refl, diff, spec, refr, light);
    }
    else if (objectInProgress == "CYLINDER")
    {
        obj = new Cylinder(pos, dir, length, radius, color, refl, diff, spec, refr, light);
    }

    if (obj != NULL)
    {
        objects->push_back(obj);
        if (light != 0)
        {
            lights->push_back(obj);
        }
    }

    reset(refl, diff, refr, spec, light, radius, length, pos, color, dim, dir);
}

void loadFromScene(std::vector<Object *> * objects,
    std::vector<Object *> * lights,
    std::string fileName,
    int &size,
    double &samples,
    Vec3 &camera_origin,
    Vec3 &camera_direction,
    Vec3 &camera_up,
    double &field_of_view,
    Vec3 &ambient
    )
{
    std::ifstream inputFile(fileName);
    if(inputFile.is_open())
    {
        // Reading file and adding everything into tokens vector
        std::vector<std::string> tokens;
        std::string line;
        while (getline(inputFile, line))
        {
            std::vector<std::string> vec = split(line, ' ');
            for (auto it = vec.begin(); it != vec.end(); ++it)
            {
                tokens.push_back(*it);
            }
        }
        inputFile.close();

        std::string objectInProgress = "";
        double refl = 0;
        double diff = 0;
        double refr = 0;
        double spec = 0;
        double light = 0;
        double radius = 0;
        double length = 0;
        Vec3 pos = Vec3();
        Vec3 color = Vec3();
        Vec3 dim = Vec3();
        Vec3 dir = Vec3();
        ambient = Vec3();



        // parsing tokens
        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            std::string token;
            token = (*it);

            // now acting on it
            if (token == "SIZE")
            {
                ++it;
                size = std::stoi(*it);
            }
            else if (token == "SAMPLES")
            {
                ++it;
                samples = std::stod(*it);
            }
            else if (token == "CAMERA_ORIGIN")
            {
                ++it;
                camera_origin.x = std::stod(*it);
                ++it;
                camera_origin.y = std::stod(*it);
                ++it;
                camera_origin.z = std::stod(*it);
            }
            else if (token == "CAMERA_DIRECTION")
            {
                ++it;
                camera_direction.x = std::stod(*it);
                ++it;
                camera_direction.y = std::stod(*it);
                ++it;
                camera_direction.z = std::stod(*it);
            }
            else if (token == "CAMERA_UP")
            {
                ++it;
                camera_up.x = std::stod(*it);
                ++it;
                camera_up.y = std::stod(*it);
                ++it;
                camera_up.z = std::stod(*it);
            }
            else if (token == "FIELD_OF_VIEW")
            {
                ++it;
                field_of_view = std::stoi(*it);
            }
            else if (token == "SPHERE")
            {
                addPrevObject(objectInProgress, refl, diff, refr, spec, light, radius, length, pos, color, dim, dir, objects, lights);
                objectInProgress = "SPHERE";
            }
            else if (token == "BOX")
            {
                addPrevObject(objectInProgress, refl, diff, refr, spec, light, radius, length, pos, color, dim, dir, objects, lights);
                objectInProgress = "BOX";
            }
            else if (token == "CYLINDER")
            {
                addPrevObject(objectInProgress, refl, diff, refr, spec, light, radius,length, pos, color, dim, dir, objects, lights);
                objectInProgress = "CYLINDER";
            }
            else if (token == "POS")
            {
                ++it;
                pos.x = std::stod(*it);
                ++it;
                pos.y = std::stod(*it);
                ++it;
                pos.z = std::stod(*it);
            }
            else if (token == "DIM")
            {
                ++it;
                dim.x = std::stod(*it);
                ++it;
                dim.y = std::stod(*it);
                ++it;
                dim.z = std::stod(*it);
            }
            else if (token == "DIR")
            {
                ++it;
                dir.x = std::stod(*it);
                ++it;
                dir.y = std::stod(*it);
                ++it;
                dir.z = std::stod(*it);
            }
            else if (token == "RADIUS")
            {
                ++it;
                radius = std::stod(*it);
            }
            else if (token == "LENGTH")
            {
                ++it;
                length = std::stod(*it);
            }
            else if (token == "COLOR")
            {
                ++it;
                color.x = std::stod(*it);
                ++it;
                color.y = std::stod(*it);
                ++it;
                color.z = std::stod(*it);
            }
            else if (token == "AMBIENT")
            {
                ++it;
                ambient.x = std::stod(*it);
                ++it;
                ambient.y = std::stod(*it);
                ++it;
                ambient.z = std::stod(*it);
            }
            else if (token == "REFL")
            {
                ++it;
                refl = std::stod(*it);
            }
            else if (token == "REFR")
            {
                ++it;
                refr = std::stod(*it);
            }
            else if (token == "DIFF")
            {
                ++it;
                diff = std::stod(*it);
            }
            else if (token == "SPEC")
            {
                ++it;
                spec = std::stod(*it);
            }
            else if (token == "LIGHT")
            {
                ++it;
                light = std::stod(*it);
            }
        }
        // Add the last one
        addPrevObject(objectInProgress, refl, diff, refr, spec, light, radius,length, pos, color, dim, dir, objects, lights);
    }
    else
    {
        std::cout << "Cannot open input file" << std::endl;
    }

}

}
