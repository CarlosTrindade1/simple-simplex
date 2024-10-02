/*
    Autor: Carlos Eduardo da Silva Trindade
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#define EPSILON 0.00001

using namespace std;

struct Problem {
    int n;
    int m;
    vector<double> c;
    vector<vector<double>> A;
    vector<double> b;
    vector<double> base;
    vector<vector<double>> tableau;
};

void print_tableau(Problem *problem) {
    for (int i = 0; i < problem->tableau.size(); i++) {
        for (int j = 0; j < problem->tableau[i].size(); j++) {
            printf("%.5f ", problem->tableau[i][j]);
        }
        cout << endl;
    }
}

void read_numbers(string line, vector<double> *numbers) {
    istringstream iss(line);
    string part;

    while (iss >> part) {
        try {
            int number = std::stoi(part);
            numbers->push_back(number);
        } catch (const std::invalid_argument& e) {
            // Ignora partes que não são números
        }
    }
}

void read_file(char* filename, Problem *problem) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        // P line
        if (line[0] == 'P') {
            sscanf(line.c_str(), "P %d %d", &problem->n, &problem->m);
        } else if (line[0] == 'F') {
            read_numbers(line, &problem->c);
        } else if (line[0] == 'R') {
            vector<double> *row = new vector<double>();
            read_numbers(line, row);
            problem->A.push_back(*row);
        } else if (line[0] == 'B') {
            read_numbers(line, &problem->base);
        }
    }
}

void create_problem(char* filename, Problem *problem) {
    read_file(filename, problem);

    for (int i = 0; i < problem->m; i++) {
        problem->b.push_back(problem->A[i].back());
        problem->A[i].pop_back();
    }
}

void create_tableau(Problem *problem) {
    problem->tableau.push_back(problem->c);

    for (int i = 0; i < problem->c.size(); i++) {
        problem->tableau[0][i] *= -1;
    }

    problem->tableau[0].push_back(0);

    for (int i = 0; i < problem->base.size(); i++) {
        for (int j = 0; j < problem->m; j++) {
            int var_index = problem->base[i] - 1;

            if (problem->base[i] <= problem->n - problem->m) {
                problem->tableau.push_back(problem->A[var_index]);
                problem->tableau.back().push_back(problem->b[var_index]);
                break;
            } else  if (problem->A[j][var_index] != 0) {
                problem->tableau.push_back(problem->A[j]);
                problem->tableau.back().push_back(problem->b[j]);
                break;
            }
        }
    }
}

int pivoteia(Problem *problem, int row, int col) {
    double pivot = problem->tableau[row][col];

    if (pivot <= EPSILON && pivot >= -EPSILON) {
        return 1;
    }

    for (int i = 0; i < problem->tableau[row].size(); i++) {
        problem->tableau[row][i] /= pivot;
    }

    for (int i = 0; i < problem->tableau.size(); i++) {
        if (i == row) {
            continue;
        }

        double factor = problem->tableau[i][col] * -1;

        for (int j = 0; j < problem->tableau[i].size(); j++) {
            problem->tableau[i][j] += problem->tableau[row][j] * factor;
        }
    }

    return 0;
}

bool checks_basis_viability(Problem *problem) {
    for (int i = 0; i < problem->base.size(); i++) {
        if(pivoteia(problem, i + 1, problem->base[i] - 1)) {
            return false;
        }
    }

    for (int i = 1; i < problem->tableau.size(); i++) {
        if (problem->tableau[i].back() < -EPSILON) {
            return false;
        }
    }

    return true;
}

void print_solution(Problem *problem) {
    int row = 1;

    printf("X ");

    for (int i = 0; i < problem->n; i++) {
        bool is_base = false;

        for (int j = 0; j < problem->base.size(); j++) {
            if (problem->base[j] - 1 == i) {
                is_base = true;
                break;
            }
        }

        if (is_base) {
            printf("%.3f ", problem->tableau[row].back());
            row++;
        } else {
            printf("0.000 ");
        }
    }
    cout << endl;
}

void print_reduced_cost(Problem *problem) {
    printf("C ");
    for (int i = 0; i < problem->tableau[0].size() - 1; i++) {
        double value = problem->tableau[0][i];

        value < -EPSILON || value > EPSILON ? value = problem->tableau[0][i] * -1 : value = problem->tableau[0][i];

        printf("%.3f ", value);
    }

    cout << endl;
}

int main(int argv, char** argc) {
    char* filename = argc[1];

    Problem *problem = new Problem();

    create_problem(filename, problem);

    create_tableau(problem);

    bool is_viable = checks_basis_viability(problem);

    printf("V %c\n", is_viable ? 'S' : 'N');

    if (!is_viable) {
        return 0;
    }

    print_solution(problem);

    print_reduced_cost(problem);

    printf("Z %.3f\n", problem->tableau[0].back());

    delete problem;

    return 0;
}