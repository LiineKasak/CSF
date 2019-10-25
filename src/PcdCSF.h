//
// Created by Liine Kasak on 09/10/2019.
//

#ifndef CSF_PCDCSF_H
#define CSF_PCDCSF_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split

#include "point_cloud.h"
// #include "PcdMetadata.h"
#include "CSF.h"

#include <pcl/PCLPointCloud2.h>

using namespace std;

bool loadCloud (const std::string &file_name, pcl::PCLPointCloud2 &cloud);
void saveCloud (const std::string &file_name, const pcl::PCLPointCloud2 &output);

class PcdCSF {

private:
    string filename;
    // PcdMetadata metadata;
    CSF csf;

    pcl::PCLPointCloud2 cloud; //   (new pcl::PCLPointCloud2)

    void readPointsFromFile();

public:
    explicit PcdCSF(const string &filename);

    void doFiltering(std::vector<int> &groundIndexes,
                     std::vector<int> &offGroundIndexes,
                     bool exportCloth = false);

    void writeFile(const string &file_Name, const vector<int> &indexes);
};


#endif //CSF_PCDCSF_H
