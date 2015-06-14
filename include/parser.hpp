#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <object.hpp>
#include <sphere.hpp>
#include <box.hpp>

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

void loadFromScene(std::vector<Object *> * objects,
    std::vector<Object *> * lights,
    std::string fileName,
    int &size,
    double &samples,
    Vec3 &camera_origin,
    Vec3 &camera_direction,
    Vec3 &camera_up,
    double &field_of_view
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
        Vec3 pos = Vec3();
        Vec3 color = Vec3();
        Vec3 dim = Vec3();


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
                if (objectInProgress == "SPHERE")
                {
                    Object* obj = new Sphere(pos, radius, color, refl, diff, spec, refr, light);
                    objects->push_back(obj);
                    if (light != 0)
                    {
                        lights->push_back(obj);
                    }
                    //reset
                    refl = 0;
                    diff = 0;
                    refr = 0;
                    spec = 0;
                    light = 0;
                    radius = 0;
                    pos = Vec3();
                    color = Vec3();
                    dim = Vec3();
                }
                else if (objectInProgress == "BOX")
                {
                    Object* obj = new Box(pos, Vec3(), dim, color, refl, diff, spec, refr, light);
                    objects->push_back(obj);
                    if (light != 0)
                    {
                        lights->push_back(obj);
                    }
                    //reset
                    refl = 0;
                    diff = 0;
                    refr = 0;
                    spec = 0;
                    light = 0;
                    radius = 0;
                    pos = Vec3();
                    color = Vec3();
                    dim = Vec3();
                }
                objectInProgress = "SPHERE";
            }
            else if (token == "BOX")
            {
                if (objectInProgress == "SPHERE")
                {
                    Object* obj = new Sphere(pos, radius, color, refl, diff, spec, refr, light);
                    objects->push_back(obj);
                    if (light != 0)
                    {
                        lights->push_back(obj);
                    }
                    //reset
                    refl = 0;
                    diff = 0;
                    refr = 0;
                    spec = 0;
                    light = 0;
                    radius = 0;
                    pos = Vec3();
                    color = Vec3();
                    dim = Vec3();
                }
                else if (objectInProgress == "BOX")
                {
                    Object* obj = new Box(pos, Vec3(), dim, color, refl, diff, spec, refr, light);
                    objects->push_back(obj);
                    if (light != 0)
                    {
                        lights->push_back(obj);
                    }
                    //reset
                    refl = 0;
                    diff = 0;
                    refr = 0;
                    spec = 0;
                    light = 0;
                    radius = 0;
                    pos = Vec3();
                    color = Vec3();
                    dim = Vec3();
                }
                objectInProgress = "BOX";
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
            else if (token == "RADIUS")
            {
                ++it;
                radius = std::stod(*it);
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
        if (objectInProgress == "SPHERE")
        {
            Object* obj = new Sphere(pos, radius, color, refl, diff, spec, refr, light);
            objects->push_back(obj);
            if (light != 0)
            {
                lights->push_back(obj);
            }
            //reset
            refl = 0;
            diff = 0;
            refr = 0;
            spec = 0;
            light = 0;
            radius = 0;
            pos = Vec3();
            color = Vec3();
            dim = Vec3();
        }
        else if (objectInProgress == "BOX")
        {
            Object* obj = new Box(pos, Vec3(), dim, color, refl, diff, spec, refr, light);
            objects->push_back(obj);
            if (light != 0)
            {
                lights->push_back(obj);
            }
            //reset
            refl = 0;
            diff = 0;
            refr = 0;
            spec = 0;
            light = 0;
            radius = 0;
            pos = Vec3();
            color = Vec3();
            dim = Vec3();
        }
    }
    else
    {
        std::cout << "Cannot open input file" << std::endl;
    }

}

}
