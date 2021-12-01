
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <float.h>
#include <sys/stat.h>
#include <iomanip>
#include <random> /* 亂數函式庫 */
#include <ctime>
#include <functional>
#include "TestingFunction.cpp"
// #include "CEC17.hpp"

std::random_device rd;
std::mt19937_64 generator(rd());
using namespace std;

typedef vector<char> c1d;
typedef vector<c1d> c2d;
typedef vector<c2d> c3d;
typedef vector<c3d> c4d;
typedef vector<int> i1d;
typedef vector<i1d> i2d;
typedef vector<i2d> i3d;
typedef vector<i3d> i4d;
typedef vector<double> d1d;
typedef vector<d1d> d2d;
typedef vector<d2d> d3d;
typedef vector<d3d> d4d;

class DE
{
public:
    d1d Run_result;
    d1d Run_evaluation;
    double Run_avg;

public:
    void run(int run, int pop_size, int dim, int fun_num, int ben_num)
    {

        Run_result.clear();
        Run_result.resize(run);
        Run_avg = 0.0;
        for (int i = 0; i < run; i++)
        {
            Set(pop_size, dim, fun_num, ben_num);
            Initial(dim, fun_num, ben_num);

            while (nfe < MAX_NFE)
            {
                Transition(dim, pop_size);
                Selection(dim, pop_size, fun_num, ben_num);
                Find_best();
            }
            Run_result[i] = current_best;
            Run_avg += Run_result[i];
        }
        cout << fun_num << ' ' << ben_num << ' ' << Run_avg / run << endl;
    }

private:
    double CR, F;
    double max, min;
    int nfe, MAX_NFE;
    double current_best;
    d2d pop;
    d1d objective;
    d2d vector;

private:
    inline double randDouble(double min, double max)
    {
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
        // return (max - min) * rand() / (RAND_MAX + 1.0) + min;
    }
    inline int randInt(int min, int max)
    {
        std::uniform_int_distribution<int> int_distribution(min, max);
        return int_distribution(generator);
        // return rand() % (max - min + 1) + min;
    }
    void Set(int pop_size, int dim, int fun_num, int ben_num)
    {
        cout << scientific << setprecision(8);
        max = 100.0;
        min = -100.0;
        if (ben_num == -1) // testing function
            Set_Range(max, min, fun_num, dim);
        nfe = 0;
        MAX_NFE = dim * 10000;

        CR = 0.6;
        F = 0.6;

        pop.clear();
        pop.swap(pop);
        pop.assign(pop_size, d1d(dim));

        objective.clear();
        objective.swap(objective);
        objective.resize(pop_size);

        vector.clear();
        vector.swap(vector);
        vector.assign(pop_size, d1d(dim));

        current_best = DBL_MAX;
    }
    void Initial(int dim, int fun_num, int ben_num)
    {
        for (int i = 0; i < pop.size(); i++)
        {
            for (int j = 0; j < pop[i].size(); j++)
            {
                pop[i][j] = (max - min) * randDouble(0.0, 1.0) + min;
            }
            Evaluation(pop[i], objective[i], dim, fun_num, ben_num);
        }
    }
    void Transition(int dim, int pop_size)
    {
        for (int i = 0; i < pop_size; i++)
        {
            int r1, r2, r3;
            int rind = randInt(0, dim - 1);
            do
            {
                r1 = randInt(0, pop_size - 1);
            } while (r1 == i);
            do
            {
                r2 = randInt(0, pop_size - 1);
            } while (r2 == r1 || r2 == i);
            do
            {
                r3 = randInt(0, pop_size - 1);
            } while (r3 == r2 || r3 == r1 || r3 == i);
            for (int j = 0; j < dim; j++)
            {
                if (randDouble(0.0, 1.0) < CR || j == rind)
                    vector[i][j] = pop[r1][j] + F * (pop[r2][j] - pop[r3][j]);
                else
                    vector[i][j] = pop[i][j];

                if (vector[i][j] > max)
                    vector[i][j] = max;
                if (vector[i][j] < min)
                    vector[i][j] = min;
            }
        }
    }
    void Selection(int dim, int pop_size, int fun_num, int ben_num)
    {
        for (int i = 0; i < pop_size; i++)
        {
            double temp_obj;
            Evaluation(vector[i], temp_obj, dim, fun_num, ben_num);
            if (temp_obj < objective[i])
            {
                pop[i] = vector[i];
                objective[i] = temp_obj;
            }
            if (nfe == MAX_NFE)
                break;
        }
    }
    void Evaluation(d1d &position, double &objective, int dim, int fun_num, int ben_num)
    {

        Testing_Function(&position[0], &objective, dim, 1, fun_num);
        nfe++;
    }
    void Find_best()
    {
        for (int i = 0; i < objective.size(); i++)
        {
            if (current_best > objective[i])
            {
                current_best = objective[i];
            }
        }
    }
};