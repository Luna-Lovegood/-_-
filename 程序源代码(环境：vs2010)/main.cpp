


#include "main.h"

bool parse_arg(int argc, char** argv)
{
	if(argc < 2)
    {
		return false;
	}
	int i=1;
	while(i<argc){
		string arg = argv[i];
		if(arg == "-m"){
			if(i+1 < argc){
				g_setting.matting_filename = argv[++i];
				g_setting.matting_mode = true;
			}
			else{
				return false;
			}
		}
		else if(arg == "-mr" || arg == "--max-refine") {
			if(i+1 < argc && argv[i+1][0] != '-') {
				stringstream ss(argv[++i]);
				ss>>g_setting.max_refine_iterations;
				g_setting.max_refine_iterations = std::max(g_setting.max_refine_iterations, 1);
				g_setting.max_refine_iterations = std::min(g_setting.max_refine_iterations, 100);
			}
		}
		else if(arg == "-op") {
			g_setting.output_prediction = true;
		}
		else if(arg == "-t"){
			if(i+1 < argc){
				g_setting.training_filename = argv[++i];
				g_setting.training_mode = true;
			}else{
				return false;
			}
		}
		else if(arg == "-ta"){
			if(i+1 < argc) {
				g_setting.training_dir = argv[++i];
				g_setting.training_batch_mode = true;
				if(i+1 < argc && argv[i+1][0] != '-'){
					stringstream ss(argv[++i]);
					ss>>g_setting.max_training_images;
				}
			}else {
				return false;
			}
		}
		else if(arg == "-td") {
			if(i+1 < argc ) {
				g_setting.training_database_filename = argv[++i];
			} else {
				return false;
			}
		}
		else if(arg == "-e") {
			g_setting.enable_evaluation = true;
		}
		else if(arg == "-g") {
			g_setting.enable_gui = false;
		}
		else if(arg == "-ev") {
			if(i+2 < argc){
				g_setting.evaluation_mode = true;
				g_setting.profile_filename = argv[++i];
				g_setting.profile_ground_truth_filename = argv[++i];
			}
			else {
				return false;
			}
		}
		else if(arg == "-h") {
			return false;
		}
		else if(arg == "-resize") {
			g_setting.resize_mode = true;
			if(i+1 < argc) {
				g_setting.resize_filename = argv[++i];
			}else{
				return false;
			}
			if(i+1 < argc && argv[i+1][0] != '-') {
				stringstream ss(argv[++i]);
				ss>>g_setting.resize_long_edge;
			}
			else{
				g_setting.resize_long_edge = 240;
			}
		}
		else if(arg == "-test") {
			if(i+2 < argc){
				g_setting.test_mode = true;
				g_setting.test_profilename = argv[++i];
				g_setting.test_filename = argv[++i];

			}
			else {
				return false;
			}

		}
		else
		{
			g_setting.matting_batch_mode=true;
			g_setting.input_dir = arg;
			g_setting.enable_gui = false;
		}
		++i;
	}
	return true;
}

void print_usage(int argc, char** argv)
{
	cout<<"usage: "<<argv[0]<<" [Options]"<<endl;
	cout<<"examples:"<<endl;
	cout<<"\t"<<argv[0]<<" -e -m 001.jpg "<<"# matting & evaluating 001.jpg with 001-profile.jpg"<<endl;
	cout<<endl;
	cout<<"options:"<<endl;
	cout<<"\t[-g]: no gui"<<endl;
	cout<<"\t[-e]: enable evaluation, will NOT save result to file"<<endl;
	cout<<"\t[-m filename]: mat single image"<<endl;
	cout<<"\t[-mr iterations]: max refine iterations"<<endl;
	cout<<"\t[-ev profile ground_truth]: evaluation profile with ground_truth"<<endl;
	cout<<"\t[-ta input_dir]: train entire directory"<<endl;
	cout<<"\t[-td filename]: use custom training database"<<endl;
	cout<<"\t[-op]: output predition map"<<endl;
	cout<<"\t[-resize filename [long_edge=360]]: resize the image"<<endl;
	cout<<"\t[input_dir]: mat entire directory"<<endl;
	cout<<"\t[-h]: show this message"<<endl;
}

string get_profile_name(const string& input){
	return input.substr(0, input.find_last_of('.')) + "-profile.bmp";
}

vector<string> get_files(const string& input_dir)
{
	vector<string> files;
    int fileNum = 12;
    char fileName[100];
    int i;

    for (i =1; i <= fileNum; i++)
    {
        sprintf_s(fileName, "%05d.jpg", i);
        files.push_back(fileName);
    }

	return files;
}

bool training(Statistics& stat, const string& input, const string& profile, long& pixel_count)
{
	double time;
	Mat img_org, img_profile;//原图和黑白描边图
	double read_file_cost, training_cost;

	time = (double)getTickCount();

	img_org = imread(input);
	if(!img_org.data)//发生意外时候的输出
	{
		cerr << " ! Error ! Could not open or find the image" <<input<< std::endl ;
		return false;
	}

	img_profile =  imread(profile, CV_LOAD_IMAGE_GRAYSCALE);
	if(!img_profile.data)//发生意外时候的输出
	{
		cerr << " ! Error ! Could not open or find the image "<<profile<< std::endl ;
		return false;
	}

	img_org = MatHelper::resize(img_org, LONG_EDGE_TRAIN);//缩小原图
	img_profile = MatHelper::resize(img_profile, LONG_EDGE_TRAIN);//缩小描边图

	read_file_cost = ((double)getTickCount() - time)/getTickFrequency();//读取文件时间

    time = (double)getTickCount();//时间

	stat.stat(img_org, img_profile);

	training_cost = ((double)getTickCount() - time)/getTickFrequency();//计算训练时间

	cerr <<"[Training] from "<<input<<" & "<<profile//输出训练细节
		<<" size = "<<img_org.rows<<"x"<<img_org.cols
		<<" read  =  "  << read_file_cost*1000. <<"ms"
        <<" train =  "  << training_cost*1000.  <<"ms"<<endl;

	pixel_count += img_org.rows * img_org.cols;//像素点计算

	return true;
}

void run_batch(const string& input_dir)
{

	cerr<<"========================== BEGIN ============================"<<endl;

	Statistics stat;

	double tt = (double)getTickCount();

	if(stat.read_data(g_setting.training_database_filename))
	{
		cerr << "- Training data loaded from "
             << g_setting.training_database_filename <<" in "
             << ((double)getTickCount() - tt)/getTickFrequency() <<"ms"<<endl;
	}
	else {
		cerr<<"! Error ! Can't open training data : "<<g_setting.training_database_filename<<endl;
		return;
	}

	cerr<<"- Batch started"<<endl;

	vector<string> files = get_files(input_dir);

	cerr<<"- "<<files.size()<<" images(s) got"<<endl;

	int jobid = 0;

	for(vector<string>::const_iterator it = files.begin(); it != files.end(); ++ it)
	{
		double t = (double)getTickCount();

        const string filename = input_dir + "/" + *it;
        const string profile_filename = get_profile_name(filename);
        string output_filename = g_setting.output_dir + "/" + *it;

        cerr<<"["<<jobid++<<"/"<<files.size()<<"] [Matting] " + filename + "...";

        Mat ori = imread(filename);

        if(!ori.data)
        {
            cerr<<"! Error ! can't open or read image file "<<filename<<"!"<<endl;
            continue;
        }

        Mat output, predict_raw, predict_drawing;

        Matting::mat(stat, ori, output, predict_raw, predict_drawing);

        imwrite(output_filename, output);

        cerr<<" done Time = "<<((double)getTickCount()-t)/getTickFrequency()
            <<" s" << endl;
        cerr<<" Result saved to "<<output_filename<<endl;

        if(g_setting.output_prediction){
            imwrite(output_filename + ".predict.png", predict_drawing);
            imwrite(output_filename + ".predict-raw.png", predict_raw);

            cerr<<"- Prediction file saved to " + filename + ".predict.png"<<endl;
        }

        if(g_setting.enable_evaluation) {
            evaluate(output_filename, profile_filename);
        }

	}

	cerr<<"- Batch done "<<files.size()<<" image(s) processed!"<<endl;
	cerr<<"- Total time : "<<(double)getTickCount()-tt<<"s"<<" Average time:"
	    <<(files.size()==0?0:((double)getTickCount()-tt)*1000./files.size())
        <<"ms"<<endl;
}


void train_batch(const string& input_dir)
{

	double t = (double)getTickCount();//计算时间

	Statistics stat;

	vector<string> files = get_files(input_dir);//放文件的容器

	int trained = 0;
	long pixel_count = 0;

	for(vector<string>::const_iterator it = files.begin(); it != files.end(); ++ it)//训练开始，对每一张图片进行训练
	{
		const string& filename = input_dir + "/" + *it;
		string training_filename = get_profile_name(filename);

		cerr<<"["<<trained<<"/"<<(g_setting.max_training_images == 0 ?
                            files.size() : g_setting.max_training_images)<<"] ";
		trained += training(stat, filename, training_filename, pixel_count);//训练核心函数

		if(g_setting.max_training_images != 0 && g_setting.max_training_images <= trained) break;//训练终止条件
	}

	double training_cost = (double)getTickCount()-t;//计算训练时间

	stat.save_data(g_setting.training_database_filename);//保存
	cout<<"- Training data saved to "<<g_setting.training_database_filename<<endl;//输出存储地址

	cout<<trained<<" images with "<< pixel_count <<" pixels trained in "<<training_cost<<"ms"<<endl;//输出结果
}

void evaluate(const string& profile, const string& ground_truth)
{
	Mat p = cv::imread(profile, cv::IMREAD_UNCHANGED);
	Mat g = cv::imread(ground_truth, cv::IMREAD_UNCHANGED);
	vector<Mat> ch;
	cv::split(p, ch);
	p = ch.back();
	ch.clear();
	split(g, ch);
	g = ch.back();
	if(ch.size() != 4)
    {
		cerr << "! Warning ! "<<ground_truth<<" doest not contain alpha channel!"<<endl;
		g = 255 - g;
	}
	Matting m;
	double score = m.evaluate(g, p);
	printf("- Score = %.4lf | Profile = %s | Ground_truth = %s\n",
        score, profile.c_str(), ground_truth.c_str());

}


void salient(const string& salient_path, const string& input_path){


	Mat salient = imread(salient_path, IMREAD_UNCHANGED);
	Mat input = imread(input_path);

	int org_width = input.cols;
	int org_height = input.rows;
	input = MatHelper::resize(input, salient.cols, salient.rows);

	Mat mask;
	mask.create(input.size(), CV_8UC1);
	mask = cv::GC_PR_FGD;

	Mat bgdModel;
	Mat fgdModel;

	int width = salient.cols;
	int height = salient.rows;

	for(int i=0;i<height;i++) {
		for(int j=0;j<width;j++) {
			int value = salient.at<byte>(i,j);
			mask.at<byte>(i,j) = (value>127) ? cv::GC_PR_FGD : cv::GC_PR_BGD;
		}
	}

	Rect boundRect;
	cv::grabCut(input, mask, boundRect, bgdModel, fgdModel, g_setting.max_refine_iterations, cv::GC_INIT_WITH_MASK);

	Mat seg = Mat(input.size(), CV_8UC1, Scalar(255));
	Mat output;
	seg.copyTo(output, mask & 1);

	output = MatHelper::resize(output, org_width, org_height);

	string output_path = salient_path + ".salient-out.png";
	imwrite(output_path, output);

	evaluate(output_path, get_profile_name(input_path));
}

int main(int argc, char** argv)
{
	int temp;
	GF_train_batch();//训练数据、存储数据，.bin文件
	getchar();//中途暂停，检查结果
    GF_run_batch();//.bin文件，对于test文件中的图片进行实例分割
	cin>>temp;
	return 0;
}


// Gaofeng add train code
void GF_train_batch()
{

    // open training_batch mode
    g_setting.training_batch_mode = true;//是否为训练模式
    g_setting.training_dir = "./train";//训练文件夹

    get_files(g_setting.training_dir);//读取训练文件

	if(g_setting.training_batch_mode)//若是训练模式
	{
		train_batch(g_setting.training_dir);//进行训练
	}

}

// Gaofeng add run_batch code
void GF_run_batch()
{
    // open training_batch mode
    g_setting.matting_batch_mode = true;
    g_setting.input_dir = "./test";
    g_setting.output_prediction = true;

    if(g_setting.matting_batch_mode)
	{
		run_batch(g_setting.input_dir);
	}
}





























