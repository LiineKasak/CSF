//
// Created by Liine Kasak on 12/10/2019.
//

#include <iostream>
#include <fstream>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split
#include <sstream>
#include "PcdMetadata.h"

int PcdMetadata::STANDARD_HEADER_SIZE = 10; // standard for pcd files
int PcdMetadata::VIEWPOINT_SIZE = 7; // standard

PcdMetadata::PcdMetadata(const string &filename) {
    cout << "Reading header from " << filename << "..." << endl;

    ifstream fin(filename.c_str(), ios::in);
    string line;
    this->headerSize = 0;

    while (getline(fin, line)) {
        this->headerSize++;
        if (line[0] == '#' || line.length() < 2) continue;

        vector<string> words;
        split(words, line, boost::is_any_of(" "), boost::token_compress_on);
        resolveProperty(words);
        if (words[0] == "DATA") break;
    }
}

void PcdMetadata::resolveProperty(vector<string> words) {
    string property = words[0];
    string firstValue = words[1];

    if (property == "VERSION") {
        this->version = firstValue;
    } else if (property == "DATA") {
        this->data = firstValue;
    } else if (property == "WIDTH") {
        this->width = stoi(firstValue);
    } else if (property == "HEIGHT") {
        this->height = stoi(firstValue);
    } else if (property == "POINTS") {
        this->points = stoi(firstValue);
    } else if (property == "FIELDS") {
        vector<string> fromSecondElement(words.begin() + 1, words.end());
        this->fields = fromSecondElement;
    } else if (property == "SIZE") {
        for (unsigned long i = 0; i < this->fields.size(); i++) {
            this->sizes.push_back(stoi(words[i + 1]));
        }
    } else if (property == "COUNT") {
        for (unsigned long i = 0; i < this->fields.size(); i++) {
            this->counts.push_back(stoi(words[i + 1]));
        }
    } else if (property == "VIEWPOINT") {
        for (unsigned long i = 0; i < PcdMetadata::VIEWPOINT_SIZE; i++) {
            this->viewpoints.push_back(stod(words[i + 1]));
        }
    } else if (property == "TYPE") {
        for (unsigned long i = 0; i < this->fields.size(); i++) {
            this->types.push_back(words[i + 1][0]);
        }
    }
}

string PcdMetadata::getHeader() {
    // TODO: version to write binary files
    ostringstream h;
    h << "VERSION " << version << endl << "FIELDS";

    for (auto const& field: fields) h << " " << field;

    h << endl << "SIZE";
    for (auto const& size: sizes) h << " " << 4; // can it be something else?

    h << endl << "TYPE";
    for (auto const& type: types) h << " " << "F"; // for now, don't want to deal with bin files

    h << endl << "COUNT";
    for (auto const& count: counts) h << " " << count;

    h << endl << "WIDTH " << width << endl;
    h << "HEIGHT " << height << endl << "VIEWPOINT";

    for (auto const& viewpoint: viewpoints) h << " " << viewpoint;

    h << endl << "POINTS " << points << endl;
    h << "DATA " << "ascii" << endl; // for now, don't want to deal with bin files
    return h.str();
}

PcdMetadata::PcdMetadata() = default;
