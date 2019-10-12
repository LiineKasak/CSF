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
#include "PcdMetadata.h"
#include "CSF.h"

using namespace std;


class PcdCSF {

private:
    string filename;
    PcdMetadata metadata;
    CSF csf;

    void readPointsFromFile();

public:
    explicit PcdCSF(const string &filename);

    void doFiltering(std::vector<int> &groundIndexes,
                     std::vector<int> &offGroundIndexes,
                     bool exportCloth = false);

    void writeFile(const string &fileName, const vector<int> &indexes);
};


#endif //CSF_PCDCSF_H
