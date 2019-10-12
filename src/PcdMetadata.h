//
// Created by Liine Kasak on 12/10/2019.
//

#ifndef CSF_PCDMETADATA_H
#define CSF_PCDMETADATA_H

#include <vector>
#include <string>

using namespace std;

class PcdMetadata {
    void resolveProperty(vector<string> words);

public:
    static int STANDARD_HEADER_SIZE;
    static int STANDARD_VIEWPOINT_SIZE;

    int headerSize;
    string version;
    vector<string> fields;
    vector<int> sizes;
    vector<char> types;
    vector<int> counts;
    long width;
    long height;
    vector<double> viewpoints;
    long points;
    string data; // "ascii" or "binary_compressed"

    PcdMetadata();

    explicit PcdMetadata(const string &filename);

    string getHeader();
};


#endif //CSF_PCDMETADATA_H
