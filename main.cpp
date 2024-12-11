#include <iostream>
#include "csv.h"
#include <fstream>
#include <regex>
#include <dlib/matrix.h>

template <std::size_t... Idx, typename T, typename R>
bool read_row_help(std::index_sequence<Idx...>, T& row, R& r)
{
    return r.read_row(std::get<Idx>(row)...);
}

//transforming a row tuple object to our value vectors
template <std::size_t... Idx, typename T>
void fill_values(std::index_sequence<Idx...>, T& row, std::vector<double>& data)
{
    data.insert(data.end(), {std::get<Idx>(row)...});
}

int main()
{
    const uint32_t columns_num = 5;
    io::CSVReader<columns_num> csv_reader("./iris/iris.data");

    //storing the values we read
    std::vector<std::string> categorical_column;
    std::vector<double> values;

    //describe values for a row
    using RowType = std::tuple<double, double, double, double, std::string>;
    RowType row;

    try {
    bool done = false;
    while (!done) {
        done = !read_row_help(
        std::make_index_sequence<std::tuple_size<RowType>::value>{}, row,
            csv_reader);
        if (!done) {
            categorical_column.push_back(std::get<4>(row));
            fill_values(std::make_index_sequence<columns_num - 1>{}, row, values);
        }
    }
 } catch (const io::error::no_digit& err) {

 // ignore badly formatted samples
 std::cerr << err.what() << std::endl;
}

    dlib::matrix<double> data;
    std::ifstream file("./iris_fix.csv");
    file >> data;
    std::cout << data << std::endl;

   return 0;
}
