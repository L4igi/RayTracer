//
// Created by StudentIn on 06/01/2019.
//

#ifndef RAYTRACINGTASK4_XMLSTUFF_H
#define RAYTRACINGTASK4_XMLSTUFF_H
std::vector<double> background_color;

struct camera {
    std::vector<double> position, lookat, up, horizontal_fov, resolution, max_bounces;
} xmlcamera, xmlcameraempty;

struct ambientlight {
    std::vector<double> color;
} xmlambientlight, xmlambientlightempty;

struct parallellights {
    std::vector<double> color, direction;
} xmlparallellights, xmlparallellightsempty;

struct pointlight {
    std::vector<double> color, position;
} xmlpointlight1, xmlpointlight2, xmlpointlightempty;

int spherecounter = 0;

int pointlightcounter = 0;

struct sphere {
    std::vector<double> radius, position, color, phong, reflectance, transmittance, refraction;
} xmlsphere1, xmlsphere2, xmlsphere3, xmlsphereempty;
//https://stackoverflow.com/questions/650162/why-the-switch-statement-cannot-be-applied-on-strings

struct mesh {
    std::string name;
    std::string texture;
    std::vector<double> color, phong, reflectance, transmittance, refraction;
} xmlmesh, xmlmeshempty;

std::string currentxmlmode = "scene";

enum string_code {
    ebackground_color,
    ecamera,
    eposition,
    elookat,
    eup,
    ehorizontalfov,
    eresolution,
    emax_bounces,
    elights,
    eambient_light,
    eparallel_light,
    ecolor,
    esurfaces,
    esphere,
    ematerial,
    ephong,
    ereflectance,
    etransmittance,
    erefraction,
    edefault,
    eradius,
    edirection,
    ename,
    emesh,
    epointlight,
    etexture,
    ematerial_textured
};

string_code hashit(std::string const &inString) {
    if (inString == "background_color") {
        return ebackground_color;
    }
    if (inString == "camera") {
        return ecamera;
    }
    if (inString == "position") {
        return eposition;
    }
    if (inString == "lookat") {
        return elookat;
    }
    if (inString == "up") {
        return eup;
    }
    if (inString == "horizontal_fov") {
        return ehorizontalfov;
    }
    if (inString == "resolution") {
        return eresolution;
    }
    if (inString == "max_bounces") {
        return emax_bounces;
    }
    if (inString == "lights") {
        return elights;
    }
    if (inString == "ambient_light") {
        return eambient_light;
    }
    if (inString == "parallel_light") {
        return eparallel_light;
    }
    if (inString == "color") {
        return ecolor;
    }
    if (inString == "direction") {
        return edirection;
    }
    if (inString == "surfaces") {
        return esurfaces;
    }
    if (inString == "sphere") {
        return esphere;
    }
    if (inString == "material_solid") {
        return ematerial;
    }
    if (inString == "phong") {
        return ephong;
    }
    if (inString == "reflectance") {
        return ereflectance;
    }
    if (inString == "transmittance") {
        return etransmittance;
    }
    if (inString == "refraction") {
        return erefraction;
    }
    if (inString == "radius") {
        return eradius;
    }
    if (inString == "mesh") {
        return emesh;
    }
    if (inString == "name") {
        return ename;
    }
    if (inString == "point_light") {
        return epointlight;
    }
    if (inString == "texture") {
        return etexture;
    }
    if (inString == "material_textured") {
        return ematerial_textured;
    }

    return edefault;
}

// tag::impl[]
struct simple_walker : pugi::xml_tree_walker {
    virtual bool for_each(pugi::xml_node &node) {
        //for (int i = 0; i < depth(); ++i) std::cout << "  "; // indentation

        //std::cout << node.name();

        switch (hashit(node.name())) {
            case ecamera:
                currentxmlmode = "camera";
                break;
            case eambient_light:
                currentxmlmode = "ambient_light";
                break;
            case eparallel_light:
                currentxmlmode = "parallel_light";
                break;
            case epointlight:
                currentxmlmode = "point_light";
                pointlightcounter++;
                break;
            case esurfaces:
                currentxmlmode = "surfaces";
                break;
            case esphere:
                currentxmlmode = "sphere";
                spherecounter++;
                break;
            case ematerial:
                if (currentxmlmode == "sphere") {
                    currentxmlmode = "materialsphere";
                } else if (currentxmlmode == "mesh") {
                    currentxmlmode = "materialmesh";
                }
                break;
            case ematerial_textured:
                currentxmlmode = "material_textured";
                break;
            case emesh:
                currentxmlmode = "mesh";
                break;
				default:
				break;
        }


        for (pugi::xml_attribute attr: node.attributes()) {
            //if(attr.name() || attr.value())
            //std::cout << " " << attr.name() << "=" << attr.value();

            if (currentxmlmode == "scene") {
                switch (hashit(node.name())) {
                    case ebackground_color:
                        background_color.push_back(std::stod(attr.value()));
                        break;
                    default:
                        break;
                }
            } else if (currentxmlmode == "camera") {
                switch (hashit(node.name())) {
                    case eposition:
                        xmlcamera.position.push_back(std::stod(attr.value()));
                        break;
                    case elookat:
                        xmlcamera.lookat.push_back(std::stod(attr.value()));
                        break;
                    case eup:
                        xmlcamera.up.push_back(std::stod(attr.value()));
                        break;
                    case ehorizontalfov:
                        xmlcamera.horizontal_fov.push_back(std::stod(attr.value()));
                        break;
                    case eresolution:
                        xmlcamera.resolution.push_back(std::stod(attr.value()));
                        break;
                    case emax_bounces:
                        xmlcamera.max_bounces.push_back(std::stod(attr.value()));
                        break;
                    default:
                        break;
                }
            } else if (currentxmlmode == "ambient_light") {
                switch (hashit(node.name())) {
                    case ecolor:
                        xmlambientlight.color.push_back(std::stod(attr.value()));
                        break;
                    default:
                        break;
                }
            } else if (currentxmlmode == "parallel_light") {
                switch (hashit(node.name())) {
                    case ecolor:
                        xmlparallellights.color.push_back(std::stod(attr.value()));
                        break;
                    case edirection:
                        xmlparallellights.direction.push_back(std::stod(attr.value()));
                        break;
                    default:
                        break;
                }
            } else if (currentxmlmode == "point_light") {
                auto obj = &xmlpointlight1;
                switch (pointlightcounter) {
                    case 0:
                        obj = &xmlpointlight1;
                        break;
                    case 1:
                        obj = &xmlpointlight2;
                        break;
                }
                switch (hashit(node.name())) {
                    case ecolor:
                        obj->color.push_back(std::stod(attr.value()));
                        break;
                    case eposition:
                        obj->position.push_back(std::stod(attr.value()));
                        break;
                    default:
                        break;
                }
            } else if (currentxmlmode == "sphere") {
                auto obj = &xmlsphere1;
                switch (spherecounter) {
                    case 1:
                        obj = &xmlsphere1;
                        break;
                    case 2:
                        obj = &xmlsphere2;
                        break;
                    case 3:
                        obj = &xmlsphere3;
                }
                switch (hashit(node.name())) {
                    case esphere:
                        obj->radius.push_back(std::stod(attr.value()));
                        break;
                    case eposition:
                        obj->position.push_back(std::stod(attr.value()));
                        break;
						default:
						break;
                }
            } else if (currentxmlmode == "materialsphere") {
                auto obj = &xmlsphere1;
                switch (spherecounter) {
                    case 1:
                        obj = &xmlsphere1;
                        break;
                    case 2:
                        obj = &xmlsphere2;
                        break;
                    case 3:
                        obj = &xmlsphere3;
                }
                switch (hashit(node.name())) {
                    case ecolor:
                        obj->color.push_back(std::stod(attr.value()));
                        break;
                    case ephong:
                        obj->phong.push_back(std::stod(attr.value()));
                        break;
                    case ereflectance:
                        obj->reflectance.push_back(std::stod(attr.value()));
                        break;
                    case etransmittance:
                        obj->transmittance.push_back(std::stod(attr.value()));
                        break;
                    case erefraction:
                        obj->refraction.push_back(std::stod(attr.value()));
                        break;
                    default:
                        break;
                }
            } else if (currentxmlmode == "mesh") {
                switch (hashit(node.name())) {
                    case emesh:
                        xmlmesh.name = attr.value();
                        break;
						default:
						break;
                }
            } else if (currentxmlmode == "materialmesh") {
                switch (hashit(node.name())) {
                    case ecolor:
                        xmlmesh.color.push_back(std::stod(attr.value()));
                        break;
                    case ephong:
                        xmlmesh.phong.push_back(std::stod(attr.value()));
                        break;
                    case ereflectance:
                        xmlmesh.reflectance.push_back(std::stod(attr.value()));
                        break;
                    case etransmittance:
                        xmlmesh.transmittance.push_back(std::stod(attr.value()));
                        break;
                    case erefraction:
                        xmlmesh.refraction.push_back(std::stod(attr.value()));
                        break;
                    default:
                        break;
                }
            } else if (currentxmlmode == "material_textured") {
                switch (hashit(node.name())) {
                    case etexture:
                        xmlmesh.texture = attr.value();
                        break;
                    case ephong:
                        xmlmesh.phong.push_back(std::stod(attr.value()));
                        break;
                    case ereflectance:
                        xmlmesh.reflectance.push_back(std::stod(attr.value()));
                        break;
                    case etransmittance:
                        xmlmesh.transmittance.push_back(std::stod(attr.value()));
                        break;
                    case erefraction:
                        xmlmesh.refraction.push_back(std::stod(attr.value()));
                        break;
                    default:
                        break;
                }
            }
        }
        //std::cout << std::endl;


        return true; // continue traversal
    }

};
// end::impl[]


#endif //RAYTRACINGTASK4_XMLSTUFF_H
