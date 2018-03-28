//
//  main.cpp
//  diversity
//
//  Created by Denis Musatov on 02/11/16.
//  Copyright 2018 Frangou Lab.
//


#include <iostream>
#include <vector>

#include "stats.h"
#include "Splitter.hpp"
#include "PlainStringInputStream.hpp"
#include "StringInputStream.hpp"
#include "StringOutputStream.hpp"

using std::vector;
using std::function;
using std::string;

void ShowHelp()
{
    std::cout << 
"Diversity Indexes Calculator: v0.1.1\n" <<
"USAGE: diversity [options] <input path> [<output path>]\n" <<
"\n" <<
"OPTIONS:\n" <<
"   -h          – Show this message.\n" <<
"   -by-columns – Compute diversity indexes by columns.\n" <<
"   -by-rows    – Compute diversity indexes by rows (default).\n" <<
"\n" <<
"INPUT FORMAT:\n" <<
"   The first column should contain some form of row identification. The rest\n" <<
"   of columns are treated as numerical data.\n" <<
"\n" <<
"EXAMPLES:\n" <<
"   diversity -by-rows data_matrix.tsvcr  – Compute diversity indexes for\n" <<
"                                           each row and save the output table\n" <<
"                                           into data_matrix-diversity.tsvcr\n" <<
"   diversity -by-rows -by-columns data_matrix.tsvcr output_name.tsvcr  – Compute diversity indexes\n" << 
"                                                                         for each row and each column\n" << 
"                                                                         and save the output table into\n" << 
"                                                                         output_name.tsvcr\n";
}

vector<function<double(const vector<double>)>> stat_functions = {
        SimpsonIndex,
        DominanceIndex,
        ReciprocalSimpsonIndex,
        ShannonIndexE,
        ShannonIndex2,
        ShannonIndex10,
        MenhinickIndex,
        Buzas_GibsonsIndex,
        EquitabilityIndex,
        SimpsonIndexApproximation,
        DominanceIndexApproximation,
        AlternateReciprocalSimpsonIndex,
        Berger_ParkerDominanceIndex,
        MargalefRichnessIndex,
        RenyiEntropy0,
        RenyiEntropy2,
        RenyiEntropyINF,
        GiniCoefficient
};

vector<string> stat_names = {
        "Simpson Index",
        "Dominance Index",
        "Reciprocal Simpson Index",
        "Shannon_ln Index",
        "Shannon_log2 Index",
        "Shannon_log10 Index",
        "Menhinick Index",
        "Buzas and Gibson's Index",
        "Equitability Index",
        "Simpson Index Approximation",
        "Dominance Index Approximation",
        "Alternate Reciprocal Simpson Index",
        "Berger-Parker Dominance Index",
        "Margalef Richness Index",
        "Renyi Entropy(r=0)",
        "Renyi Entropy(r=2)",
        "Renyi Entropy(r=INF)",
        "Gini Coefficient"
};

static int ReadHeader_(std::unique_ptr<StringInputStream>& in,
                       char delimiter,
                       vector<string>& header)
{ 
    Splitter splitter(delimiter);
    string header_line = in->ReadLine();
    splitter.SetExpression(header_line);
    while (splitter.ReadNext()) {
        header.push_back(splitter.GetNextValue());
    }
    return 0;
}

static char GetDelimiterForFileName_(const std::string& file_path)
{ 
    char delimiter = ',';
    string extension;
    auto dot_position = file_path.rfind('.');
    if (dot_position == string::npos) {
        std::cerr << "error: unable to determine table delimiter type\n";
        return 1;
    } else {
        if (file_path.size() > dot_position && file_path[dot_position + 1] == 't')
                delimiter = '\t';
    }
    return delimiter;
}

static int PopulateDataMatrix_(std::unique_ptr<StringInputStream>& stream,
                               char delimiter,
                               vector<string>& out_ids,
                               vector<vector<double>>& out_m)
{
    string line;
    Splitter splitter(delimiter);
    while (!stream->empty()) {
        line = stream->ReadLine();
        vector<double> data_row;
        splitter.SetExpression(line);
        int i = 0;
        while (splitter.ReadNext()) {
            string val = splitter.GetNextValue();
            if (i == 0) {
                out_ids.push_back(val);
            } else {
                data_row.push_back(std::stod(val));
            }
            i++;
        }
        out_m.push_back(data_row);
    }
    return 0;
}

static vector<vector<double>> 
CalculateRowwiseStats_(const vector<vector<double>>& data_matrix)
{
    vector<vector<double>> stats;
    const int64_t rows_count = data_matrix.size();

    for (int row_index = 0; row_index < rows_count; ++row_index) {
        vector<double> current_row_stats;
        for (const auto& func : stat_functions) {
            current_row_stats.push_back(func(data_matrix[row_index]));
        }
        stats.emplace_back(std::move(current_row_stats));
    }
    return stats;
}

static vector<vector<double>>
CalculateColumnwiseStats_(const vector<vector<double>>& data_matrix,
                          bool compute_by_rows)
{
    vector<vector<double>> columnwise_stats;
    // The number of columns depends on whether we additionally compute
    // per-row statistics. Hence the 'compute_by_rows' flag
    const int64_t columns_count = data_matrix[0].size() - (compute_by_rows ? stat_functions.size() : 0);
    const int64_t rows_count = data_matrix.size();

    for (int column_index = 0; column_index < columns_count; ++column_index) {
        vector<double> column;
        column.reserve(columns_count);
        for (int i = 0; i < rows_count; ++i) {
            column.push_back(data_matrix[i][column_index]);
        }
        vector<double> current_column_stats;
        for (const auto& func : stat_functions) {
            current_column_stats.push_back(func(column));
        }
        columnwise_stats.emplace_back(std::move(current_column_stats));
    }
    return columnwise_stats;
}

static int PrintNewHeader_(std::unique_ptr<StringOutputStream>& out,
                           const vector<string>& header,
                           char delimiter,
                           bool compute_by_rows)
{
    string file_line; 
    for (int i = 0; i < header.size(); ++i) {
        file_line += header[i] + delimiter;  // Print back the old headings
    }
    
    if (compute_by_rows) {
        for (int i = 0; i < stat_names.size(); ++i) {
            file_line += stat_names[i];  // ...and the new stat names
            if (i != stat_names.size() - 1)
                file_line += delimiter;
        }
    } else {
        // Erase the last 'delimiter'
        file_line.erase(file_line.size() - 1);
    }
    out->WriteLine(file_line);
    return 0;
}

static void AppendRight_(vector<vector<double>>& dest_matrix,
                         const vector<vector<double>>& m_to_append)
{
    const int64_t rows_count = dest_matrix.size();

    for (int row = 0; row < rows_count; ++row) {
        const auto& append_row = m_to_append[row];
        for (double val : append_row) {
            dest_matrix[row].push_back(val);
        }
    }
}

static void AppendBottom_(vector<vector<double>>& dest_matrix,
                          const vector<vector<double>>& m_to_append)
{
    for (int row_stat = 0; row_stat < stat_names.size(); ++row_stat) {
        vector<double> stat_row;
        stat_row.reserve(m_to_append.size());
        for (const auto& append_column : m_to_append) {
            stat_row.push_back(append_column[row_stat]);
        }
        dest_matrix.emplace_back(std::move(stat_row));
    }
}

static int ParseArguments_(int argc,
                           char *argv[],
                           bool& compute_by_rows,
                           bool& compute_by_columns,
                           string& input_path,
                           string& output_path)
{
    if (argc < 2) {
        ShowHelp();
        return 1;
    }
    compute_by_rows = false;
    compute_by_columns = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg[0] == '-') {
            if (arg == "-by-rows")
                compute_by_rows = true;
            else if (arg == "-by-columns")
                compute_by_columns = true;
            else if (arg == "-h" || arg == "-help" || arg == "--help") {
                ShowHelp();
                exit(0);
            } else {
                std::cerr << "error: unrecognized option '" << arg << "'\n";
                return 1;
            }
            continue;
        }
        if (input_path.empty()) {
            // First, we fill the input path
            input_path = arg;
        } else if (output_path.empty()){
            output_path = arg;
        } else {
            std::cerr << "error: too many arguments provided\n";
            return 1;
        }
    }
    if (!compute_by_rows && !compute_by_columns) {
        // Neither is provided. Apply default
        compute_by_rows = true;
    }
    if (input_path.empty() && output_path.empty()) {
        std::cerr << "error: no input file provied\n";
        return 1;
    }

    if (output_path.empty()) {
        // No special output name provided. Create a default one
        output_path = input_path; 
        int64_t dot_pos = output_path.rfind('.');
        if (dot_pos != std::string::npos) {
            output_path.insert(dot_pos, "-diversity");
        } else {
            std::cerr << "error: unrecognized input format\n";
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    string table_file_path, output_file_path;
    bool compute_by_rows, compute_by_columns;
    
    if (ParseArguments_(argc, argv,
                        compute_by_rows,
                        compute_by_columns,
                        table_file_path, output_file_path) != 0) {
        return 1;
    }
    std::unique_ptr<StringInputStream> input = std::make_unique<PlainStringInputStream>(table_file_path);
    if (!input) {
        std::cerr << "error: unable to open input file\n";
    }
    char delimiter = GetDelimiterForFileName_(table_file_path);

    vector<string> header;
    if (ReadHeader_(input, delimiter, header) != 0) {
        std::cerr << "error: malformed table header\n";
        return 1;
    }

    vector<vector<double>> data_matrix;
    vector<string> id_column;
    if (PopulateDataMatrix_(input, delimiter, id_column, data_matrix)) {
        std::cerr << "error: couldn't read data matrix\n";
        return 1; 
    }
    input = nullptr;  // Close the file and free all its resources

    const int64_t columns_count = data_matrix[0].size();
    const int64_t rows_count = data_matrix.size();
    if (columns_count == 0 || rows_count == 0) {
        std::cout << "Input table is empty. Exiting\n";
        return 0;
    }

    vector<vector<double>> rowwise_stats;
    if (compute_by_rows) {
        rowwise_stats = CalculateRowwiseStats_(data_matrix);
        AppendRight_(data_matrix, rowwise_stats);
    }

    vector<vector<double>> columnwise_stats;
    if (compute_by_columns) {
        columnwise_stats = CalculateColumnwiseStats_(data_matrix,
                                                     compute_by_rows);
        AppendBottom_(data_matrix, columnwise_stats);
    }

    // Re-assemble the .tsvcr output
    auto out = std::make_unique<StringOutputStream>(output_file_path);
    if (!out) {
        std::cerr << "error: unable to write into " << output_file_path << '\n';
        return 1;
    }
    PrintNewHeader_(out, header, delimiter, compute_by_rows);

    int stat_count = 0;
    const int64_t data_width = data_matrix[0].size(); // This is needed to determine when to fill the short last lines with 'delimiter'
    for (int row_i = 0; row_i < data_matrix.size(); ++row_i) {
        string file_line;
        const auto& row = data_matrix[row_i];
        if (row.size() == data_width && row_i < id_column.size()) {
            file_line += id_column[row_i] + delimiter;
        } else {
            file_line += stat_names[stat_count++] + delimiter;
        }
        for (int column_i = 0; column_i < row.size(); ++column_i) {
            string num_string = std::to_string(row[column_i]);
            num_string.erase(num_string.find_last_not_of('0') + 1, std::string::npos);
            if (num_string[num_string.size() - 1] == '.') {
                num_string.erase(num_string.end() - 1);
            }
            file_line += num_string;
            if (column_i != row.size() - 1)
                file_line += delimiter;
        }

        if (row.size() != data_width) {
            // This is the last short line. Need to fill the rest with remaining delimiters
            for (int i = 0; i < data_width - row.size(); ++i)
                file_line += delimiter;
        }
        out->WriteLine(file_line);
    }
    return 0;
}

