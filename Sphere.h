//
// Created by StudentIn on 07/01/2019.
//

#ifndef UNTITLED_SPHERE_H
#define UNTITLED_SPHERE_H

struct Sphere {
    Vec3 center;                           /// position of the sphere
    double radius;                 /// sphere radius and radius^2
    Vec3 color;        /// surface transparency and reflectivity
    double ka, kd, ks, shininess_exponent, reflectance, transmittance, refraction;

    Sphere () {}

    Sphere(Vec3 center, double radius, Vec3 color, double ka, double kd, double ks, double shininess_exponent,
           double reflectance, double transmittance, double refraction) {
        this->center = center;
        this->radius = radius;
        this->color = color;
        this->ka = ka;
        this->kd = kd;
        this->ks = ks;
        this->shininess_exponent = shininess_exponent;
        this->reflectance = reflectance;
        this->transmittance = transmittance;
        this->refraction = refraction;
    }

    bool intersect(Vec3 rayorig,Vec3 raydir, double &t0, double &t1) const
    {
        //http://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing
        //calculate the ray vector
        //http://kylehalladay.com/blog/tutorial/math/2013/12/24/Ray-Sphere-Intersection.html
        //Tutorium Slides Ray Sphere Intersection
        Vec3 L = center - rayorig;
        double tca = L.dot(raydir);
        //if tca is negative vector points in opposit direction (over 90 degrees)
        if (tca < 0) return false;
        //Pythagoras, d is oppsite site of right triangle
        //Vector dot the same vector returns the Vector abs squared
        //Pythagoras a^2+b^2=c^2 ==> b=sqrt(c^2-a^2) ==> d=sqrt(L^2 - tca^2)
        double d = L.dot(L) - pow(tca,2);
        //if d > sphere radius => ray misses the sphere
        if (d > pow(radius,2)) return false;
        //Pythagoras
        double thc = sqrt(pow(radius,2) - d);
        if(tca-thc <0 && tca+thc<0) return false;
        if(tca-thc > tca+thc) {
            //first point of intersection
            t1 = tca - thc;
            //second point of intersection
            t0 = tca + thc;
        }
        else{
            t0 = tca - thc;
            t1 = tca + thc;
        }

        if (t0 < 0) {
            t0 = t1; // if t0 is negative, let's use t1 instead
        }

        return true;

    }
};
#endif //UNTITLED_SPHERE_H
