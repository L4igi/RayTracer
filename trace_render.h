//
// Created by StudentIn on 07/01/2019.
//

#ifndef UNTITLED_TRACE_RENDER_H
#define UNTITLED_TRACE_RENDER_H

#include "xmlstuff.h"
#include "calculatesurfacecolor.h"

Vec3 trace(
        Vec3 rayorig,
        Vec3 raydir,
        std::vector<Sphere> spheres,
        int depth,
        std::string currentfile,
        std::vector<Vec3> obj_room,
        Mesh meshobj, bool shadowray, std::vector<Vec3> &twoDTextureVector) {
    double tnear = 0;
    int max_bounces = xmlcamera.max_bounces[0];
    //if there are no spheres to draw stays NULL
    const Sphere *sphere = NULL;
    // find intersection of this ray with the sphere

    for (unsigned i = 0; i < spheres.size(); ++i) {
        //t0 and t0 are the points of intersection ray, object
        double t0 = INFINITY, t1 = INFINITY;
        if (spheres[i].intersect(rayorig, raydir, t0, t1)) {
            //todo
            tnear = t0;
            sphere = &spheres[i];
        }
    }
    // if there's no intersection return black or background color
    Vec3 surfaceColor = Vec3(background_color[0], background_color[1], background_color[2]);

    if ((currentfile == "example3" || currentfile == "example4") && !shadowray) {
        double t = 0;
        double u = 0;
        double v = 0;
        bool hitobj = false;
        // use temp obj to save save triangle the intersection is happening
        std::vector<Vec3> tempobjvec;
        for (unsigned i = 0; i < obj_room.size(); i += 3) {
            hitobj = meshobj.rayTriangleIntersect(rayorig, raydir, obj_room[i], obj_room[i + 1], obj_room[i + 2], t, u,
                                                  v);
            if (hitobj) {
                tempobjvec.push_back(obj_room[i]);
                tempobjvec.push_back(obj_room[i + 1]);
                tempobjvec.push_back(obj_room[i + 2]);
                break;
            }
        }
        //http://mathforum.org/library/drmath/view/54899.html

        //if there was an intersection calculate phong for triangle
        if (hitobj) {

            //calculate triangle normals
            //https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
            Vec3 currentcolor = meshobj.color;

            Vec3 vecone(tempobjvec[1] - tempobjvec[0]);
            Vec3 vectwo(tempobjvec[2] - tempobjvec[0]);

            Vec3 Trianglenormal = (vecone.crossProduct(vectwo)).normalize();

            Vec3 lightDirection(xmlparallellights.direction[0], xmlparallellights.direction[1],
                                xmlparallellights.direction[2]);

            lightDirection = lightDirection.normalize() * -1;


            Vec3 ambient = currentcolor * meshobj.ka;

            Vec3 diffuse = currentcolor * meshobj.kd;

            Vec3 reflectiondirection =
                    Trianglenormal * lightDirection.dot(Trianglenormal) * 2 - lightDirection;

            Vec3 lookatnormalized = Vec3(xmlcamera.lookat[0], xmlcamera.lookat[1], xmlcamera.lookat[2]).normalize();


            //use lambertian to find out if an intersection point lies in the shadow (ther is no light besides ambient and no specular spot)
            double lambertian = std::max(Trianglenormal.dot(lightDirection), 0.0);

            Vec3 specular(0);

            if (lambertian > 0) {
                //calculate the vector to the sphere origin (to get right specular spots because its parallel light i guess)
                specular = Vec3(xmlparallellights.color[0], xmlparallellights.color[1], xmlparallellights.color[2]) *
                           pow(reflectiondirection.dot(lookatnormalized), meshobj.shininess_exponent);
            }


            //add up everything for phong
            surfaceColor = ambient + diffuse * std::max(Trianglenormal.dot(lightDirection), 0.0) +
                           specular;

            //todo Licht

            if (currentfile == "example4") {

                //https://stackoverflow.com/questions/2151084/map-a-2d-array-onto-a-1d-array

                int x = u * 450;
                int y = v * 338;

                int i = x + 450 * y;

                //divide by 255 because at draw it multplies by 255 again
                currentcolor = twoDTextureVector[i] / 255;

                Vec3 intersection_point = rayorig + raydir * t; // point of intersection

                //point light direction intersection point- light position = light vector
                Vec3 lightDirection = intersection_point -
                                      Vec3(xmlpointlight1.position[0], xmlpointlight1.position[1],
                                           xmlpointlight1.position[2]);

                //add up everything for phong
                surfaceColor = calculatemeshcolor(tempobjvec, meshobj, currentcolor, currentfile, lightDirection);

                //second light source

                lightDirection = intersection_point -
                                 Vec3(xmlpointlight2.position[0], xmlpointlight2.position[1],
                                      xmlpointlight2.position[2]);

                //add up everything for phong
                surfaceColor = surfaceColor * 0.3 +
                               calculatemeshcolor(tempobjvec, meshobj, currentcolor, currentfile, lightDirection);

            }
            //todo
            //surfaceColor = u * surfaceColor.x + v * surfaceColor.y + (1 - u - v) * surfaceColor.z;
            //surfaceColor = Vec3(meshobj.color.x, meshobj.color.y,  meshobj.color.z);

            //point of intersection to light direction inversed vector sphere intersection if true color = black

            Vec3 triangleintersectionpoint = rayorig + raydir * t; // point of intersection

            if (currentfile == "example3") {
                Vec3 shadowtrace = trace(triangleintersectionpoint, lightDirection, spheres, 0, currentfile,
                                         obj_room, meshobj, true, twoDTextureVector);

                // if the shadowray does not return black => it hits something return the ambient color of the hit obj. ==>> creates shadow

                if (!(shadowtrace == Vec3(background_color[0], background_color[1], background_color[2]))) {
                    //surfaceColor=Vec3(background_color[0], background_color[1], background_color[2])*0.3;
                    return (surfaceColor = Vec3(xmlmesh.color[0], xmlmesh.color[1], xmlmesh.color[2]) * xmlmesh.phong[0]);
                }
            }
            else if (currentfile == "example4") {

                int x = u * 450;
                int y = v * 338;

                int i = x + 450 * y;

                Vec3 currentcolor = twoDTextureVector[i] / 255;

                lightDirection = ((triangleintersectionpoint -
                                   Vec3(xmlpointlight2.position[0], xmlpointlight2.position[1],
                                        xmlpointlight2.position[2])) * -1).normalize();

                Vec3 shadowtrace1 = trace(triangleintersectionpoint, lightDirection, spheres, 0, currentfile,
                                          obj_room, meshobj, true, twoDTextureVector);

                if (!(shadowtrace1 == Vec3(background_color[0], background_color[1], background_color[2]))) {
                    //surfaceColor=Vec3(background_color[0], background_color[1], background_color[2])*0.3;
                    surfaceColor = currentcolor * xmlmesh.phong[0]+currentcolor*diffuse;
                }

                lightDirection = ((triangleintersectionpoint -
                                   Vec3(xmlpointlight1.position[0], xmlpointlight1.position[1],
                                        xmlpointlight1.position[2])) * -1).normalize();


                Vec3 shadowtrace2 = trace(triangleintersectionpoint, lightDirection, spheres, 0, currentfile,
                                          obj_room, meshobj, true, twoDTextureVector);

                if (!(shadowtrace2 == Vec3(background_color[0], background_color[1], background_color[2]))) {
                    //surfaceColor=Vec3(background_color[0], background_color[1], background_color[2])*0.3;
                    if (!(surfaceColor == currentcolor * xmlmesh.phong[0]+currentcolor*diffuse)) {
                        surfaceColor = currentcolor * xmlmesh.phong[0]+currentcolor*diffuse;;
                    }
                    else {
                        //overloaying shadows are darker
                        surfaceColor = surfaceColor*xmlmesh.phong[0];
                    }
                }

            }
        }
    }

    if (!sphere) return surfaceColor;

    //std::cout << background_color[0]<< " "<<background_color[1]<< " "<<background_color[2]<<std::endl;
    if (currentfile == "example1") {
        //if only ambient light is used set light to ambient
        surfaceColor = sphere->color * sphere->ka;
    }

        //https://mrl.nyu.edu/~perlin/courses/fall2005ugrad/phong.html
        //http://vda.univie.ac.at/Teaching/Graphics/18w/LectureNotes/07_illum_18w.pdf
    else if (currentfile == "example2" || currentfile == "example3") {
        Vec3 intersection_point = rayorig + raydir * tnear; // point of intersection
        Vec3 intersection_point_normal = intersection_point - sphere->center; // normal at the intersection point
        intersection_point_normal = intersection_point_normal.normalize(); // normalize normal direction

        Vec3 lightDirection(xmlparallellights.direction[0], xmlparallellights.direction[1],
                            xmlparallellights.direction[2]);

        lightDirection = lightDirection.normalize() * -1;

        Vec3 ambient = sphere->color * sphere->ka;

        Vec3 diffuse = sphere->color * sphere->kd;

        Vec3 reflectiondirection =
                intersection_point_normal * lightDirection.dot(intersection_point_normal) * 2 - lightDirection;


        Vec3 lookatnormalized = Vec3(sphere->center.x, sphere->center.y, sphere->center.z) - rayorig;

        lookatnormalized.normalize();

        //use lambertian to find out if an intersection point lies in the shadow (ther is no light besides ambient and no specular spot)
        double lambertian = std::max(intersection_point_normal.dot(lightDirection), 0.0);

        Vec3 specular(0);

        if (lambertian > 0) {
            //calculate the vector to the sphere origin (to get right specular spots because its parallel light i guess)
            specular = (Vec3(xmlparallellights.color[0], xmlparallellights.color[1], xmlparallellights.color[2]) * sphere->ks*
                        pow(reflectiondirection.dot(lookatnormalized), sphere->shininess_exponent));
        }


        //add up everything for phong
        surfaceColor = ambient + diffuse * std::max(intersection_point_normal.dot(lightDirection), 0.0) +
                       specular;
    } else if (currentfile == "example4") {

        //to avoid acne
        //Ray Tracer tutorial 1
        //https://bheisler.github.io/post/writing-raytracer-in-rust-part-2/
        float bias = 1e-4;

        Vec3 intersection_point = rayorig + raydir * tnear; // point of intersection
        Vec3 intersection_point_normal = intersection_point - sphere->center; // normal at the intersection point
        intersection_point_normal = intersection_point_normal.normalize(); // normalize normal direction


        Vec3 lightDirection = intersection_point -
                              Vec3(xmlpointlight1.position[0], xmlpointlight1.position[1], xmlpointlight1.position[2]);

        lightDirection = lightDirection.normalize() * -1;

        Vec3 ambient = sphere->color * sphere->ka;

        Vec3 diffuse = sphere->color * sphere->kd;

        Vec3 reflectiondirection =
                intersection_point_normal * lightDirection.dot(intersection_point_normal) * 2 - lightDirection;

        reflectiondirection.normalize();


        Vec3 lookatnormalized = Vec3(sphere->center.x, sphere->center.y, sphere->center.z) - rayorig;

        lookatnormalized.normalize();

        //std::cout << reflectioncolor << " reflectioncolor " << std:: endl;

        //use lambertian to find out if an intersection point lies in the shadow (ther is no light besides ambient and no specular spot)
        double lambertian = std::max(intersection_point_normal.dot(lightDirection), 0.0);

        Vec3 specular(0);

        if (lambertian > 0) {
            //calculate the vector to the sphere origin (to get right specular spots because its parallel light i guess)
            specular = (Vec3(xmlpointlight1.color[0], xmlpointlight1.color[1], xmlpointlight1.color[2]) *
                        pow(reflectiondirection.dot(lookatnormalized), sphere->shininess_exponent));
        }

        Vec3 reflectioncolor(0,0,0);

        if (depth < max_bounces) {
            //calculate new reflection direction from intersection point
            //call recurively up to number of max bounces
            //https://github.com/jansel/opentuner/blob/master/examples/gccflags/apps/raytracer.cpp
            Vec3 refldir = (raydir - intersection_point_normal* 2 * raydir.dot(intersection_point_normal));
            (refldir.normalize());

            //std::cout << "current depth " <<depth<< std::endl;
            reflectioncolor = trace(intersection_point+ intersection_point_normal* bias, refldir, spheres, depth + 1, currentfile,
                                    obj_room, meshobj, false, twoDTextureVector);
        }

        //add up everything for phong
        surfaceColor = ambient + diffuse * std::max(intersection_point_normal.dot(lightDirection), 0.0) +
                       specular;

        lightDirection = intersection_point -
                         Vec3(xmlpointlight2.position[0], xmlpointlight2.position[1], xmlpointlight2.position[2]);

        lightDirection = lightDirection.normalize() * -1;

        reflectiondirection =
                intersection_point_normal * lightDirection.dot(intersection_point_normal) * 2 - lightDirection;

        lambertian = std::max(intersection_point_normal.dot(lightDirection), 0.0);

        if (lambertian > 0) {
            //calculate the vector to the sphere origin (to get right specular spots because its parallel light i guess)
            specular = Vec3(xmlpointlight1.color[0], xmlpointlight1.color[1], xmlpointlight1.color[2]) *
                       pow(reflectiondirection.dot(lookatnormalized), sphere->shininess_exponent);
        }

        //add up everything for phong
        surfaceColor =
                (surfaceColor + (ambient + diffuse * std::max(intersection_point_normal.dot(lightDirection), 0.0) +
                                 specular));


            surfaceColor =  surfaceColor*(1-sphere->reflectance)+ reflectioncolor*(sphere->reflectance);


    }

    return surfaceColor;
}

void render(const std::vector<Sphere> &spheres, std::string currentfile, std::vector<Vec3> obj_room, Mesh meshobj,
            std::vector<Vec3> &twoDTextureVector) {
    //unsigned for easier creation of image array
    unsigned width = xmlcamera.resolution[0], height = xmlcamera.resolution[1];

    //http://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing 218-246
    Vec3 *image = new Vec3[width * height];
    //assign pixel to image vector
    Vec3 *pixel = image;
    //https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
    double invWidth = 1 / double(width);
    double invHeight = 1 / double(height);
    double fov = xmlcamera.horizontal_fov[0];
    double aspectratio = width / double(height);
    double angle = tan(M_PI * fov / 180.);


    //loop threw each pixel
    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x, ++pixel) {
            //multiply by two to be in the range -1 to 1 (without that range would be 0 to 1
            //take inverse hight to scale to 0 to 1 range
            double xx = (2 * (x * invWidth) - 1) * angle * aspectratio;
            double yy = (1 - 2 * (y * invHeight)) * angle;

            //
            Vec3 raydir(xx, yy, -1);

            raydir = raydir.normalize();


            //trace ray for every pixel
            *pixel = trace(Vec3(xmlcamera.position[0], xmlcamera.position[1], xmlcamera.position[2]), raydir, spheres,
                           0, currentfile, obj_room, meshobj, false, twoDTextureVector);
        }
    }
    // store color in pixel
    //image write taken from scratchapixel.com
    if (currentfile == "example1") {
        std::ofstream ofs("./example1.ppm", std::ios::out | std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";
        //todo
        for (unsigned i = 0; i < width * height; ++i) {
            //multiply color by 255 to get rgb value
            ofs << (unsigned char) (std::min(double(1), image[i].x) * 255) <<
                (unsigned char) (std::min(double(1), image[i].y) * 255) <<
                (unsigned char) (std::min(double(1), image[i].z) * 255);
        }
        ofs.close();
        delete[] image;
    } else if (currentfile == "example2") {
        std::ofstream ofs("./example2.ppm", std::ios::out | std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";
        for (unsigned i = 0; i < width * height; ++i) {
            ofs << (unsigned char) (std::min(double(1), image[i].x) * 255) <<
                (unsigned char) (std::min(double(1), image[i].y) * 255) <<
                (unsigned char) (std::min(double(1), image[i].z) * 255);
        }
        ofs.close();
        delete[] image;
    } else if (currentfile == "example3") {
        std::ofstream ofs("./example3.ppm", std::ios::out | std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";
        for (unsigned i = 0; i < width * height; ++i) {
            ofs << (unsigned char) (std::min(double(1), image[i].x) * 255) <<
                (unsigned char) (std::min(double(1), image[i].y) * 255) <<
                (unsigned char) (std::min(double(1), image[i].z) * 255);
        }
        ofs.close();
        delete[] image;
    } else if (currentfile == "example4") {
        std::ofstream ofs("./example4.ppm", std::ios::out | std::ios::binary);
        ofs << "P6\n" << width << " " << height << "\n255\n";
        for (unsigned i = 0; i < width * height; ++i) {
            ofs << (unsigned char) (std::min(double(1), image[i].x) * 255) <<
                (unsigned char) (std::min(double(1), image[i].y) * 255) <<
                (unsigned char) (std::min(double(1), image[i].z) * 255);
        }
        ofs.close();
        delete[] image;
    }
}


#endif //UNTITLED_TRACE_RENDER_H
