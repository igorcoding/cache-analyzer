#include "main.h"

#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <sstream>
#include <cassert>
#include <ctime>

#define TWICE(x) x x
#define ACCESS_CYCLES 2048
#define ACCESS_COUNT 256

#define TESTS_COUNT 16

#define MAX_ARR_SHIFT 26

const unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();

int main()
{
    N_expirement<0>();
    N_expirement<7>();
    N_expirement<15>();
    N_expirement<31>();

    return 0;
}



/******************** Implementations ********************/


void fill_linearly(std::vector<size_t>& indices) {
    size_t size = indices.size();
    for (size_t i = 0; i < size - 1; ++i) {
        indices[i] = i + 1;
    }
    indices[size-1] = 0; // loopback
}

void fill_randomly(std::vector<size_t>& indices, bool fill) {
    if (fill) {
        fill_linearly(indices);
    }
    my_shuffle(indices.begin(), indices.end(), std::default_random_engine(SEED));
}

template <size_t N>
void connect_arr(std::vector<el<N> >& arr, const std::vector<size_t>& indices)
{
    assert(arr.size() == indices.size());
    auto size = arr.size();

    for (size_t i = 0; i < size; ++i) {
        auto next_id = indices[i];
        arr[i].next = &arr[next_id];
    }
}

template <size_t N>
double experiment(el<N>* first_el)
{
    volatile auto element = first_el;

    auto begin_time = std::chrono::steady_clock::now();

    for (int i = 0; i < ACCESS_CYCLES; ++i) {
        TWICE(
        TWICE(
        TWICE(
        TWICE(
        TWICE(
        TWICE(
        TWICE(
        TWICE(
              element = element->next;
        ))))))))
    }

    auto end_time = std::chrono::steady_clock::now();
    auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - begin_time);
    auto delta = dur / (ACCESS_CYCLES * ACCESS_COUNT);
    return delta.count();
}

std::string get_plot_data(const std::vector<point>& arr) {
    std::stringstream ss;
    ss << "{";
    for (auto& e : arr) {
        ss << "{" << e.arr_size << "," << e.time << "},";
    }
    ss << "}";
    return ss.str();
}

size_t my_log_2(size_t n) {
    int k = 0;
    while (n >= 2) {
        n >>= 1;
        ++k;
    }
    return k;
}

template <typename Iter, typename Generator>
void my_shuffle(Iter first, Iter last, Generator&& generator) {
    for (auto i = (last-first) - 1; i > 0; --i) {
        size_t rand_index = generator() % (i + 1);
        std::swap(first[i], first[rand_index]);
    }
}


template <size_t N>
void N_expirement() {
    std::cout << "NPAD = " << N << std::endl;

    size_t el_size = sizeof (el<N>);
    size_t el_factor = my_log_2(el_size);

    std::vector<point> linear_stats;
    std::vector<point> random_stats;

    for (size_t i = el_factor; i <= MAX_ARR_SHIFT; ++i) {
#ifdef _DEBUG
        std::cout << "Current shift: " << i << std::endl;
#endif
//        long arr_total_size = 1 << i;
        long arr_size = 1 << (i - el_factor);

        double total_linear_time = 0.0;
        double total_random_time = 0.0;

        for (int j = 0; j < TESTS_COUNT; ++j) {
#ifdef _DEBUG
            std::cout << "-- test #" << j << std::endl;
#endif

            std::vector<el<N>> elems(arr_size);
            std::vector<size_t> indices(elems.size());

            // Linear
            fill_linearly(indices);
            connect_arr(elems, indices);
            double linear_time = experiment(&elems[0]);

            // Random
            fill_randomly(indices, false);
            connect_arr(elems, indices);
            double random_time = experiment(&elems[0]);

            total_linear_time += linear_time;
            total_random_time += random_time;
        }

        linear_stats.push_back(point(i, total_linear_time / TESTS_COUNT));
        random_stats.push_back(point(i, total_random_time / TESTS_COUNT));
    }

    std::string linear_plot = get_plot_data(linear_stats);
    std::string random_plot = get_plot_data(random_stats);

    std::cout << "linear: \n" << linear_plot << std::endl
              << "random: \n" << random_plot << std::endl;
}

