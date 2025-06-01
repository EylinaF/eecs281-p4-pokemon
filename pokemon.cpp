// project identifer  = 5949F553E20B650AB0FB2266D3C0822B13D248B0

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cfloat>
#include <iomanip>
#include <math.h>

using namespace std;

class Pokemon
{
private:
    struct point
    {
        bool k = false;
        double d = DBL_MAX;
        size_t p = 0;
        char area = 'l';
    };

    vector<point> points;
    vector<pair<double, double>> coordinate;

    double BestWeight = 0;
    double currentWeight = 0;
    double MSTWeight = 0;
    vector<size_t> path;
    vector<size_t> pathBest;

public:
    // constructor
    Pokemon(bool partA)
    {
        size_t total = 0;
        cin >> total;
        coordinate.resize(total);

        for (size_t i = 0; i < total; ++i)
        {
            cin >> coordinate[i].first >> coordinate[i].second;
            if (partA)
            {
                points.resize(total);
                if ((coordinate[i].first == 0 && coordinate[i].second <= 0) || (coordinate[i].second == 0 && coordinate[i].first <= 0))
                {
                    points[i].area = 'c';
                }
                else if (coordinate[i].first < 0 && coordinate[i].second < 0)
                {
                    points[i].area = 's';
                }
            }
        }
    }

    double distance(pair<double, double> coordinate1, pair<double, double> coordinate2, bool root)
    {
        double dis = (coordinate1.first - coordinate2.first) * (coordinate1.first - coordinate2.first) + (coordinate1.second - coordinate2.second) * (coordinate1.second - coordinate2.second);

        if (root)
        {
            dis = sqrt(dis);
        }

        return dis;
    }
    // calculate distance for MST
    double distanceMST(point point1, point point2, pair<double, double> coordinate1, pair<double, double> coordinate2, bool partA)
    {
        double dis = DBL_MAX;

        if (point1.area == point2.area || point1.area == 'c' || point2.area == 'c')
        {

            dis = distance(coordinate1, coordinate2, !partA);
        }
        else
        {
        }

        return dis;
    }

    void MST(bool partA)
    {
        MSTCalculate(partA, coordinate, points);
    }

    void MSTCalculate(bool partA, vector<pair<double, double>> &position, vector<point> &pointsB)
    {
        size_t i = 0;
        vector<pair<size_t, size_t>> answer;
        double weight = 0;
        pointsB[0].d = 0;

        for (size_t m = 0; m < position.size(); ++m)
        {
            double min = DBL_MAX;
            for (size_t j = 0; j < position.size(); ++j)
            {
                if (!pointsB[j].k && pointsB[j].d < min)
                {
                    min = pointsB[j].d;
                    i = j;
                }
            }
            if (min == DBL_MAX)
            {
                cerr << "Cannot construct MST";
                exit(1);
            }

            pointsB[i].k = true;
            weight += sqrt(min);

            if (partA)
            {
                if (pointsB[i].p != i)
                {
                    answer.push_back({std::min(pointsB[i].p, i), max(pointsB[i].p, i)});
                }
            }

            for (size_t l = 0; l < pointsB.size(); ++l)
            {
                if (!pointsB[l].k)
                {
                    double dis = 0;
                    if (partA)
                    {
                        dis = distanceMST(pointsB[i], pointsB[l], position[i], position[l], partA);
                    }
                    else
                    {
                        dis = distance(position[i], position[l], false);
                    }

                    if (dis < pointsB[l].d)
                    {
                        pointsB[l].d = dis;
                        if (partA)
                        {
                            pointsB[l].p = i;
                        }
                    }
                }
            }
        }

        if (partA)
        {
            cout << weight << "\n";
            for (size_t n = 0; n < answer.size(); ++n)
            {
                cout << answer[n].first << " " << answer[n].second << "\n";
            }
        }
        else
        {
            MSTWeight = weight;
        }
    }

    void FASTTSP(bool print)
    {
        int index = 0;
        path.push_back(0);
        path.push_back(1);
        path.push_back(2);
        path.push_back(0);

        for (size_t i = 3; i < coordinate.size(); ++i)
        {
            double min = DBL_MAX;
            for (size_t j = 0; j < path.size() - 1; ++j)
            {
                double dis = distance(coordinate[i], coordinate[path[j]], true) + distance(coordinate[i], coordinate[path[j + 1]], true) - distance(coordinate[path[j]], coordinate[path[j + 1]], true);
                if (dis < min)
                {
                    min = dis;
                    index = static_cast<int>(j);
                }
            }
            path.insert(path.begin() + index + 1, i);
        }

        for (size_t k = 0; k < path.size() - 1; ++k)
        {
            BestWeight += distance(coordinate[path[k]], coordinate[path[k + 1]], true);
        }

        if (print)
        {
            cout << BestWeight << "\n";

            for (size_t m = 0; m < path.size() - 1; ++m)
            {
                cout << path[m] << " ";
            }
            cout << "\n";
        }
        else
        {
            path.erase(path.end() - 1);
            pathBest = path;
        }
    }

    // The B&B check
    bool promising(size_t permLength)
    {

        bool promise = true;
        if (permLength >= path.size() - 4)
        {
            return promise;
        }

        if (currentWeight >= BestWeight)
        {
            promise = false;
        }

        vector<pair<double, double>> position;
        for (size_t i = permLength; i < path.size(); ++i)
        {
            position.push_back(coordinate[path[i]]);
        }

        vector<point> pointsB;
        pointsB.resize(position.size());

        MSTCalculate(false, position, pointsB);

        double minWeightFirst = DBL_MAX;
        double minWeightEnd = DBL_MAX;
        for (size_t j = permLength; j < path.size(); ++j)
        {
            double disFirst = distance(coordinate[path[0]], coordinate[path[j]], true);
            double disEnd = distance(coordinate[path[permLength - 1]], coordinate[path[j]], true);
            if (disFirst < minWeightFirst)
            {
                minWeightFirst = disFirst;
            }

            if (disEnd < minWeightEnd)
            {
                minWeightEnd = disEnd;
            }
        }
        if (currentWeight + MSTWeight + minWeightFirst + minWeightEnd >= BestWeight)
        {
            promise = false;
        }
        // for (size_t i = 0; i < path.size(); ++i)
        // {
        //     cerr << setw(2) << path[i] << ' ';
        // } // for
        // cerr << setw(4) << permLength << setw(10) << currentWeight;
        // cerr << setw(10) << minWeightFirst << setw(10) << minWeightEnd;
        // cerr << setw(10) << MSTWeight << setw(10) << currentWeight + MSTWeight + minWeightFirst + minWeightEnd << "  " << promise << '\n';
        return promise;
    }

    void genPerms(size_t permLength)
    {
        if (permLength == path.size())
        {
            double dis = distance(coordinate[path[path.size() - 1]], coordinate[0], true);
            currentWeight += dis;
            if (currentWeight < BestWeight)
            {
                pathBest = path;
                BestWeight = currentWeight;
            }
            currentWeight -= dis;
            return;
        }

        if (!promising(permLength))
        {
            return;
        }

        for (size_t i = permLength; i < path.size(); ++i)
        {
            swap(path[permLength], path[i]);
            currentWeight += distance(coordinate[path[permLength]], coordinate[path[permLength - 1]], true);
            genPerms(permLength + 1);
            currentWeight -= distance(coordinate[path[permLength]], coordinate[path[permLength - 1]], true);
            swap(path[permLength], path[i]);
        }
    }

    void OPTTSP()
    {
        FASTTSP(false);
        genPerms(1);

        cout << BestWeight << "\n";
        for (size_t m = 0; m < pathBest.size(); ++m)
        {
            cout << pathBest[m] << " ";
        }
        cout << "\n";
    }
};