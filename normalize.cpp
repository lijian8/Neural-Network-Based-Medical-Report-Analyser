/*

Author : Samarth Khare
Created : March 2015

*/
#include <bits/stdc++.h>

double a[140][10];

int main()
{
	int n = 163;
	int i;
	int j, id;
	freopen("testing.txt", "r", stdin);
	for (i = 0; i < n; i++) {
		scanf("%d", &id);
		//printf("%d\n", id);
		for (j = 0; j < 10; j++) {
			scanf("%lf", &a[i][j]);
		}
	}
	//fclose(fp)

	for (i = 0; i < 9; i++) {
		double max = a[0][i], min = a[0][i];
		for (j = 1; j < n; j++) {
			if (max < a[j][i]) {
				max = a[j][i];
			}
			if (min > a[j][i]) {
				min = a[j][i];
			}
		}
		for (j = 0; j < n; j++) {
			a[j][i] /= max;
		}
	}

	freopen("test.txt", "w", stdout);
	for (i = 0; i < n; i++) {
		for (j = 0; j < 10; j++){
			printf("%lf ", a[i][j]);
		}
		printf("\n");
	}
	return 0;
}
