//
// Created by Liine Kasak on 09/10/2019.
//

#include <iomanip>
#include "PcdCSF.h"

// START: René
#include <pcl/io/pcd_io.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/console/time.h>
#include <pcl/console/parse.h>
#include <pcl/console/print.h>
#include <pcl/common/eigen.h>

Eigen::Vector4f    translation;
Eigen::Quaternionf orientation;


bool loadCloud (const std::string &filename, pcl::PCLPointCloud2 &cloud)
{
  pcl::console::TicToc tt;
  pcl::console::print_highlight ("Loading point cloud data ");
  pcl::console::print_value ("%s ", filename.c_str ());

  tt.tic ();
  if (pcl::io::loadPCDFile (filename, cloud, translation, orientation) < 0)
    return (false);
  pcl::console::print_info ("[done, ");
  pcl::console::print_value ("%g", tt.toc ());
  pcl::console::print_info (" ms : ");
  pcl::console::print_value ("%d", cloud.width * cloud.height);
  pcl::console::print_info (" points]\n");
  pcl::console::print_info ("Available dimensions: ");
  pcl::console::print_value ("%s\n", pcl::getFieldsList (cloud).c_str ());

  return (true);
}


void saveCloud (const std::string &filename, const pcl::PCLPointCloud2 &output)
{
    // if (output.data.size() > 0)
    if (output.width * output.height > 0)
    {
        pcl::console::TicToc tt;
        tt.tic ();

        pcl::console::print_highlight ("Saving ");
        pcl::console::print_value ("%s ", filename.c_str ());

        pcl::PCDWriter w;
        w.writeBinaryCompressed (filename, output, translation, orientation);

        pcl::console::print_info ("[done, ");
        pcl::console::print_value ("%g", tt.toc ());
        pcl::console::print_info (" ms : ");
        pcl::console::print_value ("%d", output.width * output.height);
        pcl::console::print_info (" points]\n");
    }
    else
    {
        /* code */
        pcl::console::print_highlight ("Saving ");
        pcl::console::print_value ("%s ", filename.c_str ());
        pcl::console::print_highlight ("FAILED: no data!\n");
    }
}
// END: René

void PcdCSF::readPointsFromFile() {

    csf::PointCloud *pointCloud = &csf.getPointCloud();

    // pcl::PCLPointCloud2::Ptr cloud   (new pcl::PCLPointCloud2);
    // Load the PCD file using PCL
    std::string file_input (filename);
    if (!loadCloud (file_input, this->cloud))
    {
        pcl::console::print_error("FAILED ");
        pcl::console::print_info("\n");
        // pcl::console::print_value ("%s\n", argv[p_file_indices[0]]);
        // return (-1);
    }

    pcl::PointCloud<pcl::PointXYZ>::Ptr xyz (new pcl::PointCloud<pcl::PointXYZ>);
    fromPCLPointCloud2 (this->cloud,    *xyz);

    // auto i = xyz->begin();
    // while (i != xyz->end())
    // {
    //     std::cout << i->x << std::endl;
    // }

    // cout << "Reading points from " << filename << "..." << endl;

    ifstream fin(filename.c_str(), ios::in);
    char line[500];

    int headerSize = max(PcdMetadata::STANDARD_HEADER_SIZE, this->metadata.headerSize);
    for (int i = 0; i < headerSize; i++) fin.getline(line, sizeof(line)); // skip header lines

    auto i = xyz->begin();
    while (i != xyz->end())
    {
        // std::cout <<                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      i->x << std::endl;
    // while (fin.getline(line, sizeof(line))) {
    //     stringstream words(line);

    //     string x, y, z;
    //     words >> x;
    //     words >> y;
    //     words >> z;

        csf::Point point;
        // point.x = atof(x.c_str());
        // point.y = -atof(z.c_str());
        // point.z = atof(y.c_str());

        point.x = i->x;
        point.y = -i->z;
        point.z = i->y;

        i++;

        // string valueStr;
        // double value;
        // for (string key : metadata.fields) {
        //     words >> valueStr;
        //     value = atof(valueStr.c_str());
        //     point.values.push_back(value);
        // }

        pointCloud->push_back(point);
    }
    cout << "Read " << pointCloud->size() << " points.";
}

void PcdCSF::writeFile(const string &fileName, const vector<int> &indexes) {
    cout << "Saving points to " << fileName << endl;
    if (fileName.empty()) {
        return;
    }

    pcl::PCLPointCloud2::Ptr tmp   (new pcl::PCLPointCloud2);

    // pcl::PointCloud<pcl::PointXYZ>::Ptr abc (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::PointCloud<pcl::PointXYZ>::Ptr xyz (new pcl::PointCloud<pcl::PointXYZ>);

    // fromPCLPointCloud2 (this->cloud,    *abc);

    // ofstream f1(fileName.c_str(), ios::out);

    // if (!f1)
    //     return;

    // write header
    int points = indexes.size();
    cout << "Saving " << points << " points." << endl;
    this->metadata.points = points;
    this->metadata.width = points;
    // f1 << this->metadata.getHeader();

    // std::cout << this->metadata.getHeader() << std::endl;

    // write points
    csf::PointCloud pointCloud = this->csf.getPointCloud();

    for (int i : indexes) {
        (*xyz).push_back (
            pcl::PointXYZ (
                pointCloud[i].x, 
                pointCloud[i].z, 
                -pointCloud[i].y)); 
        
        // f1 << fixed << setprecision(2)
        //    << pointCloud[i].x << " "
        //    << pointCloud[i].z << " " // CSF has switched z and y
        //    << -pointCloud[i].y;

        // // write additional point parameters
        // for (unsigned long k = 0; k < metadata.fields.size(); k++) {
        //     f1 << fixed << setprecision(2) << " " << pointCloud[i].values[k];
        // }
        // f1 << endl;
    }

    // pcl::copyPointCloud (this->cloud, indexes, *tmp);

    // pcl::PointIndices::Ptr everything_but_the_plane (new pcl::PointIndices);
    // std::vector<int> indices_fullset (abc->size ());
    // for (int p_it = 0; p_it < static_cast<int> (indices_fullset.size ()); ++p_it)
    // indices_fullset[p_it] = p_it;

    // vector<int> out_indexes;

    // // Copying vector by copy function 
    // copy(indexes.begin(), indexes.end(), back_inserter(out_indexes)); 

    // std::sort (out_indexes.begin (), out_indexes.end ());
    // set_difference (indices_fullset.begin (), indices_fullset.end (),
    //                 indexes.begin (), indexes.end (),
    //                 inserter (everything_but_the_plane->indices, everything_but_the_plane->indices.begin ()));

    // Convert data back
    // pcl:: PCLPointCloud2 output;
    // pcl:: copyPointCloud (this->cloud, everything_but_the_plane->indices, *tmp);
    toPCLPointCloud2(*xyz, *tmp);
    saveCloud("./non-ground.pcd", *tmp);
    // saveCloud(filename, *tmp);
    // saveCloud(filename, *cloud);

    // f1.close();
}

PcdCSF::PcdCSF(const string &filename) {
    this->filename = filename;
    this->metadata = PcdMetadata(filename);
    this->readPointsFromFile();
    pcl::PCLPointCloud2::Ptr tmp   (new pcl::PCLPointCloud2);
    this->cloud =  *tmp;
}

void PcdCSF::doFiltering(std::vector<int> &groundIndexes, std::vector<int> &offGroundIndexes, bool exportCloth) {
    this->csf.do_filtering(groundIndexes, offGroundIndexes, exportCloth);
}
