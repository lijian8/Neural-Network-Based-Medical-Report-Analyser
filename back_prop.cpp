/*

Author : Samarth Khare
Created : March 2015

*/

#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <numeric>
#include <limits>
#include <string.h>
#include <bitset>
#include <assert.h>

 //#include <bits/stdc++.h>
#define pb push_back
#define mp make_pair

using namespace std;
#define DEBUG(x) cout << '>' << #x << ':' << x << endl;
#define imax numeric_limits<int>::max()
#define imin numeric_limits<int>::min()
#define lmax numeric_limits<LL>::max()
#define lmin numeric_limits<LL>::min()
#define SZ(x) (int)x.size()
#define M(x,t) memset (x, t, sizeof(x))
typedef long long LL;
#define MAX 10
#define alpha 0.5
double in[1][MAX],w1[MAX][MAX],w2[MAX][1],delta[MAX],hidden[1][MAX];
double t_error, final_delta, output,desired_output;
double l_rate = 1.0;


double bias[1][MAX], final_bias;
double prev[MAX][MAX], final_prev[MAX][1];
double prev_bias[1][MAX], final_prev_bias;


double sig( double value)
{
    double t = 1 + exp(-value);
    return 1.0/t;
}

//calculating hidden layer matrix
void multiply1()
{
    int r1,c2,c1;
    r1 = 1;
    c2  = c1 = MAX-1;
    for(int i=0;i<r1;i++)
        {
            for(int j=0;j<c2;j++)
            {
                hidden[i][j]=0;
                for(int k=0;k<c1;k++)
                    hidden[i][j] += in[i][k]*w1[k][j];

                hidden[i][j] =  sig(hidden[i][j] + bias[0][j]);
               // printf("%d\t",mult[i][j]);
            }
          //  printf("\n");
        }
}


// calculating expected output

void multiply2()
{
    int r1,c2,c1;
    r1 = c2 =  1;
    c1  = MAX-1;

    output = 0.0;
    int i,j,k;
    for( i=0;i<r1;i++)
        {
            for(j=0;j<c2;j++)
            {
                //mult[i][j]=0;
                for(k=0;k<c1;k++)
                    output += hidden[i][k]*w2[k][j];
               // printf("%d\t",mult[i][j]);
            }
          //  printf("\n");
        }
    output = sig(output + final_bias);
}

void print()
{

    for ( int i = 0; i < 1;i++){
        for ( int j = 0; j < MAX-1;j++) {
        cout << hidden[i][j] << " " ;
        }
        cout << endl;
    }

}
/*
int check()
{
    ifstream infile("abc.txt");
    string str;




    int correct ,wrong;
    correct = wrong = 0;
    int cnt = 0;

    // taking input values
    while(getline(infile,str)){
        if ( cnt > 100)
            break;
        cnt++;
        istringstream iss(str);
        double inp;
        iss>>inp;
        int i;
        for(i=0;i<9;i++){
            iss>>inp;
            in[0][i] = inp/10.0;
        }
        iss>>desired_output;

        if ( (int)desired_output == 2){
            desired_output = 0;
            //cout<<"0000000000\n";
        } else{
            desired_output = 1;
            //cout<<"1111111111\n";
        }
            multiply1();
            multiply2();

            if ( abs(output - 0.0) < abs(1.0 - output))
                output = 0;
            else
                output = 1;
            if ( output == desired_output) {

                correct++;
        }
            else
                wrong++;
            //cout<<output <<" "<<desired_output<<"\n";
    }
    if (4*(correct) > 3*(correct+wrong)){
        return 1;
    }
    return 0;
}
*/

int main()
{

    int correct ,wrong;
    correct = wrong = 0;

    //initailising random weights
    for ( int i = 0; i < MAX; i++ ) {
        for ( int j =0 ; j < MAX;j++) {
           int t = rand() % 100;
           w1[i][j] = t * ( 1.0) / 100;

        }
        int l = rand() % 100;
        w2[i][0] = bias[0][i] = final_bias =   l * ( 1.0)/100;

    }


    // taking input values
    //find maximum value and then calculate percemntGE
   /* for ( int i = 0; i < MAX-1; i++) {
        cin >> in[0][i];
        //in[0][i] = sig(in[0][i]);
    }
    */
    int cnt = 0;
    int cn1,cn2;
    cn1 = cn2 = 0;
    for ( int y = 0; y < 999; y++) {
        ifstream infile("abc.txt");
        string str;
        while(getline(infile,str)){

            //cnt++;
            istringstream iss(str);
            double inp;
            iss>>inp;
          //  vector<vector<double> > input_values(9);
          int i;
            for(i=0;i<9;i++){
                iss>>inp;
                in[0][i] = inp/10.0;
            }
            //print(input_weights);
            //print(input_values);
           // vector<vector<double> > output_value(1);
            iss>>desired_output;

            if ( (int)desired_output == 2){
                desired_output = 0;
                //cout<<"0000000000\n";
            } else{
                desired_output = 1;
                //cout<<"1111111111\n";
            }



        //intialising desired output
      //  desired_output = 0.0;

        if ( y < 999) {
            output = 0.5;

                multiply1();
                multiply2();
                final_delta = desired_output - output;
                final_delta  *= (output * ( 1 - output));
                // error for hidden layers
                for ( int i = 0; i < MAX -1 ;i++) {
                    delta[i] = hidden[0][i] * (1 - hidden[0][i]) * final_delta * w2[i][0];
                }

                //back propogation algorithm
                for ( int i = 0;i < MAX-1; i++) {
                    for ( int j =0 ; j < MAX -1;j++) {
                    w1[i][j] += delta[j] * l_rate * in[0][i] + alpha*prev[i][j];
                    prev[i][j] = delta[j] * l_rate * in[0][i];
                    }
                    bias[0][i] += delta[i] *( l_rate) * 1 + alpha * prev_bias[0][i];
                    prev_bias[0][i] = delta[i] *( l_rate) * 1;
                }

                for ( int i = 0; i < MAX -1; i++) {
                    w2[i][0] += l_rate * final_delta * hidden[0][i] + alpha*final_prev[i][0];
                    final_prev[i][0] = l_rate * final_delta * hidden[0][i];
                }
                final_bias +=  l_rate * final_delta * 1 + alpha * final_prev_bias;
                final_prev_bias = l_rate * final_delta * 1;


               // if ( check() == 1)
                 //   break;
           // print();

            } else {

                multiply1();
                multiply2();

                if ( abs(output - 0.0) < abs(1.0 - output))
                    output = 0;
                else
                    output = 1;
                if ( output == desired_output) {
                    correct++;
                }
                else
                    wrong++;
                cout<<output <<" asda "<<desired_output<<"\n";
                if (output == 0)
                    cn1++;
                else
                    cn2++;
                cout << correct <<  " " << wrong << endl;
        cout << cn1 << " "<< cn2 << endl;
        double ans = 1.0 * correct;
        ans /= (1.0)*(correct + wrong);
        cout<<ans<<"\n";

            //cout<<output<<" "<<desired_output<<"\n";
            }


            //cout << output << endl;
        }
    }
    ifstream infile1("breast.txt");
    string str;

    while(getline(infile1,str)){

            cnt++;
            istringstream iss(str);
            double inp;
            iss>>inp;
          //  vector<vector<double> > input_values(9);
          int i;
            for(i=0;i<9;i++){
                iss>>inp;
                in[0][i] = inp/10.0;
            }
            //print(input_weights);
            //print(input_values);
           // vector<vector<double> > output_value(1);
            iss>>desired_output;

            if ( (int)desired_output == 2){
                desired_output = 0;
                //cout<<"0000000000\n";
            } else{
                desired_output = 1;
                //cout<<"1111111111\n";
            }



        //intialising desired output
      //  desired_output = 0.0;

                multiply1();
                multiply2();

                if ( abs(output - 0.0) < abs(1.0 - output))
                    output = 0;
                else
                    output = 1;
                if ( output == desired_output) {
                    correct++;
                }
                else
                    wrong++;
                cout<<output <<" asda "<<desired_output<<"\n";
                if (output == 0)
                    cn1++;
                else
                    cn2++;
     //           cout << correct <<  " " << wrong << endl;
     //   cout << cn1 << " "<< cn2 << endl;
        double ans = 1.0 * correct;
        ans /= (1.0)*(correct + wrong);
        cout<<ans<<"\n";

            //cout<<output<<" "<<desired_output<<"\n";
            }
            //cout << output << endl;
    return 0;
}













