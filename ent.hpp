// MIT No Attribution
// 
// ent.hpp - A header-only class, that applies various tests to sequences of bytes 
// stored in files and reports the results of those tests. The class is useful 
// for evaluating pseudorandom number generators for encryption and statistical 
// sampling applications, compression algorithms, and other applications where 
// the information density of a file is of interest.
// Version 0.1 beta (23:rd of June 2023).
// Copyright (c) 2023 Håkan Blomqvist
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"), to deal in the Software
// without restriction, including without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>
#include <cctype> // for std::tolower

#define BYTE_VAL_COUNT 256

namespace Ent {

class Ent {
private:
    std::vector<unsigned char> data;
    double entropy;
    double compression;
    double chisquare;
    double p_value;
    double mean;
    double pi_estimate;
    double serial_correlation;
    bool streamOfBitsMode;
    bool printTableMode;
    bool foldCaseMode;
    bool terseMode;
    bool printResultMode;

    std::vector<unsigned char> load_file_data(const std::string &path) {
        std::ifstream file(path, std::ios::binary);
        return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }

    void print_result() {
        std::string samp = streamOfBitsMode ? "bit" : "byte";
        std::cout << "Entropy = " + std::to_string(entropy) + " bits per " + samp + ".\n\n";
        std::cout << "Optimum compression would reduce the size\nof this " + std::to_string(int(data.size()*(streamOfBitsMode ? 8.0 : 1.0))) + " " + samp + " file by " + std::to_string((int) ((100 * ((streamOfBitsMode ? 1 : 8) - entropy) / (streamOfBitsMode ? 1.0 : 8.0)))) + " percent.\n\n";
        std::cout << "Chi square distribution for " + std::to_string(int(data.size()*(streamOfBitsMode ? 8.0 : 1.0))) + " samples is " + std::to_string(chisquare) + ", and randomly\n";
        if (p_value < 0.0001) {
            std::cout << "would exceed this value less than 0.01 percent of the times.\n\n";
        } else if (p_value > 0.9999) {
            std::cout << "would exceed this value more than than 99.99 percent of the times.\n\n";
        } else {
            std::cout << "would exceed this value " + std::to_string(p_value * 100) + " percent of the times.\n\n";
        }
        std::cout << "Arithmetic mean value of data bytes is " + std::to_string(mean) + " (" + std::to_string(streamOfBitsMode ? 0.5 : 127.5) + " = random).\n";
        std::cout << "Monte Carlo value for Pi is " + std::to_string(pi_estimate) + " (error " + std::to_string(std::fabs(pi_estimate - M_PI) / M_PI * 100.0) + " percent).\n";
        std::cout << "Serial correlation coefficient is ";
        if (serial_correlation >= -99999) {
            std::cout << std::to_string(serial_correlation) + " (totally uncorrelated = 0.0).\n";
        } else {
            std::cout << "undefined (all values equal!).\n";
        }
    }

    void print_table() {
        if (streamOfBitsMode) {
            std::vector<int> bitOccurrences(2, 0);
    
            // Iterate over each byte and then each bit within that byte
            for (auto &byte : data) {
                for(int bit=0; bit<8; bit++) {
                    int bitValue = (byte >> bit) & 1;
                    bitOccurrences[bitValue]++;
                }
            }
    
            // Print bit occurrences and fraction
            for (int bitValue = 0; bitValue <= 1; ++bitValue) {
                double fraction = bitOccurrences[bitValue] / static_cast<double>(data.size() * 8);
                std::cout << "Value: " << bitValue << " Occurrences: " << bitOccurrences[bitValue] << " Fraction: " << fraction << "\n";
            }
        } else {
            std::vector<int> byteOccurrences(256, 0);
    
            // Iterate over each byte
            for (auto &byte : data) {
                byteOccurrences[byte]++;
            }
    
            // Print byte occurrences and fraction
            for (int byteValue = 0; byteValue < 256; ++byteValue) {
                double fraction = byteOccurrences[byteValue] / static_cast<double>(data.size());
                std::cout << "Value: " << byteValue << " Char: " << char(isprint(byteValue) ?  byteValue : ' ') << " Occurrences: " << byteOccurrences[byteValue] << " Fraction: " << fraction << "\n";
            }
        }

        std::cout << "\nTotal: " << data.size() << " 1.0\n\n";
    }

    void print_result_terse() {
        std::string samp = streamOfBitsMode ? "bit" : "byte";
        int totalc = streamOfBitsMode ? (data.size()*8) : (data.size());
        std::cout << "0,File-"+ samp +"s,Entropy,Chi-square,Mean,Monte-Carlo-Pi,Serial-Correlation\n1,";
        std::cout << totalc << "," << entropy << "," << chisquare << "," << mean << "," << pi_estimate << "," << serial_correlation << "\n";
    }

    void print_table_terse() {
        std::cout << "2,Value,Occurrences,Fraction\n";
        if (streamOfBitsMode) {
            std::vector<int> bitOccurrences(2, 0);
    
            // Iterate over each byte and then each bit within that byte
            for (auto &byte : data) {
                for(int bit=0; bit<8; bit++) {
                    int bitValue = (byte >> bit) & 1;
                    bitOccurrences[bitValue]++;
                }
            }
            for(int i=0; i<2; ++i) {
                std::cout << "3," << i << "," << bitOccurrences[i] << "," << (bitOccurrences[i] / static_cast<double>(data.size()*8)) << "\n";
            }
        } else {
            std::vector<int> byteOccurrences(256, 0);
    
            // Iterate over each byte
            for (auto &byte : data) {
                byteOccurrences[byte]++;
            }
            for(int i=0; i<256; ++i) {
                std::cout << "3," << i << "," << byteOccurrences[i] << "," << (byteOccurrences[i] / static_cast<double>(data.size())) << "\n";
            }
        }
    }

    // Numerical approximation of the CDF for a standard normal distribution
    double norm_cdf(double x) {
        return 0.5 * erfc(-x * M_SQRT1_2);
    }

    void calculate_entropy() {
        if (streamOfBitsMode) {
            std::vector<double> frequencies(2, 0);  // Frequencies for 2 possible bit values: 0 and 1
    
            for (auto &byte : data) {
                for(int bit=0; bit<8; bit++) {
                    int bitValue = (byte >> bit) & 1;
                    frequencies[bitValue]++;
                }
            }
    
            entropy = 0.0;
            double totalBits = 8.0 * data.size();
            for (auto &frequency : frequencies) {
                frequency /= totalBits;
                if (frequency > 0) {
                    entropy -= frequency * (std::log2(frequency));
                }
            }
    
            // Calculate the optimal compression percentage
            double max_entropy = 1.0;  // Max entropy in bits per bit
            compression = 100.0 * (1.0 - entropy / max_entropy);
        } else {
            std::vector<double> frequencies(BYTE_VAL_COUNT, 0);
    
            for (auto &byte : data) {
                frequencies[byte]++;
            }
    
            entropy = 0.0;
            for (auto &frequency : frequencies) {
                frequency /= data.size();
                if (frequency > 0) {
                    entropy -= frequency * (std::log2(frequency));
                }
            }
    
            // Calculate the optimal compression percentage
            double max_entropy = 8.0;  // Max entropy in bits per byte
            compression = 100.0 * (1.0 - entropy / max_entropy);
        }
    }


    void calculate_chisquare() {
        if (streamOfBitsMode) {
            double expected = 8.0 * data.size() / 2.0;  // For bits, only two possibilities 0 and 1
            std::vector<int> observed(2, 0);  // For bits
    
            for (auto &byte : data) {
                for(int bit=0; bit<8; bit++) {
                    int bitValue = (byte >> bit) & 1;
                    observed[bitValue]++;
                }
            }
    
            chisquare = 0.0;
            for (int i = 0; i < 2; ++i) {
                double diff = observed[i] - expected;
                chisquare += diff * diff / expected;
            }
    
            int degree_of_freedom = 1;  // For bits
    
            // Transform Chi-square to z-value
            double z = std::sqrt(chisquare - degree_of_freedom);
    
            // Calculate p-value from standard normal distribution
            p_value = 1 - norm_cdf(z);
        } else {
            double expected = data.size() / static_cast<double>(BYTE_VAL_COUNT);
            std::vector<int> observed(BYTE_VAL_COUNT, 0);
    
            for (auto &byte : data) {
                observed[byte]++;
            }
    
            chisquare = 0.0;
            for (int i = 0; i < BYTE_VAL_COUNT; ++i) {
                double diff = observed[i] - expected;
                chisquare += diff * diff / expected;
            }
    
            int degree_of_freedom = BYTE_VAL_COUNT - 1;
    
            // Transform Chi-square to z-value
            double z = std::sqrt(chisquare - degree_of_freedom);
    
            // Calculate p-value from standard normal distribution
            p_value = 1 - norm_cdf(z);
        }
    }


    void calculate_mean() {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        mean = sum / data.size();
    }

    void calculate_pi() {
        unsigned long long radiusSquared = static_cast<unsigned long long>(1) << 48;

        unsigned long hits = 0;
        unsigned long total = 0;

        for (size_t i = 0; i < data.size() - 5; i += 6) {
            unsigned long long x = static_cast<unsigned long long>(data[i]) << 16 | static_cast<unsigned long long>(data[i+1]) << 8 | data[i+2];
            unsigned long long y = static_cast<unsigned long long>(data[i+3]) << 16 | static_cast<unsigned long long>(data[i+4]) << 8 | data[i+5];

            unsigned long long distanceSquared = x * x + y * y;

            if (distanceSquared < radiusSquared) {
                hits++;
            }
            total++;
        }

        pi_estimate = 4.0 * hits / total;
    }

    void calculate_serial_correlation() {
        unsigned long long sumX = 0;
        unsigned long long sumY = 0;
        unsigned long long sumXY = 0;
        unsigned long long sumX2 = 0;
        unsigned long long sumY2 = 0;

        for (size_t i = 1; i < data.size(); ++i) {
            unsigned long long X = data[i - 1];
            unsigned long long Y = data[i];

            sumX += X;
            sumY += Y;
            sumXY += X * Y;
            sumX2 += X * X;
            sumY2 += Y * Y;
        }

        double n = data.size() - 1;
        serial_correlation = (n * sumXY - sumX * sumY) / std::sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));
    }
public:
    Ent(const std::string &filePath) : streamOfBitsMode(false), printTableMode(false), foldCaseMode(false), terseMode(false), printResultMode(true) {
        data = load_file_data(filePath);
        entropy = 0.0;
        compression = 0.0;
        chisquare = 0.0;
        p_value = 0.0;
        mean = 0.0;
        pi_estimate = 0.0;
        serial_correlation = 0.0;
    }

    Ent() : streamOfBitsMode(false), printTableMode(false), foldCaseMode(false), terseMode(false), printResultMode(true) {
        entropy = 0.0;
        compression = 0.0;
        chisquare = 0.0;
        p_value = 0.0;
        mean = 0.0;
        pi_estimate = 0.0;
        serial_correlation = 0.0;
        std::istreambuf_iterator<char> start(std::cin), end;
        data = {start, end};
    }

    void calculate() {
        if (foldCaseMode) {
            for(std::vector<unsigned char>::size_type i = 0; i != data.size(); ++i) {
                if (std::isalpha(data[i]) && std::isupper(data[i])) {
                    data[i] = std::tolower(data[i]);
                }
            }
        }
        calculate_entropy();
        calculate_chisquare();
        calculate_mean();
        calculate_pi();
        calculate_serial_correlation();
        if (terseMode) {
            if (printResultMode && terseMode) {
                print_result_terse();
            }
            if (printResultMode && printTableMode) {
                print_table_terse();
            }
        } else {
            if (printResultMode && printTableMode) {
                print_table();
            }
            if (printResultMode) {
                print_result();
            }
        }
    }

    void setStreamOfBitsMode(bool mode) {
        streamOfBitsMode = mode;
    }

    void setPrintTableMode(bool mode) {
        printTableMode = mode;
    }

    void setFoldCaseMode(bool mode) {
        foldCaseMode = mode;
    }

    void setTerseMode(bool mode) {
        terseMode = mode;
    }

    void setPrintResultMode(bool mode) {
        printResultMode = mode;
    }

    double get_entropy() {
        return entropy;
    }
    double get_compression() {
        return compression;
    }
    double get_chisquare() {
        return chisquare;
    }
    double get_p_value() {
        return p_value;
    }
    double get_mean() {
        return mean;
    }
    double get_pi_estimate() {
        return pi_estimate;
    }
    double get_serial_correlation() {
        return serial_correlation;
    }
};

} // namespace Ent
