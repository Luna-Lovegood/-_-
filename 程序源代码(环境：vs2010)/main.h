#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

#include "setting.h"
#include "Matting.h"
#include "Statistics.h"

#include "MatHelper.h"




bool parse_arg(int argc, char** argv);
void print_usage(int argc, char** argv);

string get_profile_name(const string& input);

vector<string> get_files(const string& input_dir);

void evaluate(const string& profile, const string& ground_truth);
bool training(Statistics& stat, const string& input, const string& profile, long& pixel_count);
void run_batch(const string& input_dir);
void train_batch(const string& input_dir);


setting g_setting;



// gaofeng add function
void GF_train_batch();
void GF_run_batch();


#endif /* __MAIN_H__ */
