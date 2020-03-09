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

Eigen::Vector4f translation;
Eigen::Quaternionf orientation;

bool PcdCSF::loadCloud(const std::string &file_name)
{
    pcl::console::TicToc tt;
    pcl::console::print_highlight("Loading point cloud data ");
    pcl::console::print_value("%s ", file_name.c_str());

    tt.tic();
    if (pcl::io::loadPCDFile(file_name, this->cloud, translation, orientation) < 0)
        return (false);
    pcl::console::print_info("[done, ");
    pcl::console::print_value("%g", tt.toc());
    pcl::console::print_info(" ms : ");
    pcl::console::print_value("%d", this->cloud.width * this->cloud.height);
    pcl::console::print_info(" points]\n");
    pcl::console::print_info("Available dimensions: ");
    pcl::console::print_value("%s\n", pcl::getFieldsList(this->cloud).c_str());

    return (true);
}

void PcdCSF::saveCloud(const std::string &file_name, const pcl::PCLPointCloud2 &output)
{
    if (output.width * output.height > 0)
    {
        pcl::console::TicToc tt;
        tt.tic();

        pcl::console::print_highlight("Saving ");
        pcl::console::print_value("%s ", file_name.c_str());

        pcl::PCDWriter w;
        w.writeBinaryCompressed(file_name, output, translation, orientation);

        pcl::console::print_info("[done, ");
        pcl::console::print_value("%g", tt.toc());
        pcl::console::print_info(" ms : ");
        pcl::console::print_value("%d", output.width * output.height);
        pcl::console::print_info(" points]\n");
    }
    else
    {
        pcl::console::print_highlight("Saving ");
        pcl::console::print_value("%s ", file_name.c_str());
        pcl::console::print_highlight("FAILED: no data!\n");
    }
}
// END: René

void PcdCSF::readPointsFromFile()
{

    csf::PointCloud *pointCloud = &csf.getPointCloud();

    // Load the PCD file using PCL
    std::string file_input(this->filename);
    if (!loadCloud(file_input))
    {
        pcl::console::print_error("FAILED ");
        pcl::console::print_info("\n");
    }

    pcl::PointCloud<pcl::PointXYZ>::Ptr xyz(new pcl::PointCloud<pcl::PointXYZ>);
    fromPCLPointCloud2(this->cloud, *xyz);

    this->cloud2 = this->cloud;

    auto i = xyz->begin();
    while (i != xyz->end())
    {

        csf::Point point;

        point.x = i->x;
        point.y = -i->z;
        point.z = i->y;

        i++;

        pointCloud->push_back(point);
    }
    cout << "Read " << pointCloud->size() << " points.";
}

void PcdCSF::writeFile(const string &file_name, const vector<int> &indexes)
{
    cout << "Saving points to " << file_name << endl;
    if (file_name.empty())
    {
        return;
    }

    pcl::PCLPointCloud2::Ptr tmp(new pcl::PCLPointCloud2);

    int points = indexes.size();
    cout << "Saving " << points << " points." << endl;

    copyPointCloud(this->cloud2, indexes, *tmp);
    saveCloud(file_name, *tmp);
}

PcdCSF::PcdCSF(const string &filename)
{
    this->filename = filename;
    this->readPointsFromFile();
    pcl::PCLPointCloud2::Ptr tmp(new pcl::PCLPointCloud2);
    this->cloud = *tmp;
}

void PcdCSF::doFiltering(std::vector<int> &groundIndexes, std::vector<int> &offGroundIndexes, bool exportCloth)
{
    this->csf.do_filtering(groundIndexes, offGroundIndexes, exportCloth);
}
