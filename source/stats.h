//
//  stats.h
//  diversity
//
//  Created by Denis Musatov on 02/11/16.
//  Copyright 2018 Frangou Lab.
//

#include <vector>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <map>
#include <iostream>

/*
 * double SimpsonIndex(const vector<double>&);
 * double DominanceIndex(const vector<double>&);
 * double ReciprocalSimpsonIndex(const vector<double>&);
 * double ShannonIndexE(const vector<double>&);
 * double ShannonIndex2(const vector<double>&);
 * double ShannonIndex10(const vector<double>&);
 * double MenhinickIndex(const vector<double>&);
 * double Buzas_GibsonsIndex(const vector<double>&);
 * double EquitabilityIndex(const vector<double>&);
 * double SimpsonIndexApproximation(const vector<double>&);
 * double DominanceIndexApproximation(const vector<double>&);
 * double AlternateReciprocalSimpsonIndex(const vector<double>&);
 * double Berger_ParkerDominanceIndex(const vector<double>&);
 * double MargalefRichnessIndex(const vector<double>&);
 * double RenyiEntropy(const vector<double>&, double alpha);  // not implemented
 * double GiniCoefficient(const vector<double>&);
 * */

std::map<double, int> FrequencyTable_(const std::vector<double>& a)
{
    std::map<double, int> count;

    for (auto test : a) {
        if (count.find(test) != count.cend())
            count[test]++;
        else
            count[test] = 1;
    }
    return count;
}

double SimpsonIndex(const std::vector<double>& a)
{
    auto table = FrequencyTable_(a);
    double N = a.size();
    double index = 0.0;
    
    for (const auto& pair : table) {
         index += pair.second*(pair.second - 1);
    }
    return index/(N*(N - 1.0));
}

double DominanceIndex(const std::vector<double>& a)
{
    return 1.0 - SimpsonIndex(a);
}

double ReciprocalSimpsonIndex(const std::vector<double>& a)
{
    return 1.0/SimpsonIndex(a);
}

template <typename LogFunc_>
double ShannonIndexGeneric(const std::vector<double>& a, LogFunc_ log_)
{
    auto table = FrequencyTable_(a);
    double N = a.size();
    double index = 0.0;

    for (const auto& pair : table) {
        double count_over_N = pair.second/N;
        index += count_over_N*log_(count_over_N);
    }
    return -index;
}

double ShannonIndexE(const std::vector<double>& a)
{
    auto ln = [](double n) {return std::log(n);};
    return ShannonIndexGeneric(a, ln);
}

double ShannonIndex2(const std::vector<double>& a)
{
    auto log2 = [](double n){return std::log2(n);};
    return ShannonIndexGeneric(a, log2);
}

double ShannonIndex10(const std::vector<double>& a)
{
    auto log10 = [](double n){return std::log10(n);};
    return ShannonIndexGeneric(a, log10);
}

double MenhinickIndex(const std::vector<double>& a)
{
    double S = FrequencyTable_(a).size();
    double N = a.size();
    return S/std::sqrt(N);
}

double Buzas_GibsonsIndex(const std::vector<double>& a)
{
    double S = FrequencyTable_(a).size();
    double shannon_E = ShannonIndexE(a);
    return std::exp(shannon_E)/S;
}

double EquitabilityIndex(const std::vector<double>& a)
{
    return ShannonIndexE(a)/std::log(a.size());
}

double SimpsonIndexApproximation(const std::vector<double>& a)
{
    double N = a.size();
    auto table = FrequencyTable_(a);
    double sum_species = 0.0;
    for (const auto& pair : table)
        sum_species += pair.second*pair.second;

    return sum_species/(N*N);
}

double DominanceIndexApproximation(const std::vector<double>& a)
{
    return 1.0 - SimpsonIndexApproximation(a);
}

double AlternateReciprocalSimpsonIndex(const std::vector<double>& a)
{
    return 1.0/SimpsonIndexApproximation(a);
}

double Berger_ParkerDominanceIndex(const std::vector<double>& a)
{
    auto table = FrequencyTable_(a);
    double N = a.size();
    return std::max_element(table.cbegin(), table.cend(), [](auto&& a, auto&& b){
            return a.second < b.second;
            })->second/N;
}

double InvertedBerger_ParkerDominanceIndex(const std::vector<double>& a)
{
    return 1.0/Berger_ParkerDominanceIndex(a);
}

double MargalefRichnessIndex(const std::vector<double>& a)
{
    double S = FrequencyTable_(a).size();
    double N = a.size();
    return (S - 1)/std::log(N);
}

double RenyiEntropy0(const std::vector<double>& a)
{
    double S = FrequencyTable_(a).size();
    return std::log(S);
}

/* Renyi Entropy with alpha=1 is Shannon Entropy. So skip it */

double RenyiEntropy2(const std::vector<double>& a)
{
    auto table = FrequencyTable_(a);
    double N = a.size();
    double sum_sqr_prob = 0.0;
    for (const auto& pair : table) {
        sum_sqr_prob += (pair.second/N)*(pair.second/N);
    }
    return -std::log(sum_sqr_prob);
}

double RenyiEntropyINF(const std::vector<double>& a)
{
    double max_prob = 0.0;
    double N = a.size();
    auto table = FrequencyTable_(a);
    for (const auto& pair : table) {
        max_prob = std::max(max_prob, pair.second/N);
    }
    return -std::log(max_prob);
}

double GiniCoefficient(const std::vector<double>& a)
{
    double N = a.size();
    double min_elem;
    auto a_copy = a;
    if ((min_elem = *std::min_element(a_copy.cbegin(), a_copy.cend())) < 0) {
        // All values must be positive
        std::for_each(a_copy.begin(), a_copy.end(), [min_elem](double& n){n -= min_elem;});
    }
    std::sort(a_copy.begin(), a_copy.end());
    std::for_each(a_copy.begin(), a_copy.end(), [](double& n){n += nextafter(0, 1);});
    double nominator = 0.0, denominator = 0.0;
    for (int i = 1; i <= a_copy.size(); ++i) {
        nominator += i*a_copy[i - 1];
        denominator += a_copy[i - 1];
    }
    nominator *= 2;
    denominator *= N;
    return nominator/denominator - (N + 1)/N;
}

