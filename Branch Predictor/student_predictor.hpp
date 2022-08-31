#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

class your_own {
 private:
  int num_predictor_entry;
  int c_bit;
  int *pred_arr;
  // ADDED
  int reg_size;
  int table_size;
  int mask;
  int* BHR;	// Branch history register
  int* gPHT; // Global pattern history table
  int* mPHT; // Meta pattern history table

 public:
  your_own();
  your_own(int, int);
  ~your_own();
  int get_pred(int);
  int getGlobalPred(int);
  int getMetaPred(int);
  void update(int, int);
};
