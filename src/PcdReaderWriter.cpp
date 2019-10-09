//
// Created by Liine Kasak on 09/10/2019.
//

#include <iomanip>
#include "PcdReaderWriter.h"

void PcdReaderWriter::initFields() {
    string fieldsStr = header.at(1); // standard for pcd files
    split(fields, fieldsStr, boost::is_any_of(", "), boost::token_compress_on);
}

void PcdReaderWriter::readHeader(const string& fileName) {
    cout << "Reading header from " << fileName << "..." << endl;

    ifstream fin(fileName.c_str(), ios::in);
    string line;

    header.resize(0);
    for (int i = 0; i < headerSize; i++) {
        getline(fin, line);
        header.push_back(line);
    }

    initFields();
}

void PcdReaderWriter::readPoints(const string& fileName, csf::PointCloud* pointCloud) {
    cout << "Reading points from " << fileName << "..." << endl;

    ifstream fin(fileName.c_str(), ios::in);
    char line[500];

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
        for (string key : fields) {
            words >> valueStr;
            value = atof(valueStr.c_str());
            point.values.push_back(value);
        }

        pointCloud->push_back(point);
    }
}

void PcdReaderWriter::readFile(const string& fileName, csf::PointCloud* pointCloud) {
    readHeader(fileName);
    readPoints(fileName, pointCloud);
}

void PcdReaderWriter::updatePoints(int points) {
    header.at(8) = "POINTS " + to_string(points); // pcd standard line for points
}

void PcdReaderWriter::updateWidth(int width) {
    header.at(5) = "WIDTH " + to_string(width); // pcd standard line for width
}

void PcdReaderWriter::writeFile(const string& fileName, const vector<int>& indexes, const csf::PointCloud& pointCloud) {
    cout << "Saving points to " << fileName << endl;
    if (fileName.empty()) {
        return;
    }

    ofstream f1(fileName.c_str(), ios::out);

    if (!f1)
        return;

    // write header
    int points = indexes.size();
    updatePoints(points);
    updateWidth(points);
    for (auto const& line: header) f1 << line << endl;

    // write points
    for (int i : indexes) {
        f1 << fixed << setprecision(2)
           << pointCloud[i].x << " "
           << pointCloud[i].z << " " // CSF has switched z and y
           << -pointCloud[i].y;

        // write additional point parameters
        for (unsigned long k = 0; k < fields.size(); k++) {
            f1 << fixed << setprecision(2) << " " << fields[k] << " " << pointCloud[i].values[k];
        }
        f1 << endl;
    }

    f1.close();
}