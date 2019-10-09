//
// Created by Liine Kasak on 09/10/2019.
//

#ifndef CSF_PCDREADERWRITER_H
#define CSF_PCDREADERWRITER_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split

#include "point_cloud.h"

using namespace std;


class PcdReaderWriter {
private:
    vector<string> header;
    vector<string> fields;
    int headerSize = 10; // standard for pcd files

    void initFields();
    void updatePoints(int points);
    void updateWidth(int width);
    void readHeader(const string& fileName);
    void readPoints(const string& fileName, csf::PointCloud* pointCloud);
public:
    void readFile(const string& fileName, csf::PointCloud* pointCloud);
    void writeFile(const string& fileName, const vector<int>& indexes, const csf::PointCloud& pointCloud);
};


#endif //CSF_PCDREADERWRITER_H
