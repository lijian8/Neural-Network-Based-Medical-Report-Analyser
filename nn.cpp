/*

Author : Samarth Khare
Created : March 2015

*/

#include <bits/stdc++.h>

// one input layer -> number of nodes depend upon the data sets
// one hidden layer -> number of nodes is the mean of number of nodes in input layer and output layer
// one output layer -> number of nodes is one if regression
//                     number of nodes is equal to each class label if classifier
// node means a neuron

#define input_nodes 9
#define hidden_nodes 9
#define output_nodes 1
#define alpha 0.1

using namespace std;

double input_hidden[input_nodes][hidden_nodes];          // weights of input-hidden connection
double hidden_output[hidden_nodes][output_nodes];        // weights of hidden-output connection
double x[input_nodes];									 // input to be given
double hidden_z[hidden_nodes];							 // z = b + x * weight
double hidden_y[hidden_nodes];   						 // sigmoid of z
double output_z[output_nodes];                           // z(output) = b(output) + weight * (hidden_y)
double output_y[output_nodes];                           // y(output) = sigmoid of z(output)
double bias_hidden[hidden_nodes];						 // initial bias
double bias_output[output_nodes];
double prev_bias_hidden[hidden_nodes];
double prev_bias_output[output_nodes];
double desired;
double learning_rate;
double errorfactor_hidden[hidden_nodes];
double errordelta_hidden[hidden_nodes];
double errorfactor_output[output_nodes];
double errordelta_output[output_nodes];
double prev_errordelta_hidden[hidden_nodes];
double prev_errordelta_output[output_nodes];
int id;
FILE *fp, *fp1;

void nn_init();
void setInputValues();
void setDesiredOutput();
double train_nn();
double test_nn();
void feedForward_hidden();
double feedForward_output();
double calSigmoid(double);
void backPropagate();
void Delta_output();
void Delta_hidden();
void adjustError();
void initialize_weights();
void initialize_bias();
void initialize_error();
void normalize_data(double *, int);
void normalize_input_hidden();
void normalize_hidden_output();
void print();

int main()
{


	int t;
	int i;

	//srand(time(NULL));

	initialize_weights();
	initialize_bias();
	initialize_error();

	double output;
	int j, n;

	t = 300;
	learning_rate = 0.2;
	for (j = 0; j < 100; j++) {
        n = t;
        fp = fopen("train.txt", "r");
        while (n--) {
            nn_init();
            output = train_nn();
            //return 0;
        }
        fclose(fp);
       // initialize_error();
       // adjustError((double)t);
	}
	//print();
	//learning_rate = 0.000001;
	fp = fopen("valid.txt", "r");
	n = 120;
	t = n;
	double hit = 0.0;
	printf("Testing starts here\n\n");
	while (n--) {
        nn_init();
        output = test_nn();
        printf("\n\ndesired = %lf and actual = %lf\n", desired, output);
        if (abs(output - 0.0) < abs(1.0 - output)) {
        	if (desired == 2) {
        		hit++;
        	}
        } else {
        	if (desired == 4) {
            	hit++;
            }
        }
	}

	fclose(fp);
	printf("Accuracy = %lf\n", hit/t);
	return 0;
}

void initialize_weights()
{
    int i, j;
	for (i = 0; i < input_nodes; i++) {
		for (j = 0; j < hidden_nodes; j++) {
			input_hidden[i][j] = ((float) (rand() % 200) - 100) / 100;             // weight is initialized
		}
	}
	for (i = 0; i < hidden_nodes; i++) {
		for (j = 0; j < output_nodes; j++) {
			hidden_output[i][j] = ((float) (rand() % 200) - 100) / 100;            // weight is initialized
		}
	}
	return ;
}

void initialize_bias()
{
    int i;
    for (i = 0; i < hidden_nodes; i++) {
		bias_hidden[i] = ((float) (rand() % 200) - 100) / 100;
		prev_bias_hidden[i] = 0;
		prev_errordelta_hidden[i] = 0;
	}
	for (i = 0; i < output_nodes; i++) {
		bias_output[i] = ((float) (rand() % 200) - 100) / 100;
		prev_bias_output[i] = 0;
		prev_errordelta_output[i] = 0;
	}
	return ;
}

void initialize_error()
{
	int i;
	for (i = 0; i < hidden_nodes; i++) {
		errordelta_hidden[i] = 0;
		errorfactor_hidden[i] = 0;
	}
	for (i = 0; i < output_nodes; i++) {
		errordelta_output[i] = 0;
		errorfactor_output[i] = 0;
	}

	return ;
}

void nn_init()
{
	//fscanf(fp, "%d", &id);
	setInputValues();
	setDesiredOutput();
	return ;
}

void setDesiredOutput()
{
	fscanf(fp, "%lf", &desired);
	return ;
}

void setInputValues()
{
	int i;
	for (i = 0; i < input_nodes; i++) {
		fscanf(fp, "%lf", &x[i]);
	}
	//normalize_data(x, input_nodes);
	return ;
}

void normalize_data(double a[], int n)
{
    double max = a[0], min = a[0];
    int i;
	for (i = 1; i < n; i++) {
		if (max < a[i]) {
			max = a[i];
		}
		if (min > a[i]) {
			min = a[i];
		}
	}
	for (i = 0; i < n; i++) {
		a[i] = 2*((a[i] - min) / (max - min)) - 1;
	}
	return ;
}

/*void normalize_input_hidden()
{
	int i, j;
	double max = input_hidden[0][0], min = input_hidden[0][0];

	for (i = 0; i < input_nodes; i++) {
		for (j = 0; j < hidden_nodes; j++) {
			if (max < input_hidden[i][j]) {
				max = input_hidden[i][j];
			}
			if (min > input_hidden[i][j]) {
				min = input_hidden[i][j];
			}
		}
	}

	for (i = 0; i < input_nodes; i++) {
		for (j = 0; j < hidden_nodes; j++) {
			input_hidden[i][j] = 2*((input_hidden[i][j]-min)/(max-min)) - 1;
		}
	}

	return ;
}

void normalize_hidden_output()
{
	int i, j;
	double max = hidden_output[0][0], min = hidden_output[0][0];

	for (i = 0; i < hidden_nodes; i++) {
		for (j = 0; j < output_nodes; j++) {
			if (max < hidden_output[i][j]) {
				max = hidden_output[i][j];
			}
			if (min > hidden_output[i][j]) {
				min = hidden_output[i][j];
			}
		}
	}

	for (i = 0; i < hidden_nodes; i++) {
		for (j = 0; j < output_nodes; j++) {
			hidden_output[i][j] = 2*((hidden_output[i][j]-min)/(max-min)) - 1;
		}
	}
	return ;
}*/

double train_nn()
{
    double output;
	feedForward_hidden();
	output = feedForward_output();
	backPropagate();

	//print();
	return output;
}

double test_nn()
{
	double output;
    feedForward_hidden();
    output = feedForward_output();
    //backPropagate(9);

    return output;
}

void feedForward_hidden()
{
	int i, j;
	for (j = 0; j < hidden_nodes; j++) {
		hidden_z[j] = bias_hidden[j];
		for (i = 0; i < input_nodes; i++) {
			hidden_z[j] += x[i] * input_hidden[i][j];   // z is calculated for hidden layer
		}
	}
	//normalize_data(hidden_z, hidden_nodes);

	for (i = 0; i < hidden_nodes; i++) {
		hidden_y[i] = calSigmoid(hidden_z[i]);          // y is calculated for hidden layer
	}
	return ;
}

double feedForward_output()
{
	int i, j;
	for (j = 0; j < output_nodes; j++) {
		output_z[j] = bias_output[j];
		for (i = 0; i < hidden_nodes; i++) {
			output_z[j] += hidden_y[i] * hidden_output[i][j];     // z is calculated for output layer
		}
	}
	//normalize_data(output_z, output_nodes);

	for (i = 0; i < output_nodes; i++) {
		output_y[i] = calSigmoid(output_z[i]);                    // y is calculated for output layer
	}

	return output_y[0];
}

double calSigmoid(double z)
{
	return (1.0 / (1.0 + exp(-z)));
}

void backPropagate()
{
	Delta_output();
	Delta_hidden();
	//initialize_error();
	adjustError();
	return ;
}

void Delta_output()
{
	int i;
	if (desired == 2) {
		desired = 0;
	} else {
		desired = 1;
	}
	for (i = 0; i < output_nodes; i++) {
		errorfactor_output[i] = desired - output_y[i];
	}

	for (i = 0; i < output_nodes; i++) {
		errordelta_output[i] = output_y[i] * (1 - output_y[i]) * errorfactor_output[i];
	}
	return ;
}

void Delta_hidden()
{
	int i, j;
	for (i = 0; i < hidden_nodes; i++) {
		errorfactor_hidden[i] = 0;
		for (j = 0; j < output_nodes; j++) {
			errorfactor_hidden[i] += errordelta_output[j] * hidden_output[i][j];
		}
	}

	//normalize_data(errorfactor_hidden, hidden_nodes);

	for (i = 0; i < hidden_nodes; i++) {
		errordelta_hidden[i] = hidden_y[i] * (1 - hidden_y[i]) * errorfactor_hidden[i];
	}

	return ;
}

void adjustError()
{
	int i, j;

	for (i = 0; i < output_nodes; i++) {
		bias_output[i] += learning_rate * errordelta_output[i] + alpha * prev_bias_output[i];
		prev_bias_output[i] = learning_rate * errordelta_output[i];
	}

	//normalize_data(bias_output, output_nodes);

	for (i = 0; i < hidden_nodes; i++) {
		for (j = 0; j < output_nodes; j++) {
			hidden_output[i][j] += learning_rate * errordelta_output[j] * hidden_y[i] + alpha * prev_errordelta_output[j];
			prev_errordelta_output[j] = learning_rate * errordelta_output[j] * hidden_y[i];
		}
	}

	//normalize_hidden_output();

	for (i = 0; i < hidden_nodes; i++) {
		bias_hidden[i] += learning_rate * errordelta_hidden[i] + alpha * prev_bias_hidden[i];
		prev_bias_hidden[i] = learning_rate * errordelta_hidden[i];
	}

	//normalize_data(bias_hidden, hidden_nodes);

	for (i = 0; i < input_nodes; i++) {
		for (j = 0; j < hidden_nodes; j++) {
			input_hidden[i][j] += learning_rate * errordelta_hidden[j] * x[i] + alpha * prev_errordelta_hidden[j];
			prev_errordelta_hidden[j] = learning_rate * errordelta_hidden[j] * x[i];
		}
	}

	//normalize_input_hidden();
}

/*void print()
{
	int i, j;
	for (i = 0; i < input_nodes; i++) {
		printf("%lf ", x[i]);
	}

	cout << endl;

	/*for (i = 0; i < input_nodes; i++) {
		for (j = 0; j < hidden_nodes; j++) {
			printf("%lf ", input_hidden[i][j]);
		}
		cout << endl;
	}

	cout << endl;

	for (i = 0; i < hidden_nodes; i++) {
		printf("%lf ", bias_hidden[i]);
	}

	cout << endl;

	/*for (i = 0; i < hidden_nodes; i++) {
		for (j = 0; j < output_nodes; j++) {
			printf("%lf ", hidden_output[i][j]);
		}
		cout << endl;
	}

	cout << endl;

	for (i = 0; i < output_nodes; i++) {
		printf("%lf ", bias_output[i]);
	}

	cout << endl;
}*/
