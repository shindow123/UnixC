#include <stdio.h>

int removeDuplicates(int* nums, int numsSize) {
  if (numsSize <= 2) return numsSize;
  int i = 0;
  int j = 2;
  for (; j < numsSize; j++) {
	if (nums[i] != nums[j]) {
	  nums[i+2] = nums[j];
	  i++;
	}
  }

  return i+2;

/*
  if (numsSize < 2) return 0;
  int cur_idx = 0;
  int i = 0;
  int j = 2;
  for (; j < numsSize; j++, i++) {
	if (nums[i] != nums[j]) {
		nums[i+1] = nums[j];
	}
  }
*/
  /*
  int last_val = nums[0];
  int i = 1;
  for (; i < numsSize; i++) {
	if (nums[i] != nums[cur_idx]) {
	  if (i == cur_idx+1) {
		cur_idx++;
		continue;
	  }
	  nums[++cur_idx] = nums[i];
	}
  }
  */
  return i;
}

int main() {
		   //1,2,2,3,3,44,55,66,77,88,88,88,90,93,95,95,97,97,100,192
  int a[] = {1,2,2,2,3,3,44,55,66,77,88,88,88,90,93,95,95,97,97,100};
  //int a[] = {1,1,1,2,2,3};
  int size = removeDuplicates(a, sizeof(a)/4 + 1);
  int i = 0;
  for (; i < size; i++) {
	printf("%d,", a[i]);
  }
  return 0;
}
