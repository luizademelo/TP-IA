#include <cstdlib>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

typedef struct point
{
    int x;
    int y;
    double cost;
    vector<point> path;

    point(int x = -1, int y = -1, int cost = 0)
        : x(x), y(y), cost(cost) {}
} point;

struct compare
{
    bool operator()(const point &p1, const point &p2) const
    {
        if (p1.x == p2.x)
        {
            return p1.y < p2.y;
        }
        return p1.x < p2.x;
    }
};

typedef struct solution
{
    point node;
    double cost;
    vector<point> path;
    bool valid = 1;
} solution;

int num_lines, num_columns;
point inicio;
point fim;
vector<string> graph;

bool goal_test(point &node)
{
    return node.x == fim.x && node.y == fim.y;
}

bool is_point_valid(point &node)
{
    return node.x >= 0 && node.y >= 0 && node.x < num_lines && node.y < num_columns && graph[node.x][node.y] != '@';
}

double get_cost(point &node)
{
    if (graph[node.x][node.y] == '.')
        return 1;
    if (graph[node.x][node.y] == ';')
        return 1.5;
    if (graph[node.x][node.y] == '+')
        return 2.5;
    if (graph[node.x][node.y] == 'x')
        return 6.0;
}

struct compare_min_heap
{
    bool operator()(point &p1, point &p2)
    {
        return get_cost(p1) > get_cost(p2); // Min-heap: lower cost has higher priority
    }
};

double distance_from_goal(point &p1)
{
    return sqrt(pow(p1.x - fim.x, 2) + pow(p1.y - fim.y, 2));
}

struct compare_greedy
{
    bool operator()(point &p1, point &p2)
    {
        return distance_from_goal(p1) > distance_from_goal(p2);
    }
};

struct compare_a_star
{
    bool operator()(point &p1, point &p2)
    {
        return distance_from_goal(p1) + get_cost(p1) > distance_from_goal(p2) + get_cost(p2);
    }
};

vector<point> generate_actions(point node)
{
    vector<point> actions;
    actions.push_back(point(node.x + 1, node.y));
    actions.push_back(point(node.x - 1, node.y));
    actions.push_back(point(node.x, node.y + 1));
    actions.push_back(point(node.x, node.y - 1));
    return actions;
}

solution a_star()
{
    solution s;
    point node = inicio;
    priority_queue<point, vector<point>, compare_a_star> frontier;
    set<point, compare> explored;
    frontier.push(node);

    while (!frontier.empty())
    {

        node = frontier.top();
        frontier.pop();
        explored.insert(node);

        // testando cada ação disponível
        // o nó pode ir pra cima, baixo, esquerda e direita

        for (auto child : generate_actions(node))
        {
            if (is_point_valid(child) && !explored.count(child))
            {
                for (int i = 0; i < node.path.size(); i++)
                {
                    child.path.push_back(node.path[i]);
                }
                child.path.push_back(node);
                child.cost += node.cost + get_cost(child);
                frontier.push(child);

                if (goal_test(child))
                {
                    s.cost = child.cost;
                    s.node = child;
                    for (int i = 0; i < child.path.size(); i++)
                    {
                        s.path.push_back(child.path[i]);
                    }
                    s.path.push_back(fim);

                    return s;
                }
            }
        }
    }
    std::cout << "Falha ao tentar encontrar a solucao\n";
}

solution greedy()
{
    solution s;
    point node = inicio;
    priority_queue<point, vector<point>, compare_greedy> frontier;
    set<point, compare> explored;
    frontier.push(node);

    while (!frontier.empty())
    {

        node = frontier.top();
        frontier.pop();
        explored.insert(node);

        // testando cada ação disponível
        // o nó pode ir pra cima, baixo, esquerda e direita

        for (auto child : generate_actions(node))
        {
            if (is_point_valid(child) && !explored.count(child))
            {
                for (int i = 0; i < node.path.size(); i++)
                {
                    child.path.push_back(node.path[i]);
                }
                child.path.push_back(node);
                child.cost += node.cost + get_cost(child);
                frontier.push(child);

                if (goal_test(child))
                {
                    s.cost = child.cost;
                    s.node = child;
                    for (int i = 0; i < child.path.size(); i++)
                    {
                        s.path.push_back(child.path[i]);
                    }
                    s.path.push_back(fim);

                    return s;
                }
            }
        }
    }
    std::cout << "Falha ao tentar encontrar a solucao\n";
}

solution ucs()
{
    solution s;
    point node = inicio;
    priority_queue<point, vector<point>, compare_min_heap> frontier;
    set<point, compare> explored;
    frontier.push(node);

    while (!frontier.empty())
    {

        node = frontier.top();
        frontier.pop();
        explored.insert(node);

        // testando cada ação disponível
        // o nó pode ir pra cima, baixo, esquerda e direita

        for (auto child : generate_actions(node))
        {
            if (is_point_valid(child) && !explored.count(child))
            {
                for (int i = 0; i < node.path.size(); i++)
                {
                    child.path.push_back(node.path[i]);
                }
                child.path.push_back(node);
                child.cost += node.cost + get_cost(child);
                frontier.push(child);

                if (goal_test(child))
                {
                    s.cost = child.cost;
                    s.node = child;
                    for (int i = 0; i < child.path.size(); i++)
                    {
                        s.path.push_back(child.path[i]);
                    }
                    s.path.push_back(fim);

                    return s;
                }
            }
        }
    }
    std::cout << "Falha ao tentar encontrar a solucao\n";
}

solution bfs()
{
    solution s;
    point node = inicio;
    node.cost = 0;

    if (goal_test(node))
    {
        s.cost = node.cost;
        s.node = node;
        return s;
    }

    queue<point> frontier;
    set<point, compare> explored;
    frontier.push(node);

    while (!frontier.empty())
    {

        node = frontier.front();
        frontier.pop();
        explored.insert(node);

        // testando cada ação disponível
        // o nó pode ir pra cima, baixo, esquerda e direita

        for (auto child : generate_actions(node))
        {
            if (is_point_valid(child) && !explored.count(child))
            {
                for (int i = 0; i < node.path.size(); i++)
                {
                    child.path.push_back(node.path[i]);
                }
                child.path.push_back(node);
                child.cost += 1;
                frontier.push(child);

                if (goal_test(child))
                {
                    s.cost = child.cost + 1;
                    s.node = child;
                    for (int i = 0; i < child.path.size(); i++)
                    {
                        s.path.push_back(child.path[i]);
                    }
                    s.path.push_back(fim);

                    return s;
                }
            }
        }
    }
    std::cout << "Falha ao tentar encontrar a solucao\n";
}

// depth limited search
solution dls(point node, int current_depth, int max_depth, set<point, compare> &explored)
{
    solution s;
    if (current_depth > max_depth)
    {
        s.valid = false;
        return s;
    }
    explored.insert(node);
    for (auto child : generate_actions(node))
    {
        if (is_point_valid(child) && !explored.count(child))
        {
            for (int i = 0; i < node.path.size(); i++)
            {
                child.path.push_back(node.path[i]);
            }
            child.path.push_back(node);
            child.cost += node.cost + get_cost(child);

            if (goal_test(child))
            {
                s.cost = child.cost;
                s.node = child;
                for (int i = 0; i < child.path.size(); i++)
                {
                    s.path.push_back(child.path[i]);
                }
                s.path.push_back(fim);

                return s;
            }
            solution s_child = dls(child, current_depth + 1, max_depth, explored);
            if (s_child.valid)
            {
                return s_child;
            }
        }
    }
}

void read_file(string &filename)
{

    ifstream file(filename);

    if (!file)
    {
        std::cout << "Não foi possível abrir o arquivo\n";
        exit(1);
    }

    file >> num_lines >> num_columns;

    file.ignore();
    string line;

    while (getline(file, line))
    {
        graph.push_back(line);
    }

    file.close();
}

void parse_input(int argc, char *argv[], string &filename, string &metodo)
{
    if (argc != 7)
    {
        cerr << "Usage: ./pathfinder [caminho_para_arquivo_mapa] [identificador_metodo] xi yi xf yf" << endl;
        exit(1);
    }

    filename = argv[1];
    metodo = argv[2];
    inicio.x = atoi(argv[3]);
    inicio.y = atoi(argv[4]);
    fim.x = atoi(argv[5]);
    fim.y = atoi(argv[6]);
}

int main(int argc, char *argv[])
{

    string filename;
    string metodo;
    point inicio;
    point fim;
    //   teste.map BFS 0 1 2 3
    parse_input(argc, argv, filename, metodo);
    read_file(filename);

    if (metodo == "UCS")
    {
        solution s = ucs();
        std::cout << fixed << setprecision(2) << s.cost << endl;
        for (int i = 0; i < s.path.size(); i++)
        {
            std::cout << "(" << s.path[i].x << ", " << s.path[i].y << ") ";
        }
    }
    else if (metodo == "BFS")
    {
        solution s = bfs();
        std::cout << s.cost << endl;
        for (int i = 0; i < s.path.size(); i++)
        {
            std::cout << "(" << s.path[i].x << ", " << s.path[i].y << ") ";
        }
    }
    else if (metodo == "IDS")
    {
        for (int depth = 0; depth <= 100000; depth++)
        {
            set<point, compare> explored;
            solution s = dls(inicio, 0, depth, explored);
        }
    }
    else if (metodo == "Greedy")
    {
    }
    else if (metodo == "Astar")
    {
    }
    else
    {
        std::cout << "Método de busca desconhecido!\n";
    }

    return 0;
}
