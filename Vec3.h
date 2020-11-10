//
// Created by StudentIn on 07/01/2019.
//

#ifndef UNTITLED_VEC3_H
#define UNTITLED_VEC3_H
struct Vec3 {
    double x,y,z;
    Vec3() : x(0), y(0), z(0) {}
    Vec3(double x) : x(x), y(x), z(x) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec3 operator + (Vec3 v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
    Vec3 operator - (Vec3 v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
    Vec3 operator * (double d) const { return Vec3(x*d, y*d, z*d); }
    Vec3 operator * (Vec3 v) const { return Vec3(x*v.x, y*v.y, z*v.z); }
    Vec3 operator / (double d) const { return Vec3(x/d, y/d, z/d); }
    bool operator == (Vec3 v) const { return (x==v.x && y==v.y && z==v.z); }
    double dot(Vec3  v) const { return x * v.x + y * v.y + z * v.z; }
    friend std::ostream & operator << (std::ostream &out, Vec3 &v)
    {
        out << "[" << v.x << " " << v.y << " " << v.z << "]";
        return out;
    }
    //https://stackoverflow.com/questions/10002918/what-is-the-need-for-normalizing-a-vector
    Vec3 normalize()
    {
        double veclength = sqrt(pow(x,2)+pow(y,2)+pow(z,2));
        if(veclength>0) {
            x /= veclength;
            y /= veclength;
            z /= veclength;
        }
        return Vec3(x,y,z);
    }

    Vec3 crossProduct(Vec3 b)
    {
        auto p1 = y * b.z - z * b.y;
        auto p2 = z * b.x - x * b.z;
        auto p3 = x * b.y - y * b.x;
        return Vec3(p1,p2,p3);
    }
};

#endif //UNTITLED_VEC3_H
