#include <iostream>
#include "csv.h"
#include <fstream>
#include <regex>
#include <shark/Data/Csv.h>
#include <shark/Algorithms/Trainers/LDA.h>
#include <shark/ObjectiveFunctions/Loss/ZeroOneLoss.h>

using namespace shark;

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

    //preprocessing CSV files
    std::ifstream data_stream("./iris/iris.data");
    std::string data_string((std::istreambuf_iterator<char>(data_stream)),std::istreambuf_iterator<char>());

    data_string = std::regex_replace(data_string, std::regex("Iris-setosa"), "1");
    data_string = std::regex_replace(data_string, std::regex("Iris-versicolor"), "2");
    data_string = std::regex_replace(data_string, std::regex("Iris-virginica"), "3");

    std::ofstream out_stream("iris_fix.csv");
    out_stream << data_string;

    ClassificationDataset dataset;
    importCSV(dataset, "iris_fix.csv", LAST_COLUMN);

    std::size_t classes = numberOfClasses(dataset);
    std::cout << "Number of classes " << classes << std::endl;
    std::vector<std::size_t> sizes = classSizes(dataset);
    std::cout << "Class size: " << syd::endl;
    for (auto cs : sizes) {
        std::cout << cs << std::endl;
    }

    std::size_t dim = inputDimension(dataset);
    std::cout << "Input dimension " << dim << std::endl;

  
    return 0;
}
