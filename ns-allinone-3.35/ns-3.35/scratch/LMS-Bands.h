#ifndef LMS_BANDS_H
#define LMS_BANDS_H

/**
 This header contains the features of each band. It is included in p2p_scenarioReusable.cc.
 The band in UP-LINK and DOWN-LINK is assumed to be the same.
 **/

#include <string>
using namespace std;
using ProbMat_t = std::vector<std::vector<double>>;
using array_t = std::vector<double>;
double pktLen = 100;
typedef struct band_features
{
  string type;
  array_t mean_time;
  ProbMat_t probMat;
  array_t capFactor;
  double muLos;
} band_features_t;

map<string, band_features_t> Bands = {
    {"S", {"Distribution", {1 / 1.823, 1 / 2.003, 1 / 2.833}, {{0, 0.94076, 0.059243}, {0.77084, 0, 0.22916}, {0.49418, 0.50582, 0}}, {1, 0.5, 0.2}, 5000}},
    {"S1", {"Distribution", {1 / 1.823, 1 / 2.003, 1 / 2.833}, {{0, 0.94076, 0.059243}, {0.77084, 0, 0.22916}, {0.49418, 0.50582, 0}}, {1, 0.5, 0.2}, 13000}},
    {"Ka", {"Distribution", {1 / 3.952, 1 / 1.37, 1 / 5.999}, {{0, 0.93156, 0.068437}, {0.34526, 0, 0.65474}, {0.070012, 0.92999, 0}}, {1, 0.5, 0.2}, 100000}},
    {"Ka_truqui_5sec", {"FixedTime", {5}, {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}}, {1, 0.5, 0.2}, 100000}}, //{80,40,16}Mbps
    {"I", {"Distribution", {5, 0}, {{0, 1}, {1, 0}}, {1, 0}, 100000}},
    {"II", {"FixedTime", {5, 0.5}, {{0.75, 0.25}, {0.75, 0.25}}, {1, 0}, 100000}}};

#endif