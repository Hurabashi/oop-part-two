#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <vector>
#include <map>
#include <chrono>
#include <random>
#include <cstdlib>
#include <cctype>
#include <chrono>
#include <list>

using std::string;
using std::cout;

using std::vector;
using std::map;
using std::mt19937;


struct duomuo {
    string Vard, Pav;
    int egz = 1;
    std::vector<int> pazymiai;
    float GP = 0; float GP_med = 0;

};

class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
public:
    Timer() : start{ std::chrono::high_resolution_clock::now() } {}
    void reset() {
        start = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const {
        return std::chrono::duration<double>
            (std::chrono::high_resolution_clock::now() - start).count();
    }
};


duomuo calculations(duomuo Eil) {
    Eil.GP = Eil.GP / 5.0f;
    Eil.GP = Eil.GP * 0.4f + 0.6f * Eil.egz;

    std::sort(Eil.pazymiai.begin(), Eil.pazymiai.end());
    if (Eil.pazymiai.size() % 2 == 1) {
        Eil.GP_med = float(Eil.pazymiai[Eil.pazymiai.size() / 2]);
        Eil.GP_med = Eil.GP_med * 0.4f + 0.6f * Eil.egz;
    }
    else {
        Eil.GP_med = (Eil.pazymiai[Eil.pazymiai.size() / 2] + Eil.pazymiai[(Eil.pazymiai.size() / 2) - 1]) / 2.0f;
        Eil.GP_med = Eil.GP_med * 0.4f + 0.6f * Eil.egz;
    }
    return Eil;
}

std::vector<duomuo> reading_file(string file_name, int studentu_sk) {
    std::ifstream MyReadFile(file_name);
    if (!MyReadFile) {
        cout << "\nNepavyko pasiekti failo, pasitkrinkite ar jis tikrai tokiu pavadinimu.\n";
        exit(1);
    }
    cout << "Pavyko atidaryti faila... \n";
    duomuo Eil;
    std::vector<duomuo> Eil_vector;

    //nuskaitome
    while (!MyReadFile.eof()) {

        MyReadFile >> Eil.Vard >> Eil.Pav;
        int pazymys = 1;
        for (int i = 0; i < 5; i++) {
            MyReadFile >> pazymys;
            Eil.GP = Eil.GP + (float)pazymys;
            Eil.pazymiai.push_back(pazymys);
        }
        MyReadFile >> Eil.egz;
        //skaiciuojame
        Eil = calculations(Eil);
        //Pridejimas i sarasa
        Eil_vector.push_back(Eil);
        //saraso isvalymas isvalymas
        Eil.pazymiai.clear();

    }
    // uzdarome
    MyReadFile.close();
    return Eil_vector;
}

void create_file_from_list(std::vector<duomuo> Eil_vector, string file_name) {

    std::ofstream myfile;
    myfile.open(file_name);

    myfile << "Vardas         Pavarde          Galutinis (Vid.)   Galutinis (Med.)";
    myfile << "\n-------------------------------------------------------------\n";
    for (duomuo Eil : Eil_vector) {
        myfile << std::left << std::setw(16) << Eil.Vard;
        myfile << std::left << std::setw(16) << Eil.Pav;

        myfile << std::left << std::setw(20) << std::fixed << std::setprecision(2) << Eil.GP;
        myfile << std::left << std::setw(16) << std::fixed << std::setprecision(2) << Eil.GP_med << std::endl;
    }
    //Duomenu isvalymas ir uzdarymas
    Eil_vector.clear();
    myfile.close();
}

void testavimas(int studentu_sk, string base_file, string vargs_file, string galvoc_file) {
    Timer t;
    //cout << "Skaitome faila... \n";
    Timer t2;
    std::vector<duomuo> Eil_vector = reading_file(base_file, studentu_sk);
    cout << "Failo nuskaitymas uztruko: " << t2.elapsed() << " s\n";

    //cout << "Rusiuojame... \n";
    Timer t3;
    std::vector<duomuo> Eil_vector_vargsiukai;


    for (duomuo Eil : Eil_vector) {
        if (Eil.GP < 5.0f) {
            Eil_vector_vargsiukai.push_back(Eil);
        }
    }
    Eil_vector.erase(std::remove_if(Eil_vector.begin(), Eil_vector.end(), [](duomuo Eil)
        {
            return Eil.GP < 5.0f;
        }), Eil_vector.end());

    cout << "Failo rusiavimas uztruko: " << t3.elapsed() << " s\n";


    cout << "Kuriame failus...\n";
    Timer t4;
    create_file_from_list(Eil_vector, galvoc_file);
    //cout << "Failo 'galvociai' kurimas uztruko: " << t4.elapsed() << " s\n";
    Timer t5;
    create_file_from_list(Eil_vector_vargsiukai, vargs_file);
    //cout << "Failo 'vargsiukai' kurimas uztruko: " << t4.elapsed() << " s\n";

    cout << "\n";
    cout << "Bendrai uztruko: " << t.elapsed() << " s\n" << "\n";
}
