#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <set>

using namespace std;
typedef vector<vector<int>> graph;

void printGraph(graph& G)                   //вывод графа
{
    for (int i = 0; i < G.size(); i++)
    {
        for (int j = 0; j < G.size(); j++)
            cout << G[i][j] << "\t";
        cout << endl;
    }
}

graph createGraph(int n)                    //создание графа
{
    graph G(n, vector<int>(n, 0));
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
        {
            if (rand() % 2 == 0)
                G[i][j] = G[j][i] = 1;
        }
    return G;
}

void swapVertices(graph& G, int v1, int v2) //Свап вершин графа
{
    for (int i = 0; i < G.size(); i++)
        swap(G[v1][i], G[v2][i]);
    for (int i = 0; i < G.size(); i++)
        swap(G[i][v1], G[i][v2]);
}

void renumberVertices(graph& G)             //изминение номеров вершин в графе
{
    int count = rand() % G.size() + 1;
    for (int i = 0; i < count; i++)
    {
        int v1 = rand() % G.size();
        int v2 = rand() % G.size();
        swapVertices(G, v1, v2);
    }
}

bool check_isomorphism(graph& G1, graph& G2) //Вектор степеней вершин 2-го порядка
{
    int n = G1.size();
    vector<int> degreeG1(n), degreeG2(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (G1[i][j]) degreeG1[i]++;
            if (G2[i][j]) degreeG2[i]++;
        }
    }
    vector<multiset<int>> L1(n, multiset<int>()), L2(n, multiset<int>());
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (G1[i][j]) L1[i].insert(degreeG1[j]);
            if (G2[i][j]) L2[i].insert(degreeG2[j]);
        }
    }
    sort(L1.begin(), L1.end());
    sort(L2.begin(), L2.end());
    return L1 == L2;
}

bool FullAlgorithm2(graph& g1, graph& g2, int depth_counter = 0) //Полный перебор
{

    if (depth_counter < g1.size() - 1)
    {
        bool branch_result = false;

        for (int i = depth_counter; i < g1.size(); i++)
        {
            swapVertices(g1, depth_counter, i);
            bool flag = FullAlgorithm2(g1, g2, depth_counter + 1);
            swapVertices(g1, depth_counter, i);
            if (flag) return true;
        }
        return false;
    }
    else
        return g1 == g2;
}

double randic(graph& G)            //Индекс Рандича
{
    int n = G.size();
    vector<int> d(n);
    for (int i = 0; i < n; i++) 
    {
        int deg = 0;
        for (int j = 0; j < n; j++)
            if (G[i][j]) deg++; 
        d[i]=deg;
    }
    double r = 0;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (G[i][j]) r += 1 / sqrt(d[i] * d[j]) * 0.5;

    return r;
}

bool randicIsomorphism(graph& G, graph& H)
{
     return abs(randic(G) - randic(H)) < 0.001;
}

graph createGoodGraph(int n)        //создание лучшего случая
{
    graph G(n, vector<int>(n, 0));
    return G;
}
graph createBadGraph(int n)        //создание худшего случая
{
    graph G(n, vector<int>(n, 0));
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
        {
              G[i][j] = G[j][i] = 1;
        }
    return G;
}

int main()
{
    srand(time(0));

    int numberOfVertexes = 10;
    int numberOfIteration = 10000;

    double avg = 0;
    double best = 0;
    double worst = 0;

    for (int i = 0; i < numberOfIteration; i++)
    {
        graph fst_graph;
        graph sec_graph;

        fst_graph = createGoodGraph(numberOfVertexes);
        sec_graph = fst_graph;
        renumberVertices(sec_graph);

        auto start1 = chrono::high_resolution_clock::now();
        check_isomorphism(fst_graph, sec_graph);
        //randicIsomorphism(fst_graph, sec_graph);
        //FullAlgorithm2(fst_graph, sec_graph);
        auto end1 = chrono::high_resolution_clock::now();
        chrono::duration<double> duration1 = end1 - start1;

        best += duration1.count() * 1000;



        fst_graph = createBadGraph(numberOfVertexes);
        sec_graph = fst_graph;
        renumberVertices(sec_graph);

        auto start2 = chrono::high_resolution_clock::now();
        check_isomorphism(fst_graph, sec_graph);
        //randicIsomorphism(fst_graph, sec_graph);
        //FullAlgorithm2(fst_graph, sec_graph);
        auto end2 = chrono::high_resolution_clock::now();

        chrono::duration<double> duration2 = end2 - start2;

        worst += duration2.count()*1000;



        fst_graph = createGraph(numberOfVertexes);
        sec_graph = fst_graph;
        renumberVertices(sec_graph);

        auto start3 = chrono::high_resolution_clock::now();
        check_isomorphism(fst_graph, sec_graph);
        //randicIsomorphism(fst_graph, sec_graph);
        //FullAlgorithm2(fst_graph, sec_graph);
        auto end3 = chrono::high_resolution_clock::now();
        chrono::duration<double> duration3 = end3 - start3;

        avg+= duration3.count()*1000;

    }
    cout << "AVG=" << avg / numberOfIteration << endl;
    cout << "Worst=" << worst / numberOfIteration << endl;
    cout << "Best=" << best / numberOfIteration << endl;
}