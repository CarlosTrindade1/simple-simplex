/*
    Autor: Carlos Eduardo da Silva Trindade
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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

    if (pivot == 0) {
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

int main(int argv, char** argc) {
    char* filename = argc[1];
    int is_not_viable;

    Problem *problem = new Problem();

    create_problem(filename, problem);

    create_tableau(problem);

    is_not_viable = pivoteia(problem, 1, 0);
    is_not_viable = pivoteia(problem, 2, 1);
    is_not_viable = pivoteia(problem, 3, 2);

    if (is_not_viable) {
        cout << "Problema não viável" << endl;
    }

    // print tableau
    for (int i = 0; i < problem->tableau.size(); i++) {
        for (int j = 0; j < problem->tableau[i].size(); j++) {
            cout << problem->tableau[i][j] << " ";
        }
        cout << endl;
    }


    delete problem;

    return 0;
}