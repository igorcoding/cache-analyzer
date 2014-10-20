#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <sstream>
#include <assert.h>

#define NPAD 7
#define TWICE(x) x x
#define ACCESS_CYCLES 2048 // TODO
#define ACCESS_COUNT 64

#define MIN_ARR_SHIFT 2
#define MAX_ARR_SHIFT 20

#define TESTS_COUNT 100000

const unsigned SEED = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine RAND_ENGINE = std::default_random_engine(SEED);

struct el {
    struct el* next;
    long int pad[NPAD];

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
void connect_arr(std::vector<el>& arr, const std::vector<size_t>& indices);
double experiment(el* first_el);
void clear_dependencies(std::vector<el>& arr);
std::string get_plot_data(const std::vector<point>& arr);


int main()
{
    std::cout << sizeof (el);
    std::vector<point> linear_stats;
    std::vector<point> random_stats;

    for (int i= MIN_ARR_SHIFT; i <= MAX_ARR_SHIFT; ++i) {
        long arr_size = 1 << i;
        std::vector<el> elems(arr_size);
        std::vector<size_t> indices(elems.size());

        // Linear
        fill_linearly(indices);
        connect_arr(elems, indices);
        double linear_time = experiment(&elems[0]);

        // Random
        fill_randomly(indices, false);
        connect_arr(elems, indices);
        double random_time = experiment(&elems[0]);


        linear_stats.push_back(point(i, linear_time));
        random_stats.push_back(point(i, random_time));
    }

    std::string linear_plot = get_plot_data(linear_stats);
    std::string random_plot = get_plot_data(random_stats);


    return 0;
}






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
    std::shuffle (indices.begin(), indices.end(), RAND_ENGINE);
}

void connect_arr(std::vector<el>& arr, const std::vector<size_t>& indices)
{
    assert(arr.size() == indices.size());
    auto size = arr.size();

    for (size_t i = 0; i < size; ++i) {
        auto next_id = indices[i];
        arr[i].next = &arr[next_id];
    }
}

double experiment(el* first_el)
{
    el* element = first_el;

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

void clear_dependencies(std::vector<el>& arr)
{
    for (auto& elem : arr) {
        elem.next = nullptr;
    }
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
