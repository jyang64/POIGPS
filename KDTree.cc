#include "KDTree.h"
#include <math.h>
#include <iostream>

KDNode::KDNode(double lat, double lon, const char *desc, unsigned int dep) {
    left = NULL;
    right = NULL;
    depth = dep;
    description = desc;
    latitude = lat;
    longitude = lon;
}

KDNode::~KDNode() {
}

// returns the distance (in miles) beween this node and a given lat and lon
double KDNode::distance(double lat, double lon) {
    double param = M_PI / 180.0; // required for conversion from degrees to radians
    double rad = 3956.0;  // radius of earth in miles
    double d_lat = (lat - latitude) * param;
    double d_lon = (lon - longitude) * param;
    double dist = sin(d_lat/2) * sin(d_lat/2) + cos(latitude*param) * cos(lat*param) * sin(d_lon/2) * sin(d_lon/2);
    dist = 2.0 * atan2(sqrt(dist), sqrt(1-dist));
    return rad * dist;
}

KDTree::KDTree() {
    root = NULL;
    size = 0;
}

KDTree::~KDTree() {
    destroy(root);
}

void KDTree::destroy(KDNode *p) {
    // "Destroy" should work just like the destroy method of any tree: delete all children, then move to parent.
    if(p) {
        destroy(p->left);
        destroy(p->right);
        delete p;
    }
}

void KDTree::print(KDNode *p) {
    // "Destroy" should work just like the destroy method of any tree: delete all children, then move to parent.
    if(p) {
        print(p->left);
        print(p->right);
        std::cout << "\t[\"" << "CENTER" << "\", " << p->latitude << ", " << p->longitude << "],\n";
        std::cout << "\t[\"" << p->description << "\", " << p->latitude << ", " << p->longitude << "],\n";
    }
}

void KDTree::insert(double lat, double lon, const char *desc) {
    KDNode *parent = NULL;
    bool reference = true;
    unsigned int d = 0;
    if (root == NULL){
        root = new KDNode(lat, lon, desc, d);
        size++;
        return;
    }
    KDNode *q = root;
    while(q){
        d++;
        if(reference){
            reference = false;
            if(lon < q->longitude){
                parent = q;
                q = q->left;
            }else{
                parent = q;
                q = q->right;
            }
        }else{
            reference = true;
            if(lat < q->latitude){
                parent = q;
                q = q->left;
            }else{
                parent = q;
                q = q->right;
            }
        }
    }
    if(reference){
        if(lat < parent->latitude){
            parent->left = new KDNode(lat, lon, desc, d);
            size++;
        }else{
            parent->right = new KDNode(lat, lon, desc, d);
            size++;
        }
    }else{
        if(lon < parent->longitude){
            parent->left = new KDNode(lat, lon, desc, d);
            size++;
        }else{
            parent->right = new KDNode(lat, lon, desc, d);
            size++;
        }
    }
}

unsigned int KDTree::printNeighbors(double lat, double lon, double rad, const char *filter) {
    std::cout << "var markers = [\n";
    std::cout << "\t[\"" << "CENTER" << "\", " << lat << ", " << lon << "],\n";
    int nombre = helpPrint(lat, lon, rad, filter, root);
    std::cout << "];\n";
    return nombre;
}

unsigned int KDTree::helpPrint(double lat, double lon, double rad, const char *filter, KDNode *q){
    if(!q){
        return 0;
    }
    if(q->distance(lat, lon) < rad ){
        if(q->description.find(filter) != std::string::npos){
            std::cout << "\t[\"" << q->description << "\", " << q->latitude << ", " << q->longitude << "],\n";
            return helpPrint(lat, lon, rad, filter, q->left) + helpPrint(lat, lon, rad, filter, q->right) + 1;
        }else{
            return helpPrint(lat, lon, rad, filter, q->right) + helpPrint(lat, lon, rad, filter, q->left);
        }

    }else{
        if(q->distance(lat,q->longitude) > rad && q->depth % 2 != 0) {
            if(q->latitude > lat){
                return helpPrint(lat, lon, rad, filter, q->left);
            }else{
                return helpPrint(lat, lon, rad, filter, q->right);
            }
        }
        else if (q->distance(q->latitude,lon) > rad && q->depth % 2 == 0){
            if(q->longitude > lon){
                return helpPrint(lat, lon, rad, filter, q->left);
            }else{
                return helpPrint(lat, lon, rad, filter, q->right);
            }
        }else{
            return helpPrint(lat, lon, rad, filter, q->right) + helpPrint(lat, lon, rad, filter, q->left);
        }
    }
}

void KDTree::print(){
    std::cout << "var markers = [\n";
    print(root);
    std::cout << "];\n";
}

unsigned int KDTree::getSize() {
    return size;
}
