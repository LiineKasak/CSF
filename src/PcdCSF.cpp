//
// Created by Liine Kasak on 09/10/2019.
//

#include <iomanip>
#include "PcdCSF.h"


void PcdCSF::readPointsFromFile() {

    csf::PointCloud *pointCloud = &csf.getPointCloud();

    cout << "Reading points from " << filename << "..." << endl;

    ifstream fin(filename.c_str(), ios::in);
    char line[500];

    int headerSize = max(PcdMetadata::STANDARD_HEADER_SIZE, this->metadata.headerSize);
    for (int i = 0; i < headerSize; i++) fin.getline(line, sizeof(line)); // skip header lines

    while (fin.getline(line, sizeof(line))) {
        stringstream words(line);

        string x, y, z;
        words >> x;
        words >> y;
        words >> z;

        csf::Point point;
        point.x = atof(x.c_str());
        point.y = -atof(z.c_str());
        point.z = atof(y.c_str());

        string valueStr;
        double value;
        for (string key : metadata.fields) {
            words >> valueStr;
            value = atof(valueStr.c_str());
            point.values.push_back(value);
        }

        pointCloud->push_back(point);
    }
    cout << "Read " << pointCloud->size() << " points.";
}

void PcdCSF::writeFile(const string &fileName, const vector<int> &indexes) {
    cout << "Saving points to " << fileName << endl;
    if (fileName.empty()) {
        return;
    }

    ofstream f1(fileName.c_str(), ios::out);

    if (!f1)
        return;

    // write header
    int points = indexes.size();
    cout << "Saving " << points << " points." << endl;
    this->metadata.points = points;
    this->metadata.width = points;
    f1 << this->metadata.getHeader();

    // write points
    csf::PointCloud pointCloud = this->csf.getPointCloud();

    for (int i : indexes) {
        f1 << fixed << setprecision(2)
           << pointCloud[i].x << " "
           << pointCloud[i].z << " " // CSF has switched z and y
           << -pointCloud[i].y;

        // write additional point parameters
        for (unsigned long k = 0; k < metadata.fields.size(); k++) {
            f1 << fixed << setprecision(2) << " " << pointCloud[i].values[k];
        }
        f1 << endl;
    }

    f1.close();
}

PcdCSF::PcdCSF(const string &filename) {
    this->filename = filename;
    this->metadata = PcdMetadata(filename);
    this->readPointsFromFile();
}

void PcdCSF::doFiltering(std::vector<int> &groundIndexes, std::vector<int> &offGroundIndexes, bool exportCloth) {
    this->csf.do_filtering(groundIndexes, offGroundIndexes, exportCloth);
}
