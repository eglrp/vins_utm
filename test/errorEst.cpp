#include <fstream>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <map>

using namespace std;

string file_result = "./pose_output.txt";
string file_groundTruth = "../Data/cam_pose_tum.txt";

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        file_result = argv[1];
    }
    if (argc > 2)
    {
        file_groundTruth = argv[2];
    }

    ifstream fileGT(file_groundTruth), fileRs(file_result);

    if (!fileRs.is_open())
    {
        cerr << file_result << "not found\n";
        return 0;
    } else {
        cout << "Result data  file: " << file_result << endl;
    }

    if (!fileGT.is_open())
    {
        cerr << file_groundTruth << "not found\n";
        return 0;
    } else {
        cout << "Ground truth file: " << file_groundTruth << endl;
    }

    string sRs_line;
    map<double, Eigen::Vector3d> mRs;
    double startStamp;
    Eigen::Vector3d pt_rs0;
//    ofstream fout2("./result.txt");
    while(getline(fileRs, sRs_line) && !sRs_line.empty())
    {
        istringstream ssRsData(sRs_line);
        double dStamp;
        Eigen::Vector3d pt;
        ssRsData >> dStamp >> pt.x() >> pt.y() >> pt.z();
        if (mRs.empty())
        {
            pt_rs0 = pt;
            startStamp = dStamp;
        }
        pt -= pt_rs0; // shift the whole path
        mRs.insert(pair<double, Eigen::Vector3d>(dStamp, pt));
//        fout2 << dStamp << "\t" << pt.x() << "\t" << pt.y() << "\t" << pt.z() << endl;
    }
//    fout2.close();
    cout << mRs.size() << endl;

    auto iterRs = mRs.begin();
    string sGT_line;
    map<double, Eigen::Vector3d> mGT;
    double error_SumSqr = 0.0;
    Eigen::Vector3d pt_gt0;
//    ofstream fout1("./ground_truth.txt");
    ofstream fout("./output_gt.csv");
    bool startRecord = false;
    while (getline(fileGT, sGT_line) && !sGT_line.empty())
    {
        istringstream ssGTData(sGT_line);
        double dStamp;
        Eigen::Vector3d pt;
        ssGTData >> dStamp >> pt.x() >> pt.y() >> pt.z();
        if (dStamp > 1e18)
        {
            dStamp /= 1e9;
        }
        if (abs(dStamp - startStamp) < 1e-5)
        {
            pt_gt0 = pt;
            startRecord = true;
        }
        if (startRecord && abs(dStamp - iterRs->first) < 1e-5)
        {
            pt -= pt_gt0;
            error_SumSqr += (pt - iterRs->second).squaredNorm();
            iterRs++;
            mGT.insert(pair<double, Eigen::Vector3d>(dStamp, pt));
            fout << sGT_line << endl;
        }

//        fout1 << dStamp << "\t" << pt.x() << "\t" << pt.y() << "\t" << pt.z() << endl;
    }
//    fout1.close();
    fout.close();
    cout << mGT.size() << endl;

    double error_RMS = sqrt(error_SumSqr/ (mRs.size()-1));
    cout << "RMS = " << error_RMS << endl;

    fileGT.close();
    fileRs.close();

    return 0;
}