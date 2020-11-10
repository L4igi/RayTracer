#include "pugixml.hpp"
#include "pugixml.cpp"
#include "lodepng.cpp"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <vector>
#include <iostream>
#include <cassert>
#include "Vec3.h"
#include "Sphere.h"
#include "Mesh.h"
#include "trace_render.h"
#include "xmlstuff.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc

#include "tiny_obj_loader.h"


int main(int argc, char **argv) {

    //sets the file to read in
    std::string currentfile = "example1";

    std::vector<Vec3> obj_room_obj;

    Mesh meshobj;

    std::vector <Vec3> twoDTextureVector;

    std::vector <Vec3> twoDBumpVector;




    for (int executecounter = 0; executecounter < 4; executecounter++) {

        //https://pugixml.org/docs/quickstart.html
        // tag::code[]
        pugi::xml_document doc;

        pugi::xml_parse_result result = doc.load_file(
                "./example1.xml");

        if (currentfile == "example2") {
            result = doc.load_file("./example2.xml");
        }

        if (currentfile == "example3") {
            result = doc.load_file("./example3.xml");
        }

        if (currentfile == "example4") {
            result = doc.load_file("./example4.xml");
        }

        std::cout << "Load result: " << result.description() << ", mesh name: "
                  << doc.child("scene").attribute("output_file").value() << std::endl;

        // tag::traverse[]
        simple_walker walker;
        doc.traverse(walker);
        // end::traverse[]

        // end::code[]

        //save sphere data from xml sto separate sphere struct objects

        Sphere sphere1(Vec3(xmlsphere1.position[0], xmlsphere1.position[1], xmlsphere1.position[2]),
                       xmlsphere1.radius[0],
                       Vec3(xmlsphere1.color[0], xmlsphere1.color[1], xmlsphere1.color[2]),
                       xmlsphere1.phong[0], xmlsphere1.phong[1], xmlsphere1.phong[2], xmlsphere1.phong[3],
                       xmlsphere1.reflectance[0],
                       xmlsphere1.transmittance[0],
                       xmlsphere1.refraction[0]);
        Sphere sphere2(Vec3(xmlsphere2.position[0], xmlsphere2.position[1], xmlsphere2.position[2]),
                       xmlsphere2.radius[0],
                       Vec3(xmlsphere2.color[0], xmlsphere2.color[1], xmlsphere2.color[2]),
                       xmlsphere2.phong[0], xmlsphere2.phong[1], xmlsphere2.phong[2], xmlsphere2.phong[3],
                       xmlsphere2.reflectance[0],
                       xmlsphere2.transmittance[0],
                       xmlsphere2.refraction[0]);
        Sphere sphere3(Vec3(xmlsphere3.position[0], xmlsphere3.position[1], xmlsphere3.position[2]),
                       xmlsphere3.radius[0],
                       Vec3(xmlsphere3.color[0], xmlsphere3.color[1], xmlsphere3.color[2]),
                       xmlsphere3.phong[0], xmlsphere3.phong[1], xmlsphere3.phong[2], xmlsphere3.phong[3],
                       xmlsphere3.reflectance[0],
                       xmlsphere3.transmittance[0],
                       xmlsphere3.refraction[0]);

        Vec3 parallel_light(0);

        //std::cout << currentfile << " Sphere 1 position : " << xmlsphere1.position[0] << " " << xmlsphere1.position[1] << " " << xmlsphere1.position[2] << " Spherevector size: " << xmlsphere1.position.size()<< std::endl;
        //std::cout << currentfile << " Sphere 2 position : " << xmlsphere2.position[0] << " " << xmlsphere2.position[1] << " " << xmlsphere2.position[2] << " Spherevector size: " << xmlsphere2.position.size()<< std::endl;
        //std::cout << currentfile << " Sphere 3 position : " << xmlsphere3.position[0] << " " << xmlsphere3.position[1] << " " << xmlsphere3.position[2] << " Spherevector size: " << xmlsphere3.position.size()<< std::endl;

        //push light information to vector if file read from includes them
        if (currentfile == "example2") {
            parallel_light = Vec3(xmlparallellights.direction[0], xmlparallellights.direction[1],
                    xmlparallellights.direction[2]);
        }

        Vec3 point_light1(0);
        Vec3 point_light2(0);

        //push light information to vector if file read from includes them
        if (currentfile == "example4") {
            point_light2 = Vec3(xmlpointlight1.position[0], xmlpointlight1.position[1],
                    xmlpointlight1.position[2]);
            point_light1 = Vec3(xmlpointlight2.position[0], xmlpointlight2.position[1],
                    xmlpointlight2.position[2]);
        }


        std::vector <Vec3> Point_light_Vector;
        Point_light_Vector.push_back(point_light1);
        Point_light_Vector.push_back(point_light2);

        std::vector<Sphere> spherevector{sphere1, sphere3, sphere2};


        if (currentfile == "example3" || currentfile == "example4") {
            Mesh meshobj(xmlmesh.name,
                         Vec3(xmlmesh.color[0], xmlmesh.color[1], xmlmesh.color[2]),
                         xmlmesh.phong[0], xmlmesh.phong[1], xmlmesh.phong[2], xmlmesh.phong[3],
                         xmlmesh.reflectance[0],
                         xmlmesh.transmittance[0],
                         xmlmesh.refraction[0]);
            //https://github.com/syoyo/tinyobjloader/tree/v0.9.x
            //tiny obj loader
            //code taken from tiny obj loader tutorial


            if(currentfile == "example4"){
                //load png image
                //https://lodev.org/lodepng/
                //code taken from tutorial implementation
                const char* filename = "MarbleBeige.png";
                std::vector<unsigned char> image; //the raw pixels
                unsigned widthtexture, heighttexture;

                //decode
                unsigned error = lodepng::decode(image, widthtexture, heighttexture, filename);

                //if there's an error, display it
                if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

                //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...

                //https://stackoverflow.com/questions/42249303/how-can-i-push-back-data-in-a-2d-vector-of-type-int


                int count = 0;

                for(unsigned i = 0; i< image.size(); i+=4){
                    Vec3 tempvec (image[i], image[i+1], image[i+2]);
                    twoDTextureVector.push_back(tempvec);
                    count++;
                }
                //std::cout << "Counter " << count;
            }


            std::string inputfile = "./" + meshobj.name;
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;

            std::string warn;
            std::string err;
            bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

            if (!err.empty()) { // `err` may contain warning message.
                std::cerr << err << std::endl;
            }

            if (!ret) {
                exit(1);
            }


            // Loop over shapes
            for (size_t s = 0; s < shapes.size(); s++) {
                // Loop over faces(polygon)
                size_t index_offset = 0;
                for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                    int fv = shapes[s].mesh.num_face_vertices[f];

                    // Loop over vertices in the face.
                    for (unsigned v = 0; v < fv; v++) {
                        // access to vertex
                        tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                        tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                        tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                        tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                        obj_room_obj.push_back(Vec3(vx, vy, vz));
                        // Optional: vertex colors
                        // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
                        // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
                        // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
                    }
                    index_offset += fv;

                    // per-face material
                    shapes[s].mesh.material_ids[f];
                }
            }

            render(spherevector, currentfile, obj_room_obj, meshobj, twoDTextureVector);

        }


        if(currentfile == "example1" || currentfile == "example2"){
            render(spherevector, currentfile, obj_room_obj, meshobj, twoDTextureVector);
        }

        //Clear all xmlstruxtobjcts for next file read
        xmlsphere1=xmlsphereempty;
        xmlsphere2=xmlsphereempty;
        xmlsphere3=xmlsphereempty;

        xmlcamera = xmlcameraempty;

        xmlambientlight = xmlambientlightempty;
        xmlambientlight = xmlambientlightempty;

        xmlparallellights= xmlparallellightsempty;

        xmlpointlight1 = xmlpointlightempty;
        xmlpointlight2 = xmlpointlightempty;

        xmlmesh = xmlmeshempty;

        background_color.clear();

        pointlightcounter=0;

        spherecounter = 0;

        if (executecounter == 0) {
            currentfile = "example2";
        }
        if (executecounter == 1) {
            currentfile = "example3";
        }
        if (executecounter == 2) {
            currentfile = "example4";
        }
    }

}