/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: alecren
 *
 * Created on 21 juin 2022, 11:03

 * ajouter -lpthread dans linker 
 * 
 *  */

#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <chrono>
#include <thread>
//#include <fstream>
//#include <cstring>
//#include <unistd.h>


using namespace std;

pthread_t thread_store;

static void * fn_store(void * p_data) {
    auto start = std::chrono::high_resolution_clock::now();

    while (1) {

        cout << " tache \n";
        this_thread::sleep_for(2000ms);
    }

    return NULL;
}

int main(int argc, char** argv) {
    int ret = 0;

    int i = 10;
    cout << "Creation du thread !\n" << endl;

    ret = pthread_create(
            & thread_store, NULL,
            fn_store, NULL
            );
    if (!ret) {
        cout << "thread  cree!\n" << endl;
    }

    
    do {
        cout << '\n' << "Press enter key to exit...";
    } while (cin.get() != '\n');
    pthread_cancel(thread_store);

    return 0;
}
