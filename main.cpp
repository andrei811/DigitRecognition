#pragma warning (disable : 4996) 
#pragma optimize("/O2", on)

#include <iostream>
#include "linalg.hpp"
#include "neuralnet.hpp"
#include "mvector.hpp"
using namespace std;
#include <timer.hpp>


mine::vector<pair<ColumnVector<double>, int>> LoadTrainData();
mine::vector<pair<ColumnVector<double>, int>> LoadTestData();

int main()
{
	/*NeuralNetwork m((mine::vector<int>&&) mine::vector <int>({ 4, 3, 2 }));
	ColumnVector<double> out = OneDim<double>(2, 1);
	mine::vector<Matrix<double>> nablaW(2);
	mine::vector<ColumnVector<double>> nablaB(2);

	nablaW[0].setDim(3, 4);
	nablaW[1].setDim(2, 3);
	nablaB[0].setDim(3);
	nablaB[1].setDim(2);

	mine::vector<ColumnVector<double>> xAct, yAct;
	yAct.resize(3);
	xAct.resize(2);

	m.backprop(ColumnVector<double>({ 3, 2, 1, 2 }), out, nablaW, nablaB, xAct, yAct);

	return 0;*/


	NeuralNetwork model((mine::vector<int>&&) mine::vector <int>({784, 64, 32, 10}));

	mine::vector<std::pair<ColumnVector<double>, int>> trainData = LoadTrainData(), testData = LoadTestData();

	model.SGD(trainData, testData);
}

char basedir[] = BASE_DIR;
char path[] = "";
unsigned char buffer[50000005];
int k = 0;

FILE* fin;

mine::vector<std::pair<ColumnVector<double>, int>> LoadTrainData()
{
	int nrImg = 60000, valMax = 255, imLen = 28 * 28;
	mine::vector<std::pair<ColumnVector<double>, int>> ans(nrImg);

	char folder[] = "/Res/mnist/train-images.idx3-ubyte";
	strcpy(path, basedir);
	strcat(path, folder);
	fin = fopen(path, "rb");


	assert(fin); // check if the operation is succeeded
	fread(buffer, 47040016, 1, fin);// read all the info into a buffer
	k = 16; // skip the known information

	// read images
	for (int i = 0; i < nrImg; i++)
	{
		ans[i].first.setDim(imLen); // allocate memory
		for (int j = 0; j < imLen; j++)
			ans[i].first[j] = (buffer[k++] / 255.0); // read the info
	}

	fclose(fin);
	cout << "done\n";

	// read lables

	strcpy(folder, "/Res/mnist/train-labels.idx1-ubyte");
	strcpy(path, basedir);
	strcat(path, folder);
	fin = fopen(path, "rb");

	assert(fin); // check if the operation is succeeded
	fread(buffer, 60008, 1, fin);// read all the info into a buffer
	k = 8; // skip the known information

	for (int i = 0; i < nrImg; i++)
		ans[i].second = buffer[k++];

	fclose(fin);
	return ans;
}

mine::vector<pair<ColumnVector<double>, int>> LoadTestData()
{
	int nrImg = 10000, valMax = 255, imLen = 28 * 28;
	mine::vector<pair<ColumnVector<double>, int>> ans(nrImg);

	char folder[] = "/Res/mnist/t10k-images.idx3-ubyte";
	strcpy(path, basedir);
	strcat(path, folder);
	fin = fopen(path, "rb");

	assert(fin); // check if the operation is succeeded
	fread(buffer, 7840016, 1, fin);// read all the info into a buffer
	k = 16; // skip the known information

	// read images
	for (int i = 0; i < nrImg; i++)
	{
		ans[i].first.setDim(imLen); // allocate memory
		for (int j = 0; j < imLen; j++)
			ans[i].first[j] = (buffer[k++] / 255.0); // read the info
	}

	fclose(fin);
	cout << "done\n";

	// read lables
	strcpy(folder, "/Res/mnist/t10k-labels.idx1-ubyte");
	strcpy(path, basedir);
	strcat(path, folder);
	fin = fopen(path, "rb");

	assert(fin); // check if the operation is succeeded
	fread(buffer, 10008, 1, fin);// read all the info into a buffer
	k = 8; // skip the known information

	for (int i = 0; i < nrImg; i++)
		ans[i].second = buffer[k++];

	fclose(fin);
	return ans;
}

