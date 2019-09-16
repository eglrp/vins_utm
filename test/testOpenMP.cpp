#define EIGEN_DONT_PARALLELIZE
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility/tic_toc.h>
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <backend/eigen_types.h>
#include <omp.h>
#include <unordered_map>
#include <utility>

using namespace std;

Eigen::Matrix<double, 1, 3> computeJacobian(double abc[], double x){
    double exp_y = std::exp( abc[0]*x*x + abc[1]*x + abc[2] );

    Eigen::Matrix<double, 1, 3> jaco_abc;
    jaco_abc << x * x * exp_y, x * exp_y , exp_y;
    return jaco_abc;
}

Eigen::Matrix<double, 1, 1> computeResidual(double abc[], double x, double y){
    Eigen::Matrix<double, 1, 1> residual(MatXX::Zero(1,1));
    residual(0) = exp( abc[0]*x*x + abc[1]*x + abc[2] ) - y;
    return residual;
}

#pragma omp declare reduction (plus: Eigen::Matrix<double,-1,-1>: omp_out = omp_out+omp_in) initializer(omp_priv=Eigen::Matrix<double,-1,-1>::Zero(omp_orig.rows(), omp_orig.cols()))

int N = 100;
double  abc[3] = {1.0, 2.0, 1.0};
double abc0[3] = {0.0, 0.0, 0.0};
TicToc tic_omp;

unordered_map<double, double>dataset;

double makeHess(int numthreads, MatXX& gk, MatXX& Bk, bool showResult = false){
    MatXX gk_(MatXX::Zero(3,1));
    MatXX Bk_(MatXX::Zero(3,3));
    gk_.setZero();
    Bk_.setZero();
    tic_omp.tic();
    ofstream fout("omp.txt", ios::app);
    stringstream ss;

//    omp_set_num_threads(numthreads);
    #pragma omp parallel for reduction(plus:gk_, Bk_) //private(ss)
//    for(auto data : dataset){
    for(size_t b = 0; b<dataset.bucket_count(); ++b) {
        for(auto data = dataset.begin(b); data != dataset.end(b); data++) {

            double x = data->first, y = data->second;
            Eigen::Matrix<double, 1, 3> j = computeJacobian(abc0, x);
            Eigen::Matrix<double, 1, 1> r = computeResidual(abc0, x, y);

//        ss << i << "\t" << r(0) << endl;
//        fout << ss.str();
//        printf("%d\t%f\n", i, r(0));
//#pragma omp critical
            {
                gk_ += j.transpose() * r;
                Bk_ += j.transpose() * j;
            }
            sleep(1.0);
        }
    }
    ss << "gk = \n" << gk_ << endl << "Bk = \n" << Bk_ << endl << endl;
    fout << ss.str();
    fout.close();

    double timeCost = tic_omp.toc();
    if (showResult) {
        cout << "\t" << timeCost << "ms with " << numthreads << " threads.\n";
        //    cout << "gk = \n" << gk_.transpose() << endl;
        //    cout << "Bk = \n" << Bk_ << endl << endl;
    }
    gk = gk_;
    Bk = Bk_;
    return timeCost;
}

double sumAll(int numthreads, int& sum){
     int sum_, i;
     int N = 10000;
     double timeCost;
     sum_ = 0;
    tic_omp.tic();

    omp_set_num_threads(numthreads);
    #pragma omp parallel for private(i) reduction(+:sum_)

    for (i = 0; i < N; ++i) {
 //#pragma omp critical
         sum_ += i;
 //        printf("%d\n", i);
     }
     timeCost = tic_omp.toc();
//     timeOMP_sum += timeCost;
     cout << "omp time cost: " << timeCost << "ms, " << "sum = " << sum << endl;
     sum = sum_;
    return timeCost;
}

int main(int argc, char** argv){

    dataset.clear();
    for (int i = 0; i < N; ++i) {
        double x = (double) i / N;
        double y = exp(abc[0] * x * x + abc[1] * x + abc[2]);
        dataset.insert(make_pair(x, y));
    }
//    dataset.max_load_factor(100);




    int numProcs = 1;

        cout << "==========Using OpenMP==========\n";
        numProcs = omp_get_num_procs();

    MatXX gk(MatXX::Zero(3,1));
    MatXX Bk(MatXX::Zero(3,3));
//    ofstream fout("outlog.txt");
//#ifdef USE_OPENMP
    ofstream fout("omp.txt");
    fout.close();
    double timeMin = 100000.0; int threadMin = 1;
    int numRuns = 1;
//    for (int numthreads = 1; numthreads<= numProcs; ++numthreads){ //omp_get_num_procs()
    for (int numthreads =numProcs; numthreads >= 1; --numthreads){ //omp_get_num_procs()
//    int numthreads = 20;
        double timeCost, timeCostSum = 0.0;
        for (int i=0; i<= numRuns; ++i) {
            double t = makeHess(numthreads, gk, Bk, true);
            if (i != 0)
                timeCostSum += t;
        }
        timeCost = timeCostSum / (double)numRuns;
        if (timeCost < timeMin){
            timeMin = timeCost;
            threadMin = numthreads;
        }
        cout << numthreads << " threads avg time cost = " << timeCost << "ms\n";
    }
    cout << "===================\nTime cost = " << timeMin << "ms with " << threadMin << " threads.\n";
//    stringstream ss;
//    ss << "gk = \n" << gk << endl << "Bk = \n" << Bk << endl;
//    cout << ss.str();
    cout << "gk = \n" << gk << endl;
    cout << "Bk = \n" << Bk << endl;


    return 0;
}
