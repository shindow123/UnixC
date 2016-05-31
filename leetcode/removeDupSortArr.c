//26. Remove Duplicates from Sorted Array
#include <stdio.h>


int removeDuplicates(int* nums, int numsSize) {
  if (numsSize < 1) return 0;
  int cur_idx = 0;
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
  return cur_idx+1;
}

int main() {
  int a[] = {1,2,3,44,55,66,77,88,88,88,90,93,95,95,97,97,100};
  int size = removeDuplicates(a, sizeof(a)/4 + 1);
  int i = 0;
  for (; i < size; i++) {
	printf("%d\t", a[i]);
  }
  return 0;
}
