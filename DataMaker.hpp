#ifndef DSLRT_DATAMAKER_HPP
#define DSLRT_DATAMAKER_HPP
#include <vector>
#include <random>
#include <map>
#include <algorithm>

namespace DataMaker{

    class Query {
    private:
    public:
        double L = 0,R = 0;
        int th = 0;
        bool isCovered(double l, double r){
            return L <= l && r <= R;
        }
        Query(double l, double r, int t){
            L = l;
            R = r;
            th = t;
        }
        Query()= default;
    };

    class DataMaker{
    private:
        int intervalNum;
        int upperBound,lowerBound=0;

    public:
        std::vector<std::pair<int,Query> > op;
        std::vector<Query > intervals;
        void setIntervalNum(int N){intervalNum = N;}
        void setUpperBound(double upper){upperBound = upper;}
        void setBound(int lower, int upper){
            upperBound = upper;
            lowerBound = lower;
        }

        void getRandomIntIntervals(int N,double upper,int cov, int t){
            intervals.resize(N);
            std::mt19937 gen(std::random_device{}());
            if(cov == 0){
                std::uniform_int_distribution<int> rand(0,upper);
                for(int i=0;i<N;i++){
                    intervals[i]=Query(rand(gen),rand(gen), t);
                    if(intervals[i].L>intervals[i].R) std::swap(intervals[i].L,intervals[i].R);
                    if(intervals[i].L==intervals[i].R){
                        intervals[i].R= upper;
                        intervals[i].L = 0;
                    }
                }
            }
            else {
                double range = upper * cov / 100;
                std::map<double,int>mp;
                std::uniform_int_distribution<int> rand(0, upper - range);
                for (int i = 0; i < N; i++) {
                    double L = rand(gen);
                    while(mp.contains(L)) L = rand(gen);
                    mp[L] = 1;
                    double R = L + range - 1;
                    intervals[i] = Query(L, R, t);
                }
            }
        }

        void getRandomOp(int M,double upper,int cov, int t, int w){
            op.resize(M);
            std::mt19937 gen(std::random_device{}());
            double range = upper * cov / 100;
            std::map<double,int>mp;
            std::uniform_int_distribution<int> rand(0, upper - range);
            std::uniform_int_distribution<int> randw(1, w);
            for (int i = 0; i < M; i++) {
                int c = rand(gen)%10;
                if(c == 0) {
                    double L = rand(gen);
                    while (mp.contains(L)) L = rand(gen);
                    mp[L] = 1;
                    double R = L + range - 1;
                    op[i] = {0,Query(L, R, t)};
                }
                else{
                    int key = rand(gen);
                    int weight = randw(gen);
                    op[i] = {1,Query(key,key,weight)};
                }
            }
        }
    };

}
#endif //DSLRT_DATAMAKER_HPP
