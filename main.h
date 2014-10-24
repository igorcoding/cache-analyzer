#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include <cstdlib>
#include <string>

template <size_t N>
struct el {
    struct el* next;
    long int pad[N];

    el():
        next(nullptr)
    { }
};

struct point {
    long arr_size;
    double time;

    point(long size, double t)
        : arr_size(size),
          time(t)
    { }
};

void fill_linearly(std::vector<size_t>& indices);
void fill_randomly(std::vector<size_t>& indices, bool fill = true);

template <size_t N>
void connect_arr(std::vector<el<N>>& arr, const std::vector<size_t>& indices);

template <size_t N>
double experiment(el<N>* first_el);
std::string get_plot_data(const std::vector<point>& arr);
size_t my_log_2(size_t n);

template <size_t N>
void N_expirement();

#endif // MAIN_H
