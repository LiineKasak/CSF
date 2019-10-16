// ======================================================================================
// Copyright 2017 State Key Laboratory of Remote Sensing Science, 
// Institute of Remote Sensing Science and Engineering, Beijing Normal University

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ======================================================================================


#include <vector>
#include "Cfg.h"
#include "../src/CSF.h"
#include "../src/PcdCSF.h"
#include <ctime>
#include <cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
    Cfg cfg;
    string slop_smooth;
    const char *file = "./config.cfg";
    cfg.readConfigFile(file, "slop_smooth", slop_smooth);
    bool ss = false;
    if (slop_smooth == "true" || slop_smooth == "True") {
        ss = true;
    } else if (slop_smooth == "false" || slop_smooth == "False") {
        ss = false;
    } else {
        ss = atoi(slop_smooth.c_str()) != 0;
    }


    string class_threshold;
    cfg.readConfigFile(file, "class_threshold", class_threshold);
    string cloth_resolution;
    cfg.readConfigFile(file, "cloth_resolution", cloth_resolution);
    string iterations;
    cfg.readConfigFile(file, "iterations", iterations);
    string rigidness;
    cfg.readConfigFile(file, "rigidness", rigidness);
    string time_step;
    cfg.readConfigFile(file, "time_step", time_step);
    string terr_pointClouds_filepath;
    cfg.readConfigFile(file, "terr_pointClouds_filepath", terr_pointClouds_filepath);

    PcdCSF pcdCsf = PcdCSF(terr_pointClouds_filepath);

    clock_t start, end;
    start = clock();

    std::vector<int> groundIndexes, offGroundIndexes;

    pcdCsf.doFiltering(groundIndexes, offGroundIndexes, false);

    end = clock();
    auto dur = (double) (end - start);
    printf("Use Time:%f\n", (dur / CLOCKS_PER_SEC));

    pcdCsf.writeFile("./non-ground.pcd", offGroundIndexes);
    // pcdCsf.writeFile("ground.pcd", groundIndexes, csf.getPointCloud());

    return 0;
}
