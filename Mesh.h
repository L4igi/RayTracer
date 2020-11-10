//
// Created by StudentIn on 17/01/2019.
//

#ifndef UNTITLED_MESH_H
#define UNTITLED_MESH_H

struct Mesh {
    std::string name;
    Vec3 color;                           //color of the mesh
    double ka, kd, ks, shininess_exponent, reflectance, transmittance, refraction;

    Mesh() {};

    Mesh(std::string name, Vec3 color, double ka, double kd, double ks, double shininess_exponent, double reflectance,
         double transmittance, double refraction) {
        this->name = name;
        this->color = color;
        this->ka = ka;
        this->kd = kd;
        this->ks = ks;
        this->shininess_exponent = shininess_exponent;
        this->reflectance = reflectance;
        this->transmittance = transmittance;
        this->refraction = refraction;
    }

   //www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
    // http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/
//https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
//Ray Tracer Tutorial Part 2
//Möller–Trumbore intersection algorithm
    bool rayTriangleIntersect(
            Vec3 rayorig, Vec3 raydir,
            Vec3 v0, Vec3 v1, Vec3 v2,
            double &t, double &u, double &v) {

        double kEpsilon = 1e-8;

        Vec3 vecone(v1 - v0);
        Vec3 vectwo(v2 - v0);

        Vec3 pvec = raydir.crossProduct(vectwo);


        double det = vecone.dot(pvec);

        //det < 0 (bias Epsilon) if 0 parallel to ray, less than zero ray goes in wrong direction
        if (det < kEpsilon) return false;

        double invDet = 1 / det;

        Vec3 tvec = rayorig - v0;
        u = tvec.dot(pvec) * invDet;
        if (u < 0 || u > 1) return false;

        Vec3 qvec = tvec.crossProduct(vecone);
        v = raydir.dot(qvec) * invDet;
        if (v < 0 || u + v > 1) return false;

        t = vectwo.dot(qvec) * invDet;

        return true; // this ray hits the triangle//
    }


};


#endif //UNTITLED_MESH_H
