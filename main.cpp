#include <iostream>
#include "linalg.hpp"
#include "neuralnet.hpp"
#include <chrono>

#pragma warning (disable : 4996) 
#pragma optimize("/O2", on)

using namespace std;

class Timer
{
    public:
    Timer()
    {
        start_point = chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        Stop();
    }

    void Stop()
    {
        auto endTimepoint = chrono::high_resolution_clock::now();

        auto start = chrono::time_point_cast< chrono::milliseconds >(start_point).time_since_epoch().count();
        auto end = chrono::time_point_cast< chrono::milliseconds >(endTimepoint).time_since_epoch().count();

        auto duration = end - start;

        cout << "\nThe execution took: " << duration << "ms\n";
    }

    private:
    chrono::time_point<chrono::high_resolution_clock> start_point;
};



inline int readInt32(ifstream& fin);
inline int readInt8(ifstream& fin);
vector<pair<ColumnVector<double>, int>> LoadTrainData();
vector<pair<ColumnVector<double>, int>> LoadTestData();

int main()
{
    Timer t;
    vector<pair<ColumnVector<double>, int>> trainData = LoadTrainData(), testData = LoadTestData();
    return 0;

    NeuralNetwork model({ 5, 4, 2 });

    std::pair<std::vector<Matrix<double>>, std::vector<ColumnVector<double>>> ans = model.backprop(ColumnVector<double>({ 1, 2, 3, 4, 5 }), OneDim<double>(2, 1));

}

unsigned char buffer[50000005];
int k = 0;


// ifstream fin;
FILE* fin;

vector<pair<ColumnVector<double>, int>> LoadTrainData()
{
    int nrImg = 60000, valMax = 255, imLen = 28 * 28;
    vector<pair<ColumnVector<double>, int>> ans(nrImg);

    fin = fopen("../../Res/mnist/train-images.idx3-ubyte", "rb");
    assert(fin); // check if the operation is succeeded
    fread(buffer, 47040016, 1, fin);// read all the info into a buffer
    k = 16; // skip the known information

    // read images
    for ( int i = 0; i < nrImg; i++ )
    {
        // cout << i << endl;
        ans[i].first.setDim(imLen); // allocate memory
        for ( int j = 0; j < imLen; j++ )
            ans[i].first[j] = buffer[k++]; // read the info
    }

    fclose(fin);
    cout << "done\n";

    // read lables

    fin = fopen("../../Res/mnist/train-labels.idx1-ubyte", "rb");
    assert(fin); // check if the operation is succeeded
    fread(buffer, 60008, 1, fin);// read all the info into a buffer
    k = 8; // skip the known information

    for ( int i = 0; i < nrImg; i++ )
        ans[i].second = buffer[k++];

    fclose(fin);
    return ans;
}

vector<pair<ColumnVector<double>, int>> LoadTestData()
{
    int nrImg = 10000, valMax = 255, imLen = 28 * 28;
    vector<pair<ColumnVector<double>, int>> ans(nrImg);

    fin = fopen("../../Res/mnist/t10k-images.idx3-ubyte", "rb");
    assert(fin); // check if the operation is succeeded
    fread(buffer, 7840016, 1, fin);// read all the info into a buffer
    k = 16; // skip the known information

    // read images
    for ( int i = 0; i < nrImg; i++ )
    {
        ans[i].first.setDim(imLen); // allocate memory
        for ( int j = 0; j < imLen; j++ )
            ans[i].first[j] = buffer[k++]; // read the info
    }

    fclose(fin);
    cout << "done\n";

    // read lables

    fin = fopen("../../Res/mnist/t10k-labels.idx1-ubyte", "rb");
    assert(fin); // check if the operation is succeeded
    fread(buffer, 10008, 1, fin);// read all the info into a buffer
    k = 8; // skip the known information

    for ( int i = 0; i < nrImg; i++ )
        ans[i].second = readInt8();

    fclose(fin);
    return ans;
}


inline unsigned int readInt32()
{
    unsigned int nr = 0;

    nr |= (buffer[k] << 24);
    nr |= (buffer[k + 1] << 16);
    nr |= (buffer[k + 2] << 8);
    nr |= buffer[k + 3];

    k += 4;
    return nr;
}

inline unsigned int readInt8()
{
    return ( unsigned int )buffer[k++];
}
