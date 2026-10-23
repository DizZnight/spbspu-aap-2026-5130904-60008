#include <iostream>
#include <fstream>
#include <string>

namespace malyshev {
	const int The_number_of_command_line_arguments_does_not_match_the_specification_or_the_first_parameter_cannot_be_interpreted_as_the_indicated_job_numbers = 1;
	const int cannot_be_interpreted_as_a_two_dimensional_array_or_the_file_is_empty = 2;
	const int max_size = 10000;
	const std::string elements_more_10000 = "elements more 10000";

	void process_fix_massive(std::ifstream& ifile, int* matrix, int& rows, int& cols);
	void process_dynamic_massive(std::ifstream& ifile, int*& matrix, int& rows, int& cols);
	int long_ser_cols(int* matrix, int rows, int cols);
}

int main(int argc, char* argv[])
{
	if (argc != 4) {
		std::cerr << "The number of command-line arguments does not match the specification, or the first parameter cannot be interpreted as the indicated job numbers";
		return malyshev::The_number_of_command_line_arguments_does_not_match_the_specification_or_the_first_parameter_cannot_be_interpreted_as_the_indicated_job_numbers;
	}

	std::string num = argv[1];
	std::string input_filename = argv[2];
	std::string output_filename = argv[3];


	int rows = 0;
	int cols = 0;
	int res = 0;
	int static_matrix[10000];
	int* dynamic_matrix = nullptr;

	std::ifstream file(input_filename);
	
	if (file.fail())
	{
		std::cerr << "cannot be interpreted as a two-dimensional array, or the file is empty";
		return malyshev::cannot_be_interpreted_as_a_two_dimensional_array_or_the_file_is_empty;
	}

	if (num == "1")
	{
		malyshev::process_fix_massive(file, static_matrix, rows, cols);
	}

	else if (num == "2")
	{
		malyshev::process_dynamic_massive(file, dynamic_matrix, rows, cols);
	}

	else
	{
		std::cerr << "The number of command-line arguments does not match the specification, or the first parameter cannot be interpreted as the indicated job numbers";
		file.close();
		return malyshev::The_number_of_command_line_arguments_does_not_match_the_specification_or_the_first_parameter_cannot_be_interpreted_as_the_indicated_job_numbers;
	}

	file.close();

	if (num == "1")
	{
		res = malyshev::long_ser_cols(static_matrix, rows, cols);
	}
	else if (num == "2")
	{
		res = malyshev::long_ser_cols(dynamic_matrix, rows, cols);
	}

	std::ofstream outfile(output_filename);
	if (outfile.fail())
	{
		std::cerr << "The file was not saved";
		if (num == "2") {
			delete[] dynamic_matrix;
		}
		return malyshev::The_number_of_command_line_arguments_does_not_match_the_specification_or_the_first_parameter_cannot_be_interpreted_as_the_indicated_job_numbers;
	}

	outfile << res << "\n";
	outfile.close();

	if (num == "2") {
		delete[] dynamic_matrix;
	}
	return 0;
}

namespace malyshev{
	void process_fix_massive(std::ifstream& ifile, int* matrix, int& rows, int& cols)
	{
		ifile >> rows >> cols;
		if (rows * cols > max_size)
		{
			std::cerr << "elements more 10000";
			return;
		}
		int total_el = rows * cols;
		for (size_t i = 0; i < total_el;i++)
		{
			ifile >> matrix[i];
		}
	}

	void process_dynamic_massive(std::ifstream& ifile, int*& matrix, int& rows, int& cols)
	{
		ifile >> rows >> cols;
		matrix = new int[rows * cols];
		int total_el = rows * cols;
		for (int i = 0; i < total_el; i++)
		{
			ifile >> matrix[i];
		}
	}

	int long_ser_cols(int* matrix, int rows, int cols)
	{
		if (rows <= 0 || cols <= 0)
		{
			return 0;
		}

		int max_ser_len = 0;
		int max_cols = 0;

		for (size_t i = 0; i < cols; i++)
		{
			int current_sers = 1;
			int max_in_col = 1;

			for (size_t j = 1; j < rows; j++)
			{
				if (matrix[j * cols + i] == matrix[(j - 1) * cols + i])
				{
					current_sers++;
				}
				else
				{
					current_sers = 1;
				}
				if (current_sers > max_in_col)
				{
					max_in_col = current_sers;
				}
			}
			if (max_in_col > max_ser_len)
			{
				max_ser_len = max_in_col;
				max_cols = i + 1;
			}
		}
		return max_cols;
	}
}
