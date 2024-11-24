#include <cstdlib>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

typedef struct point
{
    int x;
    int y;
    double cost;
    point *parent;

    point() : x(0), y(0) {}

    point(int x, int y, int cost = 0, point *parent = nullptr)
        : x(x), y(y), cost(cost), parent(parent) {}
} point;

struct compare
{
    bool operator()(const point *p1, const point *p2) const
    {
        if (p1->x == p2->x)
        {
            return p1->y < p2->y;
        }
        return p1->x < p2->x;
    }
};

typedef struct solution
{
    double cost = 0;
    vector<point> path;
    bool valid = false;
} solution;

int num_lines, num_columns;
point inicio;
point fim;
vector<string> graph;

bool goal_test(point &node)
{
    return node.x == fim.x && node.y == fim.y;
}

bool is_point_valid(point *node)
{
    return node->x >= 0 && node->y >= 0 && node->x < num_lines && node->y < num_columns && graph[node->x][node->y] != '@';
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

    std::cerr << "Caractere invalido na posicao " << node.x << " " << node.y << "\n";
    return -1.0;
}

struct compare_min_heap
{
    bool operator()(point *p1, point *p2)
    {
        return get_cost(*p1) > get_cost(*p2); // Min-heap: lower cost has higher priority
    }
};

double distance_from_goal(point *p1)
{
    return sqrt(pow(p1->x - fim.x, 2) + pow(p1->y - fim.y, 2));
}

struct compare_greedy
{
    bool operator()(point *p1, point *p2)
    {
        return distance_from_goal(p1) > distance_from_goal(p2);
    }
};

struct compare_a_star
{
    bool operator()(point *p1, point *p2)
    {
        return distance_from_goal(p1) + get_cost(*p1) > distance_from_goal(p2) + get_cost(*p2);
    }
};

vector<point *> generate_actions(point *node)
{
    vector<point *> actions;
    point *bottom = new point(node->x + 1, node->y, 0, node);
    point *up = new point(node->x - 1, node->y, 0, node);
    point *right = new point(node->x, node->y + 1, 0, node);
    point *left = new point(node->x, node->y - 1, 0, node);
    if (is_point_valid(bottom))
    {
        actions.push_back(bottom);
    }
    if (is_point_valid(up))
    {
        actions.push_back(up);
    }
    if (is_point_valid(left))
    {
        actions.push_back(left);
    }
    if (is_point_valid(right))
    {
        actions.push_back(right);
    }
    return actions;
}

solution a_star()
{
    solution s;
    point *node = new point(inicio.x, inicio.y, 0, nullptr);
    priority_queue<point *, vector<point *>, compare_a_star> frontier;
    set<point *, compare> explored;
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

                child->parent = node;
                child->cost += node->cost + get_cost(*child);

                if (goal_test(*child))
                {

                    s.cost = child->cost;

                    point *curr = child;
                    while (curr)
                    {
                        s.path.push_back(*curr);
                        curr = curr->parent;
                    }

                    s.valid = true;
                    return s;
                }
                frontier.push(child);
            }
        }
    }
    std::cout << "Falha ao tentar encontrar a solucao\n";

    return s;
}

solution greedy()
{
    solution s;
    point *node = new point(inicio.x, inicio.y, 0, nullptr);

    priority_queue<point *, vector<point *>, compare_greedy> frontier;
    set<point *, compare> explored;
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
                child->parent = node;
                child->cost += node->cost + get_cost(*child);

                if (goal_test(*child))
                {
                    s.cost = child->cost;

                    point *curr = child;
                    while (curr)
                    {
                        s.path.push_back(*curr);
                        curr = curr->parent;
                    }

                    s.valid = true;
                    return s;
                }
                frontier.push(child);
            }
        }
    }
    std::cout << "Falha ao tentar encontrar a solucao\n";
    return s;
}

solution ucs()
{
    solution s;
    point *node = new point(inicio.x, inicio.y, 0, nullptr);
    priority_queue<point *, vector<point *>, compare_min_heap> frontier;
    set<point *, compare> explored;
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

                child->parent = node;
                child->cost += node->cost + get_cost(*child);

                if (goal_test(*child))
                {
                    s.cost = child->cost;
                    point *curr = child;
                    while (curr)
                    {
                        s.path.push_back(*curr);
                        curr = curr->parent;
                    }

                    s.valid = true;

                    return s;
                }
                frontier.push(child);
            }
        }
    }
    std::cout << "Falha ao tentar encontrar a solucao\n";
    return s;
}

solution bfs()
{
    solution s;
    point *node = new point(inicio.x, inicio.y, 0, nullptr);

    if (goal_test(*node))
    {
        s.cost = node->cost;
        return s;
    }

    queue<point *> frontier;
    set<point *, compare> explored;
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

                child->parent = node;
                child->cost += node->cost + get_cost(*child);

                if (goal_test(*child))
                {
                    s.cost = child->cost;
                    point *curr = child;
                    while (curr)
                    {
                        s.path.push_back(*curr);
                        curr = curr->parent;
                    }
                    s.valid = true;

                    return s;
                }
                frontier.push(child);
            }
        }
    }
    std::cout << "Falha ao tentar encontrar a solucao\n";
    return s;
}

// depth limited search
solution dls(point *node, int current_depth, int max_depth, set<point *, compare> explored)
{
    solution s;
    s.valid = false;
    if (current_depth >= max_depth)
    {
        return s;
    }
    explored.insert(node);
    for (auto child : generate_actions(node))
    {
        if (is_point_valid(child) && !explored.count(child))
        {
            child->parent = node;

            child->cost += node->cost + get_cost(*child);

            if (goal_test(*child))
            {
                s.cost = child->cost;

                point *curr = child;
                while (curr)
                {
                    s.path.push_back(*curr);
                    curr = curr->parent;
                }
                s.valid = true;

                return s;
            }
            solution s_child = dls(child, current_depth + 1, max_depth, explored);
            if (s_child.valid)
            {
                return s_child;
            }
        }
    }
    return s;
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

void print_solution(solution s)
{
    std::cout << fixed << setprecision(2) << s.cost << endl;

    for (int i = s.path.size() - 1; i >= 0; i--)
    {
        std::cout << "(" << s.path[i].x << ", " << s.path[i].y << ") ";
    }
}

int main(int argc, char *argv[])
{

    string filename;
    string metodo;
    //   teste.map IDS 0 1 2 3
    parse_input(argc, argv, filename, metodo);
    read_file(filename);

    if (metodo == "UCS")
    {
        solution s = ucs();
        print_solution(s);
    }
    else if (metodo == "BFS")
    {
        solution s = bfs();
        print_solution(s);
    }
    else if (metodo == "IDS")
    {
        solution s;
        for (int depth = 0; depth <= 100000; depth++)
        {
            set<point *, compare> explored;
            point *node = new point(inicio.x, inicio.y, 0, nullptr);

            s = dls(node, 0, depth, explored);
            if (s.valid)
            {
                break;
            }
        }
        print_solution(s);
    }
    else if (metodo == "Greedy")
    {
        solution s = greedy();
        print_solution(s);
    }
    else if (metodo == "Astar")
    {
        solution s = a_star();
        print_solution(s);
    }
    else
    {
        std::cout << "Método de busca desconhecido!\n";
    }

    return 0;
}
