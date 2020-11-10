//
// Created by StudentIn on 20/01/2019.
//

#ifndef UNTITLED_CALCULATESURFACECOLOR_H
#define UNTITLED_CALCULATESURFACECOLOR_H

//https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal

Vec3 calculatemeshcolor(std::vector<Vec3> trianglevector, Mesh meshobj, Vec3 currentcolor, std::string currentfile, Vec3 lightDirection ) {
    //https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
    Vec3 vecone(trianglevector[1] - trianglevector[0]);
    Vec3 vectwo(trianglevector[2] - trianglevector[0]);

    Vec3 Trianglenormal = (vecone.crossProduct(vectwo)).normalize();

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

    if(currentfile=="example4") {
        if (lambertian > 0) {
            //calculate the vector to the sphere origin (to get right specular spots because its parallel light i guess)
            specular = Vec3(xmlpointlight1.color[0], xmlpointlight1.color[1], xmlpointlight1.color[2]) *
                       pow(reflectiondirection.dot(lookatnormalized), meshobj.shininess_exponent);
        }
    }

    //add up everything for phong
    return (ambient + diffuse * std::max(Trianglenormal.dot(lightDirection), 0.0) +
            specular *
            std::max(pow(reflectiondirection.dot(lookatnormalized), meshobj.shininess_exponent), 0.0));
}

#endif //UNTITLED_CALCULATESURFACECOLOR_H
