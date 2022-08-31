#include "student_predictor.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
your_own::your_own() {
	int idx_bit = 14;	// changed idx_bit to 14
	num_predictor_entry = pow(2, idx_bit);
	c_bit = 2;
	reg_size = 14; // BHR size
	table_size = pow(2, reg_size); // gPHT, mPHT size
	mask = table_size - 1;
	pred_arr = new int[num_predictor_entry];
	BHR = new int[reg_size];
	gPHT = new int[table_size];
	mPHT = new int[table_size];
	// Initialize
	for (int i = 0; i < num_predictor_entry; i++) {
		pred_arr[i] = 2;
	}
	for (int i = 0; i < reg_size; i++) {
		BHR[i] = 0;
	}
	for (int i = 0; i < table_size; i++) {
		gPHT[i] = 1;
		mPHT[i] = 1;
	}
};

your_own::~your_own() {
	if (pred_arr != NULL) delete[] pred_arr;
	if (BHR != NULL) delete[] BHR;
	if (gPHT != NULL) delete[] gPHT;
	if (mPHT != NULL) delete[] mPHT;
}

/* warning!!! Don't change argument of these function   */
int your_own::get_pred(int pc) {
	int idx = pc % num_predictor_entry;
	int prediction = pred_arr[idx] / pow(2, c_bit - 1);
	if (getMetaPred(pc) == 1) {
		return getGlobalPred(pc);
	}
	else {
		return prediction;
	}
}

int your_own::getGlobalPred(int pc) {
	int idx1 = pc & mask;
	int idx2 = 0;
	for (int i = 0; i < reg_size; i++) {
		idx2 += pow(2, i) * BHR[i];
	}
	int idx3 = idx1 ^ idx2;
	return gPHT[idx3] / 2;
}

int your_own::getMetaPred(int pc) {
	int idx1 = pc & mask;
	int idx2 = 0;
	for (int i = 0; i < reg_size; i++) {
		idx2 += pow(2, i) * BHR[i];
	}
	int idx3 = idx1 ^ idx2;
	return mPHT[idx3] / 2;
}

void your_own::update(int pc, int res) {
	int idx = pc % num_predictor_entry;
	int *arr = pred_arr;
	int bit = c_bit;
	int pred = pred_arr[idx] / pow(2, c_bit - 1);
  
	int idx1 = pc & mask;
	int idx2 = 0;
	for (int i = 0; i < reg_size; i++) {
		idx2 += pow(2, i) * BHR[i];
	}
	int idx3 = idx1 ^ idx2;
	idx3 = idx3 & mask;
  
	int gRes = (getGlobalPred(pc) == res) ? 1 : 0;
	int pRes = (pred == res) ? 1 : 0;
	int foo[2] = { gRes, pRes };

	if (res == 1) {
		if (arr[idx] == 3) {
			arr[idx] = 3;
		}
		else {
			arr[idx] += 1;
		}
	}
	else {
		if (arr[idx] == 0) {
			arr[idx] = 0;
		}
		else {
			arr[idx] -= 1;
		}
	}

	if (res == 1) {
		if (gPHT[idx3] == 3) {
			gPHT[idx3] = 3;
		}
		else {
			gPHT[idx3] += 1;
		}
	}
	else {
		if (gPHT[idx3] == 0) {
			gPHT[idx3] = 0;
		}
		else {
			gPHT[idx3] -= 1;
		}
	}

	if (foo[0] == 0 && foo[1] == 1) {
		if (mPHT[idx3] != 0) {
			mPHT[idx3] -= 1;
		}
	}
	else if (foo[0] == 1 && foo[1] == 0) {
		if (mPHT[idx3] != 3) {
			mPHT[idx3] += 1;
		}
	}

	for (int i = reg_size - 1; i > 0; i--) {
		BHR[i] = BHR[i - 1];
	}
	BHR[0] = res;
}
