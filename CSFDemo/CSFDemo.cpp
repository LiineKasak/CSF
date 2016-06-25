// SimpleDLLTest.cpp : �������̨Ӧ�ó������ڵ㡣

#include <vector>
#include "Cfg.h"
#include "../CSFDLL/CSF.h" 
#pragma comment(lib, "../Release/CSFDLL.lib")
#include <process.h>
#include <locale.h>
using namespace std;

int main()
{
	//��ȡ�ı������������ڵ���
	Cfg cfg;
	string slop_smooth;
	cfg.readConfigFile("params.cfg", "slop_smooth", slop_smooth);
	bool ss = false;
	if (slop_smooth == "true" || slop_smooth == "True")
	{
		ss = true;
	}
	else if (slop_smooth == "false" || slop_smooth == "False")
	{
		ss = false;
	}
	else{
		if (atoi(slop_smooth.c_str()) == 0){
			ss = false;
		}
		else
		{
			ss = true;
		}
	}

	string class_threshold;
	cfg.readConfigFile("params.cfg", "class_threshold", class_threshold);
	string cloth_resolution;
	cfg.readConfigFile("params.cfg", "cloth_resolution", cloth_resolution);
	string iterations;
	cfg.readConfigFile("params.cfg", "iterations", iterations);
	string k_nearest_points;
	cfg.readConfigFile("params.cfg", "k_nearest_points", k_nearest_points);
	string rigidness;
	cfg.readConfigFile("params.cfg", "rigidness", rigidness);
	string time_step;
	cfg.readConfigFile("params.cfg", "time_step", time_step);
	string terr_pointClouds_filepath;
	cfg.readConfigFile("params.cfg", "terr_pointClouds_filepath", terr_pointClouds_filepath);

	CSF csf;
	//step 1 �������
	csf.readPointsFromFile(terr_pointClouds_filepath);

	//��ע����ʵ��ʹ�ù����У������������������ṩ�����ú���Ϊ
	//csf.setPointCloud(pc);//pcΪPointCloud��

	//step 2 ���ò���
	csf.params.bSloopSmooth = ss;
	csf.params.class_threshold = atof(class_threshold.c_str());
	csf.params.cloth_resolution = atof(cloth_resolution.c_str());
	csf.params.interations = atoi(iterations.c_str());
	csf.params.k_nearest_points = atoi(k_nearest_points.c_str());
	csf.params.rigidness = atoi(rigidness.c_str());
	csf.params.time_step = atof(time_step.c_str());

	//step3 ִ���˲�,result�д�����ǵ��������� 
	vector<int> result = csf.do_filtering();


	csf.saveGroundPoints(result);

	return 0;
}