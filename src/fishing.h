#include <Windows.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <format>
#include <fstream>
#include <random>
#include <iostream>

#include "fishnet/nanodet.h"
#include "rodnet/rodnet.h"
#include "utils/screenshot.h"

// music notes! qwq
#define A3 220
#define C4 262
#define D4 293
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494
#define C5 523

class Fisher {
 private:
  const int processShape[2] = {1024, 576};
  bool processWithInputShape;
  const std::string logPath = "log";

  const double steps[40][2] = {
      {0, 2},    {0, 2},    {0, 2},    {0, 2},    {0, -1},   {0, -1},
      {0, -1},   {0, -1},   {1, 0},    {1, 0},    {1, 0},    {1, 0},
      {1, 0},    {1, 0},    {1, 0},    {1, 0},    {1, 0},    {1, 0},
      {0, -1},   {1, 0},    {1, 0},    {1, 0},    {1, 0},    {1, 0},
      {1, 0},    {1, 0},    {1, 0},    {1, 0},    {1, 0},    {0, -1},
      {0.75, 0}, {0.75, 0}, {0.75, 0}, {0.75, 0}, {0.75, 0}, {0.75, 0},
      {0.75, 0}, {0.75, 0}, {0.75, 0}, {0.75, 0}};
  const int MaxRodTries = 25;
  const int MaxFishFailNum = 5;  // at most 5 times miss fish can be tolerated
  // unit: second
  const double MaxThrowWaiting = 3;
  const double MaxBiteWaiting[8] = {6, 8.5,  9.5, 10.5,
                                    7, 10.5, 10,  10};  // index is fish label
  const double MaxControlWaiting = 4;

  cv::Mat hookImg, pullImg, centralBarImg, leftEdgeImg, cursorImg, rightEdgeImg;

  NanoDet *fishNet;
  //   RodNet *rodNet;
  Screen *screen;
  double ratio;            // the ratio between user's screen and process layer
  double fishnetRatio[2];  // the ratio between process and fishnet's input size
  cv::Mat screenImage;
  std::vector<BoxInfo> bboxes;
  BoxInfo targetFish;
  BoxInfo rod;

  void checkWorking();
  void getBBoxes(bool cover);

  bool scanFish();
  void selectFish();
  void chooseBait();
  void throwRod();
  void checkBite();
  void control();
  int getRodState(BoxInfo rod, BoxInfo fish);

  void imgLog(char name[], bool bbox);
  void errLog();

  int imgid = 0;
  int id = 0;

 public:
  Fisher(NanoDet *fishnet, Screen *screen, std::string imgPath);
  ~Fisher();

  bool working;
  bool logAllImgs = false;
  bool logData = false;
  bool testing = true;
  void fishing();
  void test();
};